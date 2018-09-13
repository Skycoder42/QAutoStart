#ifndef QAUTOSTART_P_H
#define QAUTOSTART_P_H

#include "qautostart.h"
#include <QSettings>

class QAutoStartPrivate
{
	friend class QAutoStart;
	Q_DISABLE_COPY(QAutoStartPrivate)

public:
	bool isAutoStartEnabled();
	bool setAutoStartEnabled(bool autoStartEnabled);

private:
	QAutoStartPrivate(QAutoStart *q_ptr);

	QAutoStart *q_ptr;
	QString startId;
	QString program;
	QStringList arguments;
	QAutoStart::PropertyHash extraProperties;

	QString createCommand() const;

#ifdef Q_OS_WIN
	QSettings *_settings = nullptr;
	void createSettings();
#elif defined(Q_OS_MACOS)
	QString launchAgentFilePath() const;
#elif defined(Q_OS_UNIX)
	QString desktopFilePath() const;
#endif
};

#endif // QAUTOSTART_P_H
