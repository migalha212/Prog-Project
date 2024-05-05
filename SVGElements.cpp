#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE --> asa 

    // Circle::Circle(const Color &fill, const Point &center, const Point &radius) : Ellipse(fill, center, radius){};
    // void Circle::draw(PNGImage &img) const
    // {
    //     Ellipse::draw(img);
    // }
    Circle::Circle(const Point &center, 
                int radius, 
                const Color &fill)
        : center(center), radius(radius), fill(fill)
    {
    }
    void Circle::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, {radius, radius}, fill);
    }
    Polyline::Polyline(const std::vector<Point> &points, 
                    const Color &stroke)
        : points(points), stroke(stroke)
    {
    }
    void Polyline::draw(PNGImage &img) const
    {
        for (unsigned int i = 0; i < points.size() - 1; i++)
        {
            img.draw_line(points[i], points[i+1], stroke);
        }
        
        // DO NOT CONNECT FIRST AND LAST POINT
        // img.draw_line(points[0], points[points.size() - 1], stroke);
    }
    //
    Line::Line(const Point &start, 
            const Point &end, 
            const Color &stroke)
        : start(start), end(end), stroke(stroke)
    {
    }
    void Line::draw(PNGImage &img) const
    {
        img.draw_line(start, end, stroke);
    }
    //
    Polygon::Polygon(const std::vector<Point> &points, 
                    const Color &fill)
        : points(points), fill(fill)
    {
    }
    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }
    //
    Rect::Rect(const Point &position, 
            int width, 
            int height, 
            const Color &fill)
        : position(position), width(width), height(height), fill(fill)
    {
    }
    void Rect::draw(PNGImage &img) const
    {
        std::vector<Point> points = {
                                    position, 
                                    {position.x, position.y + height - 1}, 
                                    {position.x + width - 1, position.y + height - 1}, 
                                    {position.x + width - 1, position.y}
                                    };
        img.draw_polygon(points, fill);
    }
}   
