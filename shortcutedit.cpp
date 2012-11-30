#include <QtGui>

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QtDebug>

#include "shortcutedit.h"

struct ShortcutEdit::Private
{
    QLineEdit * lineedit;
};

ShortcutEdit::ShortcutEdit(QWidget *parent) :
    QWidget(parent)
{
    p = new Private;

    QLayout * layout = new QHBoxLayout();
    p->lineedit = new QLineEdit(this);
    layout->addWidget(p->lineedit);
    p->lineedit->installEventFilter(this);

    setLayout(layout);
}

ShortcutEdit::~ShortcutEdit()
{
    delete p;
}

void ShortcutEdit::handleKeyPress(QKeyEvent*)
{
}

void ShortcutEdit::resetKeySequence()
{
    qDebug() << "resetKeySequence()";
}

bool ShortcutEdit::eventFilter(QObject *obj, QEvent *e)
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
        //e->accept();
        return true;
    }

    if (e->type() == QEvent::FocusIn) {
        resetKeySequence();
    }

    return QObject::eventFilter(obj, e);
}
