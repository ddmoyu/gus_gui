#pragma once
#include <QScreen>

// 检查是否 开机自启
inline bool check_auto_start() { return false; }

// 开机自启 管理
inline void toggle_auto_start(bool flag){}

// 检查是否存在 Git
inline bool check_git_exists() { return false; }

// 获取任务栏信息
inline void get_taskbar_info(int &pos, int &height, int &width)
{
    QScreen * screen = qApp->primaryScreen();
}