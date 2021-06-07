#include "CStore.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QDateTime>

CStore::CStore(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/CStore/icons/icon.png"));
    //ui.tableWidget->setSortingEnabled(true);
    
    //connect(ui.tovarButton, SIGNAL(clicked()), this, SLOT(tovarButton()));
    //connect(ui.orderButton, SIGNAL(clicked()), this, SLOT(orderButton()));
    connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.maximizeButton, SIGNAL(clicked()), this, SLOT(Maximize()));
    connect(ui.minimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui.btn_edit, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.btn_home, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.btn_stats, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.btn_right, SIGNAL(clicked()), this, SLOT(setsIndex()));
    connect(ui.searchButton, SIGNAL(clicked()), this, SLOT(Searching()));
    connect(ui.addTovarButton, SIGNAL(clicked()), this, SLOT(addingTovar()));
    connect(ui.addOrderButton, SIGNAL(clicked()), this, SLOT(addingOrder()));
    connect(ui.searchText, SIGNAL(returnPressed()), this, SLOT(Searching()));
    connect(ui.idText, SIGNAL(returnPressed()), this, SLOT(settingFocus()));
    connect(ui.nameText, SIGNAL(returnPressed()), this, SLOT(settingFocus()));
    connect(ui.priceText, SIGNAL(returnPressed()), this, SLOT(settingFocus()));
    connect(ui.numberText, SIGNAL(returnPressed()), this, SLOT(settingFocus()));
    connect(ui.idTextOrder, SIGNAL(returnPressed()), this, SLOT(settingFocus()));
    connect(ui.idTovarText, SIGNAL(returnPressed()), this, SLOT(settingFocus()));
    connect(ui.numberOrderText, SIGNAL(returnPressed()), this, SLOT(settingFocus()));
    tovarButton();
    orderButton();
    journalButton();
    ui.tableWidgetTovar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableWidgetOrder->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setDateText();
}

void CStore::setDateText()
{
    QDateTime cur = QDateTime::currentDateTime();
    ui.dateText->setText(cur.toString("yyyy-MM-dd HH:mm:ss"));
    ui.dateText->setDisabled(true);
}

void CStore::settingFocus()
{
    if (sender()->objectName() == "idText")
        ui.nameText->setFocus();
    else if (sender()->objectName() == "nameText")
        ui.priceText->setFocus();
    else if (sender()->objectName() == "priceText")
        ui.numberText->setFocus();
    else if (sender()->objectName() == "numberText") {
        ui.addTovarButton->click();
        ui.idText->setFocus();
    }
    else if (sender()->objectName() == "idTextOrder")
        ui.idTovarText->setFocus();
    else if (sender()->objectName() == "idTovarText")
        ui.numberOrderText->setFocus();
    else if (sender()->objectName() == "numberOrderText") {
        ui.addOrderButton->click();
        ui.idTextOrder->setFocus();
    }
}

void CStore::addingJournal()
{

}
void CStore::addingOrder()
{
    if (ui.idTovarText->text() != "" && ui.numberOrderText->text() != "" && ui.idTextOrder->text() != "")
    {
        std::fstream fileInput;
        using json = nlohmann::json;
        json file;
        fileInput.open("data/data.json");
        fileInput >> file;
        ui.idText->text().toInt();
        setDateText();
        json user_info = json::object({
            { "id", ui.idTextOrder->text().toInt() },
            { "id_tovar", ui.idTovarText->text().toInt() },
            { "count", ui.numberOrderText->text().toInt() },
            { "date", ui.dateText->text().toStdString() }
            });
        std::ofstream fileOutput;
        fileOutput.open("data/data.json");
        file["order"].push_back(user_info);
        fileOutput << file << std::endl;
        fileOutput.close();
        fileInput.close();
        orderButton();
        ui.idTextOrder->clear();
        ui.idTovarText->clear();
        ui.idTextOrder->clear();
        setDateText();
    }
}

