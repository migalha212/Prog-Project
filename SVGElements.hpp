//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <vector>

namespace svg
{
    Point parsePoint(const std::string &str);
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual void transform(const std::string& operation, const Point& origin) = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        Color fill;
        Point center;
        Point radius;
    };
    
    class Circle : public SVGElement
    {
    public:
        Circle(const Point &center, int radius, const Color &fill);
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        Point center;
        int radius;
        Color fill;
    };
    class Polyline : public SVGElement
    {
    public:
        Polyline(const std::vector<Point> &points, const Color &stroke);
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        std::vector<Point> points;
        Color stroke;
    };
    class Line : public SVGElement
    {
    public:
        Line(const Point &start, const Point &end, const Color &stroke);
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        Point start;
        Point end;
        Color stroke;   
    };
    class Polygon : public SVGElement
    {
    public:
        Polygon(const std::vector<Point> &points, const Color &fill);
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        std::vector<Point> points;
        Color fill;
    };
    class Rect : public SVGElement
    {
    public:
        Rect(const Point &position, int width, int height, const Color &fill);
        void draw(PNGImage & img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        std::vector<Point> points;
        Color fill;
    };
    
}
#endif
