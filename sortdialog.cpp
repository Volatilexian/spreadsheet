#include "sortdialog.h"
#include "ui_sortdialog.h"

SortDialog::SortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SortDialog)
{
    ui->setupUi(this);
    ui->secondaryGroupBox->hide();
    ui->tertiaryGroupBox->hide();

    setColumnRange('A', 'Z');
}

SortDialog::~SortDialog()
{
    delete ui;
}

void SortDialog::setColumnRange(QChar first, QChar last)
{
    ui->columnComboBox->clear();
    ui->columnComboBox_2->clear();
    ui->columnComboBox_3->clear();

    ui->columnComboBox_2->addItem(tr("None"));
    ui->columnComboBox_3->addItem(tr("None"));
    ui->columnComboBox->setMinimumSize(ui->columnComboBox_2->sizeHint());

    QChar ch = first;
    while(ch != last)
    {
        ui->columnComboBox->addItem(QString(ch));
        ui->columnComboBox_2->addItem(QString(ch));
        ui->columnComboBox_3->addItem(QString(ch));
        ch = ch.unicode() + 1;
    }
}
