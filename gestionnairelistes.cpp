#include "gestionnairelistes.h"
#include "ui_gestionnairelistes.h"

#include <QFileDialog>
#include <QTextStream>
#include <QResizeEvent>
#include <QDebug>

GestionnaireListes::GestionnaireListes(LecteurAudio *_lecteurAudio, QWidget *parent) :
    QWidget(parent),
    lecteurAudio(_lecteurAudio),
    ui(new Ui::GestionnaireListes),
    nouvelleListe(false),
    leTitre(nullptr)
{
    ui->setupUi(this);

    InitialiserTitre();
    ui->pushButtonAjouter->setDisabled(true);
    ui->pushButtonEnregistrer->setDisabled(true);
    ui->groupBoxOrganisation->setDisabled(true);
    ui->groupBox->setDisabled(true);
}

GestionnaireListes::~GestionnaireListes()
{
    delete ui;
}

void GestionnaireListes::resizeEvent(QResizeEvent *event)
{
    QSize tailleOrganiseur = event->size() ;
    QRect  rect = ui->groupBoxOrganisation->geometry();
    int largeurOrganisation = tailleOrganiseur.width() - rect.left() - 30;
    int hauteurOrganisation = rect.height();
    ui->groupBoxOrganisation->resize(largeurOrganisation,hauteurOrganisation);
    ui->listWidgetListe->resize(largeurOrganisation-140,hauteurOrganisation-90);
}

/*---------------------- Concert ------------------------------------*/


void GestionnaireListes::on_pushButtonNouveauConcert_clicked()
{
    ui->lineEditListe->setText("Concert...");
    ui->groupBoxOrganisation->setDisabled(true);
    ui->listWidgetListe->clear();
    leConcert.clear();
    ui->pushButtonAjouter->setEnabled(true);
    ui->pushButtonEnregistrer->setDisabled(true);
    ui->lineEditNbTitres->setText("0");
    ui->groupBox->setEnabled(true);
    InitialiserTitre();
}

void GestionnaireListes::on_pushButtonAjouter_clicked()
{ 
    QString fichier = QFileDialog::getOpenFileName(this,
                                                   "Ajout de titres dans le set",
                                                   "/home/philippe/Documents/",
                                                   "set (*.xml *.XML)");
    if(!fichier.isEmpty())
    {
        QFile file(fichier);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug()<< "erreur ouverture fichier concert XML";
        else
        {
            file.seek(0);
            QXmlStreamReader xml(&file);

            int memo = leConcert.count();
            while (xml.readNextStartElement())
            {
                leTitre = new Titre;
                if(leTitre->LireXML(xml))
                {
                    leConcert.append(leTitre);
                }
                else
                {
                    delete leTitre;
                }
            }
            AfficherListeConcert(memo);
            ui->pushButtonEnregistrer->setEnabled(true);
        }
        file.close();
    }
}




