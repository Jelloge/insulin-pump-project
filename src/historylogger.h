#ifndef HISTORYLOGGER_H
#define HISTORYLOGGER_H

#include <QObject>
#include <QStandardItemModel>

class HistoryLogger : public QObject
{
    Q_OBJECT
public:
    static HistoryLogger* instance();

    // models are reused
    QStandardItemModel* bolusModel()  const { return m_bolusModel;  }
    QStandardItemModel* cgmModel()    const { return m_cgmModel;    }
    QStandardItemModel* alertModel()  const { return m_alertModel;  }

    void refresh();

private:
    explicit HistoryLogger(QObject *parent=nullptr);
    QString path(const QString& f) const;

    QStandardItemModel *m_bolusModel;
    QStandardItemModel *m_cgmModel;
    QStandardItemModel *m_alertModel;
};

#endif // HISTORYLOGGER_H
