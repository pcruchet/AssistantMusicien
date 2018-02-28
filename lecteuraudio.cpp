#include "lecteuraudio.h"
#include "ui_lecteuraudio.h"

#include <QMessageBox>



LecteurAudio::LecteurAudio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LecteurAudio),
    etat(LECTURE),
    jalon(AUCUN),
    positionJalonA(0),
    positionJalonB(0),
    nouvelleListe(false)

{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    if(!connect(player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(On_MediaStatusChanged(QMediaPlayer::MediaStatus))))
        qDebug() << "erreur connexion SIGNAL(mediaStatusChanged) dans LecteurAudio";
    if(!connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(On_MediaPlayerStateChanged(QMediaPlayer::State))))
        qDebug() << "erreur connexion SIGNAL(stateChanged) dans LecteurAudio";
    if(!connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(On_PositionChanged(qint64))))
        qDebug() << "erreur connexion SIGNAL(positionChanged) dans LecteurAudio";
    if(!connect(ui->horizontalSliderProgression,SIGNAL(sliderMoved(int)),this,SLOT(On_SliderProgessionMoved(int))))
        qDebug() << "erreur connexion SIGNAL(valueChanged - Progression) dans LecteurAudio";
    connect(player,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(On_CurrentMediaChanged(QMediaContent)));

    connect(ui->pushButtonStop,SIGNAL(clicked(bool)),player,SLOT(stop()));

    ui->groupBoxBoucle->setDisabled(true);
    ui->pushButtonJalonB->setDisabled(true);
    ui->groupBoxDebut->setDisabled(true);
    ui->groupBoxFin->setDisabled(true);
    ui->pushButtonAjouter->setDisabled(true);
    ui->pushButtonEnregistrer->setDisabled(true);
    ui->groupBoxCommandes->setDisabled(true);
    ui->groupBoxOrdre->setDisabled(true);

    player->setVolume(50);
    ui->horizontalSliderVolume->setValue(50);
    ui->lineEditVitesse->setText("x 1.00");

    playlist = new QMediaPlaylist(player);
    player->setPlaylist(playlist);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    on_pushButtonBoucle_clicked();

    ui->lineEditListeLecture->setText("Liste de lecture...");
    timerDuree.setSingleShot(true);
    connect(&timerDuree,SIGNAL(timeout()),this,SLOT(On_timeoutTimerDuree()));
}

LecteurAudio::~LecteurAudio()
{
    delete ui;
    delete player;
    delete playlist;
}

void LecteurAudio::resizeEvent(QResizeEvent *event)
{
    QSize tailleLecteurAudio = size() ;
    QSize tailleBoucle = ui->groupBoxBoucle->size();
    int largeurListeLecture = tailleLecteurAudio.width() - tailleBoucle.width() - 175;
    int hauteurListeLecture = 820 ;
    ui->listWidgetListeLecture->resize(largeurListeLecture,hauteurListeLecture);
}

void LecteurAudio::on_pushButtonLecture_clicked()
{
    if(player->state() != QMediaPlayer::PlayingState)
    {
        int index1 = ui->listWidgetListeLecture->currentRow();
        int index2 = playlist->currentIndex();
        if(index1 != index2)
        {
            playlist->setCurrentIndex(index1);
        }
        QMediaContent media = playlist->currentMedia();
        QString titre = media.canonicalUrl().fileName();
        ui->lineEditTitre->setText(SupprimerExtension(titre));
        player->play();
        timerDuree.start(1000);


    }
}

void LecteurAudio::On_timeoutTimerDuree()
{
    qint64 temps=player->duration()/1000;
    if(temps > 0)
    {
        ui->labelDuree->setText(ConvertirTempsEnHeuresMinutesSecondes(temps));
        ui->horizontalSliderProgression->setMaximum(temps);
    }
}

void LecteurAudio::On_MediaStatusChanged(QMediaPlayer::MediaStatus _status)
{
    On_timeoutTimerDuree();
}