void GestionnaireListes::on_pushButtonEnregistrer_clicked()
{
    if(leConcert.count()>0)
    {
        QString nomFichier = QFileDialog::getSaveFileName(this,
                                                          "Enregistrement d'un programme de concert",
                                                          "/home/philippe/Documents/",
                                                          "Set (*.set *.SET)");

        QFile file(nomFichier);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            qDebug()<< "erreur ouverture fichier concert SET";
        else
        {
            int index = 1;
            QXmlStreamWriter xmlWriter(&file);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();
            {
                xmlWriter.writeStartElement("Concert");
                foreach (Titre *titre, leConcert) {
                    titre->EnregisterXML(xmlWriter,index++);
                }
                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndDocument();
            file.close();
            ui->lineEditListe->setText(SupprimerExtension(nomFichier));
        }
    }

}

void GestionnaireListes::on_pushButtonMonter_clicked()
{
    int index = ui->listWidgetListe->currentIndex().row();
    leConcert.move(index, index-1);
    leTitre = leConcert.at(index-1);
    AfficherListeConcert(index-1);
}

void GestionnaireListes::on_pushButtonDescendre_clicked()
{
    int index = ui->listWidgetListe->currentIndex().row();
    leConcert.move(index, index+1);
    leTitre = leConcert.at(index+1);
    AfficherListeConcert(index+1);
}

void GestionnaireListes::on_pushButtonSupprimer_clicked()
{
    int position = ui->listWidgetListe->currentRow();
    leConcert.removeAt(position);
    if(position == 0 && leConcert.count() > 0)
        position = 1;
    AfficherListeConcert(position-1);
    if(leConcert.isEmpty())
        InitialiserTitre();
}

QString GestionnaireListes::SupprimerExtension(QString &_nomFichier)
{
    QFileInfo info(_nomFichier);
    QString nom = info.fileName();
    int indice = nom.lastIndexOf('.');
    if(indice > 0)
        nom = nom.left(indice);
    return nom;
}

void GestionnaireListes::AfficherTitre(Titre *_leTitre)
{
    ui->lineEditNom->setText(_leTitre->ObtenirNom());
    ui->lineEditInterprete->setText(_leTitre->ObtenirInterprete());
    ui->lineEditDuree->setText(_leTitre->ObtenirDuree().toString("hh:mm:ss"));
    ui->lineEditTempo->setText(QString::number(_leTitre->ObtenirTempo()));
    ui->lineEditTonalite->setText(_leTitre->ObtenirTonalite());
    ui->lineEditAudio->setText(_leTitre->ObtenirAudio());
    ui->lineEditPartition->setText(_leTitre->ObtenirPartition());
    ui->lineEditEnregistrement->setText(_leTitre->ObtenirEnregistrement());
    ui->pushButtonAjouterListe->setEnabled(true);
    ui->pushButtonRenommer->setEnabled(true);
    ui->pushButtonSauvegarder->setEnabled(true);
    ui->pushButtonModifier->setEnabled(true);
}

void GestionnaireListes::InitialiserTitre()
{
    ui->lineEditNom->clear();
    ui->lineEditNom->setDisabled(true);
    ui->lineEditInterprete->clear();
    ui->lineEditInterprete->setDisabled(true);
    ui->lineEditDuree->clear();
    ui->lineEditDuree->setDisabled(true);
    ui->lineEditTempo->clear();
    ui->lineEditTempo->setDisabled(true);
    ui->lineEditTonalite->clear();
    ui->lineEditTonalite->setDisabled(true);
    ui->lineEditAudio->clear();
    ui->lineEditPartition->clear();
    ui->lineEditEnregistrement->clear();
    ui->pushButtonLancerEnregistrement->setDisabled(true);
    ui->pushButtonLancerPartition->setDisabled(true);
    ui->pushButtonLancerAudio->setDisabled(true);
    ui->pushButtonAjouterListe->setDisabled(true);
    ui->pushButtonRenommer->setDisabled(true);
    ui->pushButtonSauvegarder->setDisabled(true);
    ui->pushButtonOuvrirTitre->setEnabled(true);
    ui->pushButtonModifier->setDisabled(true);
    ui->pushButtonModifierAudio->setDisabled(true);
    ui->pushButtonModifierEnregistrement->setDisabled(true);
    ui->pushButtonModifierPartition->setDisabled(true);
    ui->pushButtonLancerAudio->setDisabled(true);
    ui->pushButtonLancerPartition->setDisabled(true);
    ui->pushButtonLancerEnregistrement->setDisabled(true);
}

void GestionnaireListes::AjouterTire()
{
    leConcert.append(leTitre);
    AfficherListeConcert(leConcert.count()-1);
}

void GestionnaireListes::AfficherListeConcert(int _courant)
{
    ui->listWidgetListe->clear();
    QString nom;
    int index = 1;
    if(leConcert.count())
    {
        foreach (Titre *titre, leConcert)
        {
            nom.sprintf(" (%02d)  ",index++);
            nom += titre->ObtenirNom();
            ui->listWidgetListe->addItem(nom);
        }
        ui->groupBoxOrganisation->setEnabled(true);
    }
    ui->lineEditNbTitres->setText(QString::number(index-1));
    if(_courant >= 0)
    {
        ui->listWidgetListe->setCurrentRow(_courant);
        AfficherTitre(leConcert.at(_courant));
    }
}

void GestionnaireListes::on_pushButtonSauvegarder_clicked()
{
    leTitre->DonnerDuree(QTime::fromString(ui->lineEditDuree->text(),"hh:mm:ss"));
    leTitre->DonnerInterprete(ui->lineEditInterprete->text());
    leTitre->DonnerTempo(ui->lineEditTempo->text().toInt());
    leTitre->DonnerTonalite(ui->lineEditTonalite->text());


    QString nomParDefaut = "/home/philippe/Documents/";
    nomParDefaut += leTitre->ObtenirInterprete() + " - ";
    nomParDefaut += leTitre->ObtenirNom() + ".xml";
            QString nomFichier = QFileDialog::getSaveFileName(this,
                                                              "Enregistrement d'un programme de concert",
                                                              nomParDefaut,
                                                              "Set (*.xml *.XML)");

    QFile file(nomFichier);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug()<< "erreur ouverture fichier concert XML";
    else
    {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        {

            leTitre->EnregisterXML(xmlWriter,1);

        }
        xmlWriter.writeEndDocument();
        file.close();
    }

}

void GestionnaireListes::on_pushButtonOuvrirTitre_clicked()
{

    QString fichier = QFileDialog::getOpenFileName(this,
                                                   "Ouverture d'un programme de concert",
                                                   "/home/philippe/Documents/",
                                                   "set (*.xml *.XML)");
    if(!fichier.isEmpty())
    {
        QFile file(fichier);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug()<< "erreur ouverture fichier concert XML";
        else
        {
            file.seek(0);
            QXmlStreamReader xml(&file);
            if(xml.readNextStartElement())
            {
                leTitre = new Titre;
                if(leTitre->LireXML(xml))
                {
                    AfficherTitre(leTitre);
                    ui->pushButtonModifier->setEnabled(true);
                    ui->pushButtonModifierAudio->setEnabled(true);
                    ui->pushButtonModifierEnregistrement->setEnabled(true);
                    ui->pushButtonModifierPartition->setEnabled(true);
                    ui->pushButtonAjouterListe->setEnabled(true);

                }
                else
                {
                    delete leTitre;
                }


            }
            file.close();
        }
    }
}

void GestionnaireListes::on_pushButtonModifierAudio_clicked()
{
    QString fichier = QFileDialog::getOpenFileName(this,
                                                   "Modification du fichier audio",
                                                   "/home/philippe/Documents/",
                                                   "Fichier Audio (*.mp3)");
    if(!fichier.isEmpty())
    {
        ui->lineEditAudio->setText(fichier);
        if(leTitre == nullptr)
        {
            leTitre = new Titre(fichier,Titre::AUDIO);
        }
        else
        {
            leTitre->DonnerChemin(fichier,Titre::AUDIO);
        }
        if(ui->lineEditNom->text().isEmpty())
            ui->lineEditNom->setText(leTitre->ObtenirNom());
        ui->pushButtonAjouterListe->setEnabled(true);
        ui->pushButtonLancerAudio->setEnabled(true);
        ui->pushButtonModifier->setEnabled(true);
        ui->pushButtonSauvegarder->setEnabled(true);
        ui->pushButtonAjouterListe->setEnabled(true);
    }
}


void GestionnaireListes::on_pushButtonModifierPartition_clicked()
{
    QString fichier = QFileDialog::getOpenFileName(this,
                                                   "Chargement d'une liste de partition",
                                                   "/home/philippe/Documents/",
                                                   "Fichier Partition (*.pdf)");
    if(!fichier.isEmpty())
    {
        ui->lineEditPartition->setText(fichier);
        if(leTitre == nullptr)
        {
            leTitre = new Titre(fichier,Titre::PARTITION);
            ui->lineEditNom->setText(leTitre->ObtenirNom());
        }
        else
        {
            leTitre->DonnerChemin(fichier,Titre::PARTITION);
        }
        if(ui->lineEditNom->text().isEmpty())
            ui->lineEditNom->setText(leTitre->ObtenirNom());
        ui->pushButtonAjouterListe->setEnabled(true);
        ui->pushButtonLancerAudio->setEnabled(true);
        ui->pushButtonModifier->setEnabled(true);
        ui->pushButtonSauvegarder->setEnabled(true);
        ui->pushButtonAjouterListe->setEnabled(true);
    }
}



void GestionnaireListes::on_pushButtonModifierEnregistrement_clicked()
{
    QString fichier = QFileDialog::getOpenFileName(this,
                                                   "Modification du fichier enregistré",
                                                   "/home/philippe/Documents/",
                                                   "Fichier Audio (*.mp3)");
    if(!fichier.isEmpty())
    {
        ui->lineEditAudio->setText(fichier);
        if(leTitre == nullptr)
        {
            leTitre = new Titre(fichier,Titre::ENREGISTREMENT);
        }
        else
        {
            leTitre->DonnerChemin(fichier,Titre::ENREGISTREMENT);
        }
        if(ui->lineEditNom->text().isEmpty())
            ui->lineEditNom->setText(leTitre->ObtenirNom());
        ui->pushButtonAjouterListe->setEnabled(true);
        ui->pushButtonLancerAudio->setEnabled(true);
        ui->pushButtonModifier->setEnabled(true);
        ui->pushButtonSauvegarder->setEnabled(true);
        ui->pushButtonAjouterListe->setEnabled(true);
    }
}



void GestionnaireListes::on_pushButtonNouveau_clicked()
{
    if(leTitre != nullptr)
    {
        delete leTitre;
        leTitre = nullptr;
    }
    leTitre = new Titre;

    InitialiserTitre();
    ui->pushButtonModifierAudio->setEnabled(true);
    ui->pushButtonModifierEnregistrement->setEnabled(true);
    ui->pushButtonModifierPartition->setEnabled(true);

}

void GestionnaireListes::on_listWidgetListe_clicked(const QModelIndex &index)
{
    if(leTitre == nullptr)
        leTitre = new Titre;
    leTitre = leConcert.at(index.row());
    AfficherTitre(leTitre);
}

void GestionnaireListes::on_pushButtonLancerAudio_clicked()
{
    QStringList liste;
    liste.append(ui->lineEditAudio->text());
    lecteurAudio->LancerChanson(liste,true);
}

void GestionnaireListes::on_pushButtonLancerPartition_clicked()
{

}

void GestionnaireListes::on_pushButtonLancerEnregistrement_clicked()
{

}

void GestionnaireListes::on_pushButtonModifier_clicked()
{
    ui->lineEditNom->setEnabled(true);
    ui->lineEditInterprete->setEnabled(true);
    ui->lineEditTempo->setEnabled(true);
    ui->lineEditDuree->setEnabled(true);
    ui->lineEditTonalite->setEnabled(true);
    ui->pushButtonModifierAudio->setEnabled(true);
    ui->pushButtonModifierEnregistrement->setEnabled(true);
    ui->pushButtonModifierPartition->setEnabled(true);

}

void GestionnaireListes::on_pushButtonRenommer_clicked()
{

}

void GestionnaireListes::on_pushButtonAjouterListe_clicked()
{
    leConcert.append(leTitre);
    AfficherListeConcert(leConcert.count()-1);
}

void GestionnaireListes::on_pushButtonLancerSetAudio_clicked()
{
    QStringList liste;
    foreach (Titre *titre, leConcert)
    {
        liste.append(titre->ObtenirAudio());
    }
    lecteurAudio->LancerChanson(liste,true);
}

void GestionnaireListes::on_pushButtonLancerSetPartition_clicked()
{

}

void GestionnaireListes::on_pushButtonLancerSetEnregistrement_clicked()
{

}

void GestionnaireListes::on_listWidgetListe_currentRowChanged(int currentRow)
{
    if(currentRow == 0)
        ui->pushButtonMonter->setDisabled(true);
    else
        ui->pushButtonMonter->setEnabled(true);
    if(currentRow == ui->listWidgetListe->count()-1)
        ui->pushButtonDescendre->setDisabled(true);
    else
        ui->pushButtonDescendre->setEnabled(true);
}


void GestionnaireListes::on_lineEditAudio_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->pushButtonLancerAudio->setDisabled(true);

    }
    else
    {
        ui->pushButtonLancerAudio->setEnabled(true);

    }

}

