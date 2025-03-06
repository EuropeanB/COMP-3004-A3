#ifndef LOGCONSOLE_H
#define LOGCONSOLE_H

#include <QString>
#include <QTextEdit>

class LogConsole : public QTextEdit {
    Q_OBJECT

public:
    // Constructor
    LogConsole(QWidget* parent = nullptr);

    // log to UI console
    void appendLog(const QString& message);


private:

};

#endif // LOGCONSOLE_H
