#ifndef PVROOF_H
#define PVROOF_H

#include <QString>
class PVRoof;

class Coordinates {
public:
    Coordinates();

    double x;
    double y;

    Coordinates(double x, double y)
    {
        x = x;
        y = y;
    }
};

class Line {
public:
    Line();
    Coordinates start;
    Coordinates end;
    Coordinates function(double x);
};


class Rectangle {
public:
    Rectangle();
    Coordinates start;
    Coordinates end;
    double length;
    double width;

    Rectangle(Coordinates start, Coordinates end)
    {
        start = start;
        end = end;
    }

    Rectangle(Coordinates start, double length, double width)
    {
        start = start;
        length = length;
        width = width;
    }

    Rectangle(double length, double width)
    {
        length = length;
        width = width;
    }
};

class PVModule {
public:
    double length;
    double width;
    double rating;
    QString name;
    QString manufacturer;

    PVModule()
        : length(0.0), width(0.0), rating(0.0),
        name("n/a"), manufacturer("n/a")
    {}

    void setPVModule(double length,
                     double width,
                     double rating);
};

class Roof {
public:
    int type;
    double length;
    double width;
    double top;
    double bottom;
    double left;
    double right;
    double top_clearance;
    double bottom_clearance;
    double left_clearance;
    double right_clearance;
    double module_spacing;

    enum rooftype {
        RECTANGULAR, TRAPEZOIDAL
    };

    Roof() = default;

    void setRoof(int type,
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
                 double module_spacing);

    double area();
};

class PVLayout {
public:
    int rows;
    int columns;

    enum Layout {
        PORTRAIT, LANDSCAPE
    };

    PVLayout::Layout layout;

    PVLayout(const PVLayout::Layout layout)
        :rows(0), columns(0), layout(layout) {}

    const double calculate_rating(const double rating);
    double calculate_area(const PVModule& module, const double spacing);
};

class PVRoof
{
private:
    int calculate_rect_portrait();
    int calculate_rect_landscape();
    int calculate_trap_portrait();
    int calculate_trap_landscape();

public:
    Roof roof;
    PVModule module;
    PVLayout portrait;
    PVLayout landscape;

    PVRoof()
        : portrait(PVLayout::PORTRAIT), landscape(PVLayout::LANDSCAPE) {}

    int calculate_pv_layout();
};

#endif // PVROOF_H
