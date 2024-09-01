#include "pvroof.h"
#include <QRectF>

/*
PVRoof::PVRoof()
{
    roof.setRoof(0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    module.setPVModule(0.0, 0.0, 0.0);
}
*/

int PVRoof::calculate_pv_layout()
{
    switch (roof.type) {
    case Roof::RECTANGULAR:
        calculate_rect_portrait();
        calculate_rect_landscape();

    case Roof::TRAPEZOIDAL:
        calculate_trap_portrait();
        calculate_trap_landscape();

    default:
        return 0;
    }

    return std::max(portrait.calculate_rating(module.rating), landscape.calculate_rating(module.rating));

}

int PVRoof::calculate_rect_portrait()
{
    for (int i = 0; i < 65536; i++) {
        if ((roof.width - roof.top_clearance - roof.bottom_clearance - roof.module_spacing * (i ? i - 1 : i)) - module.length * i < module.length + roof.module_spacing) {
            portrait.rows = i;
            break;
        }
        if (i == 65535)
            return -1;
    }
    for (int i = 0; i < 65536; i++) {
        if (roof.length - roof.right_clearance - roof.left_clearance - roof.module_spacing * (i ? i - 1 : i) - module.width * i < module.width + roof.module_spacing) {
            portrait.columns = i;
            break;
        }
        if (i == 65535)
            return -1;
    }

    return 0;
}

int PVRoof::calculate_rect_landscape()
{
    for (int i = 0; i < 65536; i++) {
        if ((roof.width - roof.top_clearance - roof.bottom_clearance - roof.module_spacing * (i ? i - 1 : i)) - module.width * i < module.width + roof.module_spacing) {
            landscape.rows = i;
            break;
        }
        if (i == 65535)
            return -1;
    }
    for (int i = 0; i < 65536; i++) {
        if (roof.length - roof.right_clearance - roof.left_clearance - roof.module_spacing * (i ? i - 1 : i) - module.length * i < module.length + roof.module_spacing) {
            landscape.columns = i;
            break;
        }
        if (i == 65535)
            return -1;
    }

    return 0;
}

int PVRoof::calculate_trap_portrait()
{
    return 1;
}

int PVRoof::calculate_trap_landscape()
{
    return 1;
}

void Roof::setRoof(int type,
                   double length,
                   double width,
                   double top,
                   double bottom,
                   double left,
                   double right,
                   double top_clearance,
                   double bottom_clearance,
                   double left_clearance,
                   double right_clearance,
                   double module_spacing)
{
    type = type;
    width = width;
    top = top;
    bottom = bottom;
    left = left;
    right = right;
    top_clearance = top_clearance;
    bottom_clearance = bottom_clearance;
    left_clearance = left_clearance;
    right_clearance = right_clearance;
    module_spacing = module_spacing;
}

double Roof::area()
{
    return length * width;
}

void PVModule::setPVModule(double length,
                           double width,
                           double rating)
{
    length = length;
    width = width;
    rating = rating;
}

const double PVLayout::calculate_rating(const double rating) {
    return rows * columns * rating;
}

double PVLayout::calculate_area(const PVModule& module, const double spacing)
{
    switch (layout) {
    case PVLayout::PORTRAIT:
        return module.length * module.width * columns * rows
               + rows * (columns - 1) * spacing * module.length
               + columns * (rows - 1) * spacing * module.width
               + (rows - 1) * (columns - 1) * spacing * spacing;
    case PVLayout::LANDSCAPE:
        return module.length * module.width * columns * rows
               + rows * (columns - 1) * spacing * module.width
               + columns * (rows - 1) * spacing * module.length
               + (rows - 1) * (columns - 1) * spacing * spacing;
    }
}
