#include "SVGElements.hpp"
//#include <sstream>
#include <string>
using namespace std;
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
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(tinyxml2::XMLElement *elem)
    {
        center = {stoi(elem->Attribute("cx")), stoi(elem->Attribute("cy"))};
        radius = {stoi(elem->Attribute("rx")), stoi(elem->Attribute("ry"))};
        fill = parse_color(elem->Attribute("fill")); 
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
    /////
    Circle::Circle(tinyxml2::XMLElement *elem)
    {
        center = {stoi(elem->Attribute("cx")), stoi(elem->Attribute("cy"))};
        radius = stoi(elem->Attribute("r"));
        fill = parse_color(elem->Attribute("fill"));
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
    /////
    Polyline::Polyline(tinyxml2::XMLElement *elem)
    {
        string str = elem->Attribute("points");
        int x = 0, y = 0;
        char current = 'n';
        unsigned int i = 0;
        while (i < str.length())        // THIS IS REUSED FOR POLYGON
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
        stroke = parse_color(elem->Attribute("stroke"));
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
    /////
    Line::Line(tinyxml2::XMLElement *elem)
    {
        start = {stoi(elem->Attribute("x1")), stoi(elem->Attribute("y1"))};
        end = {stoi(elem->Attribute("x2")), stoi(elem->Attribute("y2"))};
        stroke = parse_color(elem->Attribute("stroke"));
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
    /////
    Polygon::Polygon(tinyxml2::XMLElement *elem)
    {
        string str = elem->Attribute("points");
        int x = 0, y = 0;
        char current = 'n';
        unsigned int i = 0;
        while (i < str.length())        // THIS IS REUSED FOR POLYLINE
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
        fill = parse_color(elem->Attribute("fill")); 
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
    /////
    Rect::Rect(tinyxml2::XMLElement *elem)
    {
        Point position = {stoi(elem->Attribute("x")), stoi(elem->Attribute("y"))};
        int width = stoi(elem->Attribute("width"));
        int height = stoi(elem->Attribute("height"));

        fill = parse_color(elem->Attribute("fill"));
        points = {position,
                  {position.x + width - 1,  position.y},
                  {position.x + width - 1,  position.y + height - 1},
                  {position.x,              position.y + height - 1}};
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
}
