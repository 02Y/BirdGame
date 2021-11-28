#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QFile>
#include <QGraphicsView>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QPainter>
#include "buttonitem.h"
#include <QTimer>
#include "RoadItem.h"
#include "PipeItem.h"

GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow),
    m_startGame(false)
{
    ui->setupUi(this);
    initControl();
    initBackgroundMusic();
    this->setWindowTitle("飞翔的小鸟");
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    //自定义paint事件
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(m_startGame)
    {
        m_birdItem->keyPressEvent(event);
    }
}

void GameWindow::initControl()
{
    loadStyleSheet("GameWindow");    //加载样式

    const QRectF gameRect = rect();     //当前gameWindow矩形区域
    m_scene = new GameScene(this, gameRect);    //初始化游戏场景

    QGraphicsView* view = new QGraphicsView(m_scene, this);    //初始化游戏视图（将场景添加到视图中）
    view->setScene(m_scene);    //设置游戏场景
    view->setStyleSheet("border:none;background:transparent;");  //视图无边框、透明
    view->setCacheMode(QGraphicsView::CacheBackground);     //设置缓存为背景模式
    startWelcome();
}

void GameWindow::initBackgroundMusic()
{
    //使用多媒体需要添加模块：multimedia
    m_welcomePlayer = new QMediaPlayer(this);

    //播放一次
//    m_welcomePlayer->setMedia(QUrl("qrc:/BirdGame/Resources/sound/welcome.mp3"));
//    m_welcomePlayer->setVolume(10);   //设置音量
//    m_welcomePlayer->play();

    m_welcomePlayerList = new QMediaPlaylist(this);
    m_welcomePlayerList->addMedia(QUrl("qrc:/BirdGame/Resources/sound/welcome.mp3"));
    //...添加其他播放音乐
    //...添加其他播放音乐
    //...添加其他播放音乐

    m_welcomePlayerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);   //循环播放当前列表中的曲目
    m_welcomePlayer->setPlaylist(m_welcomePlayerList);  //设置播放列表
    m_welcomePlayer->setVolume(7);
    m_welcomePlayer->play();
}

//加载样式
void GameWindow::loadStyleSheet(const QString &sheetName)
{
    QFile file((":/BirdGame/Resources/qss/") + sheetName + ".css");
    if(file.open(QFile::ReadOnly))
    {
        QString strStyleSheet = this->styleSheet();  //获取当前样式
        strStyleSheet += file.readAll();

        this->setStyleSheet(strStyleSheet);
        file.close();
    }
}

//启动欢迎
void GameWindow::startWelcome()
{
    //道路
    RoadItem* roadItem = new RoadItem(m_scene);

    //管道
    PipeItem* pipeItem = new PipeItem(m_scene);

    //小鸟
    m_birdItem = new BirdItem(m_scene);

    //游戏状态检测
    m_checkGameStatusTimer = new QTimer(this);
    connect(m_checkGameStatusTimer, SIGNAL(timeout()), this, SLOT(onCheckGameStatus()));  //定时检测游戏状态

    //字母
    //  飞吧,
    //像风一样自由,
    //  无法挽留...”
    const int nLetters = 15;
    struct
    {
        const char* pix;
        qreal initx, inity;     //起始值
        qreal destX, destY;     //结束值
    }letterData[nLetters] = {
    {"飞", -1000, -1000, 250, 100},    //第一行
    {"吧", -800, -1000, 300, 100},
    {"，", -600, -1000, 350, 100},
    {"像", -400, -1000, 150, 160},     //第二行
    {"风", 1000, 2000, 200, 160},
    {"一", 800, 2000, 250, 160},
    {"样", 600, 2000, 300, 160},
    {"自", 400, 2000, 350, 160},
    {"由", 200, 2000, 400, 160},
    {"，", 0, 2000, 450, 160},
    {"无", 0, 2000, 175, 220},         //第三行
    {"法", 0, 2000, 225, 220},
    {"挽", 0, 2000, 275, 220},
    {"留", 0, 2000, 325, 220},
    {"...", 0, 2000, 375, 220},
    };

    //连续动画组
    QSequentialAnimationGroup* letterGroupMoving = new QSequentialAnimationGroup(this);

    //并行动画组
    m_lettersGroupFading = new QParallelAnimationGroup(this);

    for(int i = 0; i < nLetters; i++)
    {
        //文本图形项
        QString htmlText = QString("<span style=\"font-family:'Berlin Sans FB';font-size:38px;font-weight:600;color:#194819;\">%1</span>")
                .arg(letterData[i].pix);
        QGraphicsTextItem* letter = new QGraphicsTextItem();
        letter->setHtml(htmlText);
        letter->setPos(letterData[i].initx, letterData[i].inity);

        //文本项属性动画
        QPropertyAnimation* moveAnimation = new QPropertyAnimation(letter, "pos", letterGroupMoving);
        moveAnimation->setEndValue(QPoint(letterData[i].destX, letterData[i].destY));  //设置字符动画结束的位置
        moveAnimation->setDuration(200);    //动画时长  200ms
        moveAnimation->setEasingCurve(QEasingCurve::OutElastic);    //动画曲线效果
        letterGroupMoving->addPause(50);    //暂停50ms

        //渐渐消失动画（褪色）
        QPropertyAnimation* fadeAnimation = new QPropertyAnimation(letter, "opacity", m_lettersGroupFading);
        fadeAnimation->setDuration(1000);
        fadeAnimation->setEndValue(0);   //最后透明度为0，完全透明
        fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);

        m_scene->addItem(letter);   //往游戏场景中添加字符图形像
    }

    //启动字符动画
    letterGroupMoving->start(QAbstractAnimation::DeleteWhenStopped);  //动画停止后删除（资源回收）

    //按钮图形像
    QPixmap pix = QPixmap(":/BirdGame/Resources/image/startButton.png");

    ButtonItem* btnItem = new ButtonItem(pix, m_scene);
    btnItem->setPos(QPoint(220, 340));

    //按钮渐渐消失动画
    QPropertyAnimation* btnfadeAnimation = new QPropertyAnimation(btnItem, "opacity", m_lettersGroupFading);
    btnfadeAnimation->setDuration(600);   //动画时长600ms
    btnfadeAnimation->setEndValue(0);     //opacity为0完全透明，1——0
    btnfadeAnimation->setEasingCurve(QEasingCurve::OutQuad);

    //按钮图形像被点击则开始游戏
    connect(btnItem, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
    connect(btnfadeAnimation, &QPropertyAnimation::finished, [this](){
       m_startGame = true;
       m_checkGameStatusTimer->start(50);
       m_birdItem->flyLandfallAnimation();   //下降动画

       m_welcomePlayer->stop();
       m_welcomePlayerList->clear();
       m_welcomePlayerList->addMedia(QUrl("qrc:/BirdGame/Resources/sound/background.mp3"));
       m_welcomePlayer->play();
    });
}

