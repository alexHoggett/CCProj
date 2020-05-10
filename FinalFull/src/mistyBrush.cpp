#include "mistyBrush.hpp"

void MistyBrush::draw(int x, int y){
    int maxRadius = 30;  // Increase for a wider brush
    int radiusStepSize = 5;  // Decrease for more circles (i.e. a more opaque brush)
    int alpha = 3;  // Increase for a more opaque brush
    int maxOffsetDistance = 60;  // Increase for a larger spread of circles
    // draw smaller and smaller circles and layering (increasing) opaqueness
    for (int radius=maxRadius; radius>0; radius-=radiusStepSize) {
        float angle = ofRandom(ofDegToRad(360.0));
        float distance = ofRandom(maxOffsetDistance);
        float xOffset = cos(angle) * distance;
        float yOffset = sin(angle) * distance;
        ofSetColor(255, alpha);
        ofDrawCircle(x+xOffset, y+yOffset, radius);
    }
}
