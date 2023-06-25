#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QtCharts>
#include<QWheelEvent>

namespace Ui {
class control;
}

class control : public QWidget
{
    Q_OBJECT

public:
    explicit control(QWidget *parent = nullptr);
    ~control();

    void UpdateSerialports();
    void openSerial();
    void chartinit();
private slots:
    void on_pushButton_clicked();
    void readData();
    void sendData();
    void updateChart();
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_radioButton_2_clicked(bool checked);


    void on_radioButton_clicked(bool checked);

    void on_checkBox_6_stateChanged(int arg1);

private:
    Ui::control *ui;
    QSerialPort * serialport1;

    QTimer *timer;                           //计时器
    QChart *chart;                           //画布
    QSplineSeries *series;                     //线
    int MAXlinesNum;
    int linesNum;
    QList<QSplineSeries *> SeriesList;
    QValueAxis *axisX;                    //轴
    QValueAxis *axisY;

    bool ifopened;
    bool ifautoscoll;
    long long sendNum;
    QString nowData;
    QString NotComplete;
    int xOld;
    int yOld;
    bool isClicking;
protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
};


#endif // CONTROL_H
