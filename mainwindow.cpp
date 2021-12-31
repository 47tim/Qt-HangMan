#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QList>
#include <QFile>
#include <random>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartGame()
{

    ui->lineEdit->setMaxLength(1);
    ui->usedLabel->setWordWrap(true);
    ui->pushButton->setDefault(true);
    ui->pushButton->setShortcut (Qt :: Key_Return);

    LivesLost = 0;

    Word = generateWord();
    qDebug() << Word;

    sortWord();

    displayImage(LivesLost);

    displayString();

    takeTurn();


}

QString MainWindow::generateWord()
{
    QList<QString> wordList;
    QFile wordstxt(":/new/prefix1/words.txt");
    int fileLength = 0;

    if (!wordstxt.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "failed to open word txt file";
            exit(EXIT_FAILURE);
     }

    QTextStream in(&wordstxt);
    while (!in.atEnd()) {
        QString line = in.readLine();
        wordList.push_back(line);
        fileLength++;
    }

    wordstxt.close();

    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> gen(0, fileLength - 1); // uniform, unbiased
    int r = gen(rng);
    qDebug() << r;

    return wordList[r];

}

void MainWindow::sortWord()
{
    for (int i = 0; i < Word.size(); i++){
        WordVec.push_back(Word.at(i));
    }
    for (int i = 0; i < Word.size(); i++){
        BlankVec.push_back("_");
    }

}

void MainWindow::displayImage(int ll)
{

    if (ll == 0){
        ui->picLabel->setStyleSheet("border-image:url(:/new/prefix1/hang1.png)stretch;");
    }
    if (ll == 1){
        ui->picLabel->setStyleSheet("border-image:url(:/new/prefix1/hang2.png)stretch;");
    }
    if (ll == 2){
        ui->picLabel->setStyleSheet("border-image:url(:/new/prefix1/hang3.png)stretch;");
    }
    if (ll == 3){
        ui->picLabel->setStyleSheet("border-image:url(:/new/prefix1/hang4.png)stretch;");
    }
    if (ll == 4){
        ui->picLabel->setStyleSheet("border-image:url(:/new/prefix1/hang5.png)stretch;");
    }
    if (ll == 5){
        ui->picLabel->setStyleSheet("border-image:url(:/new/prefix1/hang6.png)stretch;");
    }
    if (ll == 6){
        ui->picLabel->setStyleSheet("border-image:url(:/new/prefix1/hang7.png)stretch;");
    }


}

void MainWindow::displayString()
{
    // Sending BlankVec to screen
    QString s;

    for (int i = 0; i < BlankVec.size(); i++){
        s.append(BlankVec[i]);
        s.append("   ");
    }

    ui->label->setText(s);

    // Sending used letters to screen
    QString u;

    for (int i = 0; i < UsedVec.size(); i++){
        u.append(UsedVec[i]);
        u.append("   ");
    }

    ui->usedLabel->setText(u);

    // Setting current streak label
    QString score = "Current Streak: ";
    score.append(QString::number(STREAK));
    ui->scoreLabel->setText(score);


}

void MainWindow::takeTurn()
{
    checkWin();
    IsTurn = true;
}

void MainWindow::on_pushButton_clicked()
{
    if (IsTurn == true){

        bool a = false;

        QString input = ui->lineEdit->text();

        for (int i = 0; i < UsedVec.size(); i++){
            if (input == UsedVec[i]){
                a = true;
            }
        }
        if (input == " "){
            a = true;
        }

        ui->lineEdit->clear();

        // only checks input if it isn't in UsedVec
        if (a == true){

        }
        else {
           checkInput(input);
           displayImage(LivesLost);
           displayString();
        }

        takeTurn();
    }

}

void MainWindow::checkInput(QString input)
{
    bool loseLife = true;

    for (int i = 0; i < WordVec.size(); i++){
        if (input == WordVec[i]){
            BlankVec[i] = input;
            loseLife = false;
        }
    }

    if (loseLife == true){
        UsedVec.push_back(input);
        LivesLost++;
    }

}

void MainWindow::checkWin()
{
    bool won = true;
    bool lost = false;

    // Checking for a win
    for (int i = 0; i < WordVec.size(); i++){
        if (BlankVec[i] != WordVec[i]){
            won = false;
        }
    }

    // Checking for loss
    if (LivesLost >= 6){
        lost = true;
    }

    if (won == true){
        qDebug() << "You win";
        winEvent();
    }
    if (lost == true){
        qDebug() << "You lose";
        loseEvent();
    }

}

void MainWindow::winEvent()
{
    STREAK++;
    qDebug() << STREAK;
    QString s = QString::number(STREAK);

    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "You Win", "You Win. Streak: " + s + ", Keep Playing?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::NoButton);

    if (reply == QMessageBox::Yes) {
        WordVec.clear();
        BlankVec.clear();
        UsedVec.clear();
        Word.clear();
        LivesLost = 0;
        StartGame();
    }
    // Checking if the player really wants to quit
    if (reply == QMessageBox::No) {
        QMessageBox::StandardButton sure;

        sure = QMessageBox::question(this, "Quit?", "Are you sure you want to quit?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::NoButton);

        if (sure == QMessageBox::Yes){
            QCoreApplication::quit();
        }

        if (sure == QMessageBox::No) {
            WordVec.clear();
            BlankVec.clear();
            UsedVec.clear();
            Word.clear();
            LivesLost = 0;
            StartGame();
        }
    }
}

void MainWindow::loseEvent()
{
    STREAK = 0;
    qDebug() << STREAK;

    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "You Lose", "You Lose. Try Again?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::NoButton);

    if (reply == QMessageBox::Yes) {
        WordVec.clear();
        BlankVec.clear();
        UsedVec.clear();
        Word.clear();
        LivesLost = 0;
        StartGame();
    }
    // Checking if player really wants to quit
    if (reply == QMessageBox::No) {

        QMessageBox::StandardButton sure;

        sure = QMessageBox::question(this, "Quit?", "Are you sure you want to quit?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::NoButton);

        if (sure == QMessageBox::Yes){
            QCoreApplication::quit();
        }

        if (sure == QMessageBox::No) {
            WordVec.clear();
            BlankVec.clear();
            UsedVec.clear();
            Word.clear();
            LivesLost = 0;
            StartGame();
        }
    }
}

