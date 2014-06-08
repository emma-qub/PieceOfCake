HEADERS += \
  # Controller
  Controller/AbstractController.h \
  Controller/GameController.h \
  Controller/LevelDesignerController.h \
  # Core
  Core/Point2d.h \
  Core/Polygon.h \
  Core/Segment.h \
  Core/Vector2d.h \
  # Delegate
  Delegate/LevelDesignerTreeItemDelegate.h \
  # Item
  Item/TreeItem.h \
  # LineModifier
  LineModifier/Deviation.h \
  LineModifier/LineModifier.h \
  LineModifier/Obstacle.h \
  LineModifier/Portal.h \
  LineModifier/Tape.h \
  # Model
  Model/GameModel.h \
  Model/LevelDesignerModel.h \
  Model/TreeModel.h \
  # View
  View/GameView.h \
  View/LevelDesignerScribbleView.h \
  View/LevelDesignerTreeView.h \
  # Root
  Commands.h \
  Hint.h \
  MainWindow.h \
  ParserXML.h \
  tests.h

SOURCES += \
  # Controller
  Controller/AbstractController.cpp \
  Controller/GameController.cpp \
  Controller/LevelDesignerController.cpp \
  # Core
  Core/Point2d.cpp \
  Core/Polygon.cpp \
  Core/Segment.cpp \
  Core/Vector2d.cpp \
  # Delegate
  Delegate/LevelDesignerTreeItemDelegate.cpp \
  # Item
  Item/TreeItem.cpp \
  # LineModifier
  LineModifier/Deviation.cpp \
  LineModifier/LineModifier.cpp \
  LineModifier/Obstacle.cpp \
  LineModifier/Portal.cpp \
  LineModifier/Tape.cpp \
  # Model
  Model/GameModel.cpp \
  Model/LevelDesignerModel.cpp \
  Model/TreeModel.cpp \
  # View
  View/GameView.cpp \
  View/LevelDesignerScribbleView.cpp \
  View/LevelDesignerTreeView.cpp \
  # Root
  Commands.cpp \
  Hint.cpp \
  MainWindow.cpp \
  ParserXML.cpp \
  main.cpp

INCLUDEPATH += \
  Controller \
  Core \
  Delegate \
  Item \
  LineModifier \
  Model \
  View

CONFIG += \
  c++11

QT += \
  core \
  gui \
  widgets \
  xml
