#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "read.h"
#include "mylabel.h"
#include "gettime.h"
#include <QMessageBox>

int multi = 0;
const QChar celsius(0260);

// ---------------------- Initialize the main window ---------------------------------------Initialize the main window---------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);      //initialize the main window

    //hide the calenderetc
    ui->calendarWidget->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->comboBox_2->setVisible(false);
    ui->comboBox_3->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->qCustomPlot->setVisible(false);
    ui->label_7->setVisible(false); //the label says "sweep"
    ui->checkBox->setVisible(false);

    for (int i=0;i<60;i++){ //add hh,mm,ss to the dropdown list
        if(i<24){ui->comboBox->addItem(QString::number(i));}
        ui->comboBox_2->addItem(QString::number(i)); //must convert int to QString
        ui->comboBox_3->addItem(QString::number(i));
    }

    //connect  in order to use customerized label button
    connect(ui-> widget, SIGNAL(Previous_Pressed()), this, SLOT(Previous_Pressed()) );
    connect(ui-> widget_2, SIGNAL(Next_Pressed()), this, SLOT(Next_Pressed()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}




// ----------------------- "A specific time" button is clicked ----------------
void MainWindow::on_pushButton_clicked()
{
    //QCalendarWidget* calendarWidget = new QCalendarWidget;
    ui -> calendarWidget -> setVisible(true);
}

// ----------------------- Calendar widget is clicked ------------------------
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    ui->comboBox-> setVisible(true);
    ui->comboBox_2->setVisible(true);
    ui->comboBox_3->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_4->setVisible(true);
    ui->pushButton_3->setVisible(true);
}



// -------------------- When Label button "PREVIOUS" is pressed -----------------------------------When Label button "previous" is pressed------
void MainWindow::Previous_Pressed()
{
    thix.clear();
    thiy.clear();
    sweep = previousIV(sweep); //get pervious 100 data in thix&thiy, get new sweep number
    if (ui->checkBox->checkState() == Qt::Unchecked){
        //------------------plotting--------------
        ui->qCustomPlot->graph(0)->setData(thix,thiy);
        ui->qCustomPlot->xAxis->setRange(0,xsize+1);
        ui->qCustomPlot->yAxis->setRange(0,ysize+1);
        ui->qCustomPlot->graph(1)->setData(straightIx,straightIy);
        ui->qCustomPlot->graph(2)->setData(straightVx,straightVy);
        ui->qCustomPlot->graph(3)->setData(thix,thimpp);
        //-----------------sweep number----------------

        if (sweep >= 0) {ui->label_6->setNum(sweep);}
        else {ui->label_6->setText("no more data!");}
        //-----------------display Isc----------------
        ui->label_9->setText("Isc:"+QString::number(Isc)+"A");
        ui->label_10->setText("Voc:"+QString::number(Voc)+"V");
        ui->label_11->setText("Impp:"+QString::number(ImPP)+"A");
        ui->label_12->setText("Vmpp:"+QString::number(VmPP)+"V");
        ui->label_8->setText("Pmpp:"+QString::number(PmPP)+"W");
        ui->label_13->setText("Air temp: "+QString::number(airT)+celsius+"C");
        ui->label_14->setText("Panel temp: "+QString::number(panelT)+celsius+"C");
        ui->label_15->setText("Irradiance:"+QString::number(irr));
    }
    ui->qCustomPlot->replot();
}


