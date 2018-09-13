#include "qautostart.h"
#include "qautostart_p.h"
#include <QDir>
#ifdef QT_GUI_LIB
#include <QGuiApplication>
#else
#include <QCoreApplication>
#endif

QAutoStart::QAutoStart(QObject *parent) :
	QObject{parent},
	d{new QAutoStartPrivate{this}}
{}

QString QAutoStart::startId() const
{
	return d->startId;
}

QAutoStart::~QAutoStart() = default;

QString QAutoStart::program() const
{
	return d->program;
}

QStringList QAutoStart::arguments() const
{
	return d->arguments;
}

QAutoStart::PropertyHash QAutoStart::extraProperties() const
{
	return d->extraProperties;
}

QVariant QAutoStart::extraProperty(PropertyKey key) const
{
	return d->extraProperties.value(key);
}

bool QAutoStart::isAutoStartEnabled() const
{
	return d->isAutoStartEnabled();
}

void QAutoStart::setStartId(QString startId)
{
	if (d->startId == startId)
		return;

	d->startId = std::move(startId);
	emit startIdChanged(d->startId);
}

void QAutoStart::setProgram(QString program)
{
	if (d->program == program)
		return;

	d->program = std::move(program);
	emit programChanged(d->program);
}

void QAutoStart::setArguments(QStringList arguments)
{
	if (d->arguments == arguments)
		return;

	d->arguments = std::move(arguments);
	emit argumentsChanged(d->arguments);
}

void QAutoStart::setExtraProperties(PropertyHash extraProperties)
{
	if (d->extraProperties == extraProperties)
		return;

	d->extraProperties = std::move(extraProperties);
	emit extraPropertiesChanged(d->extraProperties);
}

void QAutoStart::setExtraProperty(QAutoStart::PropertyKey key, const QVariant &value)
{
	d->extraProperties.insert(key, value);
	emit extraPropertiesChanged(d->extraProperties);
}

bool QAutoStart::setAutoStartEnabled(bool autoStartEnabled)
{
	if(d->setAutoStartEnabled(autoStartEnabled)) {
		emit autoStartEnabledChanged(autoStartEnabled);
		return true;
	} else
		return false;
}

// ------------- PRIVATE IMPLEMENTATION -------------

QAutoStartPrivate::QAutoStartPrivate(QAutoStart *q_ptr) :
	q_ptr{q_ptr},
	startId{[](){
		const auto domain = QCoreApplication::organizationDomain();
		if(domain.isEmpty())
			return QCoreApplication::applicationName();
		else
			return QStringLiteral("%1.%2").arg(domain, QCoreApplication::applicationName());
	}()},
	program{QCoreApplication::applicationFilePath()},
	extraProperties{
#ifdef QT_GUI_LIB
		{QAutoStart::DisplayName, QGuiApplication::applicationDisplayName()},
#else
		{QAutoStart::DisplayName, QCoreApplication::applicationName()},
#endif
		{QAutoStart::IconName, QCoreApplication::applicationName()}
	}
{
	extraProperties.insert(QAutoStart::Comment, extraProperties.value(QAutoStart::DisplayName));
}

QString QAutoStartPrivate::createCommand() const
{
	QStringList command;
	command.append(QDir::toNativeSeparators(program));
	command.append(arguments);
	return QStringLiteral("\"%1\"").arg(command.join(QStringLiteral("\" \"")));
}
