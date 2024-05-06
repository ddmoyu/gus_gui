#include "window.h"
#include "ui_window.h"

#include <QMessageBox>
#include "../utils/utils.h"
#include "../components/userItem.h"
#include "../components/addItem.h"
#include "../components/addDialog.h"

window::window(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::window())
{
    ui->setupUi(this);
    initUi();
    initConnect();
    initDB();
    initGit();
    addTaskbar();
}

window::~window()
{
    delete ui;
}

void window::initUi()
{
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    const QString style = invokeStyleSheetLoad("dark");
    qApp->setStyleSheet(style);

    CustomDialog* dialog = new CustomDialog();
    dialog->exec();

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
    connect(m_list, &QListWidget::itemClicked, this, &window::onItemClicked);
}

void window::initDB()
{
    m_db = new DbManager("user.sqlite");
    readGitUser();
}

void window::initGit()
{
    const auto flag = check_git_exists();
    if (flag) {
        auto gc = getGitConfig();

        //GitConfig cfg;
        /*cfg.username = QString("Hunlongyu");
        cfg.email = QString("hunlongyu@gmail.com");
        auto res = setGitConfig(cfg);
        qDebug() << "res："  << res;*/
        //auto avatar = getAvatar("qq@qq.com");
        //qDebug() << "avatar: " << avatar;
        //QImage image;
        //image.loadFromData(avatar); // 加载 QByteArray 数据到 QImage

        //// 确定目标文件路径
        //QString desktopPath     = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        //QString destinationFile = desktopPath + "/image.jpg";

        //image.save(destinationFile, "JPG");
    }
    else {
        // todo
    }
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

void window::onItemClicked(QListWidgetItem* pItem)
{
    const int itemType = pItem->data(Qt::UserRole).value<int>();
    switch (itemType) {
        case UserItemType: {
            UserItem* userItem = static_cast<UserItem*>(m_list->itemWidget(pItem));
            User user          = userItem->getUser();
            QMessageBox::information(this, "User clicked", "User: " + user.name);
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
    qDebug() << users.count();
    if (users.count() > 0) {
        for (auto user : users) {
            QListWidgetItem* pItem = new QListWidgetItem(m_list);
            UserItem* userItem     = new UserItem(user, m_list);
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

void window::addGitUser()
{
    CustomDialog* dialog = new CustomDialog();
    dialog->exec();
    /*QPixmap pixmap(":/images/1mb.png");
    QByteArray avatar;
    QBuffer buffer(&avatar);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    User user(avatar, "ddmoyu", "daydaymoyu@gmail.com");
    m_db->addUser(user);
    readGitUser();*/
}

void window::deleteGitUser() { }

void window::switchGitUser() { }

void window::modifyGitUser() { }
