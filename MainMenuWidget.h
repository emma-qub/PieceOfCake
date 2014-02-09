#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QtWidgets>

class MainMenuWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainMenuWidget(QWidget *parent = 0);

private:
    QColor color;
    QPushButton* _tutorialButton;
    QPushButton* _playButton;
    QPushButton* _levelMakerButton;
};

#endif // MAINMENUWIDGET_H
