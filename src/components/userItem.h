#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include "../datastore/user.h"

class UserItem : public QWidget
{
    Q_OBJECT
public:
    explicit UserItem(const User& user, QWidget* parent = nullptr)
        : QWidget(parent)
        , m_user(user)
        , m_avatar(new QLabel(this))
        , m_name(new QLabel(this))
        , m_email(new QLabel(this))
        , m_main(new QHBoxLayout())
        , m_right(new QVBoxLayout())
    {
        QPixmap avatar;
        avatar.loadFromData(user.avatar);
        m_avatar->setPixmap(avatar.scaled(34, 34, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_avatar->setObjectName("u_avatar");
        m_name->setText(user.name);
        m_name->setObjectName("u_name");
        m_email->setText(user.email);
        m_email->setObjectName("u_email");
        m_right->setObjectName("u_right");

        m_right->addWidget(m_name);
        m_right->addWidget(m_email);

        m_main->addWidget(m_avatar);
        m_main->addLayout(m_right);
        m_main->addStretch(1);
        m_main->setObjectName("u_main");

        setLayout(m_main);
    }

    User getUser() { return m_user; }

private:
    User m_user;
    QLabel* m_avatar;
    QLabel* m_name;
    QLabel* m_email;
    QHBoxLayout* m_main;
    QVBoxLayout* m_right;
};