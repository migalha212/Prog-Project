
# Programming project

## Group elements


- up202304387 Miguel Borges Pereira
- up202306714 Pedro Manuel Pinto Coelho
- up202309183 Mykyta Melnykovych


## Accomplished tasks

### Created a class for each shape in the hierarchy of SVG
- We made circle a subclass of ellipse as it is a specific case of an ellipse which happens to have x and y values of it's radius be equal 
- Line is a subclass of Polyline since it is as well just a case of Polyline where there are only 2 points 
- Rect is a subclass of Polygon as it is a specific case of polygon where there are always only 4 points aligned in the x and y axis.
### Implemented the reading logic for every shape using XmlElement's member functions 
For the reading and creating of groups we felt it was necessary to make a class for them so we could use recursive functions. Then by having groups also have the whole of the reading logic for every element we are also able to allow the creation of groups within groups, even considering treating the root element of the file as a group as well as to not repeat the reading logic code but eventually scrapping the idea.
### Created functions to transform the shapes
Transform was made by using 3 member functions that would just call the built-in functions of Point and by using stringstreams we are able to parse the information about operations easily.
### Created a function for 'use' elements
Use was made an auxiliary function as we did not feel the need to make it it's own class which would just have a pointer to the copy of the element as it's private field. In order to allow 'use' to work even inside the groups we declared a map that keeps track of all IDs and pointers to their respective shape so as to copy them over with ease. It uses the desired ID as a key and then uses the value to call the 'copy()' member function
