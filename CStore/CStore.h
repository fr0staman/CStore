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
    void Maximize();
    void setsIndex();
    void Searching();
    void AddingTovar();
private:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    
    
    QPoint mousePoint;
    void fillTable(std::string name, std::vector<std::string> keys, QStringList labels);
    Ui::CStoreClass ui;
};