void GestionnaireListes::on_lineEditPartition_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->pushButtonLancerPartition->setDisabled(true);
    }
    else
    {
        ui->pushButtonLancerPartition->setEnabled(true);
        leTitre->DonnerChemin(arg1,Titre::PARTITION);
    }
}

void GestionnaireListes::on_lineEditEnregistrement_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->pushButtonLancerEnregistrement->setDisabled(true);
    }
    else
    {
        ui->pushButtonLancerEnregistrement->setEnabled(true);
        leTitre->DonnerChemin(arg1,Titre::ENREGISTREMENT);
    }
}


void GestionnaireListes::on_lineEditInterprete_editingFinished()
{
    QString texte = ui->lineEditInterprete->text();
    if(!texte.isEmpty())
        leTitre->DonnerInterprete(texte);
}

void GestionnaireListes::on_lineEditNom_editingFinished()
{
    QString texte = ui->lineEditNom->text();
    if(!texte.isEmpty())
    {
        leTitre->DonnerNom(texte);
    }
    AfficherListeConcert(ui->listWidgetListe->currentRow());
}

void GestionnaireListes::on_pushButtonOuvrir_clicked()
{
    QString fichier = QFileDialog::getOpenFileName(this,
                                                   "Ajout de titres dans le set",
                                                   "/home/philippe/Documents/",
                                                   "set (*.set *.SET)");
    if(!fichier.isEmpty())
    {
        QFile file(fichier);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug()<< "erreur ouverture fichier concert set";
        else
        {
            file.seek(0);
            QXmlStreamReader xml(&file);

            leConcert.clear();
            if(xml.readNextStartElement())
            {
                if(xml.isStartElement() && xml.name() == "Concert")
                {
                    while (xml.readNextStartElement())
                    {
                        leTitre = new Titre;
                        if(leTitre->LireXML(xml))
                        {
                            leConcert.append(leTitre);
                        }
                        else
                        {
                            delete leTitre;
                        }
                    }
                }
            }
            AfficherListeConcert(0);
            ui->pushButtonEnregistrer->setEnabled(true);
            ui->pushButtonAjouter->setEnabled(true);
            ui->groupBox->setEnabled(true);
        }
        file.close();
    }
}