void LecteurAudio::On_MediaPlayerStateChanged(QMediaPlayer::State _etat)
{ 
    int index = playlist->currentIndex();

    switch(_etat)
    {
    case QMediaPlayer::PlayingState:
        ui->pushButtonLecture->setIcon(QIcon(":/images/Actions-media-playback-pause-icon.png"));
        ui->groupBoxBoucle->setEnabled(true);
        disconnect(ui->pushButtonLecture,SIGNAL(clicked(bool)),player,SLOT(play()));
        connect(ui->pushButtonLecture,SIGNAL(clicked(bool)),player,SLOT(pause()));
        ui->listWidgetListeLecture->setCurrentRow(index);
        break;

    case QMediaPlayer::PausedState:
        ui->pushButtonLecture->setIcon(QIcon(":/images/Actions-media-playback-start-icon.png"));
        disconnect(ui->pushButtonLecture,SIGNAL(clicked(bool)),player,SLOT(pause()));
        connect(ui->pushButtonLecture,SIGNAL(clicked(bool)),player,SLOT(play()));
        break;

    case QMediaPlayer::StoppedState:
        ui->pushButtonLecture->setIcon(QIcon(":/images/Actions-media-playback-start-icon.png"));
        disconnect(ui->pushButtonLecture,SIGNAL(clicked(bool)),player,SLOT(pause()));
        connect(ui->pushButtonLecture,SIGNAL(clicked(bool)),player,SLOT(play()));
        ui->lineEditTempsA->clear();
        ui->lineEditTempsB->clear();
        ui->groupBoxDebut->setDisabled(true);
        ui->groupBoxFin->setDisabled(true);
        ui->pushButtonJalonB->setDisabled(true);
        ui->groupBoxBoucle->setDisabled(true);
        positionJalonA = 0;
        positionJalonB = 0;
        break;
    default:
        break;
    }
}

void LecteurAudio::On_PositionChanged(qint64 _position)
{
    _position /= 1000;
    ui->labelCourant->setText(ConvertirTempsEnHeuresMinutesSecondes(_position));
    ui->horizontalSliderProgression->setValue(_position);
    if(positionJalonB != 0 && _position >= positionJalonB)
    {
        ui->horizontalSliderProgression->setSliderPosition(positionJalonA);
        player->setPosition(positionJalonA*1000);
    }
}

void LecteurAudio::On_CurrentMediaChanged(QMediaContent _content)
{
    QString titre = _content.canonicalUrl().fileName();
    ui->lineEditTitre->setText(SupprimerExtension(titre));
    int index = playlist->currentIndex();
    ui->listWidgetListeLecture->setCurrentRow(index);
}

void LecteurAudio::On_SliderProgessionMoved(int _valeur)
{
    player->setPosition(_valeur*1000);
}

QString LecteurAudio::ConvertirTempsEnHeuresMinutesSecondes(qint64 temps)
{
    QString retour;
    qint64 heure=temps / 3600;
    qint64 minute=((temps % 3600)/60);
    qint64 seconde=((temps%3600)%60);
    QTime time(heure,minute,seconde);
    if(heure>0)
        retour = time.toString();
    else
        retour = time.toString("mm:ss");

    return retour;
}

/**
 * @brief LecteurAudio::SupprimerExtension
 * @param _nomFichier   Nom d'un fichier Url fromLocalHost
 * @return              Nom du fichier sans extension
 * @details             Ne conserve que le nom du fichier, sans chemin ni extension
 */
QString LecteurAudio::SupprimerExtension(QString _nomFichier)
{
    QFileInfo info(_nomFichier);
    QString nom = info.fileName();
    int indice = nom.lastIndexOf('.');
    if(indice > 0)
        nom = nom.left(indice);
    return nom;
}

void LecteurAudio::AfficherPlayList()
{
    QMediaContent media;
    int nbMedia = playlist->mediaCount();
    QString titre;
    ui->listWidgetListeLecture->clear();
    for(int index = 0 ; index < nbMedia ; index++)
    {
        media = playlist->media(index);
        titre.sprintf(" (%02d)  ",index +1);
        titre += SupprimerExtension(media.canonicalUrl().toString());
        ui->listWidgetListeLecture->addItem(titre);
    }
    ui->listWidgetListeLecture->setCurrentRow(playlist->currentIndex() != -1 ? playlist->currentIndex() : 0);
}


void LecteurAudio::on_pushButtonJalonA_clicked()
{
    if(positionJalonA == 0)
    {
        positionJalonA = player->position()/1000;
        ui->pushButtonJalonB->setEnabled(true);
        ui->groupBoxDebut->setEnabled(true);
        ui->lineEditTempsA->setText(ConvertirTempsEnHeuresMinutesSecondes(positionJalonA));
    }
    else
    {
        positionJalonA = 0;
        ui->groupBoxDebut->setDisabled(true);
        ui->lineEditTempsA->clear();
    }

}

void LecteurAudio::on_pushButtonJalonB_clicked()
{
    if(positionJalonB == 0)
    {
        positionJalonB = player->position()/1000;
        ui->groupBoxFin->setEnabled(true);
        ui->lineEditTempsB->setText(ConvertirTempsEnHeuresMinutesSecondes(positionJalonB));
    }
    else
    {
        positionJalonB = 0;
        ui->pushButtonJalonB->setDisabled(true);
        ui->groupBoxFin->setDisabled(true);
        ui->lineEditTempsB->clear();
    }

}

