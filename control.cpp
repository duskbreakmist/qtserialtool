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
    linesNum = 3;
    MAXlinesNum = 16;
    for(int i=0;i<MAXlinesNum;i++){
        QSplineSeries * temp =new QSplineSeries();
        SeriesList.append(temp);
    }
    chartinit();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateChart()));
//    timer->start(100);

}

control::~control()
{
    for(int i=0;i<MAXlinesNum;i++){
        delete SeriesList[i];
    }
    delete ui;

}

void control::chartinit(){

//    QPen peny(Qt::darkRed , 3 , Qt::SolidLine , Qt::RoundCap , Qt::RoundJoin);

    chart->legend()->hide();        //隐藏图例
    axisX->setTickCount(11);   //设置坐标轴格数
    axisY->setTickCount(5);

    axisY->setMin(-1);       //设置最小值
    axisY->setMax(1);
    axisX->setMin(0);       //设置最小值
    axisX->setMax(100);

    axisY->setLinePenColor(QColor(Qt::darkBlue));       //设置坐标轴颜色
    axisY->setGridLineColor(QColor(Qt::darkBlue));
//    axisY->setGridLineVisible(false);        //设置Y轴网格不显示

    chart->addAxis(axisX , Qt::AlignBottom);
    chart->addAxis(axisY , Qt::AlignLeft);

//    chart->addSeries(series);       //把线添加到chart中

//    series->attachAxis(axisX);
//    series->attachAxis(axisY);
//    series->setPointsVisible(true);
    for(int i=0;i<linesNum;i++){
        chart->addSeries(SeriesList[i]);
        SeriesList[i]->attachAxis(axisX);
        SeriesList[i]->attachAxis(axisY);
        SeriesList[i]->setPointsVisible(true);
    }
    axisY->setTitleText("y1");
    axisY->setTitleText("x");

    chart->setMargins(QMargins(0, 0, 0, 0));

    ui->widget->setChart(chart);
    ui->widget->setRenderHint(QPainter::Antialiasing);


}
void control::updateChart(){
    if(ui->checkBox_5->isChecked()){
        QStringList datas = nowData.split(",");

        int nCount = SeriesList[0]->points().size();
//        chart->axisX()->setMin(nCount - 100);
//        chart->axisX()->setMax(nCount);
        float temp = 0;
        for(int i=0;i<linesNum;i++){

//            SeriesList[i]->append(QPointF(nCount, rand()%40));
            if(i<datas.count()){
                temp = datas[i].toFloat();
            }
            else{
                temp = 0;
            }
            SeriesList[i]->append(QPointF(nCount,temp));
        }
//        series->append(QPointF(nCount, rand()%40));

    }


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
//            QString str = ui->textEdit->toPlainText();

            nowData = tr(buf);
            if(nowData.at(nowData.size() - 1)=='\n'){
                nowData = NotComplete+nowData;
                NotComplete.clear();
                ui->label_15->setText(nowData);
//                ui->textEdit_2->setText(nowData);
                if(ui->radioButton_2->isChecked()){
                    updateChart();
                }
            }
            else{
                NotComplete += nowData;
            }
//            str+=tr(buf);
//            ui->textEdit->clear();

            receivNum+= tr(buf).count();
            ui->label_11->setText(QString::number(receivNum));

            ui->textEdit->insertPlainText(tr(buf));
            if(ifautoscoll){
                ui->textEdit->moveCursor(QTextCursor::End);
            }
        }
        buf.clear();
    }
    else{
        const QByteArray info = serialport1->readAll();
            QByteArray buf = info.toHex();
            QString string;
            string.prepend(buf);
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
    receivNum = 0;
}


void control::on_pushButton_3_clicked()
{
    //clear send
    sendNum = 0;
    ui->textEdit_2->clear();
}


void control::mousePressEvent(QMouseEvent* event){
    if (event->button() & Qt::LeftButton) {
            isClicking = true;
            screenPos = event->globalPosition().toPoint();
        } else if (event->button() & Qt::RightButton) {
            chart->zoomReset();
        }

        QWidget::mousePressEvent(event);
}

void control::mouseMoveEvent(QMouseEvent* event){
        if (isClicking) {
            QPoint tPos = event->globalPosition().toPoint()-screenPos;


            chart->scroll(-tPos.x(),tPos.y());
            screenPos = event->globalPosition().toPoint();

            return;
        }

        QWidget::mouseMoveEvent(event);
}
void control::mouseReleaseEvent(QMouseEvent* event){
    isClicking = false;
}
void control::wheelEvent(QWheelEvent *event){
    if (event->angleDelta().y() > 0) {
            chart->zoom(1.1);
    } else {
        chart->zoom(10.0/11);
    }

    QWidget::wheelEvent(event);
}

void control::on_pushButton_6_clicked()
{
    //clear lines
    for(int i=0;i<linesNum;i++){

        SeriesList[i]->clear();

    }
}


void control::on_radioButton_2_clicked(bool checked)
{
    if(checked){
        timer->stop();
    }
}



void control::on_radioButton_clicked(bool checked)
{
    if(checked){
        timer->start(100);
    }
}


void control::on_checkBox_6_stateChanged(int arg1)
{
    if(ui->checkBox_6->isChecked()){
        ifautoscoll = true;
    }
    else{
        ifautoscoll = false;
    }
}

