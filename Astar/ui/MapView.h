#ifndef ASTAR_GRAPHICSVIEW_H
#define ASTAR_GRAPHICSVIEW_H
#include <QGraphicsView>

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    MapView(QWidget *parent = nullptr) : QGraphicsView(parent)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        setDragMode(QGraphicsView::ScrollHandDrag);
    }

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    double m_scalefactor = 1.15;
    double m_currentScale = 1.0;
    double m_minScale = 0.1;
    double m_maxScale = 10.0;
};


#endif //ASTAR_GRAPHICSVIEW_H