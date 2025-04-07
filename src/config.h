#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QRegularExpression>
#include <QString>
#include <QTimer>

class Config : public QObject {
    Q_OBJECT

public:
    static Config* instance();  // singleton
    QString getFormattedDateTime() const;
    void pauseClock();
    void resumeClock();

signals:
    void clockUpdated(const QString& formattedTime);

private slots:
    void updateClock();

private:
    explicit Config(QObject *parent = nullptr);
    static Config* _instance;

    QTimer *clockTimer;
    QDateTime currentDateTime;
};

#endif // CONFIG_H
