#include "widget.h"
#include "ui_widget.h"
#include "QMessageBox"
#include "QPushButton"
#include "QCheckBox"
#include "EditItemDialog.h"
#include "QThread"
#include "QJsonArray"
#include "QJsonDocument"
#include "QJsonObject"
#include "QCloseEvent"
#include "QFile"
#include "QMenu"
#include "aboutus.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //设置窗口大小
    this->setMaximumSize(800,840);
    this->setMinimumSize(800,840);
    //创建bgLabel对象
    bglabel = new QLabel("", this);
    bglabel->setMinimumSize(800,840);
    bglabel->setMaximumSize(800,840);
    //定时器
    timer = new QTimer;
    ui->setupUi(this);
    ui->TodayList->setCheckable(true);
    //设置ok和cnacel的文字
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("清空");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    //设置日期框
    QDate dateObj = QDate::currentDate();
    QString dateString = dateObj.toString("yyyy年MM月dd日");
    ui->date->setText(dateString);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    //设置图标
    QIcon todayicon(":/images/today.png");
    QIcon importanticon(":/images/important.png");
    QIcon planicon(":/images/PLANTODO.png");
    QIcon alllisticon(":/images/ALLTODO.png");
    ui->TodayList->setIcon(todayicon);
    ui->imList->setIcon(importanticon);
    ui->planlist->setIcon(planicon);
    ui->alllList->setIcon(alllisticon);
    //托盘
    trayIcon = new QSystemTrayIcon;
    QIcon icon(":/images/icon.png");
    trayIcon->setIcon(icon);
    trayIcon->show();
    //设置托盘的右键菜单
    QMenu *menu = new QMenu();
    //设置菜单项目
    QAction *actionShow = new QAction(icon,"显示窗口",menu);
    QAction *actionExit = new QAction("退出程序", menu);
    menu->addAction(actionShow);
    menu->addAction(actionExit);
    trayIcon->setContextMenu(menu);
    trayIcon->show();
    //打开托盘的消息提示
    trayIcon->showMessage("提示", "保持程序挂起以便适时提醒你！", icon);
    //连接设置托盘Action的响应
    connect(actionShow,SIGNAL(triggered()), this, SLOT(showNormal()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(savejsonslot()));
    //连接回车和确定按钮
    connect(ui->TodDoEdit, SIGNAL(returnPressed()), this, SLOT(on_buttonBox_accepted()));
    connect(ui->localtion, SIGNAL(returnPressed()), this, SLOT(on_buttonBox_accepted()));
    //连接单击选中和双击修改
    connect(ui->listTODO, &QListWidget::itemClicked, this, &Widget::onItemclicked);
    connect(ui->listTODO, &QListWidget::itemDoubleClicked, this, &Widget::onItemDoubleclicked);
    //定时器到时间
    connect(timer,&QTimer::timeout,this,&Widget::timeoutslot);
    connect(timer,&QTimer::timeout,this,&Widget::changebackground);
}

Widget::~Widget()
{
    delete ui;
}

//变量声明区
QList<todoINFO> todolist;
int nonum = 0;
int is_alllist = 1;
QString theme = "Minecraft";

//把结构体转化为JSON对象
QJsonObject todoToJson(const todoINFO &t)
{
    QJsonObject obj;
    obj["text"] = t.text;
    obj["location"] = t.location;
    obj["deadtime"] = t.deadtime.toString(Qt::ISODate);
    obj["clocktime"] = t.clocktime.toString(Qt::ISODate);
    obj["isImpotant"] = t.isImpotant;
    obj["doorwait"] = t.doorwait;
    return obj;
}

//把JSON对象转化回结构体
todoINFO jsonToTodo(const QJsonObject &obj) {
    todoINFO t;
    t.text = obj["text"].toString();
    t.location = obj["location"].toString();
    t.deadtime = QDate::fromString(obj["deadtime"].toString(), Qt::ISODate);
    t.clocktime = QDateTime::fromString(obj["clocktime"].toString(), Qt::ISODate);
    t.isImpotant = obj["isImpotant"].toInt();
    t.doorwait = obj["doorwait"].toInt();
    return t;
}

