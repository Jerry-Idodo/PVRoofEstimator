#include "pvdrawing.h"
#include <QDebug>

PVDrawing::PVDrawing()
    :length(800), width(600), scale(1)
{
    portrait = new QGraphicsScene();
    landscape = new QGraphicsScene();
}

PVDrawing::~PVDrawing()
{
    delete portrait;
    delete landscape;
}

void PVDrawing::draw(PVRoof& Array)
{
    QRectF Module[2];                               // 0: Portrait, 1: Landscape
    QPointF start_pos;
    QPointF increament[2];                          // 0: Portrait, 1: Landscape
    QPen pen(Qt::black);
    pen.setWidthF(0.04);

    set_module_params(Array, Module);
    set_pos_params(Array.roof, Array.module, start_pos, increament);

    switch (Array.roof.type) {
    case Roof::RECTANGULAR:
        draw_rect_roof(Array.roof);
        draw_rect_roof_array(Array.portrait, Module[0], start_pos, increament[0]);
        draw_rect_roof_array(Array.landscape, Module[1], start_pos, increament[1]);
        break;
    case Roof::TRAPEZOIDAL:
        break;
    default:
        qDebug() << "Invalid Roof Type";
        return;
    }

    scale = get_scale(Array.roof.length, Array.roof.width);
}

void PVDrawing::set_module_params(const PVRoof& Array, QRectF *Module)
{
    Module[0].setRect(Array.roof.left_clearance, Array.roof.bottom_clearance, Array.module.width, Array.module.length);
    Module[1].setRect(Array.roof.left_clearance, Array.roof.bottom_clearance, Array.module.length, Array.module.width);
}

void PVDrawing::set_pos_params(const Roof& Roof, const PVModule& Module, QPointF& start_pos, QPointF *inc)
{

    inc[0].setX(Module.width + Roof.module_spacing);
    inc[0].setY(-(Module.length + Roof.module_spacing));
    inc[1].setX(Module.length + Roof.module_spacing);
    inc[1].setY(-(Module.width + Roof.module_spacing));

    start_pos.setX(Roof.left_clearance);
    start_pos.setY(Roof.width - Roof.bottom_clearance);
}

void PVDrawing::draw_rect_roof(const Roof& Roof)
{
    QRectF Roof_Rect(0, 0, Roof.length, Roof.width);
    QRectF Limit(Roof.left_clearance,
                 Roof.top_clearance,
                 Roof.length - Roof.left_clearance - Roof.right_clearance,
                 Roof.width - Roof.top_clearance - Roof.bottom_clearance);

    QPen pen(Qt::black);
    pen.setWidthF(0.04);
    portrait->addRect(Roof_Rect, pen);
    landscape->addRect(Roof_Rect, pen);

    pen.setColor(Qt::red);
    pen.setWidthF(0.02);
    portrait->addRect(Limit, pen);
    landscape->addRect(Limit, pen);
}

void PVDrawing::draw_rect_roof_array(const PVLayout& Layout, QRectF& Module, QPointF& start_pos, QPointF& increment)
{
    int i, j;
    QPen pen(Qt::blue);
    pen.setWidthF(0.02);

    for (i = 0; i < Layout.rows; i++) {
        for (j = 0; j < Layout.columns; j++) {
            QPointF pos = start_pos + QPointF(j * increment.x(), i * increment.y());
            // qDebug() << pos;
            Module.moveBottomLeft(pos);
            if (Layout.layout == PVLayout::PORTRAIT)
                portrait->addRect(Module, pen);
            else if (Layout.layout == PVLayout::LANDSCAPE)
                landscape->addRect(Module, pen);
            else
                qDebug() << "Unable to detect layout type";
        }
    }
}

void PVDrawing::clear()
{
    portrait->clear();
    landscape->clear();
}

qreal PVDrawing::get_scale(const qreal len, const qreal wid)
{
    qreal xscale = length / len;
    qreal yscale = width / wid;
    scale = std::floor(xscale > yscale ? xscale : yscale);
    return scale;
}
