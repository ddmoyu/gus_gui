#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

class AddItem : public QWidget
{
    Q_OBJECT
public:
    explicit AddItem(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        m_add       = new QLabel;
        QPixmap add = QPixmap(":/images/add.png");
        m_add->setPixmap(add.scaled(34, 34, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_add->setObjectName("u_add");

        QVBoxLayout* vLayout = new QVBoxLayout;
        vLayout->addStretch(1);
        vLayout->addWidget(m_add);
        vLayout->addStretch(1);

        QHBoxLayout* hLayout = new QHBoxLayout;
        hLayout->addStretch(1);
        hLayout->addLayout(vLayout);
        hLayout->addStretch(1);

        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->addLayout(hLayout);

        setLayout(mainLayout);
    }

private:
    QLabel* m_add;
};