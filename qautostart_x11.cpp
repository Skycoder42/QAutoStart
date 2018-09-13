#include "qautostart_p.h"
#include <QStandardPaths>
#include <QFile>

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
			QSettings autoStartFile{autoStartPath, QSettings::IniFormat};
			autoStartFile.beginGroup(QStringLiteral("Desktop_Entry"));
			autoStartFile.setValue(QStringLiteral("Type"), QStringLiteral("Application"));
			autoStartFile.setValue(QStringLiteral("Version"), QStringLiteral("1.1"));
			autoStartFile.setValue(QStringLiteral("Name"), extraProperties.value(QAutoStart::DisplayName));
			autoStartFile.setValue(QStringLiteral("Comment"), extraProperties.value(QAutoStart::Comment));
			autoStartFile.setValue(QStringLiteral("Exec"), createCommand());
			autoStartFile.setValue(QStringLiteral("Icon"), extraProperties.value(QAutoStart::IconName));
			autoStartFile.setValue(QStringLiteral("Terminal"), false);
			autoStartFile.endGroup();
			autoStartFile.sync();

			//fix the underscore to space
			QFile fixFile{autoStartPath};
			if(!fixFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
				fixFile.remove();
				return false;
			} else {
				fixFile.seek(8);
				fixFile.write(" ", 1);
				fixFile.close();
				return true;
			}
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
