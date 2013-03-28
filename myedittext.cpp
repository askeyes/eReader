#include "myedittext.h"

MyEditText::MyEditText(int width, int height, QWidget *parent):
        QTextEdit(parent)
{
    m_width = width;
    m_height = height;
    resize(width, height);
    setReadOnly(true);

}
MyEditText::~MyEditText()
{

}
void MyEditText::mousePressEvent ( QMouseEvent * e )
{

    if (e->pos().x() < m_width / 4)
    {

        emit PageAction(true);
    }
    else if (e->pos().x() > (m_width - m_width / 4))
    {

        emit PageAction(false);
    }
}
void MyEditText::keyReleaseEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Right)
    {

        emit PageAction(false);
    }
    else if (e->key() == Qt::Key_Left)
    {

        emit PageAction(true);
    }

}
