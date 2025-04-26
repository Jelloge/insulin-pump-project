#include "historylogger.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

HistoryLogger* HistoryLogger::instance()
{
    static HistoryLogger s;   // 1 per process
    return &s;
}

HistoryLogger::HistoryLogger(QObject *parent)
    : QObject(parent),
      m_bolusModel (new QStandardItemModel(this)),
      m_cgmModel   (new QStandardItemModel(this)),
      m_alertModel (new QStandardItemModel(this))
{
    refresh();
}

QString HistoryLogger::path(const QString &f) const
{
    return QCoreApplication::applicationDirPath() + "/../" + f;
}

void HistoryLogger::refresh()
{
    // BOLUS
    m_bolusModel->clear();
    m_bolusModel->setHorizontalHeaderLabels(
        {"Timestamp","Final (U)","Immediate (U)","Extended (U)","Note"});

    QFile fb(path("bolus_log.json"));
    if (fb.open(QIODevice::ReadOnly))
    {
        QJsonArray arr = QJsonDocument::fromJson(fb.readAll()).array();
        for (const QJsonValue& v : arr)
        {
            QJsonObject o = v.toObject();
            QList<QStandardItem*> row;
            row << new QStandardItem(o["timestamp"].toString())
                << new QStandardItem(QString::number(o["finalBolus"].toDouble(),'f',2))
                << new QStandardItem(QString::number(o["immediateBolus"].toDouble(),'f',2))
                << new QStandardItem(QString::number(o["extendedBolus"].toDouble(),'f',2))
                << new QStandardItem(o["overrideMessage"].toString());
            m_bolusModel->appendRow(row);
        }
        fb.close();
    }

    //  CGM
    m_cgmModel->clear();
    m_cgmModel->setHorizontalHeaderLabels({"#","Timestamp","Value (mmol/L)","Alert"});

    QFile fc(path("glucose_log.json"));
    if (fc.open(QIODevice::ReadOnly))
    {
        QJsonObject root = QJsonDocument::fromJson(fc.readAll()).object();
        for (const QJsonValue& v : root["readings"].toArray())
        {
            QJsonObject r = v.toObject();
            m_cgmModel->appendRow({
               new QStandardItem(QString::number(r["sequence_number"].toInt())),
               new QStandardItem(r["timestamp"].toString()),
               new QStandardItem(QString::number(r["value"].toDouble(),'f',2)),
               new QStandardItem(r["alert"].toString())
            });
        }
        fc.close();
    }

    // ALERTS
    m_alertModel->clear();
    m_alertModel->setHorizontalHeaderLabels({"Timestamp","Type","Value"});

    for (int i=0;i<m_cgmModel->rowCount();++i)
    {
        QString at = m_cgmModel->item(i,3)->text();
        if (at=="NORMAL BG") continue;
        m_alertModel->appendRow({
            new QStandardItem(m_cgmModel->item(i,1)->text()),
            new QStandardItem(at),
            new QStandardItem(m_cgmModel->item(i,2)->text())
        });
    }
}
