#include "noteswindow.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

namespace {
QString findNotes2FullPath() {
    auto docs = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    auto notes2file = QDir::cleanPath(docs + "/" + "Notes2.txt");
    qInfo(": %s", qPrintable(notes2file));
    return notes2file;
}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationDomain("olofbjarnason.se");
    QCoreApplication::setOrganizationName("Objarni Productions AB");
    QCoreApplication::setApplicationName("Notes2");
    QCoreApplication::setApplicationVersion("0.5");
    auto fullPath = findNotes2FullPath();

    NotesWindow w(fullPath);
    w.show();

    return a.exec();
}
