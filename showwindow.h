#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class ShowWindow : public QWidget
{
    Q_OBJECT

public:
    ShowWindow(QWidget *parent = 0);

    void setWindowFlags(Qt::WindowFlags flags);

private:
    QTextEdit *textEdit;
    QPushButton *closeButton;
};
//! [0]

#endif
