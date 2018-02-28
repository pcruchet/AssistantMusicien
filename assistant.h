#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QWidget>
#include <QEvent>

#include "lecteuraudio.h"

namespace Ui {
class Assistant;
}

class Assistant : public QWidget
{
    Q_OBJECT

public:
    explicit Assistant(QWidget *parent = 0);
    ~Assistant();

private slots:
    void resizeEvent(QResizeEvent * event);


private:
    Ui::Assistant *ui;
    QSize tailleFenetre;        // taille du widget
    LecteurAudio *audio;         // Widget Lecteur audio
};

#endif // ASSISTANT_H
