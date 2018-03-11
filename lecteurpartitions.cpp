#include "lecteurpartitions.h"
#include "ui_lecteurpartitions.h"
#include <QFileDialog>
#include <QDebug>
#include <QResizeEvent>



LecteurPartitions::LecteurPartitions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LecteurPartitions),
    document(nullptr)
{
    ui->setupUi(this);

}

LecteurPartitions::~LecteurPartitions()
{
    delete ui;
}

void LecteurPartitions::ChargerPartition(QString _cheminPartition)
{
    if(!_cheminPartition.isEmpty())
    {
        QFileInfo info(_cheminPartition);
        parentWidget()->setWindowTitle(info.fileName());
        if(document != nullptr)
            delete document;
        images.clear();
        foreach (QLabel *img, listeLabelImages) {
            delete img;
        }
        listeLabelImages.clear();




        document = Poppler::Document::load(_cheminPartition);
        if (!document || document->isLocked())
        {
          qDebug() << "Erreur lors de l'ouverture du document PDF" ;
          delete document;
        }
        else
        {
            QImage image;

            QSize tailleWidget = ui->scrollArea->size();
            qDebug() << tailleWidget;
            int resolution = tailleWidget.width() * 120 / 1050;

            int i = 0;
            int nbPages=document->numPages();
            images.clear();
            for(i=0; i<nbPages;i++)
            {
                image = document->page(i)->renderToImage(resolution,resolution,0,0,tailleWidget.width()-50);
                images.append(image);
            }

            QLabel *ptrLabelImage;
            foreach (QImage img, images)
            {
                ptrLabelImage = new QLabel;

                listeLabelImages.append(ptrLabelImage);
                ptrLabelImage->setPixmap(QPixmap::fromImage(img));
                ui->verticalLayout->addWidget(ptrLabelImage);
            }
            ui->scrollArea->verticalScrollBar()->setValue(0);

            show();
        }
    }
}

void LecteurPartitions::on_pushButtonCharger_clicked()
{
    QString nomFichier = QFileDialog::getOpenFileName(this,
                           tr("Ouverture d'une partition"),
                           "/home/philippe/Documents/",
                           tr("Partition (*.pdf)"));

    ChargerPartition(nomFichier);

}

void LecteurPartitions::on_pushButtonFermer_clicked()
{
    delete document;
}

void LecteurPartitions::resizeEvent(QResizeEvent *event)
{
    QSize tailleLecteur = event->size() ;
    int largeurLecture = tailleLecteur.width() - 120;
    int hauteurLecture = tailleLecteur.height();
    ui->scrollArea->resize(largeurLecture,hauteurLecture);
}

void LecteurPartitions::on_pushButtonPagePrec_clicked()
{

}

void LecteurPartitions::on_pushButtonPageSuiv_clicked()
{

}

void LecteurPartitions::on_pushButtonMonter_clicked()
{

}

void LecteurPartitions::on_pushButtonDescendre_clicked()
{

}
