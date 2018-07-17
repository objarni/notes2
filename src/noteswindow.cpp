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
        auto text = ui->textEdit->toPlainText();
        saveText(text, mNotes2File);
    };

    QObject::connect(ui->textEdit, &QTextEdit::textChanged,
                     this, textChangedSlot);

    keyCtrl1 = new QShortcut(this);
    keyCtrl1->setKey(Qt::CTRL + Qt::Key_1);
    QObject::connect(keyCtrl1, &QShortcut::activated,
                     this, &NotesWindow::moveWindowTopLeft);

    keyCtrl2 = new QShortcut(this);
    keyCtrl2->setKey(Qt::CTRL + Qt::Key_2);
    QObject::connect(keyCtrl2, &QShortcut::activated,
                     this, &NotesWindow::moveWindowTopRight);

    keyCtrl3 = new QShortcut(this);
    keyCtrl3->setKey(Qt::CTRL + Qt::Key_3);
    QObject::connect(keyCtrl3, &QShortcut::activated,
                     this, &NotesWindow::moveWindowBottomLeft);

    keyCtrl4 = new QShortcut(this);
    keyCtrl4->setKey(Qt::CTRL + Qt::Key_4);
    QObject::connect(keyCtrl4, &QShortcut::activated,
                     this, &NotesWindow::moveWindowBottomRight);

}

void NotesWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if(this->isActiveWindow())
        {
            this->setWindowOpacity(0.9);
        }
        else
        {
            this->setWindowOpacity(0.55);
        }
    }
}

int NotesWindow::desktopWidth() {
    auto screen = QGuiApplication::primaryScreen();
    auto screenGeometry = screen->geometry();
    return screenGeometry.width();
}

int NotesWindow::desktopHeight() {
    auto screen = QGuiApplication::primaryScreen();
    auto screenGeometry = screen->geometry();
    return screenGeometry.height();
}

int NotesWindow::windowWidth() {
    return frameGeometry().width();
}

int NotesWindow::windowHalfHeight() {
    return frameGeometry().height() / 2;
}

void NotesWindow::moveWindowTopLeft()
{
    move(0, windowHalfHeight());
}

void NotesWindow::moveWindowTopRight()
{
    move(desktopWidth() - windowWidth(), windowHalfHeight());
}

void NotesWindow::moveWindowBottomRight()
{
    move(desktopWidth() - windowWidth(), desktopHeight() - windowHalfHeight());
}

void NotesWindow::moveWindowBottomLeft()
{
    move(0, desktopHeight() - windowHalfHeight());
}

void NotesWindow::closeEvent(QCloseEvent *) {
 QSettings settings;
 settings.setValue("mainWindowGeometry", saveGeometry());
}

void NotesWindow::paintEvent(QPaintEvent *)
{
    return;
    // TODO: decide on ordinary window border or this
    // decoration handles!
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

void NotesWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void NotesWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}





