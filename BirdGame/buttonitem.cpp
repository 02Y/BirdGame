#include "buttonitem.h"
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMouseEvent>

ButtonItem::ButtonItem(const QPixmap& pixmap, QGraphicsScene* scene)
    :m_pix(pixmap)
{
    scene->addItem(this);   //场景中添加当前按钮图形对象
    setCursor(QCursor(Qt::PointingHandCursor));    //鼠标指针变为手的形状
}

ButtonItem::~ButtonItem()
{

}

//重写按钮图形像的区域
QRectF ButtonItem::boundingRect() const
{
    return QRectF(QPoint(0, 0), m_pix.size());
}

void ButtonItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget *)
{
    painter->drawPixmap(0, 0, m_pix);
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //按下为左键则发射点击信号
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}
