#ifndef QAUTOSTART_H
#define QAUTOSTART_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QScopedPointer>

class QAutoStartPrivate;
class QAutoStart : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString startId READ startId WRITE setStartId NOTIFY startIdChanged)
	Q_PROPERTY(QString program READ program WRITE setProgram NOTIFY programChanged)
	Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments NOTIFY argumentsChanged)

	Q_PROPERTY(PropertyHash extraProperties READ extraProperties WRITE setExtraProperties NOTIFY extraPropertiesChanged)

	Q_PROPERTY(bool autoStartEnabled READ isAutoStartEnabled WRITE setAutoStartEnabled NOTIFY autoStartEnabledChanged)

public:
	enum PropertyKey {
		DisplayName,
		IconName,
		Comment,
		Interactive
	};
	Q_ENUM(PropertyKey)

	using PropertyHash = QHash<PropertyKey, QVariant>;

	explicit QAutoStart(QObject *parent = nullptr);
	~QAutoStart() override;

	QString startId() const;
	QString program() const;
	QStringList arguments() const;
	PropertyHash extraProperties() const;
	QVariant extraProperty(PropertyKey key) const;
	bool isAutoStartEnabled() const;

public slots:
	void setStartId(QString startId);
	void setProgram(QString program);
	void setArguments(QStringList arguments);
	void setExtraProperties(PropertyHash extraProperties);
	void setExtraProperty(PropertyKey key, const QVariant &value);

	bool setAutoStartEnabled(bool autoStartEnabled = true);
	inline bool setAutoStartDisabled(bool autoStartDisabled = true) {
		return setAutoStartEnabled(!autoStartDisabled);
	}

signals:
	void startIdChanged(const QString &startId);
	void programChanged(const QString &program);
	void argumentsChanged(const QStringList &arguments);
	void extraPropertiesChanged(const PropertyHash &extraProperties);
	void autoStartEnabledChanged(bool autoStartEnabled);

private:
	QScopedPointer<QAutoStartPrivate> d;
};

#endif // QAUTOSTART_H
