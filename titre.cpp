#include "titre.h"

#include <QFileInfo>


Titre::Titre(QObject *parent) : QObject(parent)
{

}

Titre::Titre(const QString &_chemin, const Titre::TYPE _type, QObject *parent):
    QObject(parent)
{
    DonnerChemin(_chemin,_type);
    ExtraireNomDutitre(_chemin);
}

void Titre::DonnerChemin(const QString &_chemin, const Titre::TYPE _type)
{
    switch(_type)
    {
    case AUDIO:
        cheminAudio = _chemin;
        break;
    case PARTITION:
        cheminPartition = _chemin;
        break;
    case ENREGISTREMENT:
        cheminEnregistrement =_chemin;
        break;
    }
    if(nom.isEmpty())
        ExtraireNomDutitre(_chemin);
}

void Titre::DonnerInterprete(const QString &_interprete)
{
    interprete = _interprete;
}

void Titre::DonnerTempo(const quint8 _tempo)
{
    tempo = _tempo;
}

void Titre::DonnerDuree(const QTime &_duree)
{
    duree = _duree;
}

void Titre::DonnerTonalite(const QString &_tonalite)
{
    tonalite = _tonalite;
}

void Titre::DonnerNom(const QString &_nom)
{
    nom = _nom;
}

QString &Titre::ObtenirNom()
{
    return nom;
}

QString &Titre::ObtenirInterprete()
{
    return interprete;
}

QString &Titre::ObtenirAudio()
{
    return cheminAudio;
}

QString &Titre::ObtenirPartition()
{
    return cheminPartition;
}

QString &Titre::ObtenirEnregistrement()
{
    return cheminEnregistrement;
}

QString &Titre::ObtenirTonalite()
{
    return tonalite;
}

quint8 Titre::ObtenirTempo()
{
    return tempo;
}

QTime &Titre::ObtenirDuree()
{
    return duree;
}

bool Titre::RenommerFichier(const QString &_nouveau, const Titre::TYPE _type)
{
    bool retour = false;
    QString chemin;
    switch(_type)
    {
    case AUDIO:
        chemin = cheminAudio;
        break;
    case PARTITION:
        chemin = cheminPartition;
        break;
    case ENREGISTREMENT:
        chemin = cheminEnregistrement;
    }
    if(!chemin.isEmpty())
    {
        QFile fichier(chemin);
        if(fichier.rename(chemin,_nouveau))
            retour = true;
        switch(_type)
        {
        case AUDIO:
            emit AudioChange(chemin);
            cheminAudio = chemin;
            break;
        case PARTITION:
            emit PartitionChange(chemin);
            cheminEnregistrement = chemin;
            break;
        case ENREGISTREMENT:
            emit EnregistrementChange(chemin);
            cheminEnregistrement = chemin;
        }
    }
    return retour;
}

void Titre::EnregisterXML(QXmlStreamWriter &xmlWriter,int _numero)
{
    xmlWriter.writeStartElement("Titre");
    {
        xmlWriter.writeAttribute("id",QString::number(_numero));
        xmlWriter.writeTextElement("Nom",nom);
        xmlWriter.writeTextElement("Interprete",interprete);
        xmlWriter.writeTextElement("Tempo",QString::number(tempo));
        xmlWriter.writeTextElement("Duree",duree.toString("hh:mm:ss"));
        xmlWriter.writeTextElement("Tonalite",tonalite);
        xmlWriter.writeTextElement("Audio",cheminAudio);
        xmlWriter.writeTextElement("Partition",cheminPartition);
        xmlWriter.writeTextElement("Enregistrement",cheminEnregistrement);
    }
    xmlWriter.writeEndElement();
}

bool Titre::LireXML(QXmlStreamReader &xmlReader)
{
    bool retour = false;
    if(xmlReader.isStartElement() && xmlReader.name() == "Titre")
    {
        while(xmlReader.readNextStartElement())
        {
            if(xmlReader.name()=="Nom")
                nom = xmlReader.readElementText();
            else if(xmlReader.name()=="Interprete")
                interprete = xmlReader.readElementText();
            else if(xmlReader.name()=="Tempo")
                tempo = xmlReader.readElementText().toInt();
            else if (xmlReader.name()=="Duree")
                duree = QTime::fromString(xmlReader.readElementText(),"hh:mm:ss");
            else if (xmlReader.name()=="Tonalite")
                tonalite = xmlReader.readElementText();
            else if (xmlReader.name()=="Audio")
                cheminAudio = xmlReader.readElementText();
            else if (xmlReader.name()=="Partition")
                cheminPartition = xmlReader.readElementText();
            else if (xmlReader.name()=="Enregistrement")
                cheminEnregistrement = xmlReader.readElementText();
            else xmlReader.skipCurrentElement();
        }
        retour = true;
    }

    return retour;
}

void Titre::ExtraireNomDutitre(QString _chemin)
{
    QFileInfo info(_chemin);
    QString leNom = info.fileName();
    int indice = leNom.lastIndexOf('.');
    if(indice > 0)
        nom = leNom.left(indice);
}
