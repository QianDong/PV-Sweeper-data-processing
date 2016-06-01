#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent)
{

}

//in mylabel.h, select mousePressEvent function, rightclick-refactor-add definition in mylabel.cpp
void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    emit Previous_Pressed();
    emit Next_Pressed();
}


