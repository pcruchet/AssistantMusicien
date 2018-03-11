#ifndef AssistantMusicien_H
#define AssistantMusicien_H

#include <QMainWindow>
#include <QToolBar>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "lecteuraudio.h"
#include "lecteurpartitions.h"
#include "gestionnairelistes.h"

namespace Ui {
class AssistantMusicien;
}

class AssistantMusicien : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssistantMusicien(QWidget *parent = 0);
    ~AssistantMusicien();

private slots:
    void AfficherAudio();
    void AfficherPartition();
    void AfficherListe();

private:
    Ui::AssistantMusicien *ui;

    QToolBar *applicationToolBar;
    QMdiArea *mdiAera;
    QMdiSubWindow *pAudio;
    QMdiSubWindow *pPartitions;
    QMdiSubWindow *pListes;

    QSize tailleFenetre;                    /// taille du widget

    LecteurAudio *audio;
    LecteurPartitions *partitions;          /// Widget Lecteur de partitions
    GestionnaireListes *listes;

};

#endif // AssistantMusicien_H
