#include "logconsole.h"

// Constructor
LogConsole::LogConsole(QWidget* parent) : QTextEdit(parent) {
    setReadOnly(true);
}

// Log the messages to logConsole(UI)
void LogConsole::appendLog(const QString& message) {
    append(message);
    moveCursor(QTextCursor::End);
}


