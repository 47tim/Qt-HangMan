#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QFile>
#include <random>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void StartGame();
    QString generateWord();
    void sortWord();
    void displayImage(int ll);
    void displayString();
    void takeTurn();
    void checkInput(QString input);
    void checkWin();
    void winEvent();
    void loseEvent();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    int LivesLost;
    QString Word;
    QVector<QString> WordVec;
    QVector<QString> BlankVec;
    QVector<QString> UsedVec;
    bool IsTurn;
    int STREAK = 0;

};
#endif // MAINWINDOW_H
