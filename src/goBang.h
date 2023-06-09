/*
 * @Author: Xyh4ng 503177404@qq.com
 * @Date: 2023-06-09 14:36:57
 * @FilePath: \gobang\src\goBang.h
 * @Description:
 * Copyright (c) 2023 by Xyh4ng, All Rights Reserved.
 */
#ifndef GOBANG_H
#define GOBANG_H

#include <QGroupBox>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>

class GoBang : public QMainWindow
{
    Q_OBJECT

public:
    GoBang(QWidget *parent = nullptr);
    ~GoBang(){};

private:
    // 定义菜单栏
    QMenu *game_menu;
    QAction *game_new;
    QAction *game_close;
    QMenu *backout_menu;
    QAction *white_backout;
    QAction *black_backout;
    QAction *help_action;

    // 绘制棋盘
    void paintEvent(QPaintEvent *event);
    // 确定落子位置
    void mouseReleaseEvent(QMouseEvent *event);
    // 确定棋手
    int player = 0; // 0为黑，1为白
    // 棋盘数组
    int chess[15][15] = {0}; // 初始为0，1为黑棋位置，2为白棋位置
    // 检测胜利
    bool game_win(int, int); // 游戏胜利检测
    // 当前落子个数
    int piece_num = 0;
    // 重开游戏
    void new_game();
    // 悔棋
    void backout(const char *str);
    // 悔棋机会
    int white_chance = 3;
    int black_chance = 3;
    // 本局是否已悔棋
    bool isBackout = false;
    // 悔棋落子位置
    int backout_pos[2] = {0};

private slots:
    // 询问是否重开游戏
    void question_new_Msg();
    // 询问是否退出游戏
    void question_close_Msg();
    // 查阅帮助信息
    void information_help_Msg();
    // 询问是否悔棋
    void question_black_backout();
    void question_white_backout();
};
#endif