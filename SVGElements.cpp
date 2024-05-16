#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    std::string SVGElement::get_id()
    {
        return this->id;
    }

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius,
                     const std::string &id)
        : fill(fill), center(center), radius(radius), id(id)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    void Ellipse::rotate(const Point &origin,const int &angle)
    {
        center = center.rotate(origin,angle);
    }
    void Ellipse::scale(const Point &origin,const int &factor)
    {
        center = center.scale(origin,factor);
        radius = radius.scale({0,0},factor);
    }
    void Ellipse::translate(const Point &p)
    {
        center = center.translate(p);
    }
    SVGElement* Ellipse::copy(std::string &id)
    {
        return new Ellipse(fill,center,radius,id);
    }
    // @todo provide the implementation of SVGElement derived classes
    // HERE -->
    Circle::Circle(const Color &fill,
                   const Point &center,
                   const int &radius,
                   const std::string &id)
        : Ellipse(fill,center,{radius,radius},id)
    {
    }  

    Polyline::Polyline(const std::vector<Point> &points, const Color &stroke,const std::string &id)
    {
        for(Point p : points)
        {
            this->points.push_back(p);
        }
        this->stroke = stroke;
        this->id = id;
        
    }
    SVGElement* Polyline::copy(std::string &id)
    {
        return new Polyline(points,stroke,id);
    }

    

    void Polyline::draw(PNGImage &img) const
    {
        Point a = points[0];
        for(Point b : points){
            if(a.x == b.x && a.y == b.y)
            {
             continue;
            }
            img.draw_line(a,b,stroke);
            a = b;
            
        }
    }

    void Polyline::rotate(const Point &origin, const int &angle)
    {
        for(Point &p : this->points)
        {
            p = p.rotate(origin,angle);
        }
    }
    void Polyline::scale(const Point &origin,const int &factor)
    {
        for(Point &p : points)
        {
            p = p.scale(origin,factor);
        }
    }
    void Polyline::translate(const Point &p)
    {
        for(Point &p1 : points)
        {
            p1 = p1.translate(p);
        }
    }

   Line::Line(const Point& start,const Point& end,const Color& stroke,const std::string &id)
    : start(start), end(end), stroke(stroke), id(id)
   {
   }

    SVGElement* Line::copy(std::string &id)
    {
        return new Line(start,end,stroke,id);
    }
    void Line::rotate(const Point &origin, const int &angle)
    {
        start = start.rotate(origin,angle);
        end = end.rotate(origin,angle);
    }

    void Line::draw(PNGImage& img) const
    {
        img.draw_line(start,end,stroke);
    }
    void Line::scale(const Point &origin,const int &factor)
    {
        start = start.scale(origin,factor);
        end = end.scale(origin,factor);
    }
    void Line::translate(const Point &p)
    {
        start = start.translate(p);
        end = end.translate(p);
    }



    Polygon::Polygon(const std::vector<Point> &points, const Color &fill,const std::string &id)
    {
        for(Point p : points)
        {
            this->points.push_back(p);
        }
        this->fill = fill;
        this->id = id;
    }
    SVGElement* Polygon::copy(std::string &id)
    {
        return new Polygon(points,fill,id);
    }

    void Polygon::rotate(const Point &origin, const int &angle)
    {
        for(Point &p : points)
        {
           p = p.rotate(origin,angle);
        }
    }
    void Polygon::scale(const Point &origin,const int &factor)
    {
        for(Point &p : points)
        {
            p = p.scale(origin,factor);
        }
    }
    void Polygon::translate(const Point &p)
    {
        for(Point &p1 : points)
        {
            p1 = p1.translate(p);
        }
    }

    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points,fill);
    }
    Rect::Rect(const std::vector<Point> &points, const Color &fill,const std::string &id) : Polygon(points,fill,id)
    {   
    }



    void Group::add_element(SVGElement* element)
    {
        elements.push_back(element);
    }

    void Group::draw(PNGImage &img) const
    {
        for(SVGElement* element : elements)
        {
            element->draw(img);
        }
    }
    void Group::rotate(const Point &origin, const int &angle)
    {
        for(SVGElement* element : elements)
        {
            element->rotate(origin,angle);
        }
    }

    void Group::scale(const Point &origin,const int &factor)
    {
        for(SVGElement* element : elements)
        {
            element->scale(origin,factor);
        }
    }
    void Group::translate(const Point &p)
    {
        for(SVGElement* element : elements)
        {
            element->translate(p);
        }
    }
    Group::Group(tinyxml2::XMLElement *Root,const std::string &id)
    {
        for(tinyxml2::XMLElement *child = Root->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
        {
            std::string id1 = "";
            if(child->Attribute("id"))
            {
                id1 = child->Attribute("id");
            }
            std::string child_name = child->Name();
            SVGElement *new_element;
           /* if(child_name == "use")
            {
                std::string copy_id = child->Attribute("href");
                for(SVGElement* element : svg_elements)
                {
                    if(copy_id.substr(1) == element->get_id())
                    new_element = element->copy(id1);
                }
            }*/
            if(child_name == "g")
            {
                new_element = new Group(child,id1);
            }
            if(child_name == "ellipse")
            {
                Point center = {child->IntAttribute("cx"),child->IntAttribute("cy")};
                Point radius = {child->IntAttribute("rx"),child->IntAttribute("ry")};
                Color fill = parse_color(child->Attribute("fill"));
                new_element = new Ellipse(fill,center,radius,id1);
            }
            if(child_name == "circle")
            {
                Point center = {child->IntAttribute("cx"),child->IntAttribute("cy")};
                int radius = {child->IntAttribute("r")};
                Color fill = parse_color(child->Attribute("fill"));
                new_element = new Circle(fill,center,radius,id1);
            }
            if(child_name == "polyline")
            {
                Color stroke = parse_color(child->Attribute("stroke"));
                std::istringstream iss(child->Attribute("points"));
                int x,y = 0;
                char skip;
                std::vector<Point> points = {};
                while(iss >> x >> skip >> y){
                    points.push_back({x,y});
                }
                new_element = new Polyline(points,stroke,id1);
            } 
            if(child_name == "line")
            {
                Color stroke = parse_color(child->Attribute("stroke"));
                Point start = {child->IntAttribute("x1"),child->IntAttribute("y1")};
                Point end = {child->IntAttribute("x2"),child->IntAttribute("y2")};
                new_element = new Line(start,end,stroke,id1);
            }
            if(child_name == "polygon")
            {
                Color fill = parse_color(child->Attribute("fill"));
                std::istringstream iss(child->Attribute("points"));
                int x,y = 0;
                char skip;
                std::vector<Point> points = {};
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
                new_element = new Polygon(points,fill,id1);
            }
            if(child_name == "rect")
            {
                Color fill = parse_color(child->Attribute("fill"));
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                Point upper = {child->IntAttribute("x"),child->IntAttribute("y")};

                std::vector<Point> points =  {upper,
                                             {upper.x + width - 1,upper.y},
                                             {upper.x + width - 1,upper.y + height - 1},
                                             {upper.x,upper.y + height - 1}};              
                new_element = new Rect(points,fill,id1);
            }

            if(child->Attribute("transform"))
            {
                Point origin = {0,0};
                if(child->Attribute("transform-origin"))
                {
                    std::istringstream iss(child->Attribute("transform-origin"));
                    int x,y;
                    iss >> x >> y;
                    origin = {x,y};
                }
                std::string transform = child->Attribute("transform");
                std::string opp = transform.substr(0,transform.find('('));
                if(opp == "rotate")
                {
                    std::istringstream valstr(transform.substr(transform.find('(')+1,5));
                    int value = 0; valstr >> value;
                    new_element->rotate(origin,value);
                }
                if(opp == "scale")
                {
                    std::istringstream valstr(transform.substr(transform.find('(')+1,5));
                    int value = 0; valstr >> value;
                    new_element->scale(origin,value);
                }
                if(opp == "translate")
                {
                    std::istringstream valstr(transform.substr(transform.find('(')+1,12));
                    int x,y = 0; valstr >> x >> y;
                    if(valstr.fail())
                    {
                        valstr.clear(); valstr.ignore(1);
                        valstr >> y;
                    }
                    new_element->translate({x,y});
                }
            }
            elements.push_back(new_element);
        }
        this->id = id;
    }
    Group::Group(std::vector<SVGElement*> elements,std::string &id)
    {
        for(SVGElement* element : elements)
        {
            this->elements.push_back(element);
        }
        this->id = id;
    }
    SVGElement* Group::copy(std::string &id)
    {
        std::vector<SVGElement*> elements;
        for(SVGElement* element : this->elements)
        {
            elements.push_back(element->copy(id));
        }
        return new Group(elements,id);
    }
    Group::~Group()
    {
        for(SVGElement* element : elements)
        {
            delete element;
        }
    }
}