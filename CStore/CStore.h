#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CStore.h"

class CStore : public QMainWindow
{
    Q_OBJECT

public:
    CStore(QWidget *parent = Q_NULLPTR);
private slots:
    void tovarButton();
    void orderButton();
    void journalButton();
    void Maximize();
    void setsIndex();
    void Searching();
    void addingTovar();
    void settingFocus();
    void setDateText();
    void addingOrder();
private:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    QPoint mousePoint;
    void addingJournal();
    void fillJournalTable(std::string name, std::vector<std::string> keys, QStringList labels);
    void fillOrderTable(std::string name, std::vector<std::string> keys, QStringList labels);
    void fillTovarTable(std::string name, std::vector<std::string> keys, QStringList labels);
    Ui::CStoreClass ui;
};
