#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);

    createActions();
    createMenus();
    createcontexMenu();
    //createToolBars();
    createStatusBar();

    readSettings();

    findDialog = 0;
    setCurrentFile("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    //the "file" on menu
    connect(ui->action_New, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAs()));
    //recent files
    for(int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(close()));

    //the "edit" on menu
    connect(ui->actionCu_t, SIGNAL(triggered()), spreadsheet, SLOT(cut()));
    connect(ui->action_Copy, SIGNAL(triggered()), spreadsheet, SLOT(copy()));
    connect(ui->action_Paste, SIGNAL(triggered()), spreadsheet, SLOT(paste()));
    connect(ui->action_Delete, SIGNAL(triggered()), spreadsheet, SLOT(del()));
    connect(ui->action_Column, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentColumn()));
    connect(ui->action_Row, SIGNAL(triggered()), spreadsheet, SLOT(selectCurrentRow()));
    connect(ui->action_All, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));
    connect(ui->action_Find, SIGNAL(triggered()), this, SLOT(find()));
    connect(ui->action_Go_To_Cell, SIGNAL(triggered()), this, SLOT(goToCell()));

    //the "option" on menu
    connect(ui->action_Show_Grid, SIGNAL(triggered(bool)), spreadsheet, SLOT(setshowGrid(bool)));

    //the "help" on menu
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    //"file"
    separatorAction = ui->menu_File->addSeparator();
    for(int i = 0; i != MaxRecentFiles; ++i)
        ui->menu_File->addAction(recentFileActions[i]);
}

void MainWindow::createcontexMenu()
{
    spreadsheet->addAction(ui->actionCu_t);
    spreadsheet->addAction(ui->action_Copy);
    spreadsheet->addAction(ui->action_Paste);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel("W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    ui->statusBar->addWidget(locationLabel);
    ui->statusBar->addWidget(formulaLabel, 1);
    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()), this, SLOT(spreadsheetModified()));
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::newFile()
{
    if(okToContinue())
    {
        spreadsheet->clear();
        setCurrentFile("");
    }
}

bool MainWindow::okToContinue()
{
    if(isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                                     tr("The document has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::No
                                     | QMessageBox::Cancel);
        if(r == QMessageBox::Yes)
            return save();
        else if(r == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::open()
{
    if(okToContinue())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Spreadsheet"), ".",
                                                        tr("spreadsheet files (*.sp)"));
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::loadFile(const QString &fileName)
{
    if(!spreadsheet->readFile(fileName))
    {
        ui->statusBar->showMessage(tr("File loaded"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    ui->statusBar->showMessage(tr("File loaded"), 2000);
    return true;
}

bool MainWindow::save()
{
    if(curfile.isEmpty())
        return saveAs();
    else
        return saveFile(curfile);
}

bool MainWindow::saveFile(const QString &fileName)
{
    if(!spreadsheet->writeFile(fileName))
    {
        ui->statusBar->showMessage(tr("Saving canceled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    ui->statusBar->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save spreadsheet"), ".",
                                                    tr("Spreadsheet files(*.sp)"));
    if(fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::find()
{
    if(!findDialog)
    {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(QString,Qt::CaseSensitivity)), spreadsheet, SLOT(findNext(QString,Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(QString,Qt::CaseSensitivity)), spreadsheet, SLOT(findPrevious(QString,Qt::CaseSensitivity)));
    }
    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void MainWindow::goToCell()
{
    GoToCell *dialog = new GoToCell(this);
    if(dialog->exec())
    {
        QString str = dialog->ui->lineEdit->text().toUpper();//it's wrong? how can i access the lineEdit of the class GoToCell
        spreadsheet->setCurrentCell(str.mid(1).toInt()-1, str[0].unicode()-'A');
    }
}

void MainWindow::sort()
{
    SortDialog dialog(this);
    QTableWidgetSelectionRange range = spreadsheet->selectedRange();
    dialog.setColumnRange('A' + range.leftColumn(), 'A' + range.rightColumn());
    if(dialog.exec())
    {
        SpreadsheetCompare compare;
        compare.keys[0] = dialog.ui->columnComboBox->currentIndex();
        compare.keys[1] = dialog.ui->columnComboBox_2->currentIndex() - 1;
        compare.keys[2] = dialog.ui->columnComboBox_3->currentIndex() - 1;
        compare.ascending[0] = (0 == dialog.ui->columnComboBox->currentIndex());
        compare.ascending[1] = (0 == dialog.ui->columnComboBox_2->currentIndex());
        compare.ascending[2] = (0 == dialog.ui->columnComboBox_3->currentIndex());
        spreadsheet->sort(compare);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About spreadsheet"),
                       tr("<h2>Spreadsheet 1.1</h2>"
                          "<p>Copyright &copy; 2008 Software Inc."
                          "<p>Spreadsheet is a small application that "
                          "demonstrates QAction, QMainWindow, QMenuBar, "
                          "QStatusBar, QTableWidget, QToolBar, and many other "
                          "Qt classes."));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue())
    {
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curfile = fileName;
    setWindowModified(false);
    QString showName = tr("Untitled");
    if(!curfile.isEmpty())
    {
        showName = strippedName(curfile);
        recentFiles.removeAll(curfile);
        recentFiles.prepend(curfile);
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(showName).arg(tr("Spreadsheet")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);

    while(i.hasNext())
        if(!QFile::exists(i.next()))
            i.remove();

    for(int j = 0; j != MaxRecentFiles; ++j)
    {
        if(j < recentFiles.count())
        {
            QString text = tr("&%1 %2").arg(j + 1).arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
            recentFileActions[j]->setVisible(false);
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::openRecentFile()
{
    if(okToContinue())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if(action)
            loadFile(action->data().toString());
    }
}


void MainWindow::writeSettings()
{
    QSettings settings("Software Inc.", "Spreadsheet");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", ui->action_Show_Grid->isChecked());
    settings.setValue("autoRecalc", ui->action_Auto_Recalculate->isChecked());
}

void MainWindow::readSettings()
{
    QSettings settings("Softwrae Inc.", "Spreadsheet");

    restoreGeometry(settings.value("geometry").toByteArray());
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
    bool showGrid = settings.value("showGrid", true).toBool();
    ui->action_Show_Grid->setChecked(showGrid);
    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    ui->action_Auto_Recalculate->setChecked(autoRecalc);
}
