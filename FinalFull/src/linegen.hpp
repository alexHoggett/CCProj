//
//  linegen.hpp
//  DrawingExamples
//
//  Created by Alex Hoggett on 08/05/2020.

#ifndef linegen_hpp
#define linegen_hpp

#include <stdio.h>
#include "ofMain.h"

// to specify points in 2d space
struct xyPoint {
    int x;
    int y;
};

class LineGen{
    // properties contained for each line
    struct line{
        xyPoint start;
        xyPoint end;
        xyPoint control1;
        xyPoint control2;
        xyPoint currentPos;
        int currentFrame;
        int totalFrames;
        bool squiggle; // is this line
        float orient; // 0 - 1
        ofColor colour;
    };
    
    public:
        LineGen();
        // add a single line to be drawn
        void addLine(xyPoint start, xyPoint end, xyPoint control1, xyPoint control2, int totalFrames, bool squiggle, float orient, ofColor colour);
        // to be ran every frame, calculates the next point to be drawn for every line in the vector array
        void run();
        void changeLine(int index, xyPoint end, xyPoint control1, xyPoint control2);
        void clearAll();
        void squiggleLine(int index);
    
    private:
        // to store all the lines
        vector <line*> lines;
        // to delete them from the vector
        void killLine(int &index);
        // allow draw to be overridden in inherited classed to allow for different 'drawing patterns'
        virtual void draw(int x, int y);
};

#endif /* linegen_hpp */
