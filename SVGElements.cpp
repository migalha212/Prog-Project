#include "SVGElements.hpp"
// #include <sstream>
#include <string>
#include <map>
using namespace std;
using namespace tinyxml2;
namespace svg
{

    Point parsePoint(const std::string &str)
    {
        int x = 0, y = 0;
        unsigned int i = 0;
        bool negative = false;
        if (str[0] == '-')
        {
            negative = true;
            i++;
        }

        while (str[i] != ' ' && str[i] != ',')
        {
            x *= 10;
            x += str[i] - '0';
            i++;
        }
        if (negative)
            x = -x;
        negative = false;
        while (str[i] == ' ' || str[i] == ',')
            i++;
        if (str[i] == '-')
        {
            negative = true;
            i++;
        }
        while (i < str.length())
        {
            y *= 10;
            y += str[i] - '0';
            i++;
        }
        if (negative)
            y = -y;
        return {x, y};
    }
    SVGElement* use(XMLElement *xml_elem, map<string, SVGElement*> &idMap)
    {
        string href = xml_elem->Attribute("href");
        href.erase(0, 1);
        SVGElement *newElement = idMap[href]->copy();
        return newElement;
    }
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    Group::Group(XMLElement *xml_elem, map<string, SVGElement*> &idMap)
    {
        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            string childName = child->Name();
            SVGElement *newElement;
            if (childName == "ellipse")
            {
                newElement = new Ellipse(child);
            }
            else if (childName == "circle")
            {
                newElement = new Circle(child);
            }
            else if (childName == "polyline")
            {
                newElement = new Polyline(child);
            }
            else if (childName == "line")
            { 
                newElement = new Line(child);
            }
            else if (childName == "polygon")
            {
                newElement = new Polygon(child);
            }
            else if (childName == "rect")
            { 
                newElement = new Rect(child);
            }
            else if (childName == "g")
            {
                newElement = new Group(child, idMap);
            }
            else if (childName == "use")
            {
                newElement = use(child, idMap);
            }
            std::string operation;
            Point origin = {0, 0};
            if (child->Attribute("transform-origin"))
            {
                origin = parsePoint(child->Attribute("transform-origin"));
            }
            if (child->Attribute("transform"))
            {
                operation = child->Attribute("transform");
                newElement->transform(operation, origin);
            }
            if (child->Attribute("id"))
            {
                string id = child->Attribute("id");
                idMap[id] = newElement;
            }   
            elements.push_back(newElement);
        }
    }
    Group::Group(const vector<SVGElement*> &elements) : elements(elements)
    {
    }
    Group::~Group()
    {
        for(SVGElement *elem : elements)
        {
            delete elem;
        }
    }
    // void draw(PNGImage &img) const override;
    // void transform(const std::string& operation, const Point& origin) override;
    void Group::draw(PNGImage &img) const
    {
        for(SVGElement *elem : elements)
        {
            elem->draw(img);
        }
    }
    void Group::transform(const std::string& operation, const Point& origin)
    {
        for(SVGElement *elem : elements)
        {
            elem->transform(operation, origin);
        }
    }
    SVGElement* Group::copy()
    {
        std::vector<SVGElement*> newElements;
        for(SVGElement *elem : elements)
        {
            newElements.push_back(elem->copy());
        }
        return new Group(newElements);
    }
    // TODO:
    // Replace stoi() with XMLElement's methods for getting int attributes
    Ellipse::Ellipse(XMLElement *xml_elem)
    {
        center = {stoi(xml_elem->Attribute("cx")), stoi(xml_elem->Attribute("cy"))};
        radius = {stoi(xml_elem->Attribute("rx")), stoi(xml_elem->Attribute("ry"))};
        fill = parse_color(xml_elem->Attribute("fill"));
    }
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
    void Ellipse::transform(const std::string &operation, const Point &origin)
    {
        if (operation.rfind("rotate", 0) == 0)
        {
            // for example, rotate(180)
            std::string str = operation.substr(7, 5); // "-120)" or "180)" or "90)" or "5)"...
            str.erase(str.length() - 1);              // remove the bracket
            int angle = stoi(str);
            center = center.rotate(origin, angle);
        }
        else if (operation.rfind("scale", 0) == 0)
        {
            std::string str = operation.substr(6, 5); // assuming we won't get values more than 9999
            str.erase(str.length() - 1);              // remove the bracket
            int scale = stoi(str);
            center = center.scale(origin, scale);
            radius = radius.scale({0, 0}, scale);
        }
        else if (operation.rfind("translate", 0) == 0)
        {
            std::string str = operation.substr(10, 12); // assuming we won't get a string longer that "-9999, -9999)"
            str.erase(str.length() - 1);                // remove the bracket
            Point offset = parsePoint(str);
            center = center.translate(offset);
        }
    }
    SVGElement* Ellipse::copy()
    {
        return new Ellipse(fill, center, radius);
    }
    /////
    Circle::Circle(XMLElement *xml_elem)
    {
        center = {stoi(xml_elem->Attribute("cx")), stoi(xml_elem->Attribute("cy"))};
        radius = stoi(xml_elem->Attribute("r"));
        fill = parse_color(xml_elem->Attribute("fill"));
    }
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
    void Circle::transform(const std::string &operation, const Point &origin)
    {
        if (operation.rfind("rotate", 0) == 0)
        {
            // for example, rotate(180)
            std::string str = operation.substr(7, 5); // "-120)" or "180)" or "90)" or "5)"...
            str.erase(str.length() - 1);              // remove the bracket
            int angle = stoi(str);
            center = center.rotate(origin, angle);
        }
        else if (operation.rfind("scale", 0) == 0)
        {
            std::string str = operation.substr(6, 5); // assuming we won't get values more than 9999
            str.erase(str.length() - 1);              // remove the bracket
            int scale = stoi(str);
            center = center.scale(origin, scale);
            radius *= scale;
        }
        else if (operation.rfind("translate", 0) == 0)
        {
            std::string str = operation.substr(10, 12); // assuming we won't get a string longer that "-9999, -9999)"
            str.erase(str.length() - 1);                // remove the bracket
            Point offset = parsePoint(str);
            center = center.translate(offset);
        }
    }
    SVGElement* Circle::copy()
    {
        return new Circle(center, radius, fill);
    }
    /////
    Polyline::Polyline(XMLElement *xml_elem)
    {
        string str = xml_elem->Attribute("points");
        int x = 0, y = 0;
        char current = 'n';
        unsigned int i = 0;
        while (i < str.length()) // THIS IS REUSED FOR POLYGON
        {
            if (str[i] == ',' || str[i] == ' ')
            {
                if (current == 'x')
                {
                    current = 'y';
                }
                else if (current == 'y')
                {
                    points.push_back({x, y});
                    x = 0;
                    y = 0;
                    current = 'n';
                }
            }
            else
            {
                if (current == 'x' || current == 'n')
                {
                    current = 'x';
                    x *= 10;
                    x += str[i] - '0';
                }
                else
                {
                    y *= 10;
                    y += str[i] - '0';
                }
            }
            i++;
        }
        points.push_back({x, y});
        stroke = parse_color(xml_elem->Attribute("stroke"));
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
            img.draw_line(points[i], points[i + 1], stroke);
        }

        // DO NOT CONNECT FIRST AND LAST POINT
        // img.draw_line(points[0], points[points.size() - 1], stroke);
    }
    void Polyline::transform(const std::string &operation, const Point &origin)
    {
        if (operation.rfind("rotate", 0) == 0)
        {
            // for example, rotate(180)
            std::string str = operation.substr(7, 5); // "-120)" or "180)" or "90)" or "5)"...
            str.erase(str.length() - 1);              // remove the bracket
            int angle = stoi(str);
            for (Point &point : points)
            {
                point = point.rotate(origin, angle);
            }
        }
        else if (operation.rfind("scale", 0) == 0)
        {
            std::string str = operation.substr(6, 5); // assuming we won't get values more than 9999
            str.erase(str.length() - 1);              // remove the bracket
            int scale = stoi(str);
            for (Point &point : points)
            {
                point = point.scale(origin, scale);
            }
        }
        else if (operation.rfind("translate", 0) == 0)
        {
            std::string str = operation.substr(10, 12); // assuming we won't get a string longer that "-9999, -9999)"
            str.erase(str.length() - 1);                // remove the bracket
            Point offset = parsePoint(str);
            for (Point &point : points)
            {
                point = point.translate(offset);
            }
        }
    }
    SVGElement* Polyline::copy()
    {
        return new Polyline(points, stroke);
    }
    /////
    Line::Line(XMLElement *xml_elem)
    {
        start = {stoi(xml_elem->Attribute("x1")), stoi(xml_elem->Attribute("y1"))};
        end = {stoi(xml_elem->Attribute("x2")), stoi(xml_elem->Attribute("y2"))};
        stroke = parse_color(xml_elem->Attribute("stroke"));
    }
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
    void Line::transform(const std::string &operation, const Point &origin)
    {
        if (operation.rfind("rotate", 0) == 0)
        {
            // for example, rotate(180)
            std::string str = operation.substr(7, 5); // "-120)" or "180)" or "90)" or "5)"...
            str.erase(str.length() - 1);              // remove the bracket
            int angle = stoi(str);
            start = start.rotate(origin, angle);
            end = end.rotate(origin, angle);
        }
        else if (operation.rfind("scale", 0) == 0)
        {
            std::string str = operation.substr(6, 5); // assuming we won't get values more than 9999
            str.erase(str.length() - 1);              // remove the bracket
            int scale = stoi(str);
            start = start.scale(origin, scale);
            end = end.scale(origin, scale);
        }
        else if (operation.rfind("translate", 0) == 0)
        {
            std::string str = operation.substr(10, 12); // assuming we won't get a string longer that "-9999, -9999)"
            str.erase(str.length() - 1);                // remove the bracket
            Point offset = parsePoint(str);
            start = start.translate(offset);
            end = end.translate(offset);
        }
    }
    SVGElement* Line::copy()
    {
        return new Line(start, end, stroke);
    }
    /////
    Polygon::Polygon(XMLElement *xml_elem)
    {
        string str = xml_elem->Attribute("points");
        int x = 0, y = 0;
        char current = 'n';
        unsigned int i = 0;
        while (i < str.length()) // THIS IS REUSED FOR POLYLINE
        {
            if (str[i] == ',' || str[i] == ' ')
            {
                if (current == 'x')
                {
                    current = 'y';
                }
                else if (current == 'y')
                {
                    points.push_back({x, y});
                    x = 0;
                    y = 0;
                    current = 'n';
                }
            }
            else
            {
                if (current == 'x' || current == 'n')
                {
                    current = 'x';
                    x *= 10;
                    x += str[i] - '0';
                }
                else
                {
                    y *= 10;
                    y += str[i] - '0';
                }
            }
            i++;
        }
        points.push_back({x, y});
        fill = parse_color(xml_elem->Attribute("fill"));
    }
    Polygon::Polygon(const std::vector<Point> &points,
                     const Color &fill)
        : points(points), fill(fill)
    {
    }
    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }
    void Polygon::transform(const std::string &operation, const Point &origin)
    {
        if (operation.rfind("rotate", 0) == 0)
        {
            // for example, rotate(180)
            std::string str = operation.substr(7, 5); // "-120)" or "180)" or "90)" or "5)"...
            str.erase(str.length() - 1);              // remove the bracket
            int angle = stoi(str);
            for (Point &point : points)
            {
                point = point.rotate(origin, angle);
            }
        }
        else if (operation.rfind("scale", 0) == 0)
        {
            std::string str = operation.substr(6, 5); // assuming we won't get values more than 9999
            str.erase(str.length() - 1);              // remove the bracket
            int scale = stoi(str);
            for (Point &point : points)
            {
                point = point.scale(origin, scale);
            }
        }
        else if (operation.rfind("translate", 0) == 0)
        {
            std::string str = operation.substr(10, 12); // assuming we won't get a string longer that "-9999, -9999)"
            str.erase(str.length() - 1);                // remove the bracket
            Point offset = parsePoint(str);
            for (Point &point : points)
            {
                point = point.translate(offset);
            }
        }
    }
    SVGElement* Polygon::copy()
    {
        return new Polygon(points, fill);
    }
    /////
    Rect::Rect(XMLElement *xml_elem)
    {
        Point position = {stoi(xml_elem->Attribute("x")), stoi(xml_elem->Attribute("y"))};
        int width = stoi(xml_elem->Attribute("width"));
        int height = stoi(xml_elem->Attribute("height"));

        fill = parse_color(xml_elem->Attribute("fill"));
        points = {position,
                  {position.x + width - 1, position.y},
                  {position.x + width - 1, position.y + height - 1},
                  {position.x, position.y + height - 1}};
    }
    Rect::Rect(const vector<Point> &points,
               const Color &fill)
        : points(points), fill(fill)
    {
    }
    void Rect::draw(PNGImage &img) const
    {

        img.draw_polygon(points, fill);
    }
    void Rect::transform(const std::string &operation, const Point &origin)
    {
        if (operation.rfind("rotate", 0) == 0)
        {
            // for example, rotate(180)
            std::string str = operation.substr(7, 5); // "-120)" or "180)" or "90)" or "5)"...
            str.erase(str.length() - 1);              // remove the bracket
            int angle = stoi(str);
            for (Point &point : points)
            {
                point = point.rotate(origin, angle);
            }
        }
        else if (operation.rfind("scale", 0) == 0)
        {
            std::string str = operation.substr(6, 5); // assuming we won't get values more than 9999
            str.erase(str.length() - 1);              // remove the bracket
            int scale = stoi(str);
            for (Point &point : points)
            {
                point = point.scale(origin, scale);
            }
        }
        else if (operation.rfind("translate", 0) == 0)
        {
            std::string str = operation.substr(10, 12); // assuming we won't get a string longer that "-9999, -9999)"
            str.erase(str.length() - 1);                // remove the bracket
            Point offset = parsePoint(str);
            for (Point &point : points)
            {
                point = point.translate(offset);
            }
        }
    }
    SVGElement* Rect::copy()
    {
        return new Rect(points, fill);
    }
}
