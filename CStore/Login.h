#pragma once

#include <QWidget>
#include "ui_Login.h"

class Login : public QWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();

private slots:
	void checkLogin();
	void setFocusPass();

private:
	Ui::Login ui;
};
