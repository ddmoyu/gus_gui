#pragma once
#include <QScreen>
#include <windows.h>
#include <shellapi.h>
#include <sass.h>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

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
    QProcess process;
    process.start("where", QStringList() << "git");
    process.waitForFinished();
    QString out(process.readAllStandardOutput());
    if (out.trimmed() != "") {
        return true;
    }
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
    const auto ctx     = sass_make_file_context(path.toStdString().c_str());
    const auto context = sass_file_context_get_context(ctx);
    const auto options = sass_context_get_options(context);
    sass_option_set_precision(options, 2);
    sass_option_set_output_style(options, SASS_STYLE_COMPACT);
    sass_compile_file_context(ctx);
    if (sass_context_get_error_status(context) != 0) {
        qFatal(sass_context_get_error_message(context));
        sass_delete_file_context(ctx);
        return QString();
    }
    const auto styleSheet = QString::fromStdString(sass_context_get_output_string(context));
    sass_delete_file_context(ctx);
    return styleSheet;
}

// 从 qrc 资源里加载样式
inline QString loadStyleSheetFromQrc(const QString& theme)
{
    const auto path = QString(":/sass/style/%1/%1.scss").arg(theme);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal("Failed to open file.");
        return QString();
    }
    QTextStream in(&file);
    const auto scssContent = in.readAll();
    file.close();

    // Create a libsass context
    auto scss_byte = scssContent.toUtf8();
    char* input    = _strdup(scss_byte.constData());
    if (!input) {
        qFatal("Failed to allocate memory.");
        return QString();
    }

    struct Sass_Data_Context* data_ctx = sass_make_data_context(input);
    if (!data_ctx) {
        free(input);
        qFatal("Failed to create Sass_Data_Context.");
        return QString();
    }

    struct Sass_Context* ctx = sass_data_context_get_context(data_ctx);
    if (!ctx) {
        sass_delete_data_context(data_ctx);
        free(input);
        qFatal("Failed to create Sass_Context.");
        return QString();
    }

    // Compile the SASS to CSS
    int status = sass_compile_data_context(data_ctx);

    // Check for errors
    if (status != 0) {
        qWarning() << "Error compiling SCSS:" << sass_context_get_error_message(ctx);
        sass_delete_data_context(data_ctx);
        free(input);
        return QString();
    }

    // Get the result
    const char* css   = sass_context_get_output_string(ctx);
    QString cssString = QString(css);

    // Cleanup
    sass_delete_data_context(data_ctx);
    return cssString;
}

struct GitConfig
{
    QString username;
    QString email;
};

// 获取 全局 git 的 用户名 和 email
inline GitConfig getGitConfig()
{
    QProcess process;
    process.start("git",
                  QStringList() << "config"
                                << "--global"
                                << "user.name");
    process.waitForFinished();
    QString nameOut(process.readAllStandardOutput());

    process.start("git",
                  QStringList() << "config"
                                << "--global"
                                << "user.email");
    process.waitForFinished();
    QString emailOut(process.readAllStandardOutput());
    process.close();

    GitConfig cfg;
    cfg.username = nameOut.trimmed();
    cfg.email    = emailOut.trimmed();
    return cfg;
}

// 设置 全局 git 用户信息
inline bool setGitConfig(const GitConfig& cfg)
{
    QProcess process;
    process.start("git",
                  QStringList() << "config"
                                << "--global"
                                << "user.name" << cfg.username);
    process.waitForFinished();

    process.start("git",
                  QStringList() << "config"
                                << "--global"
                                << "user.email" << cfg.email);
    process.waitForFinished(); // 等待进程结束
    process.close();

    auto user = getGitConfig();
    if (user.username == cfg.username && user.email == cfg.email) {
        return true;
    }
    return false;
}

// 获取 Gravatar 头像
inline QByteArray getAvatar(const QString& _email)
{
    QString email                  = _email.trimmed().toLower();
    QByteArray emailData           = email.toUtf8();
    QByteArray hash                = QCryptographicHash::hash(emailData, QCryptographicHash::Md5).toHex();
    QString gravatarUrl            = QString("https://www.gravatar.com/avatar/%1?d=identicon").arg(QString(hash));
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QUrl url(gravatarUrl);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    QByteArray avatarData;
    if (reply->error() == QNetworkReply::NoError) {
        avatarData = reply->readAll();
    }
    reply->deleteLater();
    return avatarData;
}