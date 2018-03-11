#ifndef LECTEURPARTITIONS_H
#define LECTEURPARTITIONS_H

#include <QWidget>
#include <QLabel>
#include <poppler-qt5.h>
#include <QResizeEvent>
#include <QScrollBar>

namespace Ui {
class LecteurPartitions;
}

class LecteurPartitions : public QWidget
{
    Q_OBJECT

public:
    explicit LecteurPartitions(QWidget *parent = 0);
    ~LecteurPartitions();
    void ChargerPartition(QString _cheminPartition);

private slots:
    void on_pushButtonCharger_clicked();

    void on_pushButtonFermer_clicked();
    void resizeEvent(QResizeEvent *event);

    void on_pushButtonPagePrec_clicked();

    void on_pushButtonPageSuiv_clicked();

    void on_pushButtonMonter_clicked();

    void on_pushButtonDescendre_clicked();

private:
    Ui::LecteurPartitions *ui;
    Poppler::Document* document;
    QList <QImage> images;
    QList <QLabel *> listeLabelImages;
};

#endif // LECTEURPARTITIONS_H
