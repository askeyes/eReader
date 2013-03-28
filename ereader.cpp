#include <QDesktopWidget>

#include "ereader.h"
#include "ui_ereader.h"
#include "imageSytele/imageSytele.h"
#include "explore.h"
#include "bookmarkbrowser.h"

EReader::EReader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EReader)
{
    ui->setupUi(this);
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    QSettings set("config.inf", QSettings::IniFormat);

    if (set.status() == QSettings::NoError)
    {
        m_curFileName = set.value("LastFile","").toString();
        qDebug(m_curFileName.toStdString().data());
    }
    m_winSize.setWidth(QApplication::desktop()->width());
    m_winSize.setHeight(QApplication::desktop()->height());

    //qDebug("dw=%d", m_winSize.width());
    //qDebug("dh=%d", m_winSize.height());

    //qDebug("%d, %d", QApplication::desktop()->width(), QApplication::desktop()->height());
    //setMaximumWidth(QApplication::desktop()->width());
    //setMaximumHeight(QApplication::desktop()->height());
    QString dt = QDate::currentDate().toString("yyyy-MM-dd");
    dt += " ";
    dt += QTime::currentTime().toString("hh:mm");
    m_timeTips = new QLabel(this);
    //statusBar()->addWidget(m_timeTips);
    //setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::FramelessWindowHint);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //showMaximized();


    //qDebug("ww=%d", this->width());
    //qDebug("wh=%d", this->height());
    QRect rec = QApplication::desktop()->availableGeometry();

    this->resize(rec.width(), rec.height()+25);
    int textMargin = 70;
    int textWidth = m_winSize.width() - textMargin;
    int textHeight = m_winSize.height() - 120;
    int textTop = 40;



    m_text = new MyEditText(textWidth, textHeight, this);
    m_text->move(textMargin/2, textTop);
    m_text->show();
    m_text->setFocus();
    m_text->setFont(QFont("宋体", 26));
    m_text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#if 0
    QTextDocument* doc = m_text->document();
    QTextCursor textCursor = m_text->textCursor();
    for(QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
            QTextBlockFormat textBlockFormat = it.blockFormat();
            textBlockFormat.setBottomMargin(40);//设置行间距
             textCursor.setBlockFormat(textBlockFormat);
            m_text->setTextCursor(textCursor);
    }
#endif
    m_bookSize = 1;
    m_endOfBook = true;

    //m_text->setOverwriteMode(Qt::ArrowCursor);


    QFontMetrics  fm = m_text->fontMetrics();
    int chnFontWidth = fm.width("a");
    int chnFontHeight = fm.height();
    qDebug("chnFontWidth=%d", chnFontWidth);
    qDebug("chnFontHeight=%d", chnFontHeight);
    m_linesInPage = textHeight / chnFontHeight - 2;
    m_bytesInLine = textWidth  / chnFontWidth;
    qDebug("m_bytesInLine=%d", m_bytesInLine);
    qDebug("m_linesInPage=%d", m_linesInPage);


    connect(m_text, SIGNAL(PageAction(bool)), this, SLOT(KeyMouseCtrl(bool)));

    if (!m_curFileName.isNull() && !m_curFileName.isEmpty())
    {
        ReadFile(m_curFileName);
    }
    //qDebug("linecount=%d", m_text->document()->lineCount());
    int timeTipsLeft = 50;
    int timeTipHeight = 30;
    m_timeTips->setFont(QFont("宋体", 12));
    m_timeTips->resize(m_winSize.width()-timeTipsLeft, timeTipHeight);
    m_timeTips->move(timeTipsLeft, textTop + textHeight + 15);
    updateTimeTips();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeTips()));
    timer->start(1000 * 60);

#if 0
    QStringList akey = QStyleFactory::keys();
    for(int i = 0; i < akey.count(); i++)
    {
        QString str = akey.at(i);
        qDebug(str.toStdString().data());
    }
#endif
    SetWindowStyle("ImageStyle");
}

EReader::~EReader()
{
    delete ui;
}


