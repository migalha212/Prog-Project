
#include <iostream>
#include <sstream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
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
        
        // TODO complete code -->
        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            string childName = child->Name();
            SVGElement *newElement;
            if (childName == "ellipse")
            {
                Point center = {stoi(child->Attribute("cx")), stoi(child->Attribute("cy"))};
                Point radii = {stoi(child->Attribute("rx")), stoi(child->Attribute("ry"))};
                Color fill = parse_color(child->Attribute("fill")); 
                newElement = new Ellipse(fill, center, radii);
            }
            else if (childName == "circle")
            {
                Point center = {stoi(child->Attribute("cx")), stoi(child->Attribute("cy"))};
                int radius = stoi(child->Attribute("r"));
                Color fill = parse_color(child->Attribute("fill")); 
                newElement = new Circle(center, radius, fill);
            }
            else if (childName == "polyline")
            {
                string str = child->Attribute("points");
                std::vector<Point> points;
                
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
                Color stroke = parse_color(child->Attribute("stroke")); 
                newElement = new Polyline(points, stroke);
            }
            else if (childName == "line")
            {
                Point start = {stoi(child->Attribute("x1")), stoi(child->Attribute("y1"))};
                Point end = {stoi(child->Attribute("x2")), stoi(child->Attribute("y2"))};
                Color stroke = parse_color(child->Attribute("stroke")); 
                newElement = new Line(start, end, stroke);
            }
            else if (childName == "polygon")
            {
                string str = child->Attribute("points");
                std::vector<Point> points;
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
                Color fill = parse_color(child->Attribute("fill")); 
                newElement = new Polygon(points, fill);
            }
            else if (childName == "rect")
            {
                Point position = {stoi(child->Attribute("x")), stoi(child->Attribute("y"))};
                int width = stoi(child->Attribute("width"));
                int height = stoi(child->Attribute("height"));
                Color fill = parse_color(child->Attribute("fill")); 
                newElement = new Rect(position, width, height, fill);
            }
            svg_elements.push_back(newElement);
        }
        
    }
}