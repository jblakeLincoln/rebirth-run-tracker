#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFontDatabase>
#include <QFile>
#include <QFileSystemWatcher>

#include <QGridLayout>
#include <QProcess>
#include <QString>
#include <QTextStream>
#include <QTimer>

#include <iostream>
#include <map>

#include "RebirthItem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int itemCount = 0;
    QString fontFamRebirth;
    QString fontFamPickup;

    int m_HeldItem;
    int m_LineCount;
    QProcess* process;
    QGridLayout* grLayout;


    std::map<int, RebirthItem> _itemMap;
    QTimer* m_UpdateTimer;

    void InitialiseItems();
    void ReadFile();
    void AddImageToPanel(QString name);
    QString GetXMLEntry(const QString& str);
private slots:
    void Btn_Launch_Clicked();
    void Update();
};

#endif // MAINWINDOW_H
