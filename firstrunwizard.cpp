#include "firstrunwizard.h"
#include <QtGui>
#include <QDebug>

FirstRunWizard::FirstRunWizard(QWidget *parent) : QWizard(parent)
{
    addPage(new NickPage);
    sharingpage = new SharingPage;
    addPage(sharingpage);

    setWindowTitle(tr("Streamberry first run wizard"));
}

void FirstRunWizard::accept()
{
    QString nick = field("nickname").toString();
    qDebug() << "nick is " << nick;
    //TODO: tell the database what nick was chosen

    sharingpage->getSelectedFiles();
    //TODO: tell the database which filepaths were selected
    // ^ better to return strings rather than passing the db object in?

    QDialog::accept();
}


NickPage::NickPage(QWidget *parent) : QWizardPage(parent)
{
    QLabel *intro = new QLabel(tr("Welcome to the first run wizard for Streamberry. Please select a nickname so that other people on your network can identify you."));
    intro->setWordWrap(true);
    QLineEdit *nickedit = new QLineEdit();
    QVBoxLayout *layout = new QVBoxLayout;
    registerField("nickname",nickedit);
    layout->addWidget(intro);
    layout->addWidget(nickedit);
    setLayout(layout);
}


SharingPage::SharingPage(QWidget *parent) : QWizardPage(parent)
{
    QLabel *label = new QLabel(tr("Please select which files and folders you would like to appear in your media library and share with others on your local network."));
    label->setWordWrap(true);

    tree = new QTreeView(this);
    model = new QFileSystemModel;
    model->setRootPath("");
    tree->setModel(model);
    tree->setSelectionMode(QAbstractItemView::MultiSelection);
    registerField("tree",tree);

    QVBoxLayout *layout = new QVBoxLayout;
    tree->setColumnWidth(0,300);
    //tree->setFixedSize(500,550);
    layout->addWidget(label);
    layout->addWidget(tree);
    setLayout(layout);
}

QString SharingPage::getSelectedFiles()
{
    QModelIndexList sindexes = tree->selectionModel()->selectedRows();
    int sindlength = sindexes.length();
    QStringList strs;
    for(int i = 0; i < sindlength; i++)
    {
        strs.append(model->filePath(sindexes.at(i)));
    }
    return strs.join(";");
}