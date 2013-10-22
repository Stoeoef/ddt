#include <QtGui/QGuiApplication>

#include <QtWidgets/QApplication>
#include <QtQml>
#include <QtQuick/QQuickView>
#include "speakermodel.cpp"
#include "mainmodel.h"
#include "qlistqmlwrapper.h"
#include "opdebate.h"
#include "debate.h"
#include "bpsdebate.h"
#include "tablenames.h"
#include "seattospeakermapper.h"
#include "autocompletemodel.h"
#include "mixedvaluespeakermodel.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QQmlEngine engine;

	qmlRegisterType<SpeakerModel>("ddt.model", 1, 0, "SpeakerModel");
	qmlRegisterType<QListQmlWrapper>("ddt.model", 1, 0, "QListQMLWrapper");

	qmlRegisterUncreatableType<Debate>("ddt.model", 1, 0, "Debate", "Debate is abstract.");
	qmlRegisterType<OPDebate>("ddt.model", 1, 0, "OPDDebate");
	qmlRegisterType<BPSDebate>("ddt.model", 1, 0, "BPSDebate");
	qmlRegisterType<SeatToSpeakerMapper>("ddt.model", 1, 0, "SeatToSpeakerMapper");
	qmlRegisterType<MainModel>("ddt.model", 1, 0, "MainModel");
	qmlRegisterType<TableNames>("ddt.model", 1, 0, "TableNames");
	qmlRegisterType<Team>("ddt.model", 1, 0, "Team");
	qmlRegisterType<AutoCompleteModel>("ddt.model", 1, 0, "AutoCompleteModel");
	qmlRegisterType<MixedValueSpeakerModel>("ddt.model",1, 0, "MixedValueSpeakerModel");
	QQmlComponent component(&engine);
	MainModel *rootModel = new MainModel();

	engine.rootContext()->setContextProperty("rootModel", rootModel);

	component.loadUrl(QUrl("qml/ddt/main.qml"));
	if ( !component.isReady() ) {
		qWarning("%s", qPrintable(component.errorString()));
		return -1;
	}
	QObject *topLevel = component.create();
	QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
	if ( !window ) {
		qWarning("Error: Your root item has to be a Window.");
		return -1;
	}

	QObject::connect(&engine, SIGNAL(quit()), &app, SLOT(quit()));
	window->show();
	return app.exec();

}


