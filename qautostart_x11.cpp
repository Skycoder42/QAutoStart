#include "qautostart_p.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>

bool QAutoStartPrivate::isAutoStartEnabled()
{
	return QFile::exists(desktopFilePath());
}

bool QAutoStartPrivate::setAutoStartEnabled(bool autoStartEnabled)
{
	const auto autoStartPath = desktopFilePath();
	if(autoStartEnabled) {
		if(QFile::exists(autoStartPath))
			return true;
		else {
			QFile desktopFile{autoStartPath};
			if(!desktopFile.open(QIODevice::WriteOnly | QIODevice::Text))
				return false;

			QTextStream stream{&desktopFile};
			stream << "[Desktop Entry]\n"
				   << "Type=Application\n"
				   << "Version=1.1\n"
				   << "Name=" << extraProperties.value(QAutoStart::DisplayName).toString() << "\n"
				   << "Comment=" << extraProperties.value(QAutoStart::Comment).toString() << "\n"
				   << "Exec=" << createCommand() << "\n"
				   << "Icon=" << extraProperties.value(QAutoStart::IconName).toString() << "\n"
				   << "Terminal=false\n";
			stream.flush();
			desktopFile.close();
			return true;
		}
	} else {
		if(!QFile::exists(autoStartPath) || QFile::remove(autoStartPath))
			return true;
	}

	return false;
}

QString QAutoStartPrivate::desktopFilePath() const
{
	return QStringLiteral("%1/autostart/%2.desktop")
			.arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation), startId);
}
