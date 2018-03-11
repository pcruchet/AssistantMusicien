#ifndef GESTIONNAIRELISTES_H
#define GESTIONNAIRELISTES_H

#include <QWidget>
#include <QStringList>
#include "titre.h"
#include "lecteuraudio.h"


namespace Ui {
class GestionnaireListes;
}

class GestionnaireListes : public QWidget
{
    Q_OBJECT

public:
    explicit GestionnaireListes(LecteurAudio *_lecteurAudio, QWidget *parent = 0);
    ~GestionnaireListes();

private slots:

    void resizeEvent(QResizeEvent * event);
    /*-- Concert --*/
    void on_pushButtonNouveauConcert_clicked();
    void on_pushButtonAjouter_clicked();

    void on_pushButtonEnregistrer_clicked();
    void on_pushButtonMonter_clicked();
    void on_pushButtonDescendre_clicked();
    void on_pushButtonSupprimer_clicked();

    void on_pushButtonSauvegarder_clicked();
    void on_pushButtonOuvrirTitre_clicked();
    void on_pushButtonModifierAudio_clicked();

    void on_pushButtonModifierPartition_clicked();
    void on_pushButtonModifierEnregistrement_clicked();
    void on_pushButtonNouveau_clicked();

    void on_listWidgetListe_clicked(const QModelIndex &index);

    void on_pushButtonLancerAudio_clicked();

    void on_pushButtonLancerPartition_clicked();

    void on_pushButtonLancerEnregistrement_clicked();

    void on_pushButtonModifier_clicked();

    void on_pushButtonRenommer_clicked();

    void on_pushButtonAjouterListe_clicked();

    void on_pushButtonLancerSetAudio_clicked();

    void on_pushButtonLancerSetPartition_clicked();

    void on_pushButtonLancerSetEnregistrement_clicked();

    void on_listWidgetListe_currentRowChanged(int currentRow);



    void on_lineEditAudio_textChanged(const QString &arg1);

    void on_lineEditPartition_textChanged(const QString &arg1);

    void on_lineEditEnregistrement_textChanged(const QString &arg1);



    void on_lineEditInterprete_editingFinished();

    void on_lineEditNom_editingFinished();

    void on_pushButtonOuvrir_clicked();

private:
    void AfficherListePartitions();
    QString SupprimerExtension(QString &_nomFichier);
    void AfficherTitre(Titre *_leTitre);
    void InitialiserTitre();
    void AjouterTire();
    void AfficherListeConcert(int _courant=-1);

    LecteurAudio *lecteurAudio;
    Ui::GestionnaireListes *ui;
    bool nouvelleListe;
    QStringList lesPartitions;
    Titre *leTitre;
    QList <Titre *> leConcert;


};

#endif // GESTIONNAIRELISTES_H
