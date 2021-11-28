#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QGraphicsObject>

class ButtonItem : public QGraphicsObject
{
    Q_OBJECT
public:
    ButtonItem(const QPixmap& pixmap, QGraphicsScene* scene);
    ~ButtonItem();

protected:
    QRectF boundingRect() const;    //重写图形像的矩形绘图区域

private:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);   //重写绘图事件

    void mousePressEvent(QGraphicsSceneMouseEvent* event);    //重写鼠标按下事件

signals:
    void clicked();   //点击信号

private:
    QPixmap m_pix;    //按钮位图
};

#endif // BUTTONITEM_H
