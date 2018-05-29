#include "noteswindow.h"
#include "ui_noteswindow.h"

#include <QDir>
#include <QPainter>
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

    this->setWindowFlags(Qt::WindowStaysOnTopHint);

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

void NotesWindow::paintEvent(QPaintEvent *)
{
//    static int i = 0;
//    qInfo("paint %d", i++);
    QPainter painter(this);
    painter.setPen(QPen(QColor("black")));
    int w = this->width();
    int h = this->height();
    for(int x = 0; x < 10; x++)
        for(int y = 0; y < x; y++) {
            painter.drawPoint(x-y-1, y);
            painter.drawPoint(w-x+y, y);
            painter.drawPoint(w-x+y, h-y-1);
            painter.drawPoint(x-y-1, h-y-1);
        }
}

NotesWindow::~NotesWindow()
{
    delete ui;
}
