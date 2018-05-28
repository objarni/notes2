#include "noteswindow.h"
#include "ui_noteswindow.h"

#include <QDir>
#include <QSettings>
#include <QTextStream>

namespace {
void saveText(QString const & text, QString const & fullpath) {
    QFile file(fullpath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!file.isOpen()){
        qInfo("Error: unable to open '%s' for writing, cannot save!",
              qPrintable(fullpath));
        return;
    }

    QTextStream outStream(&file);
    outStream << text;
    file.close();

    qInfo("wrote - %s", qPrintable(text));
}

QString loadText(QString const & fullpath) {
    QFile file(fullpath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    if(!file.isOpen()){
        qInfo("Error: unable to open '%s' for reading, cannot load!",
              qPrintable(fullpath));
        return "(Could not load file)";
    }

    QString content;
    QTextStream outStream(&file);
    content = outStream.readAll();
    file.close();

    qInfo("Loaded: '%s' found: %s", qPrintable(fullpath),
          qPrintable(content));

    return content;
}

}

NotesWindow::NotesWindow(QString const & notes2FullPath) :
    QMainWindow(nullptr),
    ui(new Ui::NotesWindow),
    mNotes2File(notes2FullPath)
{
    ui->setupUi(this);
    ui->textEdit->setText(loadText(mNotes2File));

    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    auto textChangedSlot = [&]() {
        saveText(ui->textEdit->toPlainText(), mNotes2File);
    };

    QObject::connect(ui->textEdit, &QTextEdit::textChanged,
                     this, textChangedSlot);
}

void NotesWindow::closeEvent(QCloseEvent *event) {
 QSettings settings;
 settings.setValue("mainWindowGeometry", saveGeometry());
}

NotesWindow::~NotesWindow()
{
    delete ui;
}
