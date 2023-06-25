#ifndef MYCHART_H
#define MYCHART_H

#include <QWidget>
#include <QChartView>
namespace Ui {
class mychart;
}

class mychart : public  QChartView
{
    Q_OBJECT

public:
    explicit mychart(QWidget *parent = nullptr);
    ~mychart();

private:
    Ui::mychart *ui;
    bool isClicking;
    QPoint screenPos;

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
};

#endif // MYCHART_H
