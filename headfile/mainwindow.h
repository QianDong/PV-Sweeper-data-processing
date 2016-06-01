#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>      //added in order to use customized label button

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void Previous_Pressed();   //manually added in order to use customized label button

    void Next_Pressed();

    void on_checkBox_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
