#include <QEvent>
#include <QKeyEvent>
#include <QtDebug>

#include "shortcutedit.h"

struct ShortcutEdit::Private
{

};

ShortcutEdit::ShortcutEdit(QWidget *parent) :
    QLineEdit(parent)
{
    p = new Private;
}

void ShortcutEdit::handleKeyPress(QKeyEvent*)
{
}

void ShortcutEdit::resetKeySequence()
{
    qDebug() << "resetKeySequence()";
}

bool ShortcutEdit::event(QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *k = static_cast<QKeyEvent*>(e);
        handleKeyPress(k);
        return true;
    }

    if (e->type() == QEvent::Shortcut || e->type() == QEvent::KeyRelease) {
        return true;
    }

    if (e->type() == QEvent::ShortcutOverride) {
        e->accept();
        return true;
    }

    if (e->type() == QEvent::FocusIn) {
        resetKeySequence();
    }
    return QLineEdit::event(e);
}
