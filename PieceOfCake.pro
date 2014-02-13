HEADERS += \
    Point2d.h \
    tests.h \
    Vector2d.h \
    Polygon.h \
    Segment.h \
    MainWindow.h \
    ParserXML.h \
    Hint.h \
    SliceArea.h \
    MainMenuWidget.h \
    PolygonTreeModel.h \
#    PolygonEditorView.h \
    PolygonScribbleView.h \
    TreeModel.h \
    TreeItem.h \
    PolygonTreeView.h \
    PolygonItemDelegate.h \
    Commands.h \
    PolygonController.h \
    GameController.hxx \
    GameModel.hxx

SOURCES += \
    Point2d.cpp \
    main.cpp \
    tests.cpp \
    Vector2d.cpp \
    Polygon.cpp \
    Segment.cpp \
    MainWindow.cpp \
    ParserXML.cpp \
    Hint.cpp \
    SliceArea.cpp \
    MainMenuWidget.cpp \
    PolygonTreeModel.cpp \
#    PolygonEditorView.cpp \
    PolygonScribbleView.cpp \
    TreeModel.cpp \
    TreeItem.cpp \
    PolygonTreeView.cpp \
    PolygonItemDelegate.cpp \
    Commands.cpp \
    PolygonController.cpp \
    GameController.cxx \
    GameModel.cxx

CONFIG += c++11

QT += \
    core \
    gui \
    widgets \
    xml
