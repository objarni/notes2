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
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;

    int desktopHeight();
    int desktopWidth();
    int windowWidth();
    int windowHalfHeight();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void moveWindowBottomLeft();
    void moveWindowBottomRight();
    void moveWindowTopRight();
    void moveWindowTopLeft();
    void insertCheckbox();

private:
    Ui::NotesWindow *ui;
    QString const & mNotes2File;
    QShortcut* keyCtrl0;
    QShortcut* keyCtrl1;
    QShortcut* keyCtrl2;
    QShortcut* keyCtrl3;
    QShortcut* keyCtrl4;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};