// -------------------- When Label button "NEXT" is pressed ---------------------------------------- When Label button "previous" is pressed ----
void MainWindow::Next_Pressed()
{
    //get pervious 100 data in thix&thiy, get new sweep number
    thix.clear();
    thiy.clear();
    sweep = nextIV(sweep);

    if (ui->checkBox->checkState() == Qt::Unchecked){
        //------------------plotting start--------------
        ui->qCustomPlot->graph(0)->setData(thix,thiy);
        ui->qCustomPlot->xAxis->setRange(0,xsize+1);
        ui->qCustomPlot->yAxis->setRange(0,ysize+1);
        ui->qCustomPlot->graph(1)->setData(straightIx,straightIy);
        ui->qCustomPlot->graph(2)->setData(straightVx,straightVy);
        ui->qCustomPlot->graph(3)->setData(thix,thimpp);
        //------------------display sweeps----------------
        ui->label_7->setVisible(true);
        if (sweep >= 0) {ui->label_6->setNum(sweep);}
        else {ui->label_6->setText("no more data!");}
        //------------------display Isc----------------
        ui->label_9->setText("Isc:"+QString::number(Isc)+"A");
        ui->label_10->setText("Voc:"+QString::number(Voc)+"V");
        ui->label_11->setText("Impp:"+QString::number(ImPP)+"A");
        ui->label_12->setText("Vmpp:"+QString::number(VmPP)+"V");
        ui->label_8->setText("Pmpp:"+QString::number(PmPP)+"W");
        ui->label_13->setText("Air temp: "+QString::number(airT)+celsius+"C");
        ui->label_14->setText("Panel temp: "+QString::number(panelT)+celsius+"C");
        ui->label_15->setText("Irradiance:"+QString::number(irr));
    }

    if (ui->checkBox->checkState() == Qt::Checked){
        ui->qCustomPlot->addGraph();
        multi += 1;      //reason can not use sweep%4, is we always need to start eith graph(0) and sweep%4 can not gurantee it
        multi = multi%4;
        if (multi == 1){
        ui->qCustomPlot->graph(multi)->setPen(QPen(Qt::red));
        ui->qCustomPlot->graph(multi)->setLineStyle(QCPGraph::lsNone);
        ui->qCustomPlot->graph(multi)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,Qt::red,Qt::white,3));
        }
        if (multi == 2){
        ui->qCustomPlot->graph(multi)->setPen(QPen(Qt::green));
        ui->qCustomPlot->graph(multi)->setLineStyle(QCPGraph::lsNone);
        ui->qCustomPlot->graph(multi)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar,Qt::green,Qt::white,3));
        }
        if (multi == 3){
        ui->qCustomPlot->graph(multi)->setPen(QPen(Qt::yellow));
        ui->qCustomPlot->graph(multi)->setLineStyle(QCPGraph::lsLine);
        ui->qCustomPlot->graph(multi)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCustom,Qt::yellow,Qt::white,3));
        }
        ui->qCustomPlot->graph(multi)->setData(thix,thiy);
        ui->qCustomPlot->graph(multi)->rescaleAxes(true);
    }
    ui->qCustomPlot->replot();
}


