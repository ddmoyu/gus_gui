#include "window.h"
#include "ui_window.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QMouseEvent>
#include "../utils/utils.h"
#include "../components/userItem.h"
#include "../components/addItem.h"
#include "../components/addDialog.h"

window::window(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::window())
{
    ui->setupUi(this);
    initTaskbar();
    initUi();
    initListContextMenu();
    initConnect();
    initDB();
    initGit();
}

window::~window()
{
    delete ui;
}

void window::initUi()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::StrongFocus);

    /*const QString style = invokeStyleSheetLoad("dark");
    qApp->setStyleSheet(style);*/
    const QString css = loadStyleSheetFromQrc("dark");
    qApp->setStyleSheet(css);

    m_list = ui->list;
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
}

void window::initConnect()
{
    connect(ui->btn_more, &QToolButton::clicked, [&]() {
        QDesktopServices::openUrl(QUrl("https://github.com/ddmoyu/gus_gui"));
    });
    connect(ui->btn_hide, &QToolButton::clicked, [&]() {
        showWindow(false);
    });
    connect(ui->btn_close, &QToolButton::clicked, [&]() {
        closeWindow();
    });
    connect(m_list, &QListWidget::itemClicked, this, &window::onItemClicked);
}

void window::initDB()
{
    m_db = new DbManager("user.sqlite");
    readGitUser();
}

void window::initGit() const
{
    const auto flag = check_git_exists();
    if (flag) {
        auto gc = getGitConfig();
        if (!gc.email.isEmpty() && !gc.username.isEmpty()) {
            auto _user = m_db->getUser(gc.username, gc.email);
            if (_user.name.isEmpty() || _user.email.isEmpty()) {
                addGitUser(gc.username, gc.email);
                readGitUser();
            }
        }
    }
}

void window::initListContextMenu()
{
    m_list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_list, &QListWidget::customContextMenuRequested, [=](const QPoint& pos) {
        QListWidgetItem* curItem = m_list->itemAt(pos);
        if (curItem == nullptr) {
            return;
        }
        QMenu* menu = new QMenu(m_list);
        menu->setWindowFlags(Qt::Popup);
        QAction* editAct    = new QAction(tr("编辑"), m_list);
        QAction* deleteAct  = new QAction(tr("删除"), m_list);
        QAction* refreshAct = new QAction(tr("刷新头像"), m_list);

        connect(editAct, &QAction::triggered, [=]() {
            UserItem* userItem = static_cast<UserItem*>(m_list->itemWidget(curItem));
            User user          = userItem->getUser();
            modifyGitUser(user);
        });

        connect(deleteAct, &QAction::triggered, [=]() {
            UserItem* userItem = static_cast<UserItem*>(m_list->itemWidget(curItem));
            User user          = userItem->getUser();
            deleteGitUser(user);
        });

        connect(refreshAct, &QAction::triggered, [=]() {
            UserItem* userItem = static_cast<UserItem*>(m_list->itemWidget(curItem));
            User user          = userItem->getUser();
            refreshGitUserAvatar(user);
        });

        menu->addAction(editAct);
        menu->addAction(deleteAct);
        menu->addAction(refreshAct);

        menu->exec(QCursor::pos());
        setFocus();
    });
}

void window::initTaskbar()
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

    QAction* quit = new QAction(tr("退出"), this);
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
    setFocus();
}

void window::closeWindow()
{
    qApp->quit();
}

void window::onItemClicked(QListWidgetItem* pItem) const
{
    const int itemType = pItem->data(Qt::UserRole).value<int>();
    switch (itemType) {
        case UserItemType: {
            UserItem* userItem = static_cast<UserItem*>(m_list->itemWidget(pItem));
            User user          = userItem->getUser();
            switchGitUser(user);
            break;
        }
        case AddItemType: {
            addGitUser();
            break;
        }
        default: break;
    }
}

void window::readGitUser() const
{
    m_list->clear();
    QList<User> users = m_db->getAllUsers();
    if (users.count() > 0) {
        auto gc = getGitConfig();
        for (auto user : users) {
            QListWidgetItem* pItem = new QListWidgetItem(m_list);
            UserItem* userItem     = new UserItem(user, m_list);
            if (gc.username == user.name && gc.email == user.email) {
                pItem->setSelected(userItem);
                QPixmap pixmap;
                pixmap.loadFromData(user.avatar);
                QIcon icon(pixmap);
                m_tray->setIcon(icon);
                QString tips = QString("%1\n%2").arg(user.name).arg(user.email);
                m_tray->setToolTip(tips);
            }
            pItem->setSizeHint(userItem->sizeHint());
            pItem->setData(Qt::UserRole, QVariant::fromValue(UserItemType));
            m_list->setItemWidget(pItem, userItem);
        }
    }

    {
        QListWidgetItem* pItem = new QListWidgetItem(m_list);
        AddItem* addItem       = new AddItem();
        pItem->setSizeHint(addItem->sizeHint());
        pItem->setData(Qt::UserRole, QVariant::fromValue(AddItemType));
        m_list->setItemWidget(pItem, addItem);

        Qt::ItemFlags flags = pItem->flags();
        flags &= ~Qt::ItemIsSelectable;
        flags &= ~Qt::ItemIsEnabled;
        pItem->setFlags(flags);
    }
}

void window::addGitUser() const
{
    CustomDialog* dialog = new CustomDialog();
    User user;
    dialog->exec();
    dialog->getUserInfo(user);
    if (user.avatar != "" && user.name != "" && user.email != "") {
        m_db->addUser(user);
        readGitUser();
    }
}

void window::addGitUser(const QString& name, const QString& email) const
{
    User user;
    const auto avatar = getAvatar(email);
    user.avatar       = avatar;
    user.name         = name;
    user.email        = email;
    m_db->addUser(user);
    readGitUser();
}

void window::deleteGitUser(const User& user) const
{
    m_db->deleteUser(user.uuid);
    readGitUser();
}

void window::switchGitUser(const User& user) const
{
    GitConfig cfg;
    cfg.username = user.name;
    cfg.email    = user.email;
    auto res     = setGitConfig(cfg);
    readGitUser();
}

void window::modifyGitUser(User user) const
{
    CustomDialog* dialog = new CustomDialog();
    dialog->setUserInfo(user);
    dialog->exec();
    dialog->getUserInfo(user);
    if (user.avatar != "" && user.name != "" && user.email != "") {
        m_db->updateUser(user);
        readGitUser();
    }
}

void window::refreshGitUserAvatar(User user) const
{
    const auto avatar = getAvatar(user.email);
    user.avatar       = avatar;
    m_db->updateUser(user);
    readGitUser();
}

void window::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
    qDebug() << event->reason();
    if (event->reason() == Qt::MouseFocusReason) {
        // hide();
    }
}
