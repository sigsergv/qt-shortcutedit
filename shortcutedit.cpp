#include <QtGui>

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QtDebug>

#include "shortcutedit.h"

struct ShortcutEdit::Private
{
    QLineEdit * lineedit;

    bool emptySequenceReady;
    QList<QString> workingSequence;

    void resetCurrentShortcut();
    QString shortcutRepr();

    // modifiers
    bool finalized;
    int pressedKeys;
    bool ctrlMod;
    bool altMod;
    bool metaMod;
    bool shiftMod;
    int keyCode;
};

void ShortcutEdit::Private::resetCurrentShortcut()
{
    ctrlMod = altMod = metaMod = shiftMod = finalized = false;

    pressedKeys = keyCode = 0;
}

QString ShortcutEdit::Private::shortcutRepr()
{
    QStringList s;

    if (shiftMod) {
        s << "Shift";
    }

    if (ctrlMod) {
        s << "Ctrl";
    }

    if (altMod) {
        s << "Alt";
    }

    if (metaMod) {
        s << "Meta";
    }

    if (keyCode) {
        QKeySequence seq(keyCode);
        s << seq.toString();
        qDebug() << "code:" << QString::number(keyCode, 16) << "parsed:" << seq.toString();

    }

    return s.join(" + ");
}

ShortcutEdit::ShortcutEdit(QWidget *parent) :
    QWidget(parent)
{
    p = new Private;
    p->emptySequenceReady = false;

    QLayout * layout = new QVBoxLayout();
    p->lineedit = new QLineEdit(this);
    layout->addWidget(p->lineedit);

    p->lineedit->installEventFilter(this);

    setLayout(layout);
}

ShortcutEdit::~ShortcutEdit()
{
    delete p;
}

void ShortcutEdit::handleKeyEvent(QKeyEvent * e)
{
    Qt::KeyboardModifiers state = e->modifiers();

    p->ctrlMod = state & Qt::ControlModifier;
    p->altMod = state & Qt::AltModifier;
    p->metaMod = state & Qt::MetaModifier;
    p->shiftMod = state & Qt::ShiftModifier;

    QString text = e->text();
    if (text.size() != 0) {
        // we are dealing with non-modifier key, so process it
        int key = e->key();
        p->keyCode = key;
        p->finalized = true;

        qDebug() << key;
    } else {
        p->keyCode = 0;
    }

    p->lineedit->setText(p->shortcutRepr());
}

void ShortcutEdit::resetKeySequence()
{
    qDebug() << "resetKeySequence()";
}

bool ShortcutEdit::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *k = static_cast<QKeyEvent*>(e);
        if (!p->finalized) {
            handleKeyEvent(k);
        }
        p->pressedKeys++;
        return true;
    }

    if (e->type() == QEvent::Shortcut || e->type() == QEvent::KeyRelease) {
        QKeyEvent * k = static_cast<QKeyEvent*>(e);
        if (!p->finalized) {
            handleKeyEvent(k);
        }
        p->pressedKeys--;

        if (p->pressedKeys <= 0) {
            p->finalized = false;
        }
        return true;
    }

    if (e->type() == QEvent::ShortcutOverride) {
        //e->accept();
        return true;
    }

    if (e->type() == QEvent::FocusIn) {
        qDebug() << "focus in";
        p->resetCurrentShortcut();
        p->emptySequenceReady = true;
    }

    if (e->type() == QEvent::FocusOut) {
        p->pressedKeys = 0;
        p->finalized = false;
    }

    return QObject::eventFilter(obj, e);
}