void LecteurAudio::on_pushButtonMoinsA_clicked()
{
    positionJalonA -=1;
    ui->lineEditTempsA->setText(ConvertirTempsEnHeuresMinutesSecondes(positionJalonA));
}

void LecteurAudio::on_pushButtonPlusA_clicked()
{
    positionJalonA +=1;
    ui->lineEditTempsA->setText(ConvertirTempsEnHeuresMinutesSecondes(positionJalonA));
}

void LecteurAudio::on_pushButtonMoinsB_clicked()
{
    positionJalonB -=1;
    ui->lineEditTempsB->setText(ConvertirTempsEnHeuresMinutesSecondes(positionJalonB));
}

void LecteurAudio::on_pushButtonPlusB_clicked()
{
    positionJalonB +=1;
    ui->lineEditTempsB->setText(ConvertirTempsEnHeuresMinutesSecondes(positionJalonB));
}


void LecteurAudio::on_pushButtonRetour_clicked()
{
    if(positionJalonA != 0)
        player->setPosition(positionJalonA*1000);
    else
        playlist->previous();

}

void LecteurAudio::on_horizontalSliderVolume_sliderMoved(int position)
{
    player->setVolume(position);
}

void LecteurAudio::on_horizontalSliderVitesse_sliderMoved(int valeur)
{
    qreal vitesse = 1.0  + (valeur - 50)/100.0;
    ui->lineEditVitesse->setText("x " + QString::number(vitesse,'g',3));
    player->setPlaybackRate(vitesse);
    qint64  position = player->position();
    player->stop();
    player->setPosition(position);
    player->play();
}

void LecteurAudio::on_pushButtonSon_clicked()
{
    if(!player->isMuted())
    {
        player->setMuted(true);
        ui->pushButtonSon->setIcon(QIcon(":/images/sound-On.png"));
    }
    else
    {
        player->setMuted(false);
        ui->pushButtonSon->setIcon(QIcon(":/images/sound-Off.png"));
    }
}


void LecteurAudio::on_listWidgetListeLecture_doubleClicked(const QModelIndex &index)
{
    playlist->setCurrentIndex(index.row());
    player->play();
}

void LecteurAudio::on_pushButtonAvance_clicked()
{
    playlist->next();
}

/*------------------ Gestion de la PlayList ------------------------------------------*/

void LecteurAudio::on_pushButtonNouvelle_clicked()
{
    ui->lineEditListeLecture->setText("Liste de lecture...");
    nouvelleListe = true ;
    on_pushButtonAjouter_clicked();
    nouvelleListe = false ;
}

