#ifndef EXPLORE_H
#define EXPLORE_H
#include <QtGui>
#include <QDialog>

namespace Ui {
    class Explore;
}

class Explore : public QDialog
{
    Q_OBJECT

public:
    explicit Explore(QWidget *parent = 0);
    ~Explore();
    QString mSelFileName;
private:
    bool SearchEbookFile(QString path);
    void DispExporeInfo(QString pos);
private:
    QString m_lastPath;
    Ui::Explore *ui;
    QStringList m_ebookFList;
    QFileSystemModel m_model;
protected:
    void closeEvent ( QCloseEvent * e );
private slots:
    void on_lsw_file_itemDoubleClicked(QListWidgetItem* item);
    void on_cbb_drivers_currentIndexChanged(int index);
    void on_trv_dir_clicked(QModelIndex index);
};

#endif // EXPLORE_H
