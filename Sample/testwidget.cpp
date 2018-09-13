#include "testwidget.h"
#include "ui_testwidget.h"

TestWidget::TestWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TestWidget),
	_autostart{new QAutoStart{this}}
{
	ui->setupUi(this);

	_autostart->setArguments({QStringLiteral("--autostart")});

	ui->checkBox->setChecked(_autostart->isAutoStartEnabled());
	connect(_autostart, &QAutoStart::autoStartEnabledChanged,
			ui->checkBox, &QCheckBox::setChecked);
	connect(ui->checkBox, &QCheckBox::clicked,
			_autostart, &QAutoStart::setAutoStartEnabled);
}

TestWidget::~TestWidget()
{
	delete ui;
}

void TestWidget::setAutoStarted()
{
	ui->label->setText(tr("Started automatically"));
}
