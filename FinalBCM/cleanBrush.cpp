#include "cleanBrush.hpp"

void LineGen::begin(){
    // create line to erase canvas
    ofColor c;
    c.set(0);
    // add 3 lines for top middle bottom
    // top
    this->addLine({-50, 0}, {ofGetWidth(), 0}, {0, 0}, {ofGetWidth(), 0}, 200, true, 0, c);
    // middle
    this->addLine({-300, ofGetHeight() / 2}, {ofGetWidth() + 300, ofGetHeight() / 2}, {(int)(ofGetWidth() * 0.2), ofGetHeight() / 2}, {(int)(ofGetWidth() * 0.7), ofGetHeight() / 2}, 200, true, 0, c);
    // bottom
    this->addLine({-50, ofGetHeight()}, {ofGetWidth(), ofGetHeight()}, {0, ofGetHeight()}, {ofGetWidth(), ofGetWidth()}, 200, true, 0, c);
}

void CleanBrush::draw(int x, int y, ofColor colour){
    int radius = 600;  // Increase for a wider brush
    ofSetColor(colour, 200);
    ofDrawCircle(x, y, radius);
}
