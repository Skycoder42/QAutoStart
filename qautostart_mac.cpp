#include "qautostart_p.h"
#include <QXmlStreamWriter>
#include <QFile>
#include <QDir>

bool QAutoStartPrivate::isAutoStartEnabled()
{
	return QFile::exists(launchAgentFilePath());
}

bool QAutoStartPrivate::setAutoStartEnabled(bool autoStartEnabled)
{
	const auto autoStartPath = launchAgentFilePath();
	if(autoStartEnabled) {
		if(QFile::exists(autoStartPath))
			return true;
		else {
			QFile launchFile{autoStartPath};
			if(!launchFile.open(QIODevice::WriteOnly | QIODevice::Text))
				return false;

			//TODO conver to use QSettings instead
			QXmlStreamWriter writer{&launchFile};
			writer.setAutoFormatting(true);
			writer.setAutoFormattingIndent(-1);
			writer.setCodec("UTF-8");

			writer.writeStartDocument(QStringLiteral("1.0"));
			writer.writeDTD(QStringLiteral("plist PUBLIC -//Apple Computer//DTD PLIST 1.0//EN http://www.apple.com/DTDs/PropertyList-1.0.dtd"));
			writer.writeStartElement(QStringLiteral("plist"));
			writer.writeAttribute(QStringLiteral("version"), QStringLiteral("1.0"));
			writer.writeStartElement(QStringLiteral("dict"));

			writer.writeTextElement(QStringLiteral("key"), QStringLiteral("Label"));
			writer.writeTextElement(QStringLiteral("string"), startId);

			writer.writeTextElement(QStringLiteral("key"), QStringLiteral("ProgramArguments"));
			writer.writeStartElement(QStringLiteral("array"));
			writer.writeTextElement(QStringLiteral("string"), QDir::toNativeSeparators(program));
			for(const auto &arg : qAsConst(arguments))
				writer.writeTextElement(QStringLiteral("string"), arg);
			writer.writeEndElement();

			writer.writeTextElement(QStringLiteral("key"), QStringLiteral("RunAtLoad"));
			writer.writeTextElement(QStringLiteral("boolean"), QStringLiteral("true"));

			writer.writeTextElement(QStringLiteral("key"), QStringLiteral("KeepAlive"));
			writer.writeTextElement(QStringLiteral("boolean"), QStringLiteral("true"));

			writer.writeTextElement(QStringLiteral("key"), QStringLiteral("ProcessType"));
			if(!extraProperties.contains(QAutoStart::Interactive))
				writer.writeTextElement(QStringLiteral("string"), QStringLiteral("Standard"));
			else if(extraProperties.value(QAutoStart::Interactive).toBool())
				writer.writeTextElement(QStringLiteral("string"), QStringLiteral("Interactive"));
			else
				writer.writeTextElement(QStringLiteral("string"), QStringLiteral("Background"));

			writer.writeEndElement();
			writer.writeEndElement();
			writer.writeEndDocument();
			launchFile.close();
		}
	} else {
		if(!QFile::exists(autoStartPath) || QFile::remove(autoStartPath))
			return true;
	}

	return false;
}

QString QAutoStartPrivate::launchAgentFilePath() const
{
	auto launchDir = QDir::home();
	const auto subDir = QStringLiteral("Library/LaunchAgents");
	launchDir.mkpath(subDir);
	launchDir.cd(subDir);
	return launchDir.absoluteFilePath(startId + QStringLiteral(".plist"));
}
