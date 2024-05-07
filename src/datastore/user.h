#pragma once
#include <QByteArray>
#include <QString>
#include <QUuid>

class User
{
public:
    QUuid uuid;
    QByteArray avatar{""};
    QString name{""};
    QString email{""};

    User() : uuid(QUuid::createUuid()) { }

    User(QUuid uuid, QByteArray avatar, QString name, QString email)
        : uuid(std::move(uuid))
        , avatar(std::move(avatar))
        , name(std::move(name))
        , email(std::move(email))
    { }

    User(QByteArray avatar, QString name, QString email)
        : uuid(QUuid::createUuid())
        , avatar(std::move(avatar))
        , name(std::move(name))
        , email(std::move(email))
    { }
};