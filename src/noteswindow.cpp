#include "noteswindow.h"
#include "ui_noteswindow.h"

#include <QDir>
#include <QPainter>
#include <QSettings>
#include <QTextStream>
#include <QScreen>
#include <QMouseEvent>

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

    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);

    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    auto textChangedSlot = [&]() {
        auto text = ui->textEdit->toPlainText();
        saveText(text, mNotes2File);
    };

    QObject::connect(ui->textEdit, &QTextEdit::textChanged,
                     this, textChangedSlot);

    keyCtrl0 = new QShortcut(this);
    keyCtrl0->setKey(Qt::CTRL + Qt::Key_0);
    QObject::connect(keyCtrl0, &QShortcut::activated,
                     this, &NotesWindow::insertCheckbox);

    keyEsc = new QShortcut(this);
    keyEsc->setKey(Qt::Key_Escape);
    QObject::connect(keyEsc, &QShortcut::activated,
                     this, &NotesWindow::closeWindow);
}

void NotesWindow::insertCheckbox()
{
    ui->textEdit->insertPlainText("[ ] ");
}

void NotesWindow::closeWindow()
{
    this->close();
}

void NotesWindow::closeEvent(QCloseEvent *) {
 QSettings settings;
 settings.setValue("mainWindowGeometry", saveGeometry());
}

NotesWindow::~NotesWindow()
{
    delete ui;
}




