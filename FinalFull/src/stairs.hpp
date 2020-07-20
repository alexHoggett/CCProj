#ifndef stairs_hpp
#define stairs_hpp

#include <stdio.h>
#include "linegen.hpp"

class Stairs{
    // only draw one set of stairs at a time
    public:
        Stairs();
        void run();
        void add(xyPoint startPoint, xyPoint endPoint, int frames, int totalLines, float orient, ofColor colour_);
        void clearAll();
        bool drawing;
    private:
        void draw(int x, int y);
        ofVec2f polarToCart(float angle, int radius);
        ofVec2f cartToPolar(float x, float y);
        xyPoint start;
        xyPoint end;
        int frame;
        int totalFrames;
        int totalLines;
        float orient; // orientation, 0 - 1
        ofColor colour;
};

#endif /* stairs_hpp */
