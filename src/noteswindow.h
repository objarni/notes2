#pragma once

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
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::NotesWindow *ui;
    QString const & mNotes2File;
};
