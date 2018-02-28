#include "assistant.h"
#include "ui_assistant.h"
#include <QDebug>
#include <QEvent>

Assistant::Assistant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Assistant),
    tailleFenetre(QSize(0,0))
{
    ui->setupUi(this);
    showMaximized();
    audio = new LecteurAudio(this);
    ui->tabWidgetApplication->setIconSize(QSize(50,50));
    //ui->tabWidgetApplication->addTab(audio,QIcon(":/images/audio.png"),nullptr);
    ui->tabWidgetApplication->addTab(audio,QIcon(":/images/icon-audio.png"),nullptr);
    //ui->tabWidgetApplication->addTab(audio,QIcon(":/images/icon-partition.png"),nullptr);


    ui->tabWidgetApplication->setTabText(2,"Partition");
}

Assistant::~Assistant()
{
    delete ui;
}

void Assistant::resizeEvent(QResizeEvent *event)
{
    tailleFenetre = size();
    int largeur = tailleFenetre.width();
    int hauteur = tailleFenetre.height();

    int grilleX = largeur/12;
    //int grilleY = hauteur/12;

    ui->tabWidgetApplication->resize(10*grilleX, hauteur);


}
