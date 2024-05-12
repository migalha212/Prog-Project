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

    void Ellipse::rotate(const Point &origin,const int &angle)
    {
        center = center.rotate(origin,angle);
    }
    void Ellipse::scale(const Point &origin,const int &factor)
    {
        center = center.scale(origin,factor);
        radius = radius.scale({0,0},factor);
    }
    void Ellipse::translate(const int &x,const int &y)
    {
        center = center.translate({x,y});
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

    void Polyline::rotate(const Point &origin, const int &angle)
    {
        for(Point &p : this->points)
        {
            p = p.rotate(origin,angle);
        }
    }
    void Polyline::scale(const Point &origin,const int &factor)
    {
        for(Point &p : points)
        {
            p = p.scale(origin,factor);
        }
    }
    void Polyline::translate(const int &x,const int &y)
    {
        for(Point &p : points)
        {
            p = p.translate({x,y});
        }
    }

   Line::Line(const Point& start,const Point& end,const Color& stroke) : start(start), end(end), stroke(stroke)
   {
   }

    void Line::rotate(const Point &origin, const int &angle)
    {
        start = start.rotate(origin,angle);
        end = end.rotate(origin,angle);
    }

    void Line::draw(PNGImage& img) const
    {
        img.draw_line(start,end,stroke);
    }
    void Line::scale(const Point &origin,const int &factor)
    {
        start = start.scale(origin,factor);
        end = end.scale(origin,factor);
    }
    void Line::translate(const int &x,const int &y)
    {
        start = start.translate({x,y});
        end = end.translate({x,y});
    }



    Polygon::Polygon(const std::vector<Point> &points, const Color &fill)
    {
        for(Point p : points)
        {
            this->points.push_back(p);
        }

        this->fill = fill;
    }

    void Polygon::rotate(const Point &origin, const int &angle)
    {
        for(Point &p : points)
        {
           p = p.rotate(origin,angle);
        }
    }
    void Polygon::scale(const Point &origin,const int &factor)
    {
        for(Point &p : points)
        {
            p = p.scale(origin,factor);
        }
    }
    void Polygon::translate(const int &x,const int &y)
    {
        for(Point &p : points)
        {
            p = p.translate({x,y});
        }
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points,fill);
    }
    Rect::Rect(const std::vector<Point> &points, const Color &fill) : Polygon(points,fill)
    {   
    }
}