#ifndef ROADITEM_H
#define ROADITEM_H

#include <QGraphicsObject>
#include <QGraphicsScene>

//道路图形像
class RoadItem : public QGraphicsObject
{
    Q_OBJECT
public:
    RoadItem(QGraphicsScene* scene);
    ~RoadItem();

protected:
    //重写图形像的绘图区域
    QRectF boundingRect() const;

    //重写绘图事件
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

private:
    void startMove();   //道路开始移动

private:
    QGraphicsScene* m_scene;   //游戏场景
};

#endif // ROADITEM_H
