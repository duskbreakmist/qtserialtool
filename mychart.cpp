#include "mychart.h"
#include "ui_mychart.h"

mychart::mychart(QWidget *parent) :
    QChartView(parent),
    ui(new Ui::mychart)
{
    ui->setupUi(this);
}

mychart::~mychart()
{
    delete ui;
}

void mychart::mousePressEvent(QMouseEvent* event){
    if (event->button() & Qt::LeftButton) {
            isClicking = true;
            screenPos = event->globalPosition().toPoint();
        } else if (event->button() & Qt::RightButton) {
            //chart()->zoom(chart().)
        }

        QWidget::mousePressEvent(event);
}

void mychart::mouseMoveEvent(QMouseEvent* event){
        if (isClicking) {
            QPoint tPos = event->globalPosition().toPoint()-screenPos;


            chart()->scroll(-tPos.x(),tPos.y());
            screenPos = event->globalPosition().toPoint();

            return;
        }

        QWidget::mouseMoveEvent(event);
}
void mychart::mouseReleaseEvent(QMouseEvent* event){
    isClicking = false;
}
void mychart::wheelEvent(QWheelEvent *event){
    if (event->angleDelta().y() > 0) {
            chart()->zoom(1.1);
    } else {
        chart()->zoom(10.0/11);
    }

    QWidget::wheelEvent(event);
}
