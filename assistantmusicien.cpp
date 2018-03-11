#include "assistantmusicien.h"
#include "ui_assistantmusicien.h"
/**
 * @brief AssistantMusicien::AssistantMusicien
 * @param parent  pointeur vers le Widget parent.
 * @details Constructeur de la classe :
 *          Création de la barre d'outil pour Afficher les applications,
 *
 */
AssistantMusicien::AssistantMusicien(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssistantMusicien)
{
    ui->setupUi(this);

    /*-- Création de la barre d'outils application --*/
    applicationToolBar = new QToolBar(tr("Applications"),this);
    applicationToolBar->setOrientation(Qt::Vertical);
    applicationToolBar->setFloatable(true);
    applicationToolBar->setIconSize(QSize(80,80));
    applicationToolBar->addAction(QIcon(":/images/icon-Liste.png"),tr("Gestionnaire de listes"),this,SLOT(AfficherListe()));
    applicationToolBar->addAction(QIcon(":/images/icon-Partition.png"),tr("Lecteur de partitions"),this,SLOT(AfficherPartition()));
    applicationToolBar->addAction(QIcon(":/images/icon-Audio.png"),tr("Lecteur audio"),this,SLOT(AfficherAudio()));
    addToolBar(Qt::LeftToolBarArea, applicationToolBar);

    /*-- Création de la zone de travail avec les différentes applications --*/
    mdiAera = new QMdiArea(this);
    setCentralWidget(mdiAera);
    mdiAera->setTabsMovable(true);
    mdiAera->setTabsClosable(false);

    audio = new LecteurAudio(this);
    pAudio = mdiAera->addSubWindow(audio);
    pAudio->setWindowFlags(Qt::WindowTitleHint );
    pAudio->setWindowTitle(tr("Lecteur audio"));
    pAudio->showMaximized();

    partitions = new LecteurPartitions(this);
    pPartitions = mdiAera->addSubWindow(partitions);
    pPartitions->setWindowFlags(Qt::WindowTitleHint );
    pPartitions->setWindowTitle(tr("Lecteur de partitions"));
    pPartitions->showMaximized();

    listes = new GestionnaireListes(audio, this);
    pListes = mdiAera->addSubWindow(listes);
    pListes->setWindowFlags(Qt::WindowTitleHint );
    pListes->setWindowTitle(tr("Gestionnaire de listes"));
    pListes->showMaximized();
    mdiAera->setActiveSubWindow(pListes);

    /*-- Apparence de l'application --*/
    showMaximized();
}

/**
 * @brief AssistantMusicien::~AssistantMusicien
 * @details Destructeur de la classe, libère la mémoire allouée dynamiquement
 */
AssistantMusicien::~AssistantMusicien()
{
    delete ui;
    delete applicationToolBar;
}

/**
 * @brief AssistantMusicien::AfficherAudio
 * @details Active le lecteur audio et l'affiche dans la zone de travail
 */
void AssistantMusicien::AfficherAudio()
{
    mdiAera->setActiveSubWindow(pAudio);
    pAudio->showMaximized();

}

/**
 * @brief AssistantMusicien::AfficherPartition
 * @details Active le lecteur de partitions et l'affiche dans la zone de travail
 */
void AssistantMusicien::AfficherPartition()
{
    mdiAera->setActiveSubWindow(pPartitions);
    pPartitions->showMaximized();
}

/**
 * @brief AssistantMusicien::AfficherListe
 * @details Active le gestionnaire de listes et l'affiche dans la zone de travail
 */
void AssistantMusicien::AfficherListe()
{
    mdiAera->setActiveSubWindow(pListes);
    pListes->showMaximized();
}