void EReader::KeyMouseCtrl(bool done)
{
    if(done)//up
    {
        //qDebug("truekkkk.");
        if (m_curBookMarkIdx == 0)
        {
            return;
        }
        if (m_endOfBook)
        {
            m_endOfBook = false;
        }
        m_curBookMarkIdx--;
        GetPageData(false);
    }
    else//down
    {
        if (m_endOfBook)
        {
            return;
        }
        m_curBookMarkIdx++;

        if (m_curBookMarkIdx == m_bookMark.size() - 1)
        {
            GetPageData(true);

        }
        else if (m_curBookMarkIdx < m_bookMark.size() - 1)
        {

            GetPageData(false);
        }

    }
    m_lastBookMark = m_bookMark.at(m_curBookMarkIdx);

}

void EReader::ChangePalette()
{
#if 0
    if (useStylePaletteCheckBox->isChecked())
    {
        QApplication::setPalette(QApplication::style()->standardPalette());
    }
    else
    {
        QApplication::setPalette(originalPalette);
    }
#endif
    QApplication::setPalette(QApplication::style()->standardPalette());
}
/*
 set the window sytle.
*/
void EReader::SetWindowStyle(const QString &styleName)
{
    if (styleName == "ImageStyle")
    {
            QApplication::setStyle(new ImageStyle);
    }
    else
    {
            QApplication::setStyle(QStyleFactory::create(styleName));
    }
    ChangePalette();
}
/*
show the time
*/
void EReader::updateTimeTips()
{
    QString dt = QDate::currentDate().toString("yyyy-MM-dd");
    dt += " ";
    dt += QTime::currentTime().toString("hh:mm");
    dt += "  ";
    dt += m_curFileName;
    dt += "     @ ";

    int rat;
    if (m_bookSize == 1)
    {
        rat = 0;
    }
    else
    {
        rat = this->m_bookMark.at(this->m_curBookMarkIdx+1) * 100 /m_bookSize;
    }

    dt += QString::number(rat);
    dt += "%";
    m_timeTips->setText(dt);
}
void EReader::paintEvent ( QPaintEvent * event )
{
    //qDebug("abc");
}

/*
get page data for display
*/
void EReader::GetPageData(bool append)
{
    QByteArray line, pageData=NULL;
    int lineCount=0,readOffset=0;
    int offset = m_bookMark.at(m_curBookMarkIdx);
    qDebug("m_curBookMarkIdx0=%d, offset = %d", m_curBookMarkIdx, offset);

    if(m_efile.isOpen())
    {
        do{
            //qDebug("offset=%d", offset);
            m_efile.seek(offset);
            while(true)
            {
                line = m_efile.readLine(m_bytesInLine);
                if (line.isNull()||line.isEmpty())
                {
                    m_endOfBook = true;
                    break;
                }

                //qDebug("linelen=%d", line.size());
                lineCount++;
                if (lineCount == m_linesInPage)
                {
                    break;
                }
                if (pageData.isNull())
                {
                    pageData = line;
                }
                else
                {
                    pageData += line;
                }
                readOffset += line.size();
            }
            if (!pageData.isNull())
            {
                unsigned char byte;
                int i = 0;
                while(i < pageData.size())//check whole word
                {
                    byte = pageData.at(i);
                    if (byte & 0x80)
                    {
                        i++;
                    }
                    i++;
                }
                if (i > pageData.size())
                {
                    readOffset--;
                }
            }

            if (append)
            {
                m_bookMark.append(offset+readOffset);
            }

            if (m_lastBookMark-offset<m_bytesInLine || !m_openNewBook)
            {
                break;
            }
            offset += readOffset;
            readOffset = 0;
            lineCount = 0;
            pageData = NULL;

        }while(m_openNewBook);

    }
    if (m_openNewBook)
    {
        m_curBookMarkIdx = m_bookMark.size() - 2;
    }

    if (!pageData.isNull()&&!pageData.isEmpty())
    {
        QString pageStr = QTextCodec::codecForName("GBK")->toUnicode(pageData);
        m_text->setText(pageStr);
        updateTimeTips();
    }
#if 0
    for(int j = 0; j < m_bookMark.size(); j++)
    {
        qDebug("m_BookMark(%d)=%d", j, m_bookMark.at(j));
    }
#endif
}

