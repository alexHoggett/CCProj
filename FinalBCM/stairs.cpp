#include "stairs.hpp"
Stairs::Stairs(){
    drawing = false;
    frame = 0;
    totalFrames = 0;
    totalLines = 0;
}

void Stairs::run(){
    if (drawing){
        // which line is being drawn
        float line = frame / (totalFrames / totalLines);
        line++; // will now correspond to a value starting from 1 - totalLines
        
        // cout << "line: " << line << endl;
        // cout << "total Lines: " << totalLines << endl;
        
        // find start point for this line
        int x = 0, y = 0;
        float amt = (float)line / (float)totalLines;
        x = ofLerp(start.x, end.x, amt);
        float yNoise = (ofNoise(line*7*0.1) - 0.5) * 250;
        y = ofLerp(start.y, end.y, amt); // + yNoise;
        
        // how far along each line to draw
        amt = frame % (totalFrames / totalLines);
        amt = amt / (totalFrames / totalLines); // 0 - 1
        
        float mag = (ofNoise(line * 0.1) - 0.5) * 500;
        
        // draw the point
        ofVec2f drawPoint = polarToCart(orient * PI, mag * amt);
        x += drawPoint.x;
        y += drawPoint.y;
        
        draw(x, y);
        
        frame++;
    }
    if (frame == totalFrames || frame / (totalFrames / totalLines) == totalLines){
        drawing = false;
    }
}

void Stairs::add(xyPoint startPoint, xyPoint endPoint, int frames, int noLines, float givenOrient, ofColor colour_){
    start = startPoint;
    end = endPoint;
    frame = 0;
    totalFrames = frames/2;
    totalLines = noLines;
    drawing = true;
    orient = givenOrient;
    colour = colour_;
}

void Stairs::clearAll(){
    drawing = false;
}

void Stairs::draw(int x, int y){
    colour.setBrightness(200);
    ofSetColor(colour, 60);
    ofDrawEllipse(x, y, 4, 4);
}

ofVec2f Stairs::polarToCart(float angle, int radius){
    // converts polar co-ordinates to cartesian
    ofVec2f cartesian;
    
    cartesian.x = radius * cos(angle);
    cartesian.y = radius * sin(angle);
    
    return cartesian;
}

ofVec2f Stairs::cartToPolar(float x, float y){
    // converts cartesian points to polar
    // storing angle in x & magnitude in y
    ofVec2f polar;
    
    // As y is taken from the top of the screen it must be inverted
    y *= -1;
    
    polar.x = atan(y / x);
    
    if (x < 0) {
        polar.x += PI / 2;
    } else {
        polar.x += 3 * PI / 2;
    }
    
    polar.y = sqrt((x * x) + (y * y));
    return polar;
}
