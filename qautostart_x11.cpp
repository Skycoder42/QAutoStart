#include "qautostart_p.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDir>

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
	QDir configDir;
	if(extraProperties.contains(QAutoStart::CustomLocation)) {
		configDir = extraProperties.value(QAutoStart::CustomLocation).toString();
		configDir.mkpath(QStringLiteral("."));
	} else {
		configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
		const auto subDir = QStringLiteral("autostart");
		configDir.mkpath(subDir);
		configDir.cd(subDir);
	}
	return configDir.absoluteFilePath(startId + QStringLiteral(".desktop"));
}
