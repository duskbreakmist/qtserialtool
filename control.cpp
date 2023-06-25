#include "control.h"
#include "ui_control.h"

control::control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::control)
{
    ui->setupUi(this);
    this->setWindowTitle("上位机：电梯");

    serialport1 = new QSerialPort(this);

    ifopened = false;
    sendNum = 0;
    UpdateSerialports();
    ui->comboBox_2->setCurrentIndex(7);
    connect(serialport1, SIGNAL(readyRead()), this, SLOT(readData()));


    chart = new QChart();
    series = new QSplineSeries();
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateChart()));
    timer->start(100);
    chartinit();
}

control::~control()
{
    delete ui;

}

void control::chartinit(){

//    QPen peny(Qt::darkRed , 3 , Qt::SolidLine , Qt::RoundCap , Qt::RoundJoin);

    chart->legend()->hide();        //隐藏图例
    chart->addSeries(series);       //把线添加到chart中
    axisX->setTickCount(11);   //设置坐标轴格数
    axisY->setTickCount(5);

    axisY->setMin(0);       //设置最小值
    axisY->setMax(100);
    axisX->setMin(0);       //设置最小值
    axisX->setMax(100);

    axisY->setLinePenColor(QColor(Qt::darkBlue));       //设置坐标轴颜色
    axisY->setGridLineColor(QColor(Qt::darkBlue));
//    axisY->setGridLineVisible(false);        //设置Y轴网格不显示

    chart->addAxis(axisX , Qt::AlignBottom);
    chart->addAxis(axisY , Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);
    series->setPointsVisible(true);

    axisY->setTitleText("y1");
    axisY->setTitleText("x");

    chart->setMargins(QMargins(0, 0, 0, 0));

    ui->widget->setChart(chart);
    ui->widget->setRenderHint(QPainter::Antialiasing);

}
void control::updateChart(){
    int nCount = series->points().size();
        chart->axisX()->setMin(nCount - 100);
        chart->axisX()->setMax(nCount);

        series->append(QPointF(nCount, rand()%40));

}
void control::UpdateSerialports(){
    QStringList serialNamePort;
    foreach (const QSerialPortInfo &inf0, QSerialPortInfo::availablePorts()) {
            serialNamePort<<inf0.portName();
    }
    ui->comboBox->addItems(serialNamePort);
}
void control::openSerial(){
    //open

    serialport1->setPortName(ui->comboBox->currentText());
    serialport1->setBaudRate(ui->comboBox_2->currentText().toInt());

    //设置校验位
    switch (ui->comboBox_3->currentIndex())
    {
        case 0:
            serialport1->setParity(QSerialPort::NoParity);
            break;
        default:
            break;
    }
    //设置数据位
    switch (ui->comboBox_4->currentIndex())
            {
            case 8:
                serialport1->setDataBits(QSerialPort::Data8);
            case 7:
                serialport1->setDataBits(QSerialPort::Data7);
            case 6:
                serialport1->setDataBits(QSerialPort::Data6);
            case 5:
                serialport1->setDataBits(QSerialPort::Data5);
                break;
            default:
                break;
    }
    //设置停止位
    switch (ui->comboBox_5->currentIndex())
    {
        case 1:
            serialport1->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serialport1->setStopBits(QSerialPort::TwoStop);
        default:
            break;
    }
    if (true == serialport1->open(QIODevice::ReadWrite))
    {
        //QMessageBox::information(this, "提示", "串口打开成功");
        ifopened = true;

//        connect(serialport1,SIGNAL(readyRead),this,SLOT(readData));

        ui->pushButton->setText("关闭串口");
    }
    else
    {
        QMessageBox::critical(this, "提示", "串口打开失败");
    }



}

void control::readData(){
    if(!ui->checkBox_2->isChecked()){
        QByteArray buf;
        buf = serialport1->readAll();
        if(!buf.isEmpty())
        {
            QString str = ui->textEdit->toPlainText();
            str+=tr(buf);
            ui->textEdit->clear();
            ui->textEdit->append(str);

        }
        buf.clear();
    }
    else{
        const QByteArray info = serialport1->readAll();
            QByteArray hexData = info.toHex();
            QString string;
            string.prepend(hexData);
            qDebug()<<"receive info:"<<info;
            ui->textEdit->append(string);
    }

}

void control::sendData(){
    long long a=0;
    //字符串形式
    if(ui->checkBox->isChecked() == false)
    {
        a=serialport1->write(ui->textEdit_2->toPlainText().toLocal8Bit().data());
    }
    else    //16进制发送（将16进制转换成Ascll码对应的字符发送）
    {
        a=serialport1->write(QByteArray::fromHex(ui->textEdit_2->toPlainText().toUtf8()).data()); //16进制数据解码后发送
    }
    //如果发送成功，a获取发送的字节长度，发送失败则返回-1；

    if(a > 0)
    {
        sendNum += a;
        ui->label_10->setText(QString::number(sendNum));
    }

}
void control::on_pushButton_clicked()
{
    //open serial
    if(!ifopened){
        openSerial();
    }
    else{
        //关闭
        ifopened = false;
        serialport1->clear();
        serialport1->close();

        ui->pushButton->setText("打开串口");
    }
}



void control::on_pushButton_4_clicked()
{
    //send
    sendData();
}


void control::on_pushButton_2_clicked()
{
    //clear rec
    ui->textEdit->clear();
}


void control::on_pushButton_3_clicked()
{
    //clear send
    ui->textEdit_2->clear();
}

