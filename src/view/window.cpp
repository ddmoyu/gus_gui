#include "window.h"
#include "ui_window.h"

#include "../utils/utils.h"

window::window(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::window())
{
    ui->setupUi(this);
    initUi();
    initConnect();
    addTaskbar();
}

window::~window()
{
    delete ui;
}

void window::initUi()
{
    // setWindowFlag(Qt::FramelessWindowHint);
    // TODO load style
}

void window::initConnect()
{
    connect(ui->btn_add, &QToolButton::clicked, [&]() {
        closeWindow();
    });
    connect(ui->btn_close, &QToolButton::clicked, [&]() {
        showWindow();
    });
}

void window::addTaskbar()
{
    m_tray = new QSystemTrayIcon(this);
    m_tray->setIcon(QIcon(":/icon/git.png"));
    addTaskbarMouseLeftBtnHandle();
    addTaskbarMouseRightBtnHandle();
    m_tray->show();
}

void window::addTaskbarMouseLeftBtnHandle()
{
    connect(m_tray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            showWindow();
        }
    });
}

void window::addTaskbarMouseRightBtnHandle()
{
    QAction * auto_star = new QAction(tr("开机自启"), this);
    connect(auto_star, &QAction::triggered, [&]() {
        toggle_auto_start();
    });

    QAction *quit = new QAction("Quit", this);
    connect(quit, &QAction::triggered, [&]() {
        qApp->quit();
    });

    m_tray_menu = new QMenu(this);
    m_tray_menu->addAction(auto_star);
    m_tray_menu->addAction(quit);
    m_tray->setContextMenu(m_tray_menu);
}

void window::showWindow()
{
    this->show();
    this->raise();
    this->activateWindow();
}

void window::closeWindow()
{
    this->hide();
}

void window::readGitUser() { }

void window::addGitUser() { }

void window::deleteGitUser() { }

void window::switchGitUser() { }

void window::modifyGitUser() { }
