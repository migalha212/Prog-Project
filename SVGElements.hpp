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
        SVGElement();          // required constructor.
        virtual ~SVGElement(); // required destructor.

        /// @brief calls for a shape to be drawn in the png image.
        /// @param img canvas which will be drawn on.
        virtual void draw(PNGImage &img) const = 0;

        /// @brief calls for a shape to be rotated.
        /// @param origin rotation origin.
        /// @param angle  rotation angle in degrees.
        virtual void rotate(const Point &origin, const int &angle) = 0;

        /// @brief calls for a shape to be moved.
        /// @param p translation offset.
        virtual void translate(const Point &p) = 0;

        /// @brief calls for a shape to be scaled.
        /// @param origin scaling origin.
        /// @param factor scaling factor.
        virtual void scale(const Point &origin, const int &factor) = 0;

        /// @brief calls for a shape to be copied.
        /// @return pointer to the copy of the shape.
        virtual SVGElement *copy() = 0;
    };

    /// @brief Reads svg elements from a file to a vector
    /// @param svg_file Filename (path) to the input file
    /// @param dimensions Height and width of the picture
    /// @param svg_elements Vector to store element pointers in
    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);

    /// @brief calls for a SVG to PNG conversion.
    /// @param svg_file SVG input file.
    /// @param png_file PNG output file.
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    /// @brief calls for a copy of an element from its id.
    /// @param xml_elem element.
    /// @param idMap map with ids as keys and with pointers to svg elements as values.
    /// @return pointer to a copy of the element.
    SVGElement *use(tinyxml2::XMLElement *xml_elem, std::map<std::string, SVGElement *> &idMap);

    class Ellipse : public SVGElement
    {
    public:
        /// @brief ellipse constructor.
        /// @param fill fill color.
        /// @param center ellipse center point.
        /// @param radius ellipse radius.
        Ellipse(const Color &fill, const Point &center, const Point &radius);

        /// @brief calls for an ellipse to be drawn in the png image.
        /// @param img canvas which will be drawn on.
        void draw(PNGImage &img) const override;

        /// @brief calls for an ellipse to be rotated.
        /// @param origin rotation origin.
        /// @param angle  rotation angle in degrees.
        void rotate(const Point &origin, const int &angle) override;

        /// @brief calls for an ellipse to be scaled.
        /// @param origin scaling origin.
        /// @param factor scaling factor.
        void scale(const Point &origin, const int &factor) override;

        /// @brief calls for an ellipse to be moved.
        /// @param p translation offset.
        void translate(const Point &p) override;

        /// @brief calls for an ellipse to be copied.
        /// @return pointer to the copy of the ellipse.
        SVGElement *copy();

    private:
        Color fill;
        Point center;
        Point radius;
    };

    class Circle : public Ellipse
    {
    public:
        /// @brief circle constructor.
        /// @param fill fill color.
        /// @param center circle center point.
        /// @param radius circle radius.
        Circle(const Color &fill, const Point &center, const int &radius);
    };

    class Polyline : public SVGElement
    {
    public:
        /// @brief polyline constructor.
        /// @param points vector which has polyline vertices coordinates.
        /// @param stroke line color.
        Polyline(const std::vector<Point> &points, const Color &stroke);

        /// @brief calls for a polyline to be drawn in the png image.
        /// @param img canvas which will be drawn on.
        void draw(PNGImage &img) const override;

        /// @brief calls for a polyline to be rotated.
        /// @param origin rotation origin.
        /// @param angle  rotation angle in degrees.
        void rotate(const Point &origin, const int &angle) override;

        /// @brief calls for a polyline to be scaled.
        /// @param origin scaling origin.
        /// @param factor scaling factor.
        void scale(const Point &origin, const int &factor) override;

        /// @brief calls for a polyline to be moved.
        /// @param p translation offset.
        void translate(const Point &p) override;

        /// @brief calls for a polyline to be copied.
        /// @return pointer to the copy of the polyline.
        SVGElement *copy();

    private:
        std::vector<Point> points;
        Color stroke;
    };

    class Line : public Polyline
    {
    public:
        /// @brief line constructor.
        /// @param start line start point.
        /// @param end line end point.
        /// @param stroke line color.
        Line(const Point &start, const Point &end, const Color &stroke);
    };

    class Polygon : public SVGElement
    {
    public:
        /// @brief polygon constructor.
        /// @param points vector which has vertices coordinates.
        /// @param fill fill color.
        Polygon(const std::vector<Point> &points, const Color &fill);

        /// @brief calls for a polygon to be drawn in the png image.
        /// @param img canvas which will be drawn on.
        void draw(PNGImage &img) const override;

        /// @brief calls for a polygon to be rotated.
        /// @param origin rotation origin.
        /// @param angle  rotation angle in degrees.
        void rotate(const Point &origin, const int &angle) override;

        /// @brief calls for a polygon to be scaled.
        /// @param origin scaling origin.
        /// @param factor scaling factor.
        void scale(const Point &origin, const int &factor) override;

        /// @brief calls for a polygon to be moved.
        /// @param p translation offset.
        void translate(const Point &p) override;

        /// @brief calls for a polygon to be copied.
        /// @return pointer to the copy of the polygon.
        SVGElement *copy();

    private:
        std::vector<Point> points;

        Color fill;
    };

    class Rect : public Polygon
    {
    public:
        /// @brief rect constructor.
        /// @param points vector which has rect vertices coordinates.
        /// @param fill fill color.
        Rect(const std::vector<Point> &points, const Color &fill);
    };

    class Group : public SVGElement
    {
    public:
        /// @brief group constructor with root element.
        /// @param Root XMLElement representing a group to be read.
        /// @param idMap vector of elements with an id and respective id.
        Group(tinyxml2::XMLElement *Root, std::map<std::string, SVGElement *> &idMap);

        /// @brief group constructor.
        /// @param elements vector with pointers to SVGEelements.
        Group(std::vector<SVGElement *> elements);

        /// @brief group destructor.
        ~Group();

        /// @brief add element to the elements vector.
        /// @param element element to add to the vector.
        void add_element(SVGElement *element);

        /// @brief calls for a group to be drawn in the png image.
        /// @param img canvas which will be drawn on.
        void draw(PNGImage &img) const override;

        /// @brief calls for a group to be rotated.
        /// @param origin rotation origin.
        /// @param angle  rotation angle in degrees.
        void rotate(const Point &origin, const int &angle) override;

        /// @brief calls for a group to be scaled.
        /// @param origin scaling origin.
        /// @param factor scaling factor.
        void scale(const Point &origin, const int &factor) override;

        /// @brief calls for a group to be moved.
        /// @param p translation offset.
        void translate(const Point &p) override;

        /// @brief calls for a group to be copied.
        /// @return pointer to the copy of the group.
        SVGElement *copy();

    private:
        std::vector<SVGElement *> elements;
    };
}
#endif
