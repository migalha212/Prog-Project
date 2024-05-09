
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
        for(XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            string child_name = child->Name();
            SVGElement *new_element;
            if(child_name == "ellipse")
            {
                Point center = {stoi(child->Attribute("cx")),stoi(child->Attribute("cy"))};
                Point radius = {stoi(child->Attribute("rx")),stoi(child->Attribute("ry"))};
                Color fill = parse_color(child->Attribute("fill"));
                new_element = new Ellipse(fill,center,radius);
            }
            if(child_name == "circle")
            {
                Point center = {stoi(child->Attribute("cx")),stoi(child->Attribute("cy"))};
                int radius = {stoi(child->Attribute("r"))};
                Color fill = parse_color(child->Attribute("fill"));
                new_element = new Circle(fill,center,radius);
            }
            if(child_name == "polyline")
            {
                Color stroke = parse_color(child->Attribute("stroke"));
                istringstream iss(child->Attribute("points"));
                int x,y = 0;
                char skip;
                vector<Point> points = {};
                while(iss >> x >> skip >> y){
                    points.push_back({x,y});
                }
                new_element = new Polyline(points,stroke);
            } 
            if(child_name == "line")
            {
                Color stroke = parse_color(child->Attribute("stroke"));
                Point start = {stoi(child->Attribute("x1")),stoi(child->Attribute("y1"))};
                Point end = {stoi(child->Attribute("x2")),stoi(child->Attribute("y2"))};
                new_element = new Line(start,end,stroke);
            }
            if(child_name == "polygon")
            {
                Color fill = parse_color(child->Attribute("fill"));
                istringstream iss(child->Attribute("points"));
                int x,y = 0;
                char skip;
                vector<Point> points = {};
                while(!iss.eof())
                {
                    iss >> x;
                    if(iss.fail()){
                        iss.clear(); iss.ignore(1);
                        continue;
                    }
                    iss >> skip >> y;

                    points.push_back({x,y});
                }
                new_element = new Polygon(points,fill);
            }
            if(child_name == "rect")
            {
                Color fill = parse_color(child->Attribute("fill"));
                int width = stoi(child->Attribute("width"));
                int height = stoi(child->Attribute("height"));
                Point upper = {stoi(child->Attribute("x")),stoi(child->Attribute("y"))};
                new_element = new Rect(upper,width,height,fill);
            }
            svg_elements.push_back(new_element);
            
        }
    }
}