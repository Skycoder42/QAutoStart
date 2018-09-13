#include "qautostart_p.h"
#include <QSettings>

bool QAutoStartPrivate::isAutoStartEnabled()
{
	createSettings();
	return _settings->contains(startId);
}

bool QAutoStartPrivate::setAutoStartEnabled(bool autoStartEnabled)
{
	createSettings();
	if(_settings->isWritable()) {
		const auto current = _settings->contains(startId);
		if(current != autoStartEnabled) {
			_settings->setValue(startId, createCommand());
			_settings->sync();
			return true;
		} else
			return true;
	} else
		return false;
}

void QAutoStartPrivate::createSettings()
{
	if(!_settings)
		_settings = new QSettings{QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat, q_ptr};
	else
		_settings->sync();
}
