#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int board[4][4];
    void resetGame();

    void startNewGame();

    void spawnNewTile();

    void updateUI();


    // 移动和合并方块的逻辑
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    // 处理键盘输入
    void keyPressEvent(QKeyEvent *event);

    bool checkGameOver();


};

#endif // MAINWINDOW_H
