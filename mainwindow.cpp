#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QKeyEvent>
#include <QTimer>
#include <cstdlib>
#include <QVector>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
    // allow MainWindow itself to accept focus
    this->setFocusPolicy(Qt::StrongFocus);
    // immediately grab it
    this->setFocus();
    */
    // in constructor, after setupUi:
    ui->widget->setFocusPolicy(Qt::StrongFocus);
    ui->widget->setFocus();



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

   resetGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startNewGame() {
    resetGame();
}
void MainWindow::resetGame() {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            board[row][col] = 0;
        }
    }

    spawnNewTile();
    spawnNewTile();

    updateUI();
}

void MainWindow::spawnNewTile() {
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
    int oldBoard[4][4];
    memcpy(oldBoard, board, sizeof(oldBoard)); // 保存移动前的状态

    if (event->key() == Qt::Key_Up) {
        moveUp();
    } else if (event->key() == Qt::Key_Down) {
        moveDown();
    } else if (event->key() == Qt::Key_Left) {
        moveLeft();
    } else if (event->key() == Qt::Key_Right) {
        moveRight();
    } else {
        QMainWindow::keyPressEvent(event);
        return;
    }

    // 检查是否发生移动或合并
    bool changed = false;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (oldBoard[i][j] != board[i][j]) {
                changed = true;
                break;
            }
        }
        if (changed) break;
    }

    if (changed) {
        spawnNewTile();
        updateUI();
        // 这里可以添加游戏结束检查
        if (checkGameOver()) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "游戏结束", "没有可用的移动了！重新开始吗？",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                resetGame();
            }
        }

    }

}


// helper: merge & compress a 1×4 line in place
static void mergeLine(int line[4]) {
    QVector<int> v;
    // 1) collect non-zeros
    for (int i = 0; i < 4; ++i)
        if (line[i] != 0)
            v.append(line[i]);

    // 2) merge neighbors
    for (int i = 0; i + 1 < v.size(); ++i) {
        if (v[i] == v[i+1]) {
            v[i] *= 2;
            v.remove(i+1);
        }
    }

    // 3) write back + pad with zeros
    for (int i = 0; i < 4; ++i)
        line[i] = (i < v.size() ? v[i] : 0);
}

void MainWindow::moveUp() {
    int colLine[4];
    for (int col = 0; col < 4; ++col) {
        // pull out the column into colLine
        for (int row = 0; row < 4; ++row)
            colLine[row] = board[row][col];

        mergeLine(colLine);

        // write back
        for (int row = 0; row < 4; ++row)
            board[row][col] = colLine[row];
    }
}

void MainWindow::moveDown() {
    int colLine[4];
    for (int col = 0; col < 4; ++col) {
        // pull bottom→top
        for (int i = 0; i < 4; ++i)
            colLine[i] = board[3 - i][col];

        mergeLine(colLine);

        // write back reversed
        for (int i = 0; i < 4; ++i)
            board[3 - i][col] = colLine[i];
    }
}

void MainWindow::moveLeft() {
    int rowLine[4];
    for (int row = 0; row < 4; ++row) {
        // pull left→right
        for (int col = 0; col < 4; ++col)
            rowLine[col] = board[row][col];

        mergeLine(rowLine);

        // write back
        for (int col = 0; col < 4; ++col)
            board[row][col] = rowLine[col];
    }
}

void MainWindow::moveRight() {
    int rowLine[4];
    for (int row = 0; row < 4; ++row) {
        // pull right→left
        for (int i = 0; i < 4; ++i)
            rowLine[i] = board[row][3 - i];

        mergeLine(rowLine);

        // write back reversed
        for (int i = 0; i < 4; ++i)
            board[row][3 - i] = rowLine[i];
    }
}


bool MainWindow::checkGameOver() {
    int tempBoard[4][4];
    bool canMove = false;

    // 检查所有四个方向是否有有效移动
    for (int dir = 0; dir < 4; dir++) {
        // 复制当前棋盘状态
        memcpy(tempBoard, board, sizeof(board));

        // 尝试移动
        switch(dir) {
        case 0: moveUp(); break;
        case 1: moveDown(); break;
        case 2: moveLeft(); break;
        case 3: moveRight(); break;
        }

        // 检查是否发生变化
        if (memcmp(tempBoard, board, sizeof(board)) != 0) {
            canMove = true;
            // 恢复原始棋盘状态
            memcpy(board, tempBoard, sizeof(board));
            break;
        }
    }

    return !canMove;
}