void CStore::addingTovar()
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
        ui.idText->clear();
        ui.nameText->clear();
        ui.priceText->clear();
        ui.numberText->clear();
    }
}
void CStore::Searching()
{
    ui.tableWidgetTovar->setRowCount(0);
    std::fstream fileInput;
    using json = nlohmann::json;
    json file;
    fileInput.open("data/data.json");
    fileInput >> file;
    std::string srch = ui.searchText->text().toStdString();
    ui.tableWidgetTovar->setRowCount(file["tovar"].size());
    int n = 0;
    for (int i = 0; i < file["tovar"].size(); i++) {
        if (std::string(file["tovar"][i]["name"]).find(srch) != std::string::npos) {
            for (int b = 0; b < 4; b++) {
                std::vector<std::string> keys = { "id", "name", "price", "number" };
                try {
                    QTableWidgetItem* its = new QTableWidgetItem
                    (QString::fromStdString(file["tovar"][i][keys[b]]));
                    ui.tableWidgetTovar->setItem(n, b, its);
                }
                catch (nlohmann::detail::type_error) {
                    QTableWidgetItem* its = new QTableWidgetItem
                    (QString::fromStdString(file["tovar"][i][keys[b]].dump()));
                    ui.tableWidgetTovar->setItem(n, b, its);
                }
            }
            ++n;
        }
    }
    fileInput.close();
}
void CStore::setsIndex()
{
    if (sender()->objectName() == "btn_home") {
        ui.stackedWidget->setCurrentIndex(0);
        ui.btn_home->setStyleSheet(
            "border-left: 22px solid qlineargradient(spread:pad, x1:0.034, y1:0, x2:0.216, y2:0, stop:0.499 rgba(255, 121, 198, 255), stop:0.5 rgba(85, 170, 255, 0)); background-color: rgb(40, 44, 52);");
        ui.btn_stats->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_edit->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_right->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
    }
    else if (sender()->objectName() == "btn_stats") {
        ui.stackedWidget->setCurrentIndex(1);
        ui.btn_stats->setStyleSheet(
            "border-left: 22px solid qlineargradient(spread:pad, x1:0.034, y1:0, x2:0.216, y2:0, stop:0.499 rgba(255, 121, 198, 255), stop:0.5 rgba(85, 170, 255, 0)); background-color: rgb(40, 44, 52);");
        ui.btn_home->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_edit->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_right->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
    }
    else if (sender()->objectName() == "btn_edit") {
        ui.stackedWidget->setCurrentIndex(2);
        ui.btn_edit->setStyleSheet(
            "border-left: 22px solid qlineargradient(spread:pad, x1:0.034, y1:0, x2:0.216, y2:0, stop:0.499 rgba(255, 121, 198, 255), stop:0.5 rgba(85, 170, 255, 0)); background-color: rgb(40, 44, 52);");
        ui.btn_home->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_stats->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_right->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
    }
    else if (sender()->objectName() == "btn_right") {
        ui.stackedWidget->setCurrentIndex(3);
        ui.btn_right->setStyleSheet(
            "border-left: 22px solid qlineargradient(spread:pad, x1:0.034, y1:0, x2:0.216, y2:0, stop:0.499 rgba(255, 121, 198, 255), stop:0.5 rgba(85, 170, 255, 0)); background-color: rgb(40, 44, 52);");
        ui.btn_home->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_stats->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
        ui.btn_edit->setStyleSheet(
            "background-color: transparent; QPushButton:hover{ background - color: rgb(40, 44, 52);} QPushButton:pressed{background - color: rgb(189, 147, 249); color: rgb(255, 255, 255);}");
    }
    else
        ui.stackedWidget->setCurrentIndex(4);
}

