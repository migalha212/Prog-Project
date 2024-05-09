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
    // HERE -->
    Circle::Circle(const Color &fill,
                   const Point &center,
                   const int &radius)
        : Ellipse(fill,center,{radius,radius})
    {
    }  

    Polyline::Polyline(const std::vector<Point> &points, const Color &stroke)
    {
        for(Point p : points)
        {
            this->points.push_back(p);
        }
        this->stroke = stroke;
        
    }

    

    void Polyline::draw(PNGImage &img) const
    {
        Point a = points[0];
        for(Point b : points){
            if(a.x == b.x && a.y == b.y)
            {
             continue;
            }
            img.draw_line(a,b,stroke);
            a = b;
            
        }
    }

   Line::Line(const Point& start,const Point& end,const Color& stroke) : start(start), end(end), stroke(stroke)
   {
   }

    void Line::draw(PNGImage& img) const
    {
        img.draw_line(start,end,stroke);
    }

    Polygon::Polygon(const std::vector<Point> &points, const Color &fill)
    {
        for(Point p : points)
        {
            this->points.push_back(p);
        }

        this->fill = fill;
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points,fill);
    }
    Rect::Rect(const Point &upper, const int &width, const int &height, const Color &fill) :
        Polygon({upper,{upper.x, upper.y + height - 1},{upper.x + width - 1,upper.y + height - 1},{upper.x + width - 1,upper.y}},fill)
    {   
    }
}