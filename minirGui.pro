#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T15:35:59
#
#-------------------------------------------------

QT       += core gui

TARGET = minirGui
TEMPLATE = app


INCLUDEPATH += /usr/include/vtk-5.8/

SOURCES += main.cpp\
        minirgui.cpp endoQuery.cpp

HEADERS  += minirgui.h endoQuery.h

FORMS    += minirgui.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lQVTK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lQVTK
else:symbian: LIBS += -lQVTK
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lQVTK

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lvtkRendering
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lvtkRendering
else:symbian: LIBS += -lvtkRendering
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lvtkRendering

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lvtkFiltering
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lvtkFiltering
else:symbian: LIBS += -lvtkFiltering
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lvtkFiltering

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lvtkHybrid
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lvtkHybrid
else:symbian: LIBS += -lvtkHybrid
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lvtkHybrid

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lvtkCommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lvtkCommon
else:symbian: LIBS += -lvtkCommon
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lvtkCommon

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lvtkIO
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lvtkIO
else:symbian: LIBS += -lvtkIO
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lvtkIO

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include
