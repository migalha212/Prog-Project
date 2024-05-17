
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
using namespace tinyxml2;

namespace svg
{
    /// @brief Read svg elements from a file to a vector
    /// @param svg_file Filename
    /// @param dimensions Height and width of the picture
    /// @param svg_elements Vector to store element pointers in
    void readSVG(const string &svg_file, Point &dimensions, vector<SVGElement *> &svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");

        map<string, SVGElement *> idMap;
        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            string child_name = child->Name();
            SVGElement *new_element;
            if (child_name == "use")
            {
                new_element = use(child, idMap);
            }
            else if (child_name == "g")
            {
                new_element = new Group(child, idMap);
            }
            else if (child_name == "ellipse")
            {
                Point center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
                Point radius = {child->IntAttribute("rx"), child->IntAttribute("ry")};
                Color fill = parse_color(child->Attribute("fill"));
                new_element = new Ellipse(fill, center, radius);
            }
            else if (child_name == "circle")
            {
                Point center = {child->IntAttribute("cx"), child->IntAttribute("cy")};
                int radius = {child->IntAttribute("r")};
                Color fill = parse_color(child->Attribute("fill"));
                new_element = new Circle(fill, center, radius);
            }
            else if (child_name == "polyline")
            {
                Color stroke = parse_color(child->Attribute("stroke"));
                istringstream iss(child->Attribute("points"));
                int x, y = 0;
                char skip;
                vector<Point> points = {};
                while (!iss.eof())
                {
                    iss >> x;
                    if (iss.fail())
                    {
                        iss.clear();
                        iss.ignore(1);
                        continue;
                    }
                    iss >> skip >> y;
                    points.push_back({x, y});
                }
                new_element = new Polyline(points, stroke);
            }
            else if (child_name == "line")
            {
                Color stroke = parse_color(child->Attribute("stroke"));
                Point start = {child->IntAttribute("x1"), child->IntAttribute("y1")};
                Point end = {child->IntAttribute("x2"), child->IntAttribute("y2")};
                new_element = new Line(start, end, stroke);
            }
            else if (child_name == "polygon")
            {
                Color fill = parse_color(child->Attribute("fill"));
                istringstream iss(child->Attribute("points"));
                int x, y = 0;
                char skip;
                vector<Point> points = {};
                while (!iss.eof())
                {
                    iss >> x;
                    if (iss.fail())
                    {
                        iss.clear();
                        iss.ignore(1);
                        continue;
                    }
                    iss >> skip >> y;

                    points.push_back({x, y});
                }
                new_element = new Polygon(points, fill);
            }
            else if (child_name == "rect")
            {
                Color fill = parse_color(child->Attribute("fill"));
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                Point upper = {child->IntAttribute("x"), child->IntAttribute("y")};

                std::vector<Point> points = {upper,
                                             {upper.x + width - 1, upper.y},
                                             {upper.x + width - 1, upper.y + height - 1},
                                             {upper.x, upper.y + height - 1}};
                new_element = new Rect(points, fill);
            }

            if (child->Attribute("transform"))
            {
                Point origin = {0, 0};
                if (child->Attribute("transform-origin"))
                {
                    istringstream iss(child->Attribute("transform-origin"));
                    int x, y;
                    iss >> x >> y;
                    origin = {x, y};
                }
                string transform = child->Attribute("transform");
                string opp = transform.substr(0, transform.find('('));
                if (opp == "rotate")
                {
                    istringstream valstr(transform.substr(transform.find('(') + 1, 5));
                    int value = 0;
                    valstr >> value;
                    new_element->rotate(origin, value);
                }
                if (opp == "scale")
                {
                    istringstream valstr(transform.substr(transform.find('(') + 1, 5));
                    int value = 0;
                    valstr >> value;
                    new_element->scale(origin, value);
                }
                if (opp == "translate")
                {
                    istringstream valstr(transform.substr(transform.find('(') + 1, 12));
                    int x, y = 0;
                    valstr >> x >> y;
                    if (valstr.fail())
                    {
                        valstr.clear();
                        valstr.ignore(1);
                        valstr >> y;
                    }
                    new_element->translate({x, y});
                }
            }
            if (child->Attribute("id"))
            {
                string id = child->Attribute("id");
                idMap[id] = new_element;
            }
            svg_elements.push_back(new_element);
        }
    }
}