void GameWindow::GameOver()
{
    //“墙都不扶，就服你！”
    const int nLetters= 9;
    struct
    {
        const char* pix;
        qreal initx, inity;     //起始值
        qreal destX, destY;     //结束值
    }letterData[nLetters] = {
    {"墙", -1000, -1000, 200, 160},
    {"都", -800, -1000, 250, 160},
    {"不", -600, -1000, 300, 160},
    {"扶", -400, -1000, 350, 160},
    {"，", 1000, 2000, 400, 160},
    {"就", 800, 2000, 225, 220},
    {"服", 600, 2000, 275, 220},
    {"你", 400, 2000, 325, 220},
    {"！", 200, 2000, 375, 220}
    };

    //game over 文字动画组
    QParallelAnimationGroup* lettersGroupMoving = new QParallelAnimationGroup(this);

    for(int i = 0; i < nLetters; i++)
    {
        //文本图形项
        QString htmlText = QString("<span style=\"font-family:'Berlin Sans FB';font-size:38px;font-weight:600;color:#194819;\">%1</span>")
                .arg(letterData[i].pix);
        QGraphicsTextItem* letter = new QGraphicsTextItem();
        letter->setHtml(htmlText);
        letter->setPos(letterData[i].initx, letterData[i].inity);

        //文本项属性动画
        QPropertyAnimation* moveAnimation = new QPropertyAnimation(letter, "pos", lettersGroupMoving);
        moveAnimation->setEndValue(QPoint(letterData[i].destX, letterData[i].destY));  //设置字符动画结束的位置
        moveAnimation->setDuration(200);    //动画时长  200ms
        moveAnimation->setEasingCurve(QEasingCurve::OutElastic);    //动画曲线效果

        //文本图形项添加到游戏场景中
        m_scene->addItem(letter);
    }
    lettersGroupMoving->start(QAbstractAnimation::DeleteWhenStopped);    //停止后进行资源回收

    //将小鸟图形像移除
    m_scene->removeItem(m_birdItem);

    //游戏结束的音效
    m_welcomePlayer->stop();
    m_welcomePlayerList->clear();
    m_welcomePlayer->setMedia(QUrl("qrc:/BirdGame/Resources/sound/gameover.wav"));
    m_welcomePlayer->setVolume(7);
    m_welcomePlayer->play();
}

void GameWindow::onStartBtnClicked()
{
    //启动渐渐消失动画，动画结束后删除
    m_lettersGroupFading->start(QAbstractAnimation::DeleteWhenStopped);   //关闭后进行资源回收
}

void GameWindow::onCheckGameStatus()
{
    if(m_birdItem->checkIsCollided())    //小鸟是否与其他场景有冲突（碰撞）
    {
        GameOver();
    }
}
