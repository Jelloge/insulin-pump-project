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

// global config, live clock, user-modifiable settings, security pin

class Config : public QObject {
    Q_OBJECT

public:
    static Config* instance();  // singleton
    static QString timeFormat; // "12" or "24"
    static QString dateFormat; // "dd MMM" or "dd/MM/yyyy"
    QString getFormattedDateTime() const;
    void pauseClock();
    void resumeClock();

    // Site Reminder
    static int siteReminderDays;
    static QString siteReminderTime;
    static bool siteReminderEnabled;

    // Temp Basal Rate
    static bool tempBasalRateEnabled;
    void setTempBasalRatePercentage(int percentage);
    int getTempBasalRatePercentage() const;
    void setTempBasalDuration(int minutes);
    int getTempBasalDuration() const;


    // Control-IQ
    static bool controlIQEnabled;
    void setWeight(double w, const QString &unit);
    double getWeight() const;
    void setWeightUnit(const QString &unit);
    QString getWeightUnit() const;
    void setTotalDailyInsulin(double val);
    double getTotalDailyInsulin() const;

    // Alerts and Reminders
    static double lowBGThreshold;
    static int lowBGRepeatDelay;
    static double highBGThreshold;
    static int highBGRepeatDelay;
    static int afterBolusTime;
    static QStringList missedMealDays;
    static QString missedMealStartTime;
    static QString missedMealEndTime;
    static QString soundVolume;

    void    setPin(const QString& pin);            // change / save
    QString pin() const;                           // current
    bool    verifyPin(const QString& candidate) const;

signals:
    void clockUpdated(const QString& formattedTime);
    void pinChanged (const QString& newPin);

private slots:
    void updateClock();

private:
    explicit Config(QObject *parent = nullptr);
    static Config* _instance;

    QTimer *clockTimer;
    QDateTime currentDateTime;

    int tempBasalRatePercentage = 100;
    int tempBasalDurationMinutes = 15;

    double weight = 0.0;
    QString weightUnit = "kg";
    double totalDailyInsulin = 0.0;

    QString m_pin = "3004"; // default pin
};

#endif // CONFIG_H
