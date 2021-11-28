#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include "GameScene.h"
#include "birditem.h"
#include <QParallelAnimationGroup>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

protected:
    //重写绘图事件
    void paintEvent(QPaintEvent* event) override;

    //重写按键事件
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initControl();      //初始化部件
    void initBackgroundMusic();      //初始化背景音乐
    void loadStyleSheet(const QString& sheetName);   //加载样式
    void startWelcome();     //启动欢迎界面
    void GameOver();         //游戏结束

private slots:
    void onStartBtnClicked();
    void onCheckGameStatus();

private:
    Ui::GameWindow *ui;
    QMediaPlayer* m_welcomePlayer;  //欢迎音乐
    QMediaPlaylist* m_welcomePlayerList;  //片头音乐播放列表
    GameScene* m_scene;      //游戏场景
    BirdItem* m_birdItem;    //小鸟对象
    bool m_startGame;        //游戏开始标志
    QParallelAnimationGroup* m_lettersGroupFading;       //渐渐消失动画组
    QTimer* m_checkGameStatusTimer;    //检测游戏状态
};

#endif // GAMEWINDOW_H
