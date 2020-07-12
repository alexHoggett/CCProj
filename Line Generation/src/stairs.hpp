//
//  stairs.hpp
//  DrawingExamples
//
//  Created by Alex Hoggett on 01/08/2020.
//

#ifndef stairs_hpp
#define stairs_hpp

#include <stdio.h>
#include "linegen.hpp"

class Stairs{
    // only draw one set of stairs at a time
    public:
        Stairs();
        void run();
        void add(xyPoint startPoint, xyPoint endPoint, int frames, int totalLines, float orient);
        void clearAll();
    private:
        void draw(int x, int y);
        ofVec2f polarToCart(float angle, int radius);
        ofVec2f cartToPolar(float x, float y);
        void kill();
        xyPoint start;
        xyPoint end;
        int frame;
        int totalFrames;
        int totalLines;
        bool drawing;
        float orient; // orientation, 0 - 1
};

#endif /* stairs_hpp */
