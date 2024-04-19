#include "statistique_finance.h"

PieChartWidget::PieChartWidget(QWidget *parent) : QWidget(parent) {
    // Création de la série de données pour le pie chart
    series = new QPieSeries();
    series->append("Catégorie 1", 30);
    series->append("Catégorie 2", 50);
    series->append("Catégorie 3", 20);

    // Création du chart
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pie Chart");

    // Création du widget de visualisation du chart
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chartView);
    setLayout(layout);
}