//窗口第一次打开时要干的事情
bool is_nextopen = false;
void Widget::showEvent(QShowEvent *event)
{
    if(is_nextopen) return;
    is_nextopen = true;
    QWidget::showEvent(event);
    //获取程序路径
    QString filePath = QCoreApplication::applicationDirPath();
    //读取设置文件
    QString settingsPath = filePath + "/settings.json";
    QFile settings(settingsPath);
    if (settings.exists() && settings.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = settings.readAll();
        settings.close();
        QJsonDocument readDoc = QJsonDocument::fromJson(data);
        if(readDoc.isObject())
        {
            QJsonObject readsettings = readDoc.object();
            theme = readsettings["themename"].toString();
        }
        //qDebug() << theme;
    }
    changebackground();//及时换背景
    //读取todo数据
    QString todofilePath = filePath + "/todos.json";
    QFile file(todofilePath);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument readDoc = QJsonDocument::fromJson(data);
        if (readDoc.isArray())
        {
            QJsonArray readArray = readDoc.array();
            for (const auto &value : readArray)
            {
                if (value.isObject())
                {
                    QJsonObject obj = value.toObject();
                    todoINFO recoveredTodo = jsonToTodo(obj);
                    recoveredTodo.todononum = nonum;
                    todolist.append(recoveredTodo);
                    nonum++;
                }
            }
        }
    }
    //把东西显示到列表中
    for (const auto &t : todolist)
    {
        //nonum = std::max(nonum,t.todononum);
        if(t.doorwait == 0 && t.is_del == false)
            createTODO(t);
    }
    timer->start(TIMEOUT);
    timeoutslot();//马上调一遍，防止当场提醒
}

//在关闭时把todo数据写入JSON
void Widget::savejsonslot(bool flag)
{
    QString filePath = QCoreApplication::applicationDirPath();
    //写设置
    QJsonObject settings;
    settings["themename"] = theme;
    QJsonDocument settingsDoc(settings);
    QString settingsPath = filePath + "/settings.json";
    QFile settingsfile(settingsPath);
    if (settingsfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        settingsfile.write(settingsDoc.toJson());
        settingsfile.close();
    }
    //保存todo数据的逻辑
    QJsonArray jsonArray;
    for (const auto &t : todolist)
    {
        if(t.is_del == false)
        {
            QJsonObject jsonObj = todoToJson(t);
            jsonArray.append(jsonObj);
        }
    }
    QJsonDocument doc(jsonArray);
    // 将todo写入磁盘的json文件
    QString todofilePath = filePath + "/todos.json";
    QFile file(todofilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(doc.toJson());
        file.close();
    }
    else
        QMessageBox::information(this,"MCZS To Do","无法打开文件进行写入！");
    if(flag)
        QApplication::exit(0);
}

//提醒我函数
void Widget::timeoutslot()
{

    QDateTime now = QDateTime::currentDateTime();
    QIcon icon(":/images/icon.png");
    for (auto &t : todolist)
    {
        if(t.doorwait == 0 && t.is_del == false && t.is_clock == false && t.clocktime.time().hour() == now.time().hour() && t.clocktime.time().minute() == now.time().minute())
        {
            trayIcon->showMessage(t.text,t.location,icon);
            t.is_clock = true;
        }
    }
}

//换背景函数
void Widget::changebackground()
{
    QDateTime time = QDateTime::currentDateTime();
    QString indexpath;
    if(theme == "Mojave")
        indexpath = ":/images/mojave/mojave";
    else if(theme == "Minecraft")
        indexpath = ":/images/minecraft/minecraft";
    else if(theme == "Catalina")
        indexpath = ":/images/catalina/catalina";
    QString imagePath;
    int hour = time.time().hour();
    //hour = 5;
    switch(hour)
    {
    case 0:case 1:case 2:case 3:case 4:
        imagePath = indexpath + "16";
        break;
    case 5:
        imagePath = indexpath + "1";
        break;
    case 6:
        imagePath = indexpath + "2";
        break;
    case 7:
        imagePath = indexpath + "3";
        break;
    case 8:
        imagePath = indexpath + "4";
        break;
    case 9:
        imagePath = indexpath + "5";
        break;
    case 10:
        imagePath = indexpath + "6";
        break;
    case 11:case 12:
        imagePath = indexpath + "7";
        break;
    case 13:case 14:
        imagePath = indexpath + "8";
        break;
    case 15:case 16:
        imagePath = indexpath + "9";
        break;
    case 17:
        imagePath = indexpath + "10";
        break;
    case 18:
        imagePath = indexpath + "11";
        break;
    case 19:
        imagePath = indexpath + "12";
        break;
    case 20:
        imagePath = indexpath + "13";
        break;
    case 21:
        imagePath = indexpath + "14";
        break;
    case 22: case 23:
        imagePath = indexpath + "15";
        break;
    default:
        imagePath = indexpath + "7";
        break;
    }
    imagePath = imagePath + ".jpg";
    QImage img;
    img.load(imagePath);
    bglabel->setPixmap(QPixmap::fromImage(img));
}