void EReader::ReadFile(QString &fileName)
{
    if (fileName.isNull()||fileName.isEmpty())
    {
        return;
    }
    m_endOfBook = false;
    m_bookSize = 1;
    GetLastBookMark(fileName);
    m_openNewBook = true;
    m_efile.setFileName(fileName);
    m_bookMark.clear();
    m_bookMark.append(0);
    m_curBookMarkIdx = 0;
    if (m_efile.isOpen())
    {
        m_efile.close();
    }
    if (m_efile.open(QIODevice::ReadOnly))
    {
        m_bookSize = m_efile.size();
        GetPageData(true);
    }
    m_openNewBook = false;
}

void EReader::on_pushButton_clicked()
{
    Explore ep(this);
    ep.exec();
    if (ep.mSelFileName.isNull()||ep.mSelFileName.isEmpty())
    {
        QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
        QMessageBox msgBox;
        msgBox.setText(tr("Hi"));
        msgBox.setInformativeText(tr("没有书籍选中"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    m_curFileName = ep.mSelFileName;
    ReadFile(ep.mSelFileName);
}
void EReader::on_pushButton_2_clicked()
{

    if(m_efile.isOpen())
    {
        m_efile.close();
    }
    exit(0);
}
void EReader::GetLastBookMark(QString &fileName)
{
    m_lastBookMark = 0;
    QSettings bookMarkSet("bookmark.inf", QSettings::IniFormat);
    if (bookMarkSet.status() == QSettings::NoError)
    {

        fileName.replace("/", "\\");
        bookMarkSet.beginGroup(fileName);

        QString last = bookMarkSet.value("last","1").toString();

        QString bookMarkStr = bookMarkSet.value(last,"0@").toString();

        m_lastBookMark = StrToIntE(bookMarkStr);

        bookMarkSet.endGroup();
        fileName.replace("\\", "/");

    }
}
int EReader::StrToIntE(QString &str)
{
    char *pStr = str.toAscii().data();
    int ret = 0;
    for(int i = 0; i < str.size(); i++)
    {
        if (pStr[i] == '@')
        {
            break;
        }
        ret *= 10;
        ret += pStr[i] - 0x30;
    }
    return ret;
}

bool EReader::ComStrAndDec(QString &str, int dec)
{
    int ret = StrToIntE(str);
    return dec == ret?true:false;
}

void EReader::on_pbtn_bookmark_clicked()
{
    if (m_lastBookMark == 0)
    {
        return;
    }
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
    QSettings set("bookmark.inf", QSettings::IniFormat);

    if (set.status() == QSettings::NoError)
    {
        m_curFileName.replace("/", "\\");
        set.beginGroup(m_curFileName);
        int last = set.value("last").toInt();
        m_curFileName.replace("\\", "/");
        QString bv;
        for(int i = 1; i <= last; i++)
        {
            bv =set.value(QString::number(i,10)).toString();
            if (ComStrAndDec(bv,m_lastBookMark))
            {
                QMessageBox msgBox;
                msgBox.setText(tr("书签已经存在"));
                msgBox.setInformativeText(tr("将不再保存当前书签"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
                return;
            }
        }
        QString value = QString::number(m_lastBookMark, 10);
        value += "@";
        value += QDate::currentDate().toString("yyyy-MM-dd");
        value += " ";
        value += QTime::currentTime().toString("hh:mm:ss");
        set.setValue("last", QString::number(last+1,10));
        set.setValue(QString::number(last+1,10), value);
        set.endGroup();

        QMessageBox sucMsgBox;
        sucMsgBox.setText(tr("保存书签成功"));
        sucMsgBox.setInformativeText(tr("(::)"));
        sucMsgBox.setStandardButtons(QMessageBox::Ok);
        sucMsgBox.exec();
    }
}
void EReader::on_pbtn_help_clicked()
{

}

void EReader::on_pbtn_bookmarkSettings_clicked()
{
    BookmarkBrowser bb;
    bb.SetBookmarkTag(m_curFileName);
    bb.exec();
    int selectedBookmark = bb.GetSelectedBookmark();
    if (selectedBookmark == -1)
    {
        return;
    }
    int i;
    for(i = 0; i < this->m_bookMark.size(); i++)
    {
        if (this->m_bookMark.at(i) == selectedBookmark)
        {
            break;
        }
    }
    if (i < this->m_bookMark.size())
    {
        this->m_curBookMarkIdx = i;
        GetPageData(false);
    }
}