void CStore::mousePressEvent(QMouseEvent* event)
{
    if (event->y() < 50) {
        mousePoint = event->globalPos();
        if (isMaximized() == true) {
            ui.maximizeButton->setIcon(QIcon(":/CStore/icons/smallWindows.png"));
            showNormal();
        }
    }
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

void CStore::journalButton()
{
    fillJournalTable("journal", {}, {});
}

void CStore::tovarButton()
{
    fillTovarTable("tovar", { "id", "name", "price", "number" }, { "ID", QString::fromLocal8Bit("Артикул"), QString::fromLocal8Bit("Ціна"), QString::fromLocal8Bit("Кількість") });
}

void CStore::orderButton()
{
    fillOrderTable("order", { "id", "id_tovar", "count", "date" }, { "ID", QString::fromLocal8Bit("Товар"), QString::fromLocal8Bit("Кількість"), QString::fromLocal8Bit("Дата") });
}

void CStore::fillJournalTable(std::string name, std::vector<std::string> keys, QStringList labels)
{
    std::fstream fileInput;
    using json = nlohmann::json;
    json file;
    fileInput.open("data/data.json");
    fileInput >> file;
    int ColumnCount = file[name][0].size();
    int RowCount = file[name].size();
    ui.tableWidgetJournal->setColumnCount(ColumnCount);
    ui.tableWidgetJournal->setRowCount(RowCount);
    ui.tableWidgetJournal->setHorizontalHeaderLabels(labels);
    for (int column = 0; column < ColumnCount; ++column) {
        for (int row = 0; row < RowCount; ++row) {
            try {
                QTableWidgetItem* its = new QTableWidgetItem
                (QString::fromStdString(file[name][row][keys[column]]));
                ui.tableWidgetJournal->setItem(row, column, its);
            }
            catch (nlohmann::detail::type_error) {
                QTableWidgetItem* its = new QTableWidgetItem
                (QString::fromStdString(file[name][row][keys[column]].dump()));
                ui.tableWidgetJournal->setItem(row, column, its);
            }
        }
    }
    fileInput.close();
}

void CStore::fillOrderTable(std::string name, std::vector<std::string> keys, QStringList labels)
{
    std::fstream fileInput;
    using json = nlohmann::json;
    json file;
    fileInput.open("data/data.json");
    fileInput >> file;
    int ColumnCount = file[name][0].size();
    int RowCount = file[name].size();
    ui.tableWidgetOrder->setColumnCount(ColumnCount);
    ui.tableWidgetOrder->setRowCount(RowCount);
    ui.tableWidgetOrder->setHorizontalHeaderLabels(labels);
    for (int column = 0; column < ColumnCount; ++column) {
        for (int row = 0; row < RowCount; ++row) {
            try {
                QTableWidgetItem* its = new QTableWidgetItem
                (QString::fromStdString(file[name][row][keys[column]]));
                ui.tableWidgetOrder->setItem(row, column, its);
            }
            catch (nlohmann::detail::type_error) {
                QTableWidgetItem* its = new QTableWidgetItem
                (QString::fromStdString(file[name][row][keys[column]].dump()));
                ui.tableWidgetOrder->setItem(row, column, its);
            }

        }
    }
    fileInput.close();
}
void CStore::fillTovarTable(std::string name, std::vector<std::string> keys, QStringList labels)
{
        std::fstream fileInput;
        using json = nlohmann::json;
        json file;
        fileInput.open("data/data.json");
        fileInput >> file;
        int ColumnCount = file[name][0].size();
        int RowCount = file[name].size();
        ui.tableWidgetTovar->setColumnCount(ColumnCount);
        ui.tableWidgetTovar->setRowCount(RowCount);
        ui.tableWidgetTovar->setHorizontalHeaderLabels(labels);
        for (int column = 0; column < ColumnCount; ++column) {
            for (int row = 0; row < RowCount; ++row) {
                try {
                    QTableWidgetItem* its = new QTableWidgetItem
                    (QString::fromStdString(file[name][row][keys[column]]));
                    ui.tableWidgetTovar->setItem(row, column, its);
                }
                catch (nlohmann::detail::type_error) {
                    QTableWidgetItem* its = new QTableWidgetItem
                    (QString::fromStdString(file[name][row][keys[column]].dump()));
                    ui.tableWidgetTovar->setItem(row, column, its);
                }
            }
        }
        fileInput.close();
    return;
}
