#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <qautostart.h>

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TestWidget(QWidget *parent = nullptr);
	~TestWidget();

	void setAutoStarted();

private:
	Ui::TestWidget *ui;
	QAutoStart *_autostart;
};

#endif // TESTWIDGET_H
