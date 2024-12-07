#ifndef DIALOGSCORE_H
#define DIALOGSCORE_H

#include <QDialog>

namespace Ui {
class Dialogscore;
}

class Dialogscore : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogscore(QWidget *parent = nullptr);  // Constructor
    ~Dialogscore(); // Destructor
    void on_pushButton_calculerScore_clicked();

private:
    Ui::Dialogscore *ui;  // Pointer to UI
};

#endif // DIALOGSCORE_H
