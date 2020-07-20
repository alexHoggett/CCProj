#include "largeBrush.hpp"

void LargeBrush::draw(int x, int y, ofColor colour){
    colour.setBrightness(200);
    ofSetColor(colour, 5);
    ofDrawEllipse(x, y, ofRandom(300, 400), ofRandom(300, 400));
}
