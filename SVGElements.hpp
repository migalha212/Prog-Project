//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <vector>
#include <map>
namespace svg
{

    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual void transform(const std::string &operation, const Point &origin) = 0;
        virtual SVGElement *copy() = 0;
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
    std::vector<Point> parsePointVector(const std::string &str);
    // returns dynamically allocated copy of an element
    SVGElement *use(tinyxml2::XMLElement *xml_elem, std::map<std::string, SVGElement *> &idMap);
    class Group : public SVGElement
    {
    public:
        Group(tinyxml2::XMLElement *xml_elem, std::map<std::string, SVGElement *> &idMap);
        Group(const std::vector<SVGElement *> &elements);
        ~Group();
        void draw(PNGImage &img) const override;
        void transform(const std::string &operation, const Point &origin) override;
        SVGElement *copy();

    private:
        std::vector<SVGElement *> elements;
    };

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(tinyxml2::XMLElement *xml_elem);
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;
        void transform(const std::string &operation, const Point &origin) override;
        SVGElement *copy();

    private:
        Color fill;
        Point center;
        Point radius;
    };

    class Circle : public SVGElement
    {
    public:
        Circle(tinyxml2::XMLElement *xml_elem);
        Circle(const Point &center, int radius, const Color &fill);
        void draw(PNGImage &img) const override;
        void transform(const std::string &operation, const Point &origin) override;
        SVGElement *copy();

    private:
        Point center;
        int radius;
        Color fill;
    };
    class Polyline : public SVGElement
    {
    public:
        Polyline(tinyxml2::XMLElement *xml_elem);
        Polyline(const std::vector<Point> &points, const Color &stroke);
        void draw(PNGImage &img) const override;
        void transform(const std::string &operation, const Point &origin) override;
        SVGElement *copy();

    private:
        std::vector<Point> points;
        Color stroke;
    };
    class Line : public SVGElement
    {
    public:
        Line(tinyxml2::XMLElement *xml_elem);
        Line(const Point &start, const Point &end, const Color &stroke);
        void draw(PNGImage &img) const override;
        void transform(const std::string &operation, const Point &origin) override;
        SVGElement *copy();

    private:
        Point start;
        Point end;
        Color stroke;
    };
    class Polygon : public SVGElement
    {
    public:
        Polygon(tinyxml2::XMLElement *xml_elem);
        Polygon(const std::vector<Point> &points, const Color &fill);
        void draw(PNGImage &img) const override;
        void transform(const std::string &operation, const Point &origin) override;
        SVGElement *copy();

    private:
        std::vector<Point> points;
        Color fill;
    };
    class Rect : public SVGElement
    {
    public:
        Rect(tinyxml2::XMLElement *xml_elem);
        Rect(const std::vector<Point> &points, const Color &fill);
        void draw(PNGImage &img) const override;
        void transform(const std::string &operation, const Point &origin) override;
        SVGElement *copy();

    private:
        std::vector<Point> points;
        Color fill;
    };

}
#endif
