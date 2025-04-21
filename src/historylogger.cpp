/*
#include "historylogger.h"
#include "ui_historylogger.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QStandardPaths>

HistoryLogger::HistoryLogger(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::HistoryLogger)
{
    ui->setupUi(this);
    setWindowTitle("Delivery History");

    // build once
    ui->tableWidget->setColumnCount(6);
    QStringList hdr = { "Timestamp", "Final", "Immediate", "Extended",
                        "Duration (h)", "Override" };
    ui->tableWidget->setHorizontalHeaderLabels(hdr);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // fill
    QJsonArray arr = loadBolusLog();
    populateBolusTable(arr);
}

HistoryLogger::~HistoryLogger() { delete ui; }

QJsonArray HistoryLogger::loadBolusLog() const
{
    QString path = QCoreApplication::applicationDirPath() + "/../bolus_log.json";
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return {};

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    return doc.isArray() ? doc.array() : QJsonArray{};
}

void HistoryLogger::populateBolusTable(const QJsonArray &arr)
{
    ui->tableWidget->setRowCount(arr.size());

    int row = 0;
    for (const QJsonValue &v : arr)
    {
        const QJsonObject o = v.toObject();

        const auto ts  = o["timestamp"].toString();
        const auto fin = o["finalBolus"].toDouble();
        const auto imm = o["immediateBolus"].toDouble();
        const auto ext = o["extendedBolus"].toDouble();
        const auto dur = o["durationHours"].toDouble();
        const auto msg = o["overrideMessage"].toString();

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ts));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(fin)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(imm)));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(ext)));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(dur)));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(msg));
        ++row;
    }
}
*/
