#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool copy = false;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CopyText();
    void ChangeTextSize();
    void SaveText();
    void ChangeWayCopy();
    void ChangeWayCut();
    void OpenGitHub();
private slots:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
