//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <vector>
#include "external/tinyxml2/tinyxml2.h"
#include <sstream>
#include <fstream>
#include <map>
#include <string>

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual void rotate(const Point &origin,const int &angle) = 0;
        virtual void translate(const Point &p) = 0;
        virtual void scale(const Point &origin,const int &factor) = 0;
        virtual SVGElement* copy() = 0;
        std::string get_id();
    private:
        
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);
    SVGElement *use(tinyxml2::XMLElement *xml_elem, std::map<std::string, SVGElement*> &idMap);
    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;
        void rotate(const Point &origin, const int &angle) override;
        void scale(const Point &origin,const int &factor) override;
        void translate(const Point &p) override;
        SVGElement* copy();
    private:
        Color fill;
        Point center;
        Point radius;
    };

    class Circle : public Ellipse
    {
    public:
        Circle(const Color &fill, const Point &center, const int &radius);
    };

    class Polyline : public SVGElement
    {
    public:
        Polyline(const std::vector<Point> &points, const Color &stroke);
        void draw(PNGImage &img) const override;
        void rotate(const Point &origin, const int &angle) override;
        void scale(const Point &origin,const int &factor) override;
        void translate(const Point &p) override;
        SVGElement* copy();
    private:
        std::vector<Point> points;
        Color stroke;
        
    };

    class Line : public Polyline
    {
    public:
        Line(const Point &start, const Point &end, const Color &stroke);
    };

    class Polygon : public SVGElement
    {
    public:
        Polygon(const std::vector<Point> &points, const Color &fill);
        void draw(PNGImage &img) const override;
        void rotate(const Point &origin, const int &angle) override;
        void scale(const Point &origin,const int &factor) override;
        void translate(const Point &p) override;
        SVGElement* copy();
    private:
    std::vector<Point> points;
    
    Color fill;
    };

    class Rect : public Polygon
    {
    public:
        Rect(const std::vector<Point> &points, const Color &fill);
    };

    class Group : public SVGElement
    {
        public:
        Group(tinyxml2::XMLElement *Root, std::map<std::string, SVGElement*> &idMap);
        Group(std::vector<SVGElement *> elements);
        ~Group();
        void add_element(SVGElement* element);
        void draw(PNGImage &img) const override;
        void rotate(const Point &origin, const int &angle) override;
        void scale(const Point &origin,const int &factor) override;
        void translate(const Point &p) override;
        SVGElement* copy();
        private:
        std::vector<SVGElement*> elements;
        
    };
}
#endif

