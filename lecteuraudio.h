#ifndef LECTEURAUDIO_H
#define LECTEURAUDIO_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QMediaContent>
#include <QUrl>
#include <QTime>
#include <QTimer>

namespace Ui {
class LecteurAudio;
}

class LecteurAudio : public QWidget
{
    Q_OBJECT

public:
    explicit LecteurAudio(QWidget *parent = 0);
    ~LecteurAudio();

private slots:

    void resizeEvent(QResizeEvent * event);



    void on_pushButtonRetour_clicked();

    void On_MediaStatusChanged(QMediaPlayer::MediaStatus _status);
    void On_MediaPlayerStateChanged(QMediaPlayer::State _etat);
    void On_PositionChanged(qint64 _position);
    void On_CurrentMediaChanged(QMediaContent 	_content);
    void On_SliderProgessionMoved(int _valeur);

    void on_pushButtonJalonA_clicked();
    void on_pushButtonJalonB_clicked();
    void on_pushButtonMoinsA_clicked();
    void on_pushButtonPlusA_clicked();
    void on_pushButtonMoinsB_clicked();
    void on_pushButtonPlusB_clicked();



    void on_horizontalSliderVolume_sliderMoved(int position);
    void on_horizontalSliderVitesse_sliderMoved(int valeur);
    void on_pushButtonSon_clicked();

    void on_listWidgetListeLecture_doubleClicked(const QModelIndex &index);

    void on_pushButtonAvance_clicked();

    void on_pushButtonNouvelle_clicked();
    void on_pushButtonAjouter_clicked();
    void on_pushButtonOuvrir_clicked();
    void on_pushButtonEnregistrer_clicked();


    void on_pushButtonAleatoire_clicked();

    void on_pushButtonBoucle_clicked();

    void on_pushButtonSeqentiel_clicked();

    void on_pushButtonMonter_clicked();

    void on_pushButtonDescendre_clicked();

    void on_pushButtonSupprimer_clicked();

    void on_listWidgetListeLecture_currentRowChanged(int currentRow);

    void on_pushButtonLecture_clicked();

    void On_timeoutTimerDuree();

private:
    QString ConvertirTempsEnHeuresMinutesSecondes(qint64 temps);
    QString SupprimerExtension(QString _nomFichier);
    void AfficherPlayList();


    Ui::LecteurAudio *ui;
    enum ETAT
    {
        LECTURE,
        PAUSE,
        ARRET
    };
    enum JALON
    {
        AUCUN,
        JALON_A,
        JALON_B
    };

    ETAT etat;
    JALON jalon;
    qint64 positionJalonA;
    qint64 positionJalonB;

    QMediaPlayer *player;
    QString chanson;
    QMediaPlaylist *playlist;
    QTimer timerDuree;

    bool nouvelleListe ;
};

#endif // LECTEURAUDIO_H
