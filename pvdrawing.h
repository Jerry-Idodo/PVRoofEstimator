#ifndef PVDRAWING_H
#define PVDRAWING_H

#include "pvroof.h"
#include <QGraphicsScene>
#include <QRectF>
#include <QList>

class PVDrawing
{
    void set_module_params(const PVRoof& Array, QRectF *Module);
    void set_pos_params(const Roof& Roof, const PVModule& Module, QPointF& start_pos, QPointF *inc);
    void draw_rect_roof(const Roof& Roof);
    void draw_rect_roof_array(const PVLayout& Layout, QRectF& Module, QPointF& start_point, QPointF& increment);
    qreal get_scale(const qreal length, const qreal width);

public:
    QGraphicsScene *portrait;       // For Portrait Layout
    QGraphicsScene *landscape;      // For landscape Layout
    qreal length;                   // Screen Length in Pixels
    qreal width;                    // Screen Width in Pixels
    qreal scale;                    // Drawing Scale

    void draw(PVRoof& Array);
    void clear();


    PVDrawing();
    ~PVDrawing();

};

#endif // PVDRAWING_H
