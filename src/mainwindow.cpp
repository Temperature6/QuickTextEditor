#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QDebug>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(331,260);
    //图标
    setWindowIcon(QIcon(":appicon.ico"));
    ui->copy_btn->setIcon(QIcon(":copy_text.ico"));
    ui->clear_button->setIcon(QIcon(":delete_text.ico"));
    //菜单栏样式表
    ui->menuBar->setStyleSheet("QMenuBar{background-color:#e8e8e8;}");
    //槽和函数链接
    void(QSpinBox::*changed)(int) = &QSpinBox::valueChanged;
    connect(ui->copy_btn, &QPushButton::clicked, this, &MainWindow::CopyText);
    connect(ui->textsize, changed, this, &MainWindow::ChangeTextSize);
    connect(ui->_save, &QAction::triggered, this, &MainWindow::SaveText);
    connect(ui->copy_all, &QAction::triggered, this, &MainWindow::ChangeWayCopy);
    connect(ui->cut_all, &QAction::triggered, this, &MainWindow::ChangeWayCut);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CopyText()
{
    QClipboard *clipboard = QApplication::clipboard(); //系统剪贴板指针
    QString str = ui->textEdit->toPlainText();
    clipboard->setText(str);
    if (!copy) //复制
    {
        ui->textEdit->setText("");
    }
}

void MainWindow::ChangeTextSize()
{
    QFont font = ui->textEdit->font();
    font.setPointSize(ui->textsize->value());
    ui->textEdit->setFont(font);
}

void MainWindow::SaveText()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存文本"),"",tr("文本文档(*.txt)"));
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::critical(this, "错误", tr("文件保存失败"),QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        stream.flush();
        file.close();
    }

}

void MainWindow::ChangeWayCopy()
{
    if (copy)
        return;
    //由剪切到复制
    ui->copy_btn->setText("复制全部");
    copy = true;
    ui->copy_btn->setIcon(QIcon(":copy_text.ico"));
}

void MainWindow::ChangeWayCut()
{
    if (!copy)
        return;
    ui->copy_btn->setText("剪切全部");
    copy = false;
    ui->copy_btn->setIcon(QIcon(":cut.ico"));
}
