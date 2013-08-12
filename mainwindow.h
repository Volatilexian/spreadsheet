#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QAction>
#include <QTableWidgetSelectionRange>
#include <QSettings>
#include <QStringList>

#include "finddialog.h"
#include "gotocell.h"
#include "sortdialog.h"
#include "spreadsheet.h"
#include "ui_gotocell.h"
#include "ui_sortdialog.h"

namespace Ui {
class MainWindow;
}

//forward declaration
class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void find();
    void goToCell();
    void sort();
    void about();
    void openRecentFile();
    void updateStatusBar();
    void spreadsheetModified();

private:
    Ui::MainWindow *ui;
    Spreadsheet *spreadsheet;
    FindDialog *findDialog;
    //status bar
    QLabel *locationLabel;
    QLabel *formulaLabel;
    QStringList recentFiles;
    QString curfile;
    enum{ MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    void createActions();
    void createMenus();
    void createcontexMenu();
    //void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
};

#endif // MAINWINDOW_H
