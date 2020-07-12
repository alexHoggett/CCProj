//
//  cleanBrush.cpp
//  DrawingExamples
//
//  Created by Alex Hoggett on 31/07/2020.
//

#include "cleanBrush.hpp"

void LineGen::begin(){
    // create line to erase canvas
    ofColor c;
    c.set(0);
    this->addLine({-300, ofGetHeight() / 2}, {ofGetWidth() + 300, ofGetHeight() / 2}, {(int)(ofGetWidth() * 0.2), ofGetHeight() / 2}, {(int)(ofGetWidth() * 0.7), ofGetHeight() / 2}, 200, true, 0, c);
}

void CleanBrush::draw(int x, int y){
    int maxRadius = 600;  // Increase for a wider brush
    int radiusStepSize = 2;  // Decrease for more circles (i.e. a more opaque brush)
    int alpha = 16;  // Increase for a more opaque brush
    int maxOffsetDistance = 60;  // Increase for a larger spread of circles
    // draw smaller and smaller circles and layering (increasing) opaqueness
    for (int radius=maxRadius; radius>0; radius-=radiusStepSize) {
        float angle = ofRandom(ofDegToRad(360.0));
        float distance = ofRandom(maxOffsetDistance);
        float xOffset = cos(angle) * distance;
        float yOffset = sin(angle) * distance;
        ofSetColor(0, alpha);
        ofDrawCircle(x+xOffset, y+yOffset, radius);
    }
}
