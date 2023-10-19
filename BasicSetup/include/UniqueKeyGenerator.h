#pragma once

#include <QString>
#include <QDateTime>
#include <QRandomGenerator>

class UniqueKeyGenerator
{
public:
    //-----------------------------------
    static QString getUniqueKey()
    {
        const auto current_datetime = QDateTime::currentDateTime();
        const QString current_timestamp = QString::number(current_datetime.toMSecsSinceEpoch());
        const QString random_number = QString::number(QRandomGenerator::global()->generate());

        return {current_timestamp + random_number};
    }

private:
    UniqueKeyGenerator() = delete;
    ~UniqueKeyGenerator() = delete;
};
