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
    bool copy = true;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CopyText();
    void ChangeTextSize();
    void SaveText();
    void ChangeWayCopy();
    void ChangeWayCut();
private slots:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
