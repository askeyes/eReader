#include <QSettings>
#include "bookmarkbrowser.h"
#include "ui_bookmarkbrowser.h"
#include "ereader.h"

BookmarkBrowser::BookmarkBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkBrowser)
{
    ui->setupUi(this);
    m_selectBookmark = -1;
    setWindowFlags(windowFlags()& ~Qt::WindowCloseButtonHint);
}

BookmarkBrowser::~BookmarkBrowser()
{
    delete ui;
}
void BookmarkBrowser::SetBookmarkTag(QString &bookmarkTag)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
    m_bookmarkTag = bookmarkTag;
    QSettings set("bookmark.inf", QSettings::IniFormat);
    if (set.status() == QSettings::NoError)
    {
        ui->lab_fileName->setText(m_bookmarkTag);
        m_bookmarkTag.replace("/", "\\");
        set.beginGroup(m_bookmarkTag);
        int last = set.value("last").toInt();
        m_bookmarkTag.replace("\\", "/");
        QString bv;
        for(int i = 1; i <= last; i++)
        {
            bv =set.value(QString::number(i,10)).toString();
            ui->lwt_bookmarks->addItem(new QListWidgetItem(bv));
        }
    }
}
int BookmarkBrowser::GetSelectedBookmark()
{

    return m_selectBookmark;
}

void BookmarkBrowser::on_pbtn_ok_clicked()
{
    QListWidgetItem *item = ui->lwt_bookmarks->currentItem();
    if (item != NULL)
    {
        ProcessSelectedItem(item->text());
    }
    else
    {
        close();
    }



}
int BookmarkBrowser::StrToIntE(QString &str)
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
void BookmarkBrowser::ProcessSelectedItem(QString itemStr)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gbk"));
    if(itemStr.isNull() || itemStr.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Hi"));
        msgBox.setInformativeText(tr("ÄãÎ´Ñ¡Ôñ±êÇ©"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    m_selectBookmark = StrToIntE(itemStr);
    qDebug("m_selectBookmark=%d", m_selectBookmark);
    close();
}

void BookmarkBrowser::on_lwt_bookmarks_itemDoubleClicked(QListWidgetItem* item)
{
    ProcessSelectedItem(item->text());
}
