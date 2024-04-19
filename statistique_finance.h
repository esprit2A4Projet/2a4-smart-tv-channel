#ifndef PIECHARTWIDGET_H
#define PIECHARTWIDGET_H

#include <QWidget>
#include <QtCharts>

class PieChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit PieChartWidget(QWidget *parent = nullptr);

private:
    QPieSeries *series;
    QChart *chart;
    QChartView *chartView;
};

#endif // PIECHARTWIDGET_H
