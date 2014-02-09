#include "MainMenuWidget.h"

MainMenuWidget::MainMenuWidget(QWidget *parent) :
    QWidget(parent) {

    QFont flatButtonFont("edosz", 30);

    _tutorialButton = new QPushButton("Tutorial");
    _tutorialButton->setFont(flatButtonFont);
    _tutorialButton->setFixedSize(300, 60);
//    _tutorialButton->move(150, 200);
    _tutorialButton->setFlat(true);
    _tutorialButton->setFocusPolicy(Qt::NoFocus);

    _playButton = new QPushButton("Play");
    _playButton->setFont(flatButtonFont);
    _playButton->setFixedSize(200, 60);
//    _playButton->move(150, 270);
    _playButton->setFlat(true);
    _playButton->setFocusPolicy(Qt::NoFocus);

    _levelMakerButton = new QPushButton("Level Maker");
    _levelMakerButton->setFont(flatButtonFont);
    _levelMakerButton->setFixedSize(350, 60);
//    _levelMakerButton->move(150, 340);
    _levelMakerButton->setFlat(true);
    _levelMakerButton->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_tutorialButton);
    mainLayout->addWidget(_playButton);
    mainLayout->addWidget(_levelMakerButton);

    setLayout(mainLayout);

    QString style = "QPushButton:flat { ";
            style += "  color: #FA6900;";
            style += "  text-align: left;";
            style += "}";
            style += "QPushButton:pressed {";
            style += "  text-align: left;";
            style += "  background-color: transparent;";
            style += "  padding-left: 1px;";
            style += "  color: #F38630;";
            style += "}";

    setStyleSheet(style);

    setFixedSize(1000, 756);
}
