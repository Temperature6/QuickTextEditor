#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QDebug>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QArrayData>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //图标
    setWindowIcon(QIcon("://pic//appicon.ico"));
    //菜单栏样式表
    ui->menuBar->setStyleSheet("QMenuBar{background-color:#e8e8e8;}");
    //配置文件
    QFile file("config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString config_str = file.readAll();
    file.close();
    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(config_str.toUtf8(),&parseJsonErr);

    if(!(parseJsonErr.error == QJsonParseError::NoError))
    {
        QMessageBox::critical(NULL, "错误", "解析json文件错误!", QMessageBox::Ok);
        ui->textsize->setValue(size);
        ui->textEdit->setFontPointSize(size);
        MainWindow::close();
    }
    //设置初始化
    else
    {
        config = document.object();
        copy = config["copy"].toInt();
        ui->textsize->setValue(config["FontSize"].toInt());
        ChangeTextSize();
    }
    //初始化文本
    QString InStr;
    QFile tempfile("temp");
    tempfile.open(QIODevice::ReadOnly|QIODevice::Text);
    while (!tempfile.atEnd())
    {
        QByteArray arr = tempfile.readLine();
        InStr.append((QString)arr);
    }
    ui->textEdit->setText(InStr.toUtf8());
    ui->textEdit->moveCursor(QTextCursor::End);
    //槽和函数链接
    void(QSpinBox::*changed)(int) = &QSpinBox::valueChanged;
    connect(ui->copy_btn, &QPushButton::clicked, this, &MainWindow::CopyText);
    connect(ui->textsize, changed, this, &MainWindow::ChangeTextSize);
    connect(ui->_save, &QAction::triggered, this, &MainWindow::SaveText);
    connect(ui->copy_all, &QAction::triggered, this, &MainWindow::ChangeWayCopy);
    connect(ui->cut_all, &QAction::triggered, this, &MainWindow::ChangeWayCut);
    connect(ui->open_github, &QAction::triggered, this, &MainWindow::OpenGitHub);

    if (copy)
    {
        ChangeWayCopy();
    }
    else
    {
        ChangeWayCut();
    }
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
    return;
}

void MainWindow::ChangeTextSize()
{
    QFont font = ui->textEdit->font();
    size = ui->textsize->value();
    font.setPointSize(size);
    ui->textEdit->setFont(font);
    return;
}

void MainWindow::SaveText()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存文本"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("文本文档(*.txt)"));
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        stream.flush();
        file.close();
    }
    return;
}

void MainWindow::ChangeWayCopy()
{
    //由剪切到复制
    ui->copy_btn->setText("复制全部");
    copy = true;
    ui->copy_btn->setIcon(QIcon("://pic//copy_text.png"));
    return;
}

void MainWindow::ChangeWayCut()
{
    //由复制到剪切
    ui->copy_btn->setText("剪切全部");
    copy = false;
    ui->copy_btn->setIcon(QIcon("://pic//cut.png"));
    return;
}

void MainWindow::OpenGitHub()
{
    QDesktopServices::openUrl(QUrl(QString("https://github.com/Temperature6/QuickTextEditor")));;
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QFile file("temp");
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream file_stream(&file);
    file_stream.setCodec("utf-8");
    file_stream << ui->textEdit->toPlainText();
    file_stream.flush();
    file.close();

    config["copy"] = (int)copy;
    config["FontSize"] = size;
    QString config_str = QString(QJsonDocument(config).toJson(QJsonDocument::Compact));
    QFile config_file("config.json");
    config_file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream config_file_stream(&config_file);
    config_file_stream.setCodec("utf-8");
    config_file_stream << config_str;
    config_file.close();
    return;
}
