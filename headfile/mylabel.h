#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>       //auto-created, because this class is inherited from QLabel
#include <QMouseEvent>  //added
#include <QEvent>       //added
#include <QDebug>       //added


class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);

    //void mouseMoveEvent(QMouseEvent * ev);  //if need this curser-move event
    void mousePressEvent(QMouseEvent * ev);  //added. *event
    //void mouseReleaseEvent(QMouseEvent * ev);

signals:
    void Previous_Pressed();
    void Next_Pressed();

public slots:

};

#endif // MYLABEL_H
