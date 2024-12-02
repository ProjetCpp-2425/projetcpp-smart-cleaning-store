#include "curvewidget.h"
#include <QPainter>
#include <QSqlError>
#include <QMessageBox>
#include <QPen>

CurveWidget::CurveWidget(QWidget *parent)
    : QWidget(parent)
{
    loadData();
}

void CurveWidget::loadData()
{
    QSqlQuery query;

    // Requête pour récupérer les prix et les quantités
    if (!query.exec("SELECT PRIXP, QUANTITE FROM PRODUIT ORDER BY PRIXP ASC")) {
        QMessageBox::critical(this, "Erreur", "Erreur de base de données : " + query.lastError().text());
        return;
    }

    // Charger les points dans le vecteur
    while (query.next()) {
        double prix = query.value(0).toDouble();
        double quantite = query.value(1).toDouble();
        dataPoints.append(QPointF(prix, quantite));
    }
}

void CurveWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (dataPoints.isEmpty()) {
        return; // Rien à dessiner si pas de données
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Définir les marges
    int margin = 60;  // Increased margin for better visibility
    QRect graphArea(margin, margin, width() - 2 * margin, height() - 2 * margin);

    // Set up pen for drawing lines
    QPen pen;
    pen.setColor(Qt::blue);  // Set line color to blue
    pen.setWidth(2);         // Set line width
    painter.setPen(pen);

    // Dessiner les axes
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(graphArea.bottomLeft(), graphArea.topLeft());    // Axe Y
    painter.drawLine(graphArea.bottomLeft(), graphArea.bottomRight()); // Axe X

    // Dessiner les graduations et les étiquettes
    double minPrix = dataPoints.first().x();
    double maxPrix = dataPoints.last().x();
    double minQuantite = dataPoints.first().y();
    double maxQuantite = minQuantite;

    for (const QPointF &point : dataPoints) {
        if (point.y() > maxQuantite) {
            maxQuantite = point.y();
        }
    }

    // Determine the scaling for the axes
    int numTicksX = 10;
    int numTicksY = 10;

    // Drawing the gridlines for better precision
    painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
    for (int i = 0; i <= numTicksX; ++i) {
        double x = graphArea.left() + i * (graphArea.width() / numTicksX);
        painter.drawLine(x, graphArea.top(), x, graphArea.bottom());
    }

    for (int i = 0; i <= numTicksY; ++i) {
        double y = graphArea.bottom() - i * (graphArea.height() / numTicksY);
        painter.drawLine(graphArea.left(), y, graphArea.right(), y);
    }

    // Draw axis labels
    painter.setPen(QPen(Qt::black, 1));
    for (int i = 0; i <= numTicksX; ++i) {
        double x = graphArea.left() + i * (graphArea.width() / numTicksX);
        double price = minPrix + i * (maxPrix - minPrix) / numTicksX;
        painter.drawText(x - 10, graphArea.bottom() + 20, QString::number(price, 'f', 2));
    }

    for (int i = 0; i <= numTicksY; ++i) {
        double y = graphArea.bottom() - i * (graphArea.height() / numTicksY);
        double quantity = minQuantite + i * (maxQuantite - minQuantite) / numTicksY;
        painter.drawText(graphArea.left() - 30, y + 5, QString::number(quantity, 'f', 2));
    }

    // Dessiner la courbe
    QPointF previousPoint;
    bool firstPoint = true;

    for (const QPointF &point : dataPoints) {
        // Convertir les coordonnées des données en coordonnées écran
        double x = graphArea.left() + (point.x() - minPrix) / (maxPrix - minPrix) * graphArea.width();
        double y = graphArea.bottom() - (point.y() - minQuantite) / (maxQuantite - minQuantite) * graphArea.height();

        QPointF screenPoint(x, y);

        if (!firstPoint) {
            painter.drawLine(previousPoint, screenPoint); // Trace la ligne entre deux points
        }

        // Draw the points as circles for better visibility
        painter.setPen(QPen(Qt::red));
        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(screenPoint, 3, 3);  // Small circle at each data point

        previousPoint = screenPoint;
        firstPoint = false;
    }

    // Add graph title
    painter.setPen(QPen(Qt::black, 1));
    painter.drawText(graphArea.left() + graphArea.width() / 2 - 40, graphArea.top() - 10, "Prix vs Quantité");
}
