#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class options; }
QT_END_NAMESPACE

class options : public QWidget
{
    Q_OBJECT

public:
    explicit options(QWidget *parent = nullptr);
    ~options();

private:
    Ui::options *ui;
};

#endif // OPTIONS_H
