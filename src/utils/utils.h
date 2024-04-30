#pragma once
#include <QScreen>
#include <windows.h>
#include <shellapi.h>

// 检查是否 开机自启
inline bool check_auto_start()
{
    return false;
}

// 开机自启 管理
inline void toggle_auto_start(bool flag) { }

// 检查是否存在 Git
inline bool check_git_exists()
{
    return false;
}

// 获取 windows 系统 任务栏 的位置和大小信息
inline void get_windows_taskbar_info(int& pos, int& width, int& height, int& abdWidth, int& abdHeight)
{
    APPBARDATA abd = {};
    abd.cbSize = sizeof(APPBARDATA);
    SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
    switch (abd.uEdge) {
        case ABE_LEFT: pos = 0; break;
        case ABE_TOP: pos = 1; break;
        case ABE_RIGHT: pos = 2; break;
        case ABE_BOTTOM: pos = 3; break;
        default: pos = 4; break;
    }
    height = abd.rc.bottom - abd.rc.top;
    width  = abd.rc.right - abd.rc.left;
    abdWidth = abd.rc.right;
    abdHeight = abd.rc.top;
}
