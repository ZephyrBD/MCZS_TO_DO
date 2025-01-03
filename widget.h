#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QListWidget>
#include <QDateTime>
#include <QDate>
#include <QCloseEvent>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QLabel>
#include <QSettings>
#include <QDir>


#define TIMEOUT 30 * 1000

struct todoINFO
{
    int todononum;//待办ID
    int doorwait = 0;//是否完成
    int isImpotant = 0;//是否重要
    bool is_clock = false;//是否在本次提醒过
    bool is_del = false;//是否在本次删除
    QString text;//具体内容
    QString location;//地点
    QDateTime clocktime;//提醒时间
    QDate deadtime;//截止时间
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
protected:


public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void showEvent(QShowEvent *event) override;
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void onItemclicked(QListWidgetItem* item);
    void onItemDoubleclicked(QListWidgetItem* item);
    void on_del_clicked();
    int todoid(int row);
    void clearALL();
    void on_listTODO_itemChanged(QListWidgetItem *item);
    void closeEvent(QCloseEvent *event) override;
    void createTODO(todoINFO t);
    void on_imList_clicked();
    void on_TodayList_clicked();
    void on_planlist_clicked();
    void on_alllList_clicked();
    void savejsonslot(bool flag = true);
    void timeoutslot();
    void changebackground();

    void on_Copyright_clicked();

private:
    Ui::Widget *ui;
    QTimer *timer;
    QSystemTrayIcon *trayIcon;
    QLabel *bglabel;
};
#endif // WIDGET_H
