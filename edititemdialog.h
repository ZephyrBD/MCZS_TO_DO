#ifndef EDITITEMDIALOG_H
#define EDITITEMDIALOG_H

#include <QDialog>
#include <QDate>
#include "widget.h"

namespace Ui {

class EditItemDialog;
}

class EditItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditItemDialog(QWidget *parent = nullptr,const todoINFO t = todoINFO(), const QString &title = QString("Edit Item"));
    ~EditItemDialog();
    todoINFO getText() const;
private slots:

private:
    Ui::EditItemDialog *ui;
};

#endif // EDITITEMDIALOG_H
