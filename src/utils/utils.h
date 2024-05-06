#pragma once
#include <QScreen>
#include <windows.h>
#include <shellapi.h>
#include <sass.h>

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
    abd.cbSize     = sizeof(APPBARDATA);
    SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
    switch (abd.uEdge) {
        case ABE_LEFT: pos = 0; break;
        case ABE_TOP: pos = 1; break;
        case ABE_RIGHT: pos = 2; break;
        case ABE_BOTTOM: pos = 3; break;
        default: pos = 4; break;
    }
    height    = abd.rc.bottom - abd.rc.top;
    width     = abd.rc.right - abd.rc.left;
    abdWidth  = abd.rc.right;
    abdHeight = abd.rc.top;
}

// 加载 sass 样式
inline QString invokeStyleSheetLoad(const QString& qValue)
{
    const auto path    = QStringLiteral("../../src/assets/style/%1/%1.sass").arg(qValue);
    const auto root    = sass_make_file_context(path.toStdString().c_str());
    const auto context = sass_file_context_get_context(root);
    const auto option  = sass_context_get_options(context);
    sass_option_set_precision(option, 2);
    sass_option_set_output_style(option, SASS_STYLE_COMPACT);
    sass_compile_file_context(root);
    if (sass_context_get_error_status(context) != 0) {
        qFatal(sass_context_get_error_message(context));
        return QString();
    }

    const auto styleSheet = QString::fromStdString(sass_context_get_output_string(context));
    sass_delete_file_context(root);
    return styleSheet;
}