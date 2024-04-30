﻿#pragma once

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui
{
class window;
}; // namespace Ui
QT_END_NAMESPACE

class window : public QWidget
{
    Q_OBJECT

public:
    window(QWidget* parent = nullptr);
    ~window() override;

private:
    Ui::window* ui;

    QSystemTrayIcon* m_tray{nullptr};
    QMenu * m_tray_menu{nullptr};

    // 初始化 UI
    void initUi();
    // 初始化 信号槽
    void initConnect();

    // 添加托盘图标
    void addTaskbar();
    // 添加托盘图标 点击事件
    void addTaskbarMouseLeftBtnHandle();
    // 添加托盘图标 右键菜单及其事件
    void addTaskbarMouseRightBtnHandle();

    // 显示视窗
    void showWindow();
    // 关闭视窗
    void closeWindow();

    // 读取 git 用户
    void readGitUser();
    // 添加 git 用户
    void addGitUser();
    // 删除 git 用户
    void deleteGitUser();
    // 切换 git 用户
    void switchGitUser();
    // 修改 git 用户
    void modifyGitUser();
};