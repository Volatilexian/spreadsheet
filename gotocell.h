#ifndef GOTOCELL_H
#define GOTOCELL_H

#include <QDialog>

namespace Ui {
class GoToCell;
}

class GoToCell : public QDialog
{
    Q_OBJECT
    
public:
    explicit GoToCell(QWidget *parent = 0);
    ~GoToCell();
    
//private:
    Ui::GoToCell *ui;

private slots:
    void enableOkButton();
};

#endif // GOTOCELL_H
