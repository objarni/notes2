#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QMainWindow>

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotesWindow(QString const & notes2File);
    ~NotesWindow();

private:
    Ui::NotesWindow *ui;
    QString const & mNotes2File;
};

#endif // NOTESWINDOW_H
