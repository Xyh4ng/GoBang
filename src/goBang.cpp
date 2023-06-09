/*
 * @Author: Xyh4ng 503177404@qq.com
 * @Date: 2023-06-09 14:36:57
 * @FilePath: \gobang\src\goBang.cpp
 * @Description:
 * Copyright (c) 2023 by Xyh4ng, All Rights Reserved.
 */
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <cstring>
#include <qmessagebox.h>
#include <qnamespace.h>

#include "goBang.h"

GoBang::GoBang(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString::fromStdString("GoBang_by_Xyh4ng"));
    resize(800, 800);

    // 实现菜单栏
    game_menu = menuBar()->addMenu(QString::fromStdString("游戏"));
    game_new = new QAction(QString::fromStdString("重开"));
    game_close = new QAction(QString::fromStdString("退出"));
    game_menu->addAction(game_new);
    game_menu->addAction(game_close);

    backout_menu = menuBar()->addMenu(QString::fromStdString("悔棋"));
    white_backout = new QAction(QString::fromStdString("白子悔棋"));
    black_backout = new QAction(QString::fromStdString("黑子悔棋"));
    backout_menu->addAction(white_backout);
    backout_menu->addAction(black_backout);

    help_action = menuBar()->addAction(QString::fromStdString("帮助"));

    GoBang::connect(game_new, SIGNAL(triggered()), this,
                    SLOT(question_new_Msg()));
    GoBang::connect(game_close, SIGNAL(triggered()), this,
                    SLOT(question_close_Msg()));
    GoBang::connect(help_action, SIGNAL(triggered()), this,
                    SLOT(information_help_Msg()));

    GoBang::connect(white_backout, SIGNAL(triggered()), this,
                    SLOT(question_white_backout()));
    GoBang::connect(black_backout, SIGNAL(triggered()), this,
                    SLOT(question_black_backout()));
}

