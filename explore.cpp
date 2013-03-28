
#include "explore.h"
#include "ui_explore.h"

Explore::Explore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Explore)
{
    ui->setupUi(this);

    QFileInfoList fil = QDir::drives();
    for(int i = 0; i < fil.count(); i++)
    {
        ui->cbb_drivers->addItem(fil.at(i).filePath());
    }

    m_lastPath = QDir::homePath();
    QSettings set("config.inf", QSettings::IniFormat);
    if (set.status() == QSettings::NoError)
    {
        m_lastPath = set.value("LastPath").toString();
    }
    QString driver = m_lastPath.mid(0, 2);
    for(int i = 0; i < ui->cbb_drivers->count(); i++)
    {
        if (ui->cbb_drivers->itemText(i).contains(driver))
        {
            ui->cbb_drivers->setCurrentIndex(i);
            break;
        }
    }
    DispExporeInfo(ui->cbb_drivers->currentText());

}

void Explore::closeEvent ( QCloseEvent * e )
{
    QSettings set("config.inf", QSettings::IniFormat);
    if (set.status() == QSettings::NoError)
    {
        set.setValue("LastPath", m_lastPath);
        set.setValue("LastFile", mSelFileName);
    }
}

void Explore::DispExporeInfo(QString pos)
{

    m_model.setRootPath(pos);
    m_model.setReadOnly(true);
    m_model.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    ui->trv_dir->setModel(&m_model);
    ui->trv_dir->setRootIndex(m_model.index(pos));
    ui->trv_dir->setIconSize(QSize(100, 100));
    ui->trv_dir->setColumnHidden(1, true);
    ui->trv_dir->setColumnHidden(2, true);
    ui->trv_dir->setColumnHidden(3, true);
    ui->trv_dir->setExpanded(m_model.index(m_lastPath), true);
    SearchEbookFile(m_lastPath);
}

bool Explore::SearchEbookFile(QString path)
{
    QListWidget *lstW = ui->lsw_file;

    if (m_ebookFList.count() > 0)
    {
        m_ebookFList.clear();
    }

    if (lstW->count() > 0)
    {
        lstW->clear();

    }


   QDir filePath(path);
   QStringList filters;
   filters << "*.txt";
   filePath.setNameFilters(filters);
    filePath.setFilter( QDir::Files | QDir::NoSymLinks );
    filePath.setSorting( QDir::Size | QDir::Reversed );
    if (filePath.count() == 0)
    {
        qDebug("No medial file found");
        return false;
    }

    if (m_ebookFList.count())
    {
        m_ebookFList.clear();
    }
    m_ebookFList = filePath.entryList();

    for(int i = 0; i < m_ebookFList.count(); i++)
     {

        QString fileName = m_ebookFList.at(i);
        lstW->addItem(new QListWidgetItem(fileName));

    }

    lstW->item(0)->setSelected(true);
   // m_curPlayIndx = 0;
    return true;
}
Explore::~Explore()
{
    delete ui;
}

void Explore::on_trv_dir_clicked(QModelIndex index)
{
    if (m_lastPath != m_model.filePath(index))
    {
        m_lastPath = m_model.filePath(index);
        qDebug(m_lastPath.toStdString().data());
        SearchEbookFile(m_lastPath);
    }

}

void Explore::on_cbb_drivers_currentIndexChanged(int index)
{
    DispExporeInfo(ui->cbb_drivers->itemText(index));
}

void Explore::on_lsw_file_itemDoubleClicked(QListWidgetItem* item)
{
    mSelFileName = m_lastPath + "/" + item->text();
    close();
}