// ----------------------- "read current time" button is clicked --------------------------- "read current time" button is clicked--------
void MainWindow::on_pushButton_2_clicked()
{
    ui->checkBox->setVisible(false);
    multi = 0;
    sweep = gettime();  //the time could changed to a new hour, always re-load the data
    readcurrent();

    if (file == NULL){QMessageBox::warning(this,tr("Error"),tr("Can not find data for selected time!")); }
    else {
        // -------------------- qCUsomePlot widget settings -------------------
        ui->qCustomPlot->clearGraphs();
        get100IV(sweep);
        QPalette pblue;
        pblue.setColor(QPalette::WindowText, Qt::blue);
        QPalette pred;
        pred.setColor(QPalette::WindowText, Qt::red);
        QPen bluePen(Qt::blue,3,Qt::DotLine,Qt::RoundCap,Qt::RoundJoin);
        QPen redDotPen;
        redDotPen.setColor(Qt::red);
        redDotPen.setStyle(Qt::DashLine);
        redDotPen.setWidth(1);
        QPen dashDotPen;
        dashDotPen.setColor(Qt::blue);
        dashDotPen.setStyle(Qt::DashDotDotLine);
        dashDotPen.setWidth(1);

        ui->label_9->setPalette(pblue);
        ui->label_10->setPalette(pblue);
        ui->label_9->setText("Isc:"+QString::number(Isc)+"A");
        ui->label_10->setText("Voc:"+QString::number(Voc)+"V");
        ui->label_9->setWordWrap(true);//for making new line
        ui->label_10->setWordWrap(true);
        ui->label_11->setPalette(pred); //set label_11 color red
        ui->label_12->setPalette(pred);  //set label_11 color red
        ui->label_11->setText("Impp:"+QString::number(ImPP) + "A");
        ui->label_12->setText("Vmpp:"+QString::number(VmPP) + "V");
        ui->label_8->setText("Pmpp:"+QString::number(PmPP) + "W");
        ui->label_13->setText("Air temp: "+QString::number(airT)+celsius+"C");
        ui->label_14->setText("Panel temp: "+QString::number(panelT)+celsius+"C");
        ui->label_15->setText("Irradiance:"+QString::number(irr));
        ui->checkBox->setVisible(false);

        if (ui->checkBox->checkState() == Qt::Unchecked){
            // ------------------- use qcustomplot to plot curve -----------------
            //In widget area, need to change the widget's object name to "qCustomPlot"in UI
            ui->widget->setVisible(true);
            ui->widget_2->setVisible(true);
            ui->label_9->setVisible(true);
            ui->label_10->setVisible(true);
            ui->label_11->setVisible(true);
            ui->label_12->setVisible(true);

            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(0)->setData(thix,thiy);    //only add 100 data to display
            ui->qCustomPlot->xAxis->setLabel("voltage (V)");
            ui->qCustomPlot->yAxis->setLabel("current (A)");
            ui->qCustomPlot->xAxis->setRange(0,xsize+1);  //set the range of scope, otherwise use default range
            ui->qCustomPlot->yAxis->setRange(0,ysize+1);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(1)->setPen(redDotPen);
            ui->qCustomPlot->graph(1)->setData(straightIx,straightIy);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(2)->setPen(redDotPen);
            ui->qCustomPlot->graph(2)->setData(straightVx,straightVy);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(3)->setPen(dashDotPen);
            ui->qCustomPlot->graph(3)->setData(thix,thimpp);
            //----------------------------- end of plotting -----------------------
        }

        if (ui->checkBox->checkState() == Qt::Checked){
            ui->widget_2->setVisible(true);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(0)->setData(thix,thiy);
            ui->qCustomPlot->xAxis->setLabel("voltage (V)");
            ui->qCustomPlot->yAxis->setLabel("current (A)");
            ui->qCustomPlot->xAxis->setRange(0,xsize+1);  //set the range of scope, otherwise use default range
            ui->qCustomPlot->yAxis->setRange(0,ysize+1);
        }

        ui->qCustomPlot->replot(); //important to re-plot
        ui->qCustomPlot->setVisible(true);
        // display the previous/next button
        QPixmap previous, next;
        previous.load("/home/pi/QtProjects/img/previous6.png");
        next.load("/home/pi/QtProjects/img/next6.png");
        ui->widget->setPixmap(previous);
        ui->widget->setGeometry(10, 350, 50, 25); //290, 140 is x and y, 50 50 is width and height
        ui->widget_2->setPixmap(next);
        ui->widget_2->setGeometry(390, 350, 50, 25); //290, 140 is x and y, 50 50 is width and height
        ui->label_7->setVisible(true);
        ui->label_6->setNum(sweep); //or:setText(QString("%1").arg(sweep));  or setText(QString::number(sweep));

    }
}

