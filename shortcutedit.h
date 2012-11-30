#ifndef SHORTCUTEDIT_H
#define SHORTCUTEDIT_H

#include <QLineEdit>

struct QEvent;
struct QKeyEvent;

class ShortcutEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ShortcutEdit(QWidget *parent = 0);
    bool event(QEvent*);

signals:
    
public slots:

protected:
    void handleKeyPress(QKeyEvent*);
    void resetKeySequence();

private:
    struct Private;
    Private * p;
};

#endif // SHORTCUTEDIT_H
