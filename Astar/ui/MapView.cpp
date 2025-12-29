//
// Created by apply on 29/12/2025.
//

#include "MapView.h"
#include <QWheelEvent>

void MapView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y()==0) return;

    if (event->angleDelta().y()>0 && m_currentScale < m_maxScale)
    {
        scale(m_scalefactor,m_scalefactor);
        m_currentScale *= m_scalefactor;
    }
    else if (event->angleDelta().y()<0 && m_currentScale > m_minScale)
    {
        scale(1/m_scalefactor,1/m_scalefactor);
        m_currentScale /= m_scalefactor;
    }
    event->accept();
}
