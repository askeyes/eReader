#ifndef BOOKMARKBROWSER_H
#define BOOKMARKBROWSER_H

#include <QDialog>
#include <QListWidgetItem>
namespace Ui {
    class BookmarkBrowser;
}

class BookmarkBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkBrowser(QWidget *parent = 0);
    ~BookmarkBrowser();
    void SetBookmarkTag(QString &bookmarkTag);
    int GetSelectedBookmark();

private:
    Ui::BookmarkBrowser *ui;
    QString m_bookmarkTag;
    int m_selectBookmark;
private:
    int StrToIntE(QString &str);
    void ProcessSelectedItem(QString itemStr);

private slots:
    void on_lwt_bookmarks_itemDoubleClicked(QListWidgetItem* item);
    void on_pbtn_ok_clicked();
};

#endif // BOOKMARKBROWSER_H
