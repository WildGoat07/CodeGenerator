#ifndef TEXTDISPLAYER_H
#define TEXTDISPLAYER_H

#include <QDialog>
#include <QString>

namespace Ui {
class TextDisplayer;
}

class TextDisplayer : public QDialog
{
    Q_OBJECT

public:
    explicit TextDisplayer(QString const &text, QWidget *parent = nullptr);
    ~TextDisplayer();

private:
    Ui::TextDisplayer *ui;
};

#endif // TEXTDISPLAYER_H
