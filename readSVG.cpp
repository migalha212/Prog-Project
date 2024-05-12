
#include <iostream>
#include <sstream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

// WIP
// enum class SVGType { };

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
        
        
        for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            string childName = child->Name();
            SVGElement *newElement;
            // Might as well get rid of this logic and treat the root element as a Group
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
                newElement = new Group(child);
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
            svg_elements.push_back(newElement);
        }
        
    }
}