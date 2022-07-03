QT += core
QT += network
QT += sql
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += sdk_no_version_check

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui

#DEFINES += THREAD_POOL_MAX=150

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(release, debug|release):QMAKE_POST_LINK=$(STRIP) $(TARGET)

INCLUDEPATH += src/include

SOURCES += \
        src/CommandHandler.cpp \
        src/CommandShell.cpp \
        src/DataEnvelope.cpp \
        src/DataEnvelopeBody.cpp \
        src/DataEnvelopeChunker.cpp \
        src/DataEnvelopeFault.cpp \
        src/DataEnvelopeHeader.cpp \
        src/DataRequestProcessor.cpp \
        src/DataResponseProcessor.cpp \
        src/DataTransmogrifier.cpp \
        src/Database.cpp \
        src/InitializeSettings.cpp \
        src/Logger.cpp \
        src/ServiceTcp.cpp \
        src/ServiceTcpProcessor.cpp \
        src/ServiceTcpProcessorTask.cpp \
        src/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/include/CommandHandler.h \
    src/include/CommandInterface.h \
    src/include/CommandShell.h \
    src/include/DataEnvelope.h \
    src/include/DataEnvelopeBody.h \
    src/include/DataEnvelopeChunker.h \
    src/include/DataEnvelopeFault.h \
    src/include/DataEnvelopeHeader.h \
    src/include/DataRequestProcessor.h \
    src/include/DataResponseProcessor.h \
    src/include/DataTransmogrifier.h \
    src/include/Database.h \
    src/include/InitializeSettings.h \
    src/include/Logger.h \
    src/include/ServiceTcp.h \
    src/include/ServiceTcpProcessor.h \
    src/include/ServiceTcpProcessorTask.h