void LecteurAudio::on_pushButtonAjouter_clicked()
{

    QFileDialog fileDialog(this,
                           "Ouverture d'un fichier audio",
                           "/home/philippe/Documents/",
                           tr("audio(*.mp3 *.wav *.ogg)"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    if( fileDialog.exec())
    {
        if(nouvelleListe)
        {
            playlist->clear();
            ui->listWidgetListeLecture->clear();
        }
        const QStringList listChanson = fileDialog.selectedFiles();
        QList<QMediaContent> musiques;

        for(const QString& chanson : listChanson)
            musiques.push_back(QUrl::fromLocalFile(chanson));

        ui->groupBoxCommandes->setEnabled(true);
        ui->groupBoxOrdre->setEnabled(true);
        playlist->addMedia(musiques);
        AfficherPlayList();

        ui->pushButtonAjouter->setEnabled(true);
        ui->pushButtonEnregistrer->setEnabled(true);
        on_pushButtonLecture_clicked();
    }
    else
    {
        ui->pushButtonAjouter->setDisabled(true);
        ui->pushButtonEnregistrer->setDisabled(true);

    }
}

void LecteurAudio::on_pushButtonEnregistrer_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(this,
                                                   "Enregistrement de la liste de lecture",
                                                   "/home/philippe/Documents/",
                                                   "Playlist (*.lst)");
    if(!fichier.isEmpty())
    {
        QFile fic(fichier);

        if(fic.open(QIODevice::WriteOnly))
        {
            QTextStream flux(&fic);
            int nb = playlist->mediaCount();
            QMediaContent content;
            for(int i= 0 ; i < nb ; i++)
            {
                content = playlist->media(i);
                flux << content.canonicalUrl().toLocalFile() <<endl;
            }
            fic.close();
        }
        ui->lineEditListeLecture->setText(SupprimerExtension(fichier));
    }
}

void LecteurAudio::on_pushButtonOuvrir_clicked()
{
    QString fichier = QFileDialog::getOpenFileName(this,
                                                   "Chargement de la liste de lecture",
                                                   "/home/philippe/Documents/",
                                                   "Playlist (*.lst)");
    if(!fichier.isEmpty())
    {
        playlist->clear();
        ui->listWidgetListeLecture->clear();
        QFile fic(fichier);

        if(fic.open(QIODevice::ReadOnly))
        {
            QString chanson;
            QList<QMediaContent> musiques;
            QTextStream flux(&fic);
            while(!flux.atEnd())
            {
                chanson = flux.readLine();
                musiques.push_back(QUrl::fromLocalFile(chanson));
            }
            playlist->addMedia(musiques);
            AfficherPlayList();


            on_pushButtonLecture_clicked();
            fic.close();
            ui->groupBoxCommandes->setEnabled(true);
            ui->groupBoxOrdre->setEnabled(true);
            ui->lineEditListeLecture->setText(SupprimerExtension(fichier));
            ui->pushButtonEnregistrer->setEnabled(true);
            ui->pushButtonAjouter->setEnabled(true);
        }

    }
    else
    {
        if(ui->listWidgetListeLecture->count() <= 0)
        {
            ui->pushButtonEnregistrer->setDisabled(true);
            ui->pushButtonAjouter->setDisabled(true);
            ui->groupBoxCommandes->setDisabled(true);
            ui->groupBoxOrdre->setDisabled(true);
        }
    }
}




void LecteurAudio::on_pushButtonAleatoire_clicked()
{
    ui->pushButtonBoucle->setIcon(QIcon(":/images/iconVertBoucle.png"));
    ui->pushButtonAleatoire->setIcon(QIcon(":/images/iconRougeAleatoire.png"));
    ui->pushButtonSeqentiel->setIcon(QIcon(":/images/iconVertSequentiel.png"));
    playlist->setPlaybackMode(QMediaPlaylist::Random);
}

void LecteurAudio::on_pushButtonBoucle_clicked()
{
    ui->pushButtonBoucle->setIcon(QIcon(":/images/iconRougeBoucle.png"));
    ui->pushButtonAleatoire->setIcon(QIcon(":/images/iconVertAleatoire.png"));
    ui->pushButtonSeqentiel->setIcon(QIcon(":/images/iconVertSequentiel.png"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
}

void LecteurAudio::on_pushButtonSeqentiel_clicked()
{
    ui->pushButtonBoucle->setIcon(QIcon(":/images/iconVertBoucle.png"));
    ui->pushButtonAleatoire->setIcon(QIcon(":/images/iconVertAleatoire.png"));
    ui->pushButtonSeqentiel->setIcon(QIcon(":/images/iconRougeSequentiel.png"));
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
}

void LecteurAudio::on_pushButtonMonter_clicked()
{
    player->stop();
    int index = ui->listWidgetListeLecture->currentIndex().row();
    playlist->moveMedia(index, index-1);
    playlist->setCurrentIndex(index-1);
    AfficherPlayList();
}

void LecteurAudio::on_pushButtonDescendre_clicked()
{
    player->stop();
    int index = ui->listWidgetListeLecture->currentIndex().row();
    playlist->moveMedia(index, index+1);
    playlist->setCurrentIndex(index+1);
    AfficherPlayList();
}

void LecteurAudio::on_pushButtonSupprimer_clicked()
{
    int index = ui->listWidgetListeLecture->currentRow();
    QMediaContent media = playlist->media(index);
    QMessageBox question(this);
    question.setWindowTitle(tr("Assistant musicien"));
    QString texte = "Etes-vous sûr de voloir supprimer\n";
    texte += SupprimerExtension(media.canonicalUrl().toString());
    question.setText(texte);
    question.addButton(tr("Annuler"),QMessageBox::ActionRole);
    question.setIcon(QMessageBox::Question);
    QAbstractButton *boutonValider = question.addButton(tr("Accepter"),QMessageBox::ActionRole);
    question.exec();
    if(question.clickedButton() == boutonValider)
    {
        player->stop();
        int index = ui->listWidgetListeLecture->currentIndex().row();
        playlist->removeMedia(index);
        playlist->setCurrentIndex(index);
        AfficherPlayList();
    }
}

void LecteurAudio::on_listWidgetListeLecture_currentRowChanged(int currentRow)
{
    if(currentRow == 0)
        ui->pushButtonMonter->setDisabled(true);
    else
        ui->pushButtonMonter->setEnabled(true);
    if(currentRow == ui->listWidgetListeLecture->count()-1)
        ui->pushButtonDescendre->setDisabled(true);
    else
        ui->pushButtonDescendre->setEnabled(true);
}


