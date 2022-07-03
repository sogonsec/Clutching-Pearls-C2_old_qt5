#include "Database.h"

Database::Database(QObject *parent) : QObject(parent)
{
    this->connectToDatabase();
}

const QSqlDatabase Database::getDb()
{
    return this->db;
}

void Database::setDb(QSqlDatabase newDb)
{
    this->db = newDb;
}


/*
Database Types
===================================================
QDB2	IBM DB2
QIBASE	Borland InterBase Driver
QMYSQL	MySQL Driver
QOCI	Oracle Call Interface Driver
QODBC	ODBC Driver (includes Microsoft SQL Server)
QPSQL	PostgreSQL Driver
QSQLITE	SQLite version 3 or above
QSQLITE2	SQLite version 2
QTDS	Sybase Adaptive Server
*/
bool Database::connectToDatabase()
{
    bool status = false;
    QSqlDatabase newDb;
    QSettings settings;

    // Read values from settings
    QString databaseType = settings.value("database/type", "").toString().toLower();
    if (databaseType == "sqlite") {
        QString defaultSqliteDatabasePath = QCoreApplication::applicationDirPath() + QDir::separator() + "clutchingpearls.db";
        QString sqliteName = settings.value("database_sqlite/path", defaultSqliteDatabasePath).toString();
        if (sqliteName == "auto") {
            sqliteName = defaultSqliteDatabasePath;
        }
        QString uniqueName = this->generateUniqueName(sqliteName);
        if(QSqlDatabase::contains(uniqueName)) {
            this->setDb(QSqlDatabase::database(uniqueName));
        }
        else {
            newDb = QSqlDatabase::addDatabase( "QSQLITE", uniqueName);
            newDb.setDatabaseName(sqliteName);
            this->setDb(newDb);
        }
        status = this->db.open();
    }
    else if (databaseType == "mysql") {
        qDebug() << "In MySQL section";

        QString databaseName;
        QString databaseHost;
        QString databaseUser;
        QString databasePass;
        QString uniqueName;

        databaseName = settings.value("database_mysql/name").toString();
        databaseHost = settings.value("database_mysql/host").toString();
        databaseUser = settings.value("database_mysql/user").toString();
        databasePass = settings.value("database_mysql/pass").toString();
        uniqueName = this->generateUniqueName(databaseName);

        qDebug() << "databaseName = " << databaseName;
        qDebug() << "databaseHost = " << databaseHost;
        qDebug() << "databaseUser = " << databaseUser;
        qDebug() << "databasePass = " << databasePass;
        qDebug() << "uniqueName = " << uniqueName;

        if(QSqlDatabase::contains(uniqueName)) {
            this->setDb(QSqlDatabase::database(uniqueName));
            qDebug() << "Using existing connection";
        }
        else {
            newDb = QSqlDatabase::addDatabase("QMYSQL");
            newDb.setHostName(databaseHost);
            newDb.setDatabaseName(databaseName);
            newDb.setUserName(databaseUser);
            newDb.setPassword(databasePass);
            this->setDb(newDb);
            qDebug() << "Using new connection";
        }
        status = this->db.open();
    }
    else if (databaseType == "postgresql") {
        qDebug() << "In PostgreSQL section";

        QString databaseName;
        QString databaseHost;
        QString databaseUser;
        QString databasePass;
        QString uniqueName;

        databaseName = settings.value("database_postgresql/name").toString();
        databaseHost = settings.value("database_postgresql/host").toString();
        databaseUser = settings.value("database_postgresql/user").toString();
        databasePass = settings.value("database_postgresql/pass").toString();
        uniqueName = this->generateUniqueName(databaseName);

        qDebug() << "databaseName = " << databaseName;
        qDebug() << "databaseHost = " << databaseHost;
        qDebug() << "databaseUser = " << databaseUser;
        qDebug() << "databasePass = " << databasePass;
        qDebug() << "uniqueName = " << uniqueName;

        if(QSqlDatabase::contains(uniqueName)) {
            this->setDb(QSqlDatabase::database(uniqueName));
            qDebug() << "Using existing connection";
        }
        else {
            newDb = QSqlDatabase::addDatabase("QPSQL");
            newDb.setHostName(databaseHost);
            newDb.setDatabaseName(databaseName);
            newDb.setUserName(databaseUser);
            newDb.setPassword(databasePass);
            this->setDb(newDb);
            qDebug() << "Using new connection";
        }
        status = this->db.open();
    }
    else {
//        // No database configured, use in-memory database
//        qDebug() << "No database defined, using SQLite in-memory database instead.";
//        QString uniqueName = this->generateUniqueName("");
//        if(QSqlDatabase::contains(uniqueName))
//            this->setDb(QSqlDatabase::database(uniqueName));
//        else {
//            newDb = QSqlDatabase::addDatabase( "QSQLITE", uniqueName);
//            newDb.setDatabaseName(":memory:");
//            this->setDb(newDb);
//        }
//        status = this->db.open();
        qCritical().noquote().nospace() << "No database configured. Please configure a database.";
    }

    if (status == false) {
        qCritical().noquote().nospace() << this->db.lastError().driverText() << ": " << this->db.lastError().databaseText();
    }
    return status;
}

QString Database::generateUniqueName(QString name)
{
    if (name.isNull() || name.isEmpty()) {
        name = QCoreApplication::applicationName().toLower();
    }
    QString uniqueName;
    uniqueName = name + "/" + QString::number((quint64)QThread::currentThread(), 16);
    return uniqueName;
}
