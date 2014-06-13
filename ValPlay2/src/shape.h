#ifndef shape_h
#define shape_h

#include "ofMain.h"

class Shape {
public:
    // Constructor
    Shape();
    
    // Methods
    void moveTo();
    void draw();
    
    // Properties
    int x;
    int y;
   
};
#endif