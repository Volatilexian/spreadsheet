#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();
    
private:
    Ui::FindDialog *ui;

signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots:
    void enableFindButton(const QString &text);
    void findClicked();
};

#endif // FINDDIALOG_H
