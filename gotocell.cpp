#include "gotocell.h"
#include "ui_gotocell.h"

GoToCell::GoToCell(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToCell)
{
    ui->setupUi(this);
    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(enableOkButton()));
}

GoToCell::~GoToCell()
{
    delete ui;
}

void GoToCell::enableOkButton()
{
    ui->okButton->setEnabled(ui->lineEdit->hasAcceptableInput());
}
