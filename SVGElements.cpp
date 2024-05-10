#include "SVGElements.hpp"
#include <sstream>
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
            std::istringstream iss(str);
            int x = 0, y = 0;
            iss >> x >> y;
            center = center.translate({x,y});
        }
    }
    /////
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
            std::istringstream iss(str);
            int x = 0, y = 0;
            iss >> x >> y;
            center = center.translate({x,y});
        }
    }
    /////
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
            std::istringstream iss(str);
            int x = 0, y = 0;
            iss >> x >> y;
            for (Point &point : points)
            {
                point = point.translate({x, y});
            }
        }
    }
    /////
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
            std::istringstream iss(str);
            int x = 0, y = 0;
            iss >> x >> y;
            start = start.translate({x, y});
            end = end.translate({x, y});
        }
    }
    /////
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
            std::istringstream iss(str);
            int x = 0, y = 0;
            iss >> x >> y;
            for (Point &point : points)
            {
                point = point.translate({x, y});
            }
        }
    }
    /////
    Rect::Rect(const Point &position,
               int width,
               int height,
               const Color &fill)
        : fill(fill)
    {
        points = {position,
                  {position.x, position.y + height - 1},
                  {position.x + width - 1, position.y + height - 1},
                  {position.x + width - 1, position.y}};
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
            std::istringstream iss(str);
            int x = 0, y = 0;
            iss >> x >> y;
            for (Point &point : points)
            {
                point = point.translate({x, y});
            }
        }
    }
}
