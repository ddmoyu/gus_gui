#pragma once

#include <QtSql>
#include "./user.h"

class DbManager
{
public:
    DbManager(const QString& dbName)
    {
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir appDataDir(appDataPath);

        QString dbPath = appDataPath + QDir::separator() + dbName;

        bool needCreate = false;
        if (!QFile::exists(dbPath)) {
            QDir appDataDir(appDataPath);
            bool didCreate = appDataDir.mkpath(appDataPath);
            if (!didCreate) {
                qDebug() << "Error: failed to create directory.";
            }
            else {
                needCreate = true;
            }
        }

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbPath);

        if (!m_db.open()) {
            qDebug() << "Error: connection with database fail";
        }
        else {
            qDebug() << "Database: connection ok";
        }

        if (needCreate) {
            createTable();
        }
    }

    ~DbManager()
    {
        if (m_db.isOpen()) {
            m_db.close();
        }
    }

    static void addUser(const User& user)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO users (uuid, avatar, name, email) "
                      "VALUES (:uuid, :avatar, :name, :email)");
        query.bindValue(":uuid", user.uuid.toString());
        query.bindValue(":avatar", user.avatar);
        query.bindValue(":name", user.name);
        query.bindValue(":email", user.email);
        query.exec();
    }

    static void deleteUser(const QUuid& uuid)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE uuid = :uuid");
        query.bindValue(":uuid", uuid.toString());
        query.exec();
    }

    static void updateUser(const User& user)
    {
        QSqlQuery query;
        query.prepare("UPDATE users SET avatar = :avatar, name = :name, email = :email WHERE uuid = :uuid");
        query.bindValue(":avatar", user.avatar);
        query.bindValue(":name", user.name);
        query.bindValue(":email", user.email);
        query.bindValue(":uuid", user.uuid.toString());
        query.exec();
    }

    static User getUser(const QUuid& uuid)
    {
        User user;
        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE uuid = :uuid");
        query.bindValue(":uuid", uuid.toString());
        query.exec();
        if (query.next()) {
            user.uuid   = QUuid(query.value(0).toString());
            user.avatar = query.value(1).toByteArray();
            user.name   = query.value(2).toString();
            user.email  = query.value(3).toString();
        }
        return user;
    }

    static User getUser(const QString& name, const QString& email)
    {
        User user;
        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE name = :name AND email = :email");
        query.bindValue(":name", name);
        query.bindValue(":email", email);
        query.exec();
        if (query.next()) {
            user.uuid   = QUuid(query.value(0).toString());
            user.avatar = query.value(1).toByteArray();
            user.name   = query.value(2).toString();
            user.email  = query.value(3).toString();
        }
        return user;
    }

    static QList<User> getAllUsers()
    {
        QList<User> users;
        QSqlQuery query("SELECT * FROM users");
        while (query.next()) {
            QUuid uuid        = QUuid(query.value(0).toString());
            QByteArray avatar = query.value(1).toByteArray();
            QString name      = query.value(2).toString();
            QString email     = query.value(3).toString();
            users.append(User(uuid, avatar, name, email));
        }
        return users;
    }

private:
    QSqlDatabase m_db;

    static void createTable()
    {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS users (uuid TEXT UNIQUE, avatar BLOB, name TEXT, email TEXT)");
    }
};