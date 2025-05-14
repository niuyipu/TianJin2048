#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>
#include <QTimer>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化按钮
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            QPushButton *button = findChild<QPushButton*>(QString("button_%1_%2").arg(row).arg(col));
            if (button) { // 检查button是否为nullptr
                button->setText("");
                button->setStyleSheet("font-size: 20px; background-color: lightgray;");
                button->setEnabled(false);
            } else {
                // 可选：输出错误信息，帮助调试
                qDebug() << "Button not found for row" << row << "and column" << col;
            }
        }
    }

   connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startNewGame);

   // resetGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startNewGame() {
    // 每次点击按钮时，重新初始化游戏
    resetGame();
}
void MainWindow::resetGame() {
    // 初始化一个4x4的空白游戏板
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            board[row][col] = 0;
        }
    }

    // 在随机位置生成两个数字2
    spawnNewTile();
    spawnNewTile();

    //updateUI();
}

void MainWindow::spawnNewTile() {
    // 找到空的位置，并随机生成2
    QList<QPair<int, int>> emptyTiles;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == 0) {
                emptyTiles.append(qMakePair(row, col));
            }
        }
    }

    if (emptyTiles.isEmpty()) return;

    // 随机选择一个空位置并生成2
    int randIndex = rand() % emptyTiles.size();
    QPair<int, int> tile = emptyTiles.at(randIndex);
    board[tile.first][tile.second] = 2;
}
/*
void MainWindow::updateUI() {
    // 更新每个按钮的文本
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            QPushButton *button = findChild<QPushButton*>(QString("button_%1_%2").arg(row).arg(col));
            if (board[row][col] != 0) {
                button->setText(QString::number(board[row][col]));
            } else {
                button->setText("");
            }
        }
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up) {
        moveUp();
    } else if (event->key() == Qt::Key_Down) {
        moveDown();
    } else if (event->key() == Qt::Key_Left) {
        moveLeft();
    } else if (event->key() == Qt::Key_Right) {
        moveRight();
    }

    spawnNewTile();
    updateUI();
}

void MainWindow::moveUp() {
    // 向上移动和合并
    for (int col = 0; col < 4; ++col) {
        // 合并方块的逻辑
        for (int row = 1; row < 4; ++row) {
            if (board[row][col] != 0) {
                int targetRow = row;
                while (targetRow > 0 && board[targetRow - 1][col] == 0) {
                    targetRow--;
                }
                if (targetRow > 0 && board[targetRow - 1][col] == board[row][col]) {
                    board[targetRow - 1][col] *= 2;
                    board[row][col] = 0;
                } else {
                    if (targetRow != row) {
                        board[targetRow][col] = board[row][col];
                        board[row][col] = 0;
                    }
                }
            }
        }
    }
}

// 向下、向左、向右的逻辑类似，可以在这些函数中实现
void MainWindow::moveDown() {
    // 向下移动和合并
}

void MainWindow::moveLeft() {
    // 向左移动和合并
}

void MainWindow::moveRight() {
    // 向右移动和合并
}

*/
