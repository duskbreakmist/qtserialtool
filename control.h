#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QtCharts>
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

private:
    Ui::control *ui;
    QSerialPort * serialport1;

    QTimer *timer;                           //计时器
    QChart *chart;                           //画布
    QSplineSeries *series;                     //线
    QValueAxis *axisX;                    //轴
    QValueAxis *axisY;

    bool ifopened;
    long long sendNum;
};


#endif // CONTROL_H
