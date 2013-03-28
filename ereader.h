#ifndef EREADER_H
#define EREADER_H
#include <QtGui>
#include "myedittext.h"
namespace Ui {
    class EReader;
}

class EReader : public QMainWindow
{
    Q_OBJECT
public:
    explicit EReader(QWidget *parent = 0);
    ~EReader();


private:
    Ui::EReader *ui;
    QLabel *m_timeTips;//show time
    QSize m_winSize;
    MyEditText *m_text;
    QFile m_efile;//file op handle

    QList<int>m_bookMark;//history bookmark
    int m_curBookMarkIdx;//current bookmark
    int m_lastBookMark;//the last stored bookmark
    int m_bytesInLine;//bytes in a line
    int m_linesInPage;//lines in a page
    QString m_curFileName;//current file name opened
    bool m_openNewBook;//if open a new book, set true
    int m_bookSize;//the size of book in bytes.
    bool m_endOfBook;


protected:
    void paintEvent ( QPaintEvent * event );
private:
    void SetWindowStyle(const QString &name);
    void ChangePalette();
    void ReadFile(QString &fileName);
    void GetPageData(bool append);
    void GetLastBookMark(QString &fileName);
    bool ComStrAndDec(QString &str, int dec);
    int StrToIntE(QString &str);


private slots:
    void on_pbtn_bookmarkSettings_clicked();
    void on_pbtn_help_clicked();
    void on_pbtn_bookmark_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void updateTimeTips(void);
    void KeyMouseCtrl(bool done);
};

#endif // EREADER_H