// ---------------------------- checkbox is clicked ---------------------------------------- checkbox is clicked------------
void MainWindow::on_checkBox_clicked()
{
    QPalette pblue;
    pblue.setColor(QPalette::WindowText, Qt::blue);
    QPalette pred;
    pred.setColor(QPalette::WindowText, Qt::red);
    QPen bluePen(Qt::blue,3,Qt::DotLine,Qt::RoundCap,Qt::RoundJoin);
    QPen redDotPen;
    redDotPen.setColor(Qt::red);
    redDotPen.setStyle(Qt::DashLine);
    redDotPen.setWidth(1);
    QPen dashDotPen;
    dashDotPen.setColor(Qt::blue);
    dashDotPen.setStyle(Qt::DashDotDotLine);
    dashDotPen.setWidth(1);


    if (ui->checkBox->checkState() == Qt::Unchecked){
        // ------------------- use qcustomplot to plot curve -----------------
        //In widget area, need to change the widget's object name to "qCustomPlot"in UI
        //ui->widget->setVisible(false);
        //ui->widget_2->setVisible(true);
        ui->qCustomPlot->clearGraphs();
        ui->widget->setVisible(false);
        ui->widget_2->setVisible(false);
        ui->label_9->setVisible(true);
        ui->label_10->setVisible(true);
        ui->label_11->setVisible(true);
        ui->label_12->setVisible(true);
        ui->qCustomPlot->addGraph();
        ui->qCustomPlot->graph(0)->setData(thix,thiy);    //only add 100 data to display
        ui->qCustomPlot->xAxis->setLabel("voltage (V)");
        ui->qCustomPlot->yAxis->setLabel("current (A)");
        ui->qCustomPlot->xAxis->setRange(0,xsize+1);  //set the range of scope, otherwise use default range
        ui->qCustomPlot->yAxis->setRange(0,ysize+1);
        ui->qCustomPlot->addGraph();
        ui->qCustomPlot->graph(1)->setPen(redDotPen);
        ui->qCustomPlot->graph(1)->setData(straightIx,straightIy);
        ui->qCustomPlot->addGraph();
        ui->qCustomPlot->addGraph();
        ui->qCustomPlot->graph(2)->setPen(redDotPen);
        ui->qCustomPlot->graph(2)->setData(straightVx,straightVy);
        ui->qCustomPlot->addGraph();
        ui->qCustomPlot->graph(3)->setPen(dashDotPen);
        ui->qCustomPlot->graph(3)->setData(thix,thimpp);
        ui->qCustomPlot->clearGraphs();

        //----------------------------- end of plotting -----------------------
    }

    if (ui->checkBox->checkState() == Qt::Checked){

        ui->label_8->setText("");
        ui->label_9->setText("");
        ui->label_10->setText("");
        ui->label_11->setText("");
        ui->label_12->setText("");
        ui->label_13->setText("");
        ui->label_14->setText("");
        ui->label_15->setText("");
        ui->qCustomPlot->clearGraphs();
        ui->qCustomPlot->addGraph();
        ui->qCustomPlot->graph(0)->setData(thix,thiy);
        ui->qCustomPlot->xAxis->setLabel("voltage (V)");
        ui->qCustomPlot->yAxis->setLabel("current (A)");
        ui->qCustomPlot->xAxis->setRange(0,xsize+1);  //set the range of scope, otherwise use default range
        ui->qCustomPlot->yAxis->setRange(0,ysize+1);
        ui->qCustomPlot->replot();

    }
    /*
    if (ui->checkBox->checkState() == Qt::Unchecked){
        ui->qCustomPlot->clearGraphs();
        //thix.clear();
        //thiy.clear();
        ui->qCustomPlot->replot(); //important to re-plot
        ui->widget->setVisible(false);
        ui->widget_2->setVisible(false);
    } */

}


