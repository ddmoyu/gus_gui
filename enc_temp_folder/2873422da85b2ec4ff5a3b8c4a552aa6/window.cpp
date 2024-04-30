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
     setWindowFlag(Qt::FramelessWindowHint);
    //this->setMinimumWidth(800);
    // TODO load style
}

void window::initConnect()
{
    connect(ui->btn_add, &QToolButton::clicked, [&]() {
        showWindow();
    });
    connect(ui->btn_close, &QToolButton::clicked, [&]() {
        closeWindow();
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
    QAction* auto_star = new QAction(tr("开机自启"), this);
    connect(auto_star, &QAction::triggered, [&]() {
        toggle_auto_start(true);
    });

    QAction* quit = new QAction("Quit", this);
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
    int pos, width, height, abdWidth, abdHeight;
    get_windows_taskbar_info(pos, width, height, abdWidth, abdHeight);
    qDebug() << "pos: " << pos;
    qDebug() << "height: " << height;
    qDebug() << "width: " << width;
    qDebug() << "abdWidth: " << abdWidth;
    qDebug() << "abdHeight: " << abdHeight;

    QPoint mousePos = QCursor::pos();
    QRect winRect   = this->rect();

    if (pos == 3) {
        QPointF center = winRect.center();
        qreal x;
        if (mousePos.x() + winRect.center().x() <= abdWidth) {
            x = mousePos.x() - center.x();
        }
        else 
        {
            x = abdWidth - winRect.width();
        }
        
        auto y = abdHeight - winRect.height();
        this->move(x, y);
    }
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
