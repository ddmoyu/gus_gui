#include "window.h"
#include "ui_window.h"

#include "../utils/utils.h"
#include "../components/userItem.h"
#include "../components/addItem.h"

window::window(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::window())
{
    ui->setupUi(this);
    initUi();
    initConnect();
    addTaskbar();
    readGitUser();
}

window::~window()
{
    delete ui;
}

void window::initUi()
{
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    hide();

    const QString style = invokeStyleSheetLoad("dark");
    setStyleSheet(style);

    m_list = ui->list;
}

void window::initConnect()
{
    connect(ui->btn_hide, &QToolButton::clicked, [&]() {
        showWindow(false);
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
            showWindow(true);
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

void window::showWindow(bool flag)
{
    if (!flag) {
        hide();
        return;
    }
    int pos, width, height, abdWidth, abdHeight;
    get_windows_taskbar_info(pos, width, height, abdWidth, abdHeight);
    QPoint mousePos = QCursor::pos();
    QRect winRect   = rect();

    if (pos == 3) {
        QPointF center = winRect.center();
        int x;
        if (mousePos.x() + winRect.center().x() <= abdWidth) {
            x = mousePos.x() - center.x();
        }
        else {
            x = abdWidth - winRect.width();
        }
        auto y = abdHeight - winRect.height();
        move(x, y);
    }
    show();
    raise();
    activateWindow();
}

void window::closeWindow()
{
    qApp->quit();
}

void window::readGitUser()
{
    for (auto i = 0; i < 6; ++i) {
        QListWidgetItem* item = new QListWidgetItem(m_list);
        QString name          = QString("name%1").arg(i);
        UserItem* userItem    = new UserItem(QPixmap(":/images/1mb.png"), name, "daydaymoyu@gmail.com", m_list);
        item->setSizeHint(userItem->sizeHint());
        m_list->setItemWidget(item, userItem);
    }
    QListWidgetItem* pItem = new QListWidgetItem(m_list);
    AddItem* addItem       = new AddItem();
    pItem->setSizeHint(addItem->sizeHint());
    m_list->setItemWidget(pItem, addItem);
}

void window::addGitUser() { }

void window::deleteGitUser() { }

void window::switchGitUser() { }

void window::modifyGitUser() { }
