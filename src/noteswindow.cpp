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

    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    auto textChangedSlot = [&]() {
        auto text = ui->textEdit->toPlainText();
        saveText(text, mNotes2File);
    };

    QObject::connect(ui->textEdit, &QTextEdit::textChanged,
                     this, textChangedSlot);
}

void NotesWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if(this->isActiveWindow())
        {
            this->setWindowOpacity(1);
        }
        else
        {
            this->setWindowOpacity(0.55);
        }
    }
}

void NotesWindow::insertCheckbox()
{
    ui->textEdit->insertPlainText("[ ] ");
}

void NotesWindow::closeEvent(QCloseEvent *) {
 QSettings settings;
 settings.setValue("mainWindowGeometry", saveGeometry());
}

NotesWindow::~NotesWindow()
{
    delete ui;
}




