//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <vector>

namespace svg
{
    
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
    Point parsePoint(const std::string &str);

    class Group : public SVGElement
    {
    public:
        Group();
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        std::vector<SVGElement*> elements;
    };

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(tinyxml2::XMLElement *elem);
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
        Circle(tinyxml2::XMLElement *elem);
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
        Polyline(tinyxml2::XMLElement *elem);
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        std::vector<Point> points;
        Color stroke;
    };
    class Line : public SVGElement
    {
    public:
        Line(tinyxml2::XMLElement *elem);
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
        Polygon(tinyxml2::XMLElement *elem);
        void draw(PNGImage &img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        std::vector<Point> points;
        Color fill;
    };
    class Rect : public SVGElement
    {
    public:
        Rect(tinyxml2::XMLElement *elem);
        void draw(PNGImage & img) const override;
        void transform(const std::string& operation, const Point& origin) override;
    private:
        std::vector<Point> points;
        Color fill;
    };
    
}
#endif
