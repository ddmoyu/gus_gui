#pragma once

#include <QDialog>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QPushButton>
#include <QStyleOption>
#include <QRegularExpression>
#include <QObject>
#include <QTimer>
#include <QUuid>
#include "../utils/utils.h"
#include "../datastore/user.h"

class CustomDialog : public QDialog
{
    Q_OBJECT
public:
    QByteArray m_avatar;
    QUuid m_uuid;
    QString m_name;
    QString m_email;
    QLabel* imageLabel;
    QLineEdit* nameLineEdit;
    QLineEdit* emailLineEdit;

    CustomDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setObjectName("CustomDialog");
        setWindowTitle(tr("Add User"));
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);
        QVBoxLayout* layout    = new QVBoxLayout;
        QHBoxLayout* horLayout = new QHBoxLayout;
        horLayout->setObjectName("horLayout");

        // 使用QLabel显示图片
        imageLabel = new QLabel;
        imageLabel->setObjectName("avatar");
        imageLabel->setText("Avatar");
        imageLabel->setAlignment(Qt::AlignCenter);
        horLayout->addWidget(imageLabel);

        // 表单部分
        QVBoxLayout* formLayout = new QVBoxLayout;
        QLabel* nameLabel       = new QLabel("Name:");
        nameLineEdit            = new QLineEdit;
        formLayout->addWidget(nameLabel);
        formLayout->addWidget(nameLineEdit);
        QLabel* emailLabel = new QLabel("Email:");
        emailLineEdit      = new QLineEdit;
        formLayout->addWidget(emailLabel);
        formLayout->addWidget(emailLineEdit);
        horLayout->addLayout(formLayout);
        layout->addLayout(horLayout);

        // 底部按钮
        QHBoxLayout* buttonBox    = new QHBoxLayout;
        QPushButton* okButton     = new QPushButton("OK");
        QPushButton* cancelButton = new QPushButton("Cancel");
        okButton->setDefault(false);
        okButton->setAutoDefault(false);
        cancelButton->setDefault(false);
        cancelButton->setAutoDefault(false);
        buttonBox->addStretch();
        okButton->setObjectName("btn_ok");
        cancelButton->setObjectName("btn_cancel");
        buttonBox->addWidget(okButton);
        buttonBox->addWidget(cancelButton);

        // 连接信号槽
        connect(okButton, &QPushButton::clicked, this, [okButton, this]() {
            m_name  = nameLineEdit->text();
            m_email = emailLineEdit->text();
            if (!m_name.isEmpty() && !m_email.isEmpty()) {
                QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
                if (emailRegex.match(m_email).hasMatch()) {
                    m_avatar = getAvatar(m_email);
                    QPixmap img;
                    img.loadFromData(m_avatar);
                    imageLabel->setPixmap(img);
                    imageLabel->setScaledContents(true);
                    okButton->setText("Verifying...");
                    QTimer::singleShot(3000, [this, okButton]() {
                        hide();
                        emit dialogAccepted();
                        okButton->setText("OK");
                    });
                }
            }
        });
        connect(cancelButton, &QPushButton::clicked, [this]() {
            hide();
            emit dialogRejected();
        });

        layout->addLayout(buttonBox);
        this->setLayout(layout);
    }

    void getUserInfo(User& user) const
    {
        user.avatar = m_avatar;
        user.name   = m_name;
        user.email  = m_email;
    }

    User getUserInfo() const
    {
        User user;
        user.uuid   = m_uuid;
        user.avatar = m_avatar;
        user.name   = m_name;
        user.email  = m_email;
        return user;
    }

    void setUserInfo(const User& user)
    {
        m_uuid   = user.uuid;
        m_avatar = user.avatar;
        m_name   = user.name;
        m_email  = user.email;
        QPixmap img;
        img.loadFromData(m_avatar);
        imageLabel->setPixmap(img);
        imageLabel->setScaledContents(true);
        nameLineEdit->setText(m_name);
        emailLineEdit->setText(m_email);
    }

    void clear()
    {
        m_avatar = "";
        m_name   = "";
        m_email  = "";
        imageLabel->clear();
        nameLineEdit->setText("");
        emailLineEdit->setText("");
    }

signals:
    void dialogAccepted();
    void dialogRejected();

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        // 禁用 ESC 键退出
        if (event->key() == Qt::Key_Escape) {
            event->ignore();
        }
        else {
            QDialog::keyPressEvent(event);
        }
    }
};