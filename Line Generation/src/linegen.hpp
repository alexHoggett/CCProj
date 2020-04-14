//
//  linegen.hpp
//  DrawingExamples
//
//  Created by Alex Hoggett on 08/05/2020.

#ifndef linegen_hpp
#define linegen_hpp

#include <stdio.h>
#include "ofMain.h"

class LineGen{
    struct xyPoint {
        int x;
        int y;
    };
    
    struct line{
        xyPoint start;
        xyPoint end;
        xyPoint control1;
        xyPoint control2;
        int currentFrame;
        int totalFrames;
    };
    
    public:
        LineGen();
        void addLine(xyPoint start, xyPoint end, xyPoint control1, xyPoint control2, int totalFrames);
        void run();
    
    private:
        vector <line*> lines;
        void killLine(int index);
        // settings for brush effect
        int maxRadius = 100;  // Increase for a wider brush
        int radiusStepSize = 3;  // Decrease for more circles (i.e. a more opaque brush)
        int alpha = 3;  // Increase for a more opaque brush
        int maxOffsetDistance = 100;  // Increase for a larger spread of circles
        // draw smaller and smaller circles and layering (increasing) opaqueness
};

#endif /* linegen_hpp */