// ----------------------- "OK" button is clicked ------------------------------------ "OK" button is clicked--------
void MainWindow::on_pushButton_3_clicked()
{   
    multi = 0;
    QDate datatime = ui->calendarWidget->selectedDate();
    dd = datatime.day();
    yy = datatime.year()-2000;
    mm = datatime.month();
    hh = ui->comboBox->currentIndex();
    minute = ui->comboBox_2->currentIndex();
    second = ui->comboBox_3->currentIndex();

    sweep = getsweep();
    read();


    QPixmap previous, next;
    previous.load("/home/pi/QtProjects/img/previous6.png");
    next.load("/home/pi/QtProjects/img/next6.png");
    ui->widget->setPixmap(previous);
    ui->widget->setGeometry(10, 350, 50, 25); //290, 140 is x and y, 50 50 is width and height
    ui->widget_2->setPixmap(next);
    ui->widget_2->setGeometry(390, 350, 50, 25); //290, 140 is x and y, 50 50 is width and height
    ui->label_7->setVisible(true);
    ui->label_6->setNum(sweep); //or:setText(QString("%1").arg(sweep));  or setText(QString::number(sweep));



    if (file == NULL){QMessageBox::warning(this,tr("Error"),tr("Can not find data for selected time!")); }
    else {
        ui->qCustomPlot->clearGraphs();
        get100IV(sweep);
        QPalette pblue;
        pblue.setColor(QPalette::WindowText, Qt::blue);
        QPalette pred;
        pred.setColor(QPalette::WindowText, Qt::red);
        QPen bluePen(Qt::blue,3,Qt::DotLine,Qt::RoundCap,Qt::RoundJoin);
        QPen redDotPen;
        redDotPen.setColor(Qt::red);
        redDotPen.setStyle(Qt::DashLine);
        redDotPen.setWidth(1);
        QPen dashDotPen;
        dashDotPen.setColor(Qt::blue);
        dashDotPen.setStyle(Qt::DashDotDotLine);
        dashDotPen.setWidth(1);

        ui->label_9->setPalette(pblue);
        ui->label_10->setPalette(pblue);
        ui->label_9->setText("Isc:"+QString::number(Isc)+"A");
        ui->label_10->setText("Voc:"+QString::number(Voc)+"V");
        ui->label_9->setWordWrap(true);//for making new line
        ui->label_10->setWordWrap(true);
        ui->label_11->setPalette(pred); //set label_11 color red
        ui->label_12->setPalette(pred);  //set label_11 color red
        ui->label_8->setPalette(pred);
        ui->label_11->setText("Impp:"+QString::number(ImPP)+"A");
        ui->label_12->setText("Vmpp:"+QString::number(VmPP)+"V");
        ui->label_8->setText("Pmpp:"+QString::number(PmPP)+"W");
        ui->label_13->setText("Air temp:"+QString::number(airT)+celsius+"C");
        ui->label_14->setText("Panel temp:"+QString::number(panelT)+celsius+"C");
        ui->label_15->setText("Irradiance:"+QString::number(irr));
        ui->checkBox->setVisible(true);


        if (ui->checkBox->checkState() == Qt::Unchecked){
            // ------------------- use qcustomplot to plot curve -----------------
            //In widget area, need to change the widget's object name to "qCustomPlot"in UI
            ui->widget->setVisible(true);
            ui->widget_2->setVisible(true);
            ui->label_9->setVisible(true);
            ui->label_10->setVisible(true);
            ui->label_11->setVisible(true);
            ui->label_12->setVisible(true);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(0)->setData(thix,thiy);    //only add 100 data to display
            ui->qCustomPlot->xAxis->setLabel("voltage (V)");
            ui->qCustomPlot->yAxis->setLabel("current (A)");
            ui->qCustomPlot->xAxis->setRange(0,xsize+1);  //set the range of scope, otherwise use default range
            ui->qCustomPlot->yAxis->setRange(0,ysize+1);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(1)->setPen(redDotPen);
            ui->qCustomPlot->graph(1)->setData(straightIx,straightIy);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(2)->setPen(redDotPen);
            ui->qCustomPlot->graph(2)->setData(straightVx,straightVy);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(3)->setPen(dashDotPen);
            ui->qCustomPlot->graph(3)->setData(thix,thimpp);
            //----------------------------- end of plotting -----------------------
        }

        if (ui->checkBox->checkState() == Qt::Checked){
            ui->widget_2->setVisible(true);
            ui->widget->setVisible(false);
            ui->qCustomPlot->addGraph();
            ui->qCustomPlot->graph(0)->setData(thix,thiy);
            ui->qCustomPlot->xAxis->setLabel("voltage (V)");
            ui->qCustomPlot->yAxis->setLabel("current (A)");
            ui->qCustomPlot->xAxis->setRange(0,xsize+1);  //set the range of scope, otherwise use default range
            ui->qCustomPlot->yAxis->setRange(0,ysize+1);
        }

        ui->qCustomPlot->replot(); //important to re-plot
        ui->qCustomPlot->setVisible(true);
        // display the previous/next button


    }
}


