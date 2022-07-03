#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QListIterator>
#include <QSettings>
#include <QtSql>
#include <QString>
#include <QTextStream>
#include <QSysInfo>

#include "Database.h"
#include "InitializeSettings.h"
#include "Logger.h"
#include "ServiceTcp.h"


#ifndef THREAD_POOL_MAX
#define THREAD_POOL_MAX  QThread::idealThreadCount()
#endif


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray timestamp = QDateTime::currentDateTime().toString().toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "[%s] DEBUG: [%s:%u] %s\r\n", timestamp.constData(), context.file, context.line, localMsg.constData());
        break;
    case QtInfoMsg:
        printf("[%s] INFO: %s\r\n", timestamp.constData(), localMsg.constData());
        break;
    case QtWarningMsg:
        printf("[%s] WARN: %s\r\n", timestamp.constData(), localMsg.constData());
        break;
    case QtCriticalMsg:
        printf("[%s] FATAL ERROR: %s\r\n", timestamp.constData(), localMsg.constData());
        printf("Exiting...\r\n");
        exit(EXIT_FAILURE);
    case QtFatalMsg:
        printf("[%s] FATAL ERROR: %s\r\n", timestamp.constData(), localMsg.constData());
        printf("Exiting...\r\n");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput); // Install the handler

    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("SogonSecurity");
    QCoreApplication::setOrganizationDomain("sogonsecurity.com");
    QCoreApplication::setApplicationName("Clutching Pearls C2");
    QCoreApplication::setApplicationVersion("v0.1");
    QTextStream out(stdout);
    QSettings settings;
    Database database;
    Logger logger;


    // Banner
    QString banner;
    banner  = QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion();
    banner += " - A PoC learning project by Chris Humphries (@sogonsec)";
    //out << Qt::endl << banner << Qt::endl;
    out << "+-" << QString("-").repeated(banner.length()) << "-+" << Qt::endl
        << "| " << banner << " |" << Qt::endl
        << "+-" << QString("-").repeated(banner.length()) << "-+" << Qt::endl;

    // System info
    qInfo().nospace().noquote() << "System Info: " << QSysInfo::prettyProductName()
                                << " (" << QSysInfo::kernelType() << " " << QSysInfo::kernelVersion()
                                << ") " << QSysInfo::buildAbi();
    // Configuration
    qInfo().nospace().noquote() << "Configuration file: " << settings.fileName();
    // Database
    qInfo().nospace().noquote() << "Database: " << database.getDb().driverName()
                                << " for database " << database.getDb().databaseName();

    out << Qt::endl;



    // ///////////////////////////////////////////////////////////////////////////////////////////////
    // CLI argument provisioning
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    // Initialize/first time stuff
    QCommandLineOption initializeOption("initialize", QCoreApplication::translate("main", "Initialize the settings data and database (equivalent of --initialize-settings and --initialize-database)."));
    QCommandLineOption initializeSettingsOption("initialize-settings", QCoreApplication::translate("main", "Initialize the settings data."));
    parser.addOption(initializeOption);
    parser.addOption(initializeSettingsOption);

    // TCP Service
    QCommandLineOption serviceTcpOption("service-tcp", QCoreApplication::translate("main", "Enable raw TCP service."));
    QCommandLineOption serviceTcpPortOption("service-tcp-port", QCoreApplication::translate("main", "Raw TCP service port number."),
                                            QCoreApplication::translate("main", "port number"));
    parser.addOption(serviceTcpOption);
    parser.addOption(serviceTcpPortOption);

    parser.process(app);
    // ///////////////////////////////////////////////////////////////////////////////////////////////



    // ///////////////////////////////////////////////////////////////////////////////////////////////
    // Initialize stuff
    if (parser.isSet(initializeOption) || parser.isSet(initializeSettingsOption)) {
        qInfo().nospace().noquote() << "Initializing settings...";
        InitializeSettings initializeSettings;
        initializeSettings.initialize();
        qInfo().nospace().noquote() << "Initialized settings";

        qInfo().nospace().noquote() << "Initialization complete. Restart server without initialize flags. Exiting...";
        exit(EXIT_SUCCESS);
    }
    // ///////////////////////////////////////////////////////////////////////////////////////////////



    // ///////////////////////////////////////////////////////////////////////////////////////////////
    // Start services
    // Set the global thread pool count
    QThreadPool::globalInstance()->setMaxThreadCount(THREAD_POOL_MAX);

    bool servicesStarted = false;

    // Raw TCP service
    ServiceTcp serviceTcp;
    if (parser.isSet(serviceTcpOption) || settings.value("service/tcp").toBool()) {
        // Start the service
        quint16 port = settings.value("tcp/port").toInt();
        if (parser.isSet(serviceTcpPortOption)) {
            port = parser.value(serviceTcpPortOption).toInt();
        }
        if (port < 1 || port > 65535) {
            qCritical().nospace().noquote() << "The raw TCP port number should be between 1 and 65535.";
        }
        qInfo().nospace().noquote() << ServiceTcp::NAME << ": attempting to start... ";
        serviceTcp.startService(port);
        if (serviceTcp.isListening()) {
            qInfo().nospace().noquote() << ServiceTcp::NAME << ": listening on port " << port << ".";
            servicesStarted = true;
        }
        else {
            qWarning().nospace().noquote() << ServiceTcp::NAME << ": NOT listening on port " << port << ".";
        }
    }
    // End - Raw TCP service

    // Check if at least one service has started
    if (servicesStarted == false) {
        qWarning().nospace().noquote() << "No services are started! You need to start a service for this software to be useful.";
    }
    // ///////////////////////////////////////////////////////////////////////////////////////////////


    // Application started without errors!
    qInfo().nospace().noquote() << QCoreApplication::applicationName() << " " << QCoreApplication::applicationVersion() << " started!";

    // Main loop
    return app.exec();
}
