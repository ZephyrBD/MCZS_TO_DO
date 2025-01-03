#include "edititemdialog.h"
#include "ui_edititemdialog.h"
#include <qpushbutton.h>

EditItemDialog::EditItemDialog(QWidget *parent,const todoINFO t,const QString &title)
    : QDialog(parent)
    , ui(new Ui::EditItemDialog)
{
    this->setMaximumSize(490,170);
    this->setMinimumSize(490,170);
    this->setWindowTitle(title);
    ui->setupUi(this);
    //定义确定和取消按钮的文本
    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setText("取消");
    okButton->setText("确定");
    //设置两个文本框的内容为修改之前的
    ui->todoedit->setText(t.text);
    ui->locationedit->setText(t.location);
    //设置deadtime为修改之前的
    ui->deadtime->setDate(t.deadtime);
    //设置clocktime和clockcheck为修改之前的
    if(t.clocktime.isValid())
    {
        ui->clockcheck->setChecked(true);
        ui->clocktime->setDateTime(t.clocktime);
    }
    else
    {
        ui->clockcheck->setChecked(false);
        ui->clocktime->setDateTime(QDateTime::currentDateTime());
    }
    //设置isimportant为修改之前的
    if(t.isImpotant == 1)
        ui->isimpor->setChecked(true);
    else
        ui->isimpor->setChecked(false);
}

todoINFO EditItemDialog::getText() const
{
    todoINFO edit;
    if(ui->isimpor->isChecked() == true)
        edit.isImpotant = 1;
    if(ui->clockcheck->isChecked() == true)
    {
        edit.clocktime = ui->clocktime->dateTime();
        edit.is_clock = false;
    }
    edit.text = ui->todoedit->text();
    edit.location = ui->locationedit->text();
    edit.deadtime = ui->deadtime->date();
    return edit;
}

EditItemDialog::~EditItemDialog()
{
    delete ui;
}

