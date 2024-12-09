#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QVector>

class CurveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurveWidget(QWidget *parent = nullptr);
    void loadData(); // Charger les données de la base

protected:
    void paintEvent(QPaintEvent *event) override; // Dessiner la courbe

private:
    QVector<QPointF> dataPoints; // Stocke les points (prix, quantité)
};

#endif // CURVEWIDGET_H
