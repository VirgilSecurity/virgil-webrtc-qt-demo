#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>

class Handler : public QObject
{
    Q_OBJECT

public:
    explicit Handler(QObject *parent);
    ~Handler() = default;

    Q_INVOKABLE void call();
    Q_INVOKABLE void answer();

signals:
    void messageLogged(const QString &);

private:
    void logMessage(const QString &message);
};

#endif // HANDLER_H