//重写closeEvent
void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("关闭MCZS To do？");
    msgBox.setText("完全关闭程序可能会导致你错过提醒！");
    msgBox.addButton("保持后台", QMessageBox::AcceptRole);
    msgBox.addButton("完全退出", QMessageBox::RejectRole);
    int result = msgBox.exec();
    //qDebug() << result;
    if (result==2)
    {
        QIcon icon(":/images/icon.png");
        trayIcon->showMessage("提示","程序会在后台保持运行",icon);
        savejsonslot(false);
        setVisible(false);
        event->ignore();
    }
    else
        savejsonslot(true);
}

//获取待办ID的函数
int Widget::todoid(int row)
{
    QListWidgetItem* todononuminfo = ui->nonum->item(row);
    if(todononuminfo == nullptr) return 0;
    QString strcliecked = todononuminfo->text();
    int clieckednum = strcliecked.toInt();
    return clieckednum;
}

//清除所有QLineEdit
void Widget::clearALL()
{
    //刷新两个复选框
    ui->clockcheck->setChecked(false);
    ui->isimportant->setChecked(false);
    //刷新两个日期框
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    //清除输入框
    ui->TodDoEdit->clear();
    ui->localtion->clear();
}

//清除所有输入框
void Widget::on_buttonBox_rejected()
{
    clearALL();
}

//在TODOLIST中创建并显示item
void Widget::createTODO(todoINFO t)
{
    //创建待办
    QListWidgetItem* textinfo = new QListWidgetItem(t.text);
    QListWidgetItem* locationinfo = new QListWidgetItem(t.location);
    //创建待办编号
    QListWidgetItem* todononuminfo = new QListWidgetItem(QString::number(t.todononum));
    //创建待办复选框
    textinfo->setFlags(textinfo->flags() | Qt::ItemIsUserCheckable);
    //复选框状态
    if(t.doorwait == 0)
        textinfo->setCheckState(Qt::Unchecked);
    else
    {
        textinfo->setCheckState(Qt::Checked);
        //textinfo删除线
        QFont font = textinfo->font();
        font.setStrikeOut(true);
        textinfo->setFont(font);
        //locationinfo删除线
        locationinfo->font();
        font.setStrikeOut(true);
        locationinfo->setFont(font);
    }
    //是否重要
    if(t.isImpotant == 1)
        textinfo->setText("★"+t.text);
    //把待办添加到两个列表
    ui->listTODO->addItem(textinfo);
    ui->listlocation->addItem(locationinfo);
    //把待办添加到编号列表
    ui->nonum->addItem(todononuminfo);
}

//新建待办
void Widget::on_buttonBox_accepted()
{
    todoINFO t;
    //初始化结构体中的待办和地点的内容
    if (ui->TodDoEdit->text().isEmpty())
    {
        // TodDoEdit没有输入内容
        clearALL();
        QMessageBox::information(this,"MCZS To Do","请正确输入你的待办内容！");
        return;
    }
    else
        t.text = ui->TodDoEdit->text(); // TodDoEdit有输入内容

    //地点
    if (ui->localtion->text().isEmpty())
        t.location = "无";// localtion没有输入内容
    else
        t.location = ui->localtion->text(); // localtion有输入内容
    //初始化结构体中的待办编号
    t.todononum = nonum;

    //读取提醒时间
    if(ui->clockcheck->isChecked() == true)
        t.clocktime = ui->dateTimeEdit->dateTime();
    //读取截止日期
    t.deadtime = ui->dateEdit->date();

    //读取是否为重要
    if(ui->isimportant->isChecked() == true)
        t.isImpotant = 1;
    //创建并且添加到列表
    switch (is_alllist)
    {
    case 2:
        if(t.deadtime == QDate::currentDate())
            createTODO(t);
        break;
    case 3:
        if(t.isImpotant == 1)
            createTODO(t);
        break;
    default:
        createTODO(t);
        break;
    }
    todolist.append(t);
    //清除
    clearALL();
    //为下一个待办增加编号
    nonum++;
}

