**[我的Blog发布地址](https://zephyrbd.github.io/p/cuitappdesignrace/)**
# 智记清单（MCZS To Do）项目说明
![智记清单](https://zephyrbd.github.io/images/cuitDRace/img5.png)
## 一、项目概述
智记清单是一款专为大学生和打工人设计的待办清单程序。它具备提醒功能，能有效帮助用户合理安排事务，其界面直观简洁，易于操作。在设计上充分考虑了用户的时间管理需求及使用体验，还通过随时间变化的背景为程序增添了趣味性。
## 二、设计思路
1. 背景与痛点：初入大学或职场，事务繁杂易遗忘重要事项，且多数人在事务安排上缺乏经验。
2. 功能需求分析：
    - 提醒功能：满足学习、社团等活动的提醒需求。
    - 今日待办：让用户清晰知晓当天任务。
    - 易上手：方便新建和管理待办事项。
3. 程序规划：
    - 数据存储模块：运用 json 文件、对象和数组保存用户提醒任务详情，确保数据持久化与高效读写。
    - 数据交互模块：作为模块间数据桥梁，保障程序流畅运行与协同工作。
    - 界面模块：呈现可视化界面，方便用户交互操作。
    - 提醒功能模块：实现提醒逻辑，包括时间设定、触发机制与多种提醒方式。
## 三、关键技术
1. json 和结构体应用：通过 todoINFO 结构体数组存放待办事项内容，利用 todoToJson 函数在关闭程序时将结构体转化为 json 对象存盘，借助 jsonArray 管理多个 json 对象，程序打开时用 jsonToTodo 函数转回结构体并添加到 Qlist 容器便于操作。
2. Qt 框架开发：
    - 信号与槽机制：实现高效对象间事件通信，如设置待办事项点击确认按钮时新建结构体变量，逻辑清晰且代码耦合度低。
    - 布局管理：多种布局管理器确保界面在不同电脑上视觉与操作良好。
    - 类库资源：丰富类库用于创建各类可视化组件，如 QPushButton 类创建按钮。
    - 用户体验支持：如实现背景切换动画，增强趣味性与体验感。
## 四、作品特色
1. 提醒功能：借助 Windows 系统通知，在 Qtimer 定时器检测下，以醒目的系统通知弹出提醒事项内容与地点。  
![提醒功能](https://zephyrbd.github.io/images/cuitDRace/img2.png)
2. 背景切换：利用 QDateTime 变量、Qtimer 定时器和 Switch-case 结构实现一天内背景随时间切换壁纸，且有多组可选。
![背景切换](https://zephyrbd.github.io/images/cuitDRace/img3.gif)
3. 图形化界面：基于 Qt 框架打造，布局合理、按钮一目了然，易于上手操作。
![图形化界面](https://zephyrbd.github.io/images/cuitDRace/img1.png)
## 五、项目信息
![团队Logo](https://zephyrbd.github.io/images/cuitDRace/img4.png)
1. 团队成员：
    - 项目主导及核心代码：ZephyrBD
    - JSON 转换方法：puuuuh
2. 指导老师：CUIT计算机学院老师
3. 版权与开源：
    - 提交给学校的 Alpha 1.0 版本用于特定比赛，请勿外泄。（Github的Releases中的Alpha 1.0版本为公共版）
    - 除背景图片组版权属于美国苹果公司外，其余内容遵循大赛共享协议，现在以 GPL - 2.0 协议开源到该 Github 仓库。
4. 项目文件：包含 MCZS_TO_DO.pro、头文件（如 aboutus.h 等）、源文件（如 aboutus.cpp 等）、界面文件（如 aboutus.ui 等）、资源文件 res.qrc 及其他文件 version.rc。
