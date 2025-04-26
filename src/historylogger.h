
#pragma once
#include <QDialog>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class HistoryLogger; }
QT_END_NAMESPACE

class HistoryLogger : public QDialog
{
    Q_OBJECT
public:
    explicit HistoryLogger(QWidget *parent = nullptr);
    ~HistoryLogger();

private:
    Ui::HistoryLogger *ui;

    QJsonArray loadBolusLog() const;              // reads bolus_log.json
    void       populateBolusTable(const QJsonArray &arr);
};
