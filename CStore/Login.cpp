#include "Login.h"
#include "CStore.h"
#include <QMessageBox>

Login::Login(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButtonLogin, SIGNAL(released()), this, SLOT(checkLogin()));
	connect(ui.lineLogin, SIGNAL(returnPressed()), this, SLOT(setFocusPass()));
	connect(ui.linePass, SIGNAL(returnPressed()), this, SLOT(checkLogin()));
}

void Login::setFocusPass()
{
	ui.linePass->setFocus();
}

void Login::checkLogin()
{
	QString l = "admin";
	QString p = "admin";
	if (ui.lineLogin->text() == l && ui.linePass->text() == p) {
		close();
		CStore* w = new CStore;
		w->show();
	}
	else {
		QMessageBox::warning(this,
			QString::fromLocal8Bit("Неправильний логін"),
			QString::fromLocal8Bit("Ви ввели неправильні дані.\nСпробуйте ще раз!"));
	}
}

Login::~Login()
{
}