//删除待办
void Widget::on_del_clicked()
{
    //读取到是第几行
    int row = ui->listTODO->currentRow();
    //删除待办和地点的内容
    ui->listTODO->takeItem(row);
    ui->listlocation->takeItem(row);
    //删除编号表中的编号
    int id = todoid(row);
    todolist[id].is_del = true;
    ui->nonum->takeItem(row);
    //清空选定框
    ui->CheckTab->clear();
}

//单击Item
void Widget::onItemclicked(QListWidgetItem* item)
{
    // 获取触发信号的 QListWidget
    QListWidget* listWidget = item->listWidget();
    QString text;
    if (listWidget == ui->listTODO)
    {
        text = item->text(); //获取待办事项的内容
        ui->CheckTab->setText(text); //显示在修改框中
    }
}

//双击修改Item
void Widget::onItemDoubleclicked(QListWidgetItem* item)
{
    QListWidget* listWidget = item->listWidget();
    int row = 0;
    if (listWidget == ui->listTODO)
        row = ui->listTODO->currentRow();
    //读取待办ID
    int clieckedid = todoid(row);
    //传递到修改框
    QString title = "修改:“" + todolist[clieckedid].text + "”";
    EditItemDialog dialog(this,todolist[clieckedid],title);
    if (dialog.exec() == QDialog::Accepted)
    {
        todoINFO newText = dialog.getText();
        if(!newText.text.isEmpty()) //判定user修改后的text是否为空
        {
            if(newText.isImpotant)
                ui->listTODO->item(row)->setText("★"+newText.text);
            else
                ui->listTODO->item(row)->setText(newText.text);
            todolist[clieckedid].text = newText.text;
        }
        //判定user修改后的location是否为空
        if(!newText.location.isEmpty())
        {
            ui->listlocation->item(row)->setText(newText.location);
            todolist[clieckedid].location = newText.location;
        }
        todolist[clieckedid].deadtime = newText.deadtime;
        todolist[clieckedid].is_clock = newText.is_clock;
        todolist[clieckedid].clocktime = newText.clocktime;
        todolist[clieckedid].isImpotant = newText.isImpotant;
    }
}

//完成复选框被勾选
void Widget::on_listTODO_itemChanged(QListWidgetItem *item)
{
    if(item == NULL) return;
    int row = item->listWidget()->row(item);
    int clieckedid = todoid(row);
    QListWidgetItem *iteml = ui->listlocation->item(row);
    if(iteml == NULL) return;
    QFont font1 = item->font(),font2 = iteml->font();
    if (item->checkState() == Qt::Checked)
    {
        font1.setStrikeOut(true);
        font2.setStrikeOut(true);
        todolist[clieckedid].doorwait = 1;
    }
    else
    {
        font1.setStrikeOut(false);
        font2.setStrikeOut(false);
        todolist[clieckedid].doorwait = 0;
    }
    item->setFont(font1);
    iteml->setFont(font2);
}

//几个菜单

//计划内
void Widget::on_planlist_clicked()
{
    is_alllist = 1;
    ui->menutitle->setText("计划内：");
    ui->listTODO->clear();
    ui->listlocation->clear();
    ui->nonum->clear();
    for (const auto &t : todolist)
    {
        if(t.doorwait == 0 && t.is_del == false)
            createTODO(t);
    }
}

//今日待办
void Widget::on_TodayList_clicked()
{
    is_alllist = 2;
    ui->menutitle->setText("我的一天：");
    ui->listTODO->clear();
    ui->listlocation->clear();
    ui->nonum->clear();
    for (const auto &t : todolist)
    {
        if(t.deadtime == QDate::currentDate() && t.is_del==false && t.doorwait == 0)
            createTODO(t);
    }
}

//重要
void Widget::on_imList_clicked()
{
    is_alllist = 3;
    ui->menutitle->setText("重要：");
    ui->listTODO->clear();
    ui->listlocation->clear();
    ui->nonum->clear();
    for (const auto &t : todolist)
    {
        nonum = std::max(nonum,t.todononum);
        if(t.isImpotant == 1 && t.is_del == false && t.doorwait == 0)
            createTODO(t);
    }
}

//全部
void Widget::on_alllList_clicked()
{
    is_alllist = 4;
    ui->menutitle->setText("全部：");
    ui->listTODO->clear();
    ui->listlocation->clear();
    ui->nonum->clear();
    for (const auto &t : todolist)
        if(t.is_del == false)
            createTODO(t);
}

void Widget::on_Copyright_clicked()
{
    aboutus dialog(this,theme);
    if (dialog.exec() == QDialog::Accepted) theme = dialog.themeselect();
    changebackground();
}
