#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>

class GameScene : public QGraphicsScene
{
public:
    GameScene(QObject* parent, const QRectF &sceneRect);
    ~GameScene();
};

#endif // GAMESCENE_H
