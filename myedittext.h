#ifndef MYEDITTEXT_H
#define MYEDITTEXT_H

#include <QtGui>

class MyEditText : public QTextEdit
{
    Q_OBJECT
public:
    MyEditText(int width, int height, QWidget *parent = 0);
    ~MyEditText();
private:
    int m_width;
    int m_height;
protected:
    void keyReleaseEvent(QKeyEvent * e);
    void mousePressEvent ( QMouseEvent * e );
signals:
    void PageAction(bool up);
};

#endif // MYEDITTEXT_H
