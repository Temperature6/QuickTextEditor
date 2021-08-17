#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QJsonObject config;
    bool copy = false;
    int size = 10;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CopyText();
    void ChangeTextSize();
    void SaveText();
    void ChangeWayCopy();
    void ChangeWayCut();
    void OpenGitHub();
    void closeEvent(QCloseEvent *event);
private slots:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
