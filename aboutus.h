#ifndef ABOUTUS_H
#define ABOUTUS_H

#include <QDialog>

namespace Ui {
class aboutus;
}

class aboutus : public QDialog
{
    Q_OBJECT

public:
    explicit aboutus(QWidget *parent = nullptr,const QString &themename = QString("themename"));
    ~aboutus();
    QString themeselect();

private slots:

private:
    Ui::aboutus *ui;
};

#endif // ABOUTUS_H
