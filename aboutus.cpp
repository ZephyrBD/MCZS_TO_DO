#include "aboutus.h"
#include "ui_aboutus.h"
#include "QPushButton"
#include "QRadioButton"

aboutus::aboutus(QWidget *parent,const QString &themename)
    : QDialog(parent)
    , ui(new Ui::aboutus)
{
    this->setMaximumSize(640,310);
    this->setMinimumSize(640,310);
    QString title = "关于程序和开发者团队";
    ui->setupUi(this);
    this->setWindowTitle(title);
    QPushButton *okButton = ui->AcceptButton->button(QDialogButtonBox::Ok);
    okButton->setText("确定");
    if(themename=="Minecraft")
    {
        ui->Minecraft->setChecked(true);
        ui->Mojave->setChecked(false);
        ui->Catalina->setChecked(false);
    }
    else if(themename=="Mojave")
    {
        ui->Minecraft->setChecked(false);
        ui->Mojave->setChecked(true);
        ui->Catalina->setChecked(false);
    }
    else if(themename=="Catalina")
    {
        ui->Minecraft->setChecked(false);
        ui->Mojave->setChecked(false);
        ui->Catalina->setChecked(true);
    }
}

aboutus::~aboutus()
{
    delete ui;
}

QString aboutus::themeselect()
{
    QRadioButton *mc = ui->Minecraft,*mojave = ui->Mojave,*catalina = ui->Catalina;
    QString themename = "Minecraft";
    if(mc->isChecked())
        themename = "Minecraft";
    else if(mojave->isChecked())
        themename = "Mojave";
    else if(catalina->isChecked())
        themename = "Catalina";
    return themename;
}
