#include "CStore.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>

CStore::CStore(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    //ui.tableWidget->setSortingEnabled(true);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui.tovarButton, SIGNAL(clicked()), this, SLOT(tovarButton()));
    connect(ui.orderButton, SIGNAL(clicked()), this, SLOT(orderButton()));
    connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.maximizeButton, SIGNAL(clicked()), this, SLOT(Maximize()));
    connect(ui.minimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui.btn_edit, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.btn_home, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.btn_stats, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.btn_right, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.searchButton, SIGNAL(clicked()), this, SLOT(Searching()));
    connect(ui.addTovarButton, SIGNAL(clicked()), this, SLOT(AddingTovar()));
    connect(ui.searchText, SIGNAL(returnPressed()), this, SLOT(Searching()));
    tovarButton();
}

void CStore::AddingTovar()
{
    if (ui.idText->text() != "" && ui.nameText->text() != "" && ui.priceText->text() != "" && ui.numberText->text() != "")
    {
        std::fstream fileInput;
        using json = nlohmann::json;
        json file;
        fileInput.open("data/data.json");
        fileInput >> file;
        ui.idText->text().toInt();
        json user_info = json::object({
            { "id", ui.idText->text().toInt() },
            { "name", ui.nameText->text().toStdString() },
            { "price", ui.priceText->text().toInt() },
            { "number", ui.numberText->text().toInt() }
            });
        std::ofstream fileOutput;
        fileOutput.open("data/data.json");
        file["tovar"].push_back(user_info);
        fileOutput << file << std::endl;
        fileOutput.close();
        fileInput.close();
        tovarButton();
    }
}
void CStore::Searching()
{
    ui.tableWidget->setRowCount(0);
    std::fstream fileInput;
    using json = nlohmann::json;
    json file;
    fileInput.open("data/data.json");
    fileInput >> file;
    std::string srch = ui.searchText->text().toStdString();
    ui.tableWidget->setRowCount(file["tovar"].size());
    int n = 0;
    for (int i = 0; i < file["tovar"].size(); i++) {
        if (std::string(file["tovar"][i]["name"]).find(srch) != std::string::npos) {
            for (int b = 0; b < 4; b++) {
                std::vector<std::string> keys = { "id", "name", "price", "number" };
                try {
                    QTableWidgetItem* its = new QTableWidgetItem
                    (QString::fromStdString(file["tovar"][i][keys[b]]));
                    ui.tableWidget->setItem(n, b, its);
                }
                catch (nlohmann::detail::type_error) {
                    QTableWidgetItem* its = new QTableWidgetItem
                    (QString::fromStdString(file["tovar"][i][keys[b]].dump()));
                    ui.tableWidget->setItem(n, b, its);
                }
            }
            ++n;
        }
    }
    fileInput.close();
}
void CStore::setsIndex()
{
    if (sender()->objectName() == "btn_home")
        ui.stackedWidget->setCurrentIndex(0);
    else if (sender()->objectName() == "btn_stats")
        ui.stackedWidget->setCurrentIndex(1);
    else if (sender()->objectName() == "btn_edit")
        ui.stackedWidget->setCurrentIndex(2);
    else if (sender()->objectName() == "btn_right")
        ui.stackedWidget->setCurrentIndex(3);
    else
        ui.stackedWidget->setCurrentIndex(4);
}

void CStore::mousePressEvent(QMouseEvent* event)
{
    mousePoint = event->globalPos();
}

void CStore::mouseMoveEvent(QMouseEvent *event)
{
    if (event->y() < 50) {
        const QPoint delta = event->globalPos() - mousePoint;
        move(x() + delta.x(), y() + delta.y());
        mousePoint = event->globalPos();
    }
}

void CStore::Maximize()
{
    if (isMaximized() == false) {
        ui.maximizeButton->setIcon(QIcon(":/CStore/icons/fullWindows.png"));
        showMaximized();
    }
    else {
        ui.maximizeButton->setIcon(QIcon(":/CStore/icons/smallWindows.png"));
        showNormal();
    }
}

void CStore::tovarButton()
{
    fillTable("tovar", { "id", "name", "price", "number" }, { "ID", "Name", "Price", "Number" });
}

void CStore::orderButton()
{
    fillTable("order", { "id", "id_tovar", "count", "date" }, { "ID", "TovarID", "Count", "Date" });
}

void CStore::fillTable(std::string name, std::vector<std::string> keys, QStringList labels)
{
    std::fstream fileInput;
    using json = nlohmann::json;
    json file;
    fileInput.open("data/data.json");
    fileInput >> file;
    int ColumnCount = file[name][0].size();
    int RowCount = file[name].size();
    ui.tableWidget->setColumnCount(ColumnCount);
    ui.tableWidget->setRowCount(RowCount);
    ui.tableWidget->setHorizontalHeaderLabels(labels);
    for (int column = 0; column < ColumnCount; ++column) {
        for (int row = 0; row < RowCount; ++row) {
            try {
                QTableWidgetItem* its = new QTableWidgetItem
                (QString::fromStdString(file[name][row][keys[column]]));
                ui.tableWidget->setItem(row, column, its);
            }
            catch(nlohmann::detail::type_error) {
                QTableWidgetItem* its = new QTableWidgetItem
                (QString::fromStdString(file[name][row][keys[column]].dump()));
                ui.tableWidget->setItem(row, column, its);
            }
            
        }
    }
    fileInput.close();
    return;
}
