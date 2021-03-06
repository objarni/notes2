#pragma once

#include <QMainWindow>
#include <QShortcut>

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotesWindow(QString const & notes2File);
    ~NotesWindow();
    void closeEvent(QCloseEvent *event) override;

public slots:
    void insertCheckbox();
    void closeWindow();

private:
    Ui::NotesWindow *ui;
    QString const & mNotes2File;
    QShortcut* keyCtrl0;
    QShortcut* keyEsc;
};