void GoBang::question_new_Msg()
{
    auto choice = QMessageBox::question(
        this, QString::fromStdString("重开"),
        QString::fromStdString("您是否确定重新开始新一局游戏？"),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
    switch (choice)
    {
    case QMessageBox::Ok:
    {
        new_game();
        break;
    }
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void GoBang::new_game()
{
    memset(chess, 0, sizeof(chess));
    player = 0;
    piece_num = 0;
    black_chance = 3;
    white_chance = 3;
    isBackout = false;
    update();
}

void GoBang::question_close_Msg()
{
    auto choice = QMessageBox::question(
        this, QString::fromStdString("退出"),
        QString::fromStdString("您是否确定退出本局游戏？"),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
    switch (choice)
    {
    case QMessageBox::Ok:
        close();
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void GoBang::information_help_Msg()
{
    QMessageBox::information(this, QString::fromStdString("帮助"),
                             QString::fromStdString("1、游戏设置为黑先白后。\n\
2、使用鼠标左键落子\n\
3、白棋和黑棋各有三次悔棋机会，且每回合只能有一人悔棋一次。\n\
4、祝您游戏愉快！"),
                             QMessageBox::Ok);
}

void GoBang::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    // 绘制棋盘
    for (int i = 0; i < 15; i++)
    {
        paint.drawLine(50, 50 + i * 50, 750, 50 + i * 50);
        paint.drawLine(50 + i * 50, 50, 50 + i * 50, 750);
    }
    // 绘制棋盘中的五个小黑点
    paint.setBrush(QColor(0, 0, 0));
    paint.drawEllipse(395, 395, 10, 10);
    paint.drawEllipse(195, 195, 10, 10);
    paint.drawEllipse(595, 595, 10, 10);
    paint.drawEllipse(195, 595, 10, 10);
    paint.drawEllipse(595, 195, 10, 10);

    // 绘制棋子
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            if (chess[i][j] == 1)
            {
                paint.setBrush(QColor(0, 0, 0));
                paint.drawEllipse((i + 1) * 50 - 15, (j + 1) * 50 - 15, 30, 30);
            }
            else if (chess[i][j] == 2)
            {
                paint.setBrush(QColor(255, 255, 255));
                paint.drawEllipse((i + 1) * 50 - 15, (j + 1) * 50 - 15, 30, 30);
            }
        }
    }
}

void GoBang::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->x() >= 25 && event->x() <= 775 && event->y() >= 25 &&
        event->y() <= 775)
    {
        int x = (event->x() - 25) / 50;
        int y = (event->y() - 25) / 50;
        backout_pos[0] = x;
        backout_pos[1] = y;
        isBackout = false;
        if (chess[x][y] == 0)
        {
            ++piece_num;
            if (player == 0) // 黑子
            {
                chess[x][y] = 1;
            }
            else // 白子
            {
                chess[x][y] = 2;
            }
            update();
            player = 1 - player;
            // 检测是否胜利
            if (game_win(x, y))
            {
                QMessageBox::information(this, QString::fromStdString("祝贺"),
                                         QString::fromStdString(player == 1 ? "黑子获胜！" : "白子获胜！"),
                                         QMessageBox::Ok);
                new_game();
                return;
            }
        }
        // 检测是否和棋
        if (piece_num == 225)
        {
            QMessageBox::information(this, QString::fromStdString("和棋"),
                                     QString::fromStdString("卧龙凤雏，绝代双骄！"),
                                     QMessageBox::Ok);
            new_game();
        }
    }
}

bool GoBang::game_win(int x, int y)
{
    // 检测水平方向
    for (int i = 0; i < 5; ++i)
    {
        if (y - i >= 0 && y - i + 4 < 15 &&
            chess[x][y - i] == chess[x][y - i + 1] &&
            chess[x][y - i] == chess[x][y - i + 2] &&
            chess[x][y - i] == chess[x][y - i + 3] &&
            chess[x][y - i] == chess[x][y - i + 4])
            return true;
    }

    // 检测垂直方向
    for (int i = 0; i < 5; ++i)
    {
        if (x - i >= 0 && x - i + 4 < 15 &&
            chess[x - i][y] == chess[x - i + 1][y] &&
            chess[x - i][y] == chess[x - i + 2][y] &&
            chess[x - i][y] == chess[x - i + 3][y] &&
            chess[x - i][y] == chess[x - i + 4][y])
            return true;
    }

    // 检测主对角线方向
    for (int i = 0; i < 5; ++i)
    {
        if (x - i >= 0 && y - i >= 0 && x - i + 4 < 15 && y - i + 4 < 15 &&
            chess[x - i][y - i] == chess[x - i + 1][y - i + 1] &&
            chess[x - i][y - i] == chess[x - i + 2][y - i + 2] &&
            chess[x - i][y - i] == chess[x - i + 3][y - i + 3] &&
            chess[x - i][y - i] == chess[x - i + 4][y - i + 4])
            return true;
    }

    // 检测次对角线方向
    for (int i = 0; i < 5; ++i)
    {
        if (x - i >= 0 && y + i < 15 && x - i + 4 < 15 && y + i - 4 >= 0 &&
            chess[x - i][y + i] == chess[x - i + 1][y + i - 1] &&
            chess[x - i][y + i] == chess[x - i + 2][y + i - 2] &&
            chess[x - i][y + i] == chess[x - i + 3][y + i - 3] &&
            chess[x - i][y + i] == chess[x - i + 4][y + i - 4])
            return true;
    }
    return false;
}

void GoBang::backout(const char *str)
{
    auto choice = QMessageBox::question(
        this, QString::fromStdString("%1悔棋").arg(str),
        QString::fromStdString("%1是否确定悔棋? (您还有%2次机会)").arg(str).arg(player == 1 ? black_chance : white_chance),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);

    switch (choice)
    {
    case QMessageBox::Ok:
    {
        isBackout = true;
        chess[backout_pos[0]][backout_pos[1]] = 0;
        update();
        player == 1 ? --black_chance : --white_chance;
        player = 1 - player;
        return;
    }
    break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void GoBang::question_black_backout()
{
    if (isBackout == false && player == 1 && black_chance != 0)
    {
        backout("黑子");
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString("抱歉"),
                             QString::fromStdString("您现在不能悔棋！"),
                             QMessageBox::Ok);
    }
}

void GoBang::question_white_backout()
{
    if (isBackout == false && player == 0 && white_chance != 0)
    {
        backout("白子");
    }
    else
    {
        QMessageBox::warning(this, QString::fromStdString("抱歉"),
                             QString::fromStdString("您现在不能悔棋！"),
                             QMessageBox::Ok);
    }
}