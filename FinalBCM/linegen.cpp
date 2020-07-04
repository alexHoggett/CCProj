//
//  linegen.cpp
//  DrawingExamples
//
//  Created by Alex Hoggett on 08/05/2020.
//

#include "linegen.hpp"
LineGen::LineGen(){
    // empty constructor
}

void LineGen::addLine(xyPoint start, xyPoint end, xyPoint control1, xyPoint control2, int totalFrames, bool squiggle, float orient, ofColor colour){
    // add a new line to the vector array
    line * newLine = new line;
    newLine->start = start;
    newLine->end = end;
    newLine->control1 = control1;
    newLine->control2 = control2;
    newLine->currentFrame = 0;
    newLine->totalFrames = totalFrames;
    newLine->squiggle = squiggle;
    newLine->orient = orient;
    newLine->colour = colour;
    this->lines.push_back(newLine);
}

void LineGen::addCrescent(xyPoint start, xyPoint end, ofColor colour){
    // Is composed of 2 curved lines
    // with points drawn inbetween
    cresc * newCresc = new cresc;
    newCresc->start = start;
    newCresc->end = end;
    
    newCresc->control1 = {start.x + (int)ofRandom(-1000, 1000), (int)ofRandom(start.y, end.y)};
    newCresc->control2 = {start.x + (int)ofRandom(-1000, 1000), (int)ofRandom(start.y, end.y)};
    
    newCresc->control3 = {end.x + (int)ofRandom(-1000, 1000), (int)ofRandom(start.y, end.y)};
    newCresc->control4 = {end.x + (int)ofRandom(-1000, 1000), (int)ofRandom(start.y, end.y)};
    
    newCresc->currentFrame = 0;
    newCresc->totalFrames = ofDist(start.x, start.y, end.x, end.y);
    newCresc->colour = colour;
    
    this->crescents.push_back(newCresc);
}

void LineGen::killLine(int &index){
    // remove a dead line from the vector
    this->lines.erase(lines.begin() + (index - 1));
}

void LineGen::killCresc(int &index){
    // remove a line from the vector
    this->crescents.erase(crescents.begin() + (index - 1));
}

void LineGen::squiggleLine(int index){
    // the line can be made 'squiggly'
    this->lines[index]->squiggle = true;
}

void LineGen::clearAll(){
    this->lines.clear();
}

void LineGen::run(){
    // manages each line
    for(int i = 0; i < this->lines.size(); i++){
        ofFill();
        line * currentLine = this->lines[i];
        // calculate where should be drawn for each line in its given frame
        xyPoint currentPoint = calcLinePoint(currentLine->start, currentLine->end, currentLine->control1, currentLine->control2, currentLine->totalFrames, currentLine->currentFrame);
        // add perlin noise in the X axis to slightly offset the line but with continuity
        if (currentLine->currentFrame >= 0){
            float noiseX = 0;
            float noiseY = 0;
            
            if (!currentLine->squiggle){
                // draw as normal
                noiseX = ofNoise(currentPoint.x * 0.01, currentPoint.y * 0.01) * 2. - 1.;
                currentPoint.x += noiseX * 0.1;
            } else {
                // line is 'squiggly' so add more noise on a given axis
                // orient is 0 - 1, 0 is vertical, 1 is horizontal
                noiseX = ofNoise(currentPoint.x * 0.01, currentPoint.y * 0.01) * 2. - 1.;
                noiseX *= currentLine->orient;
                currentPoint.x += noiseX * 100;
                
                noiseY = ofNoise(currentPoint.x * 0.01, currentPoint.y * 0.01) * 2. - 1.;
                noiseY *= 1 - currentLine->orient;
                currentPoint.y += noiseY * 100;
            }
        }
        
        ofSetColor(currentLine->colour);
        
        // stored current position so that we can change direction from last drawn point
        this->lines[i]->currentPos = {currentPoint.x, currentPoint.y};
        
        // draw at currernt point
        this->draw(currentPoint.x, currentPoint.y);
        // upadte the frame number
        this->lines[i]->currentFrame++;
        
        // delete if at their final frame
        if (this->lines[i]->currentFrame >= this->lines[i]->totalFrames){
            this->killLine(i);
        }
    }
    
    // manages drawing crescents
    for (int i = 0; i < this->crescents.size(); i++){
        cresc * currentCrescent = this->crescents[i];
        
        // calc point for line 1
        xyPoint currentPoint1;
        currentPoint1 = calcLinePoint(currentCrescent->start, currentCrescent->end, currentCrescent->control1, currentCrescent->control2, currentCrescent->totalFrames, currentCrescent->currentFrame);
        
        // calc point for line 2
        xyPoint currentPoint2 = calcLinePoint(currentCrescent->start, currentCrescent->end, currentCrescent->control3, currentCrescent->control4, currentCrescent->totalFrames, currentCrescent->currentFrame);
        
        // draw between these 2 points
        float dist = ofDist(currentPoint1.x, currentPoint1.y, currentPoint2.x, currentPoint2.y);
        for (int i = 0; i < dist; i++){
            float x, y;
            x = ofLerp(currentPoint1.x, currentPoint2.x, (float)i / dist);
            y = ofLerp(currentPoint1.y, currentPoint2.y, (float)i / dist);
            ofSetColor(currentCrescent->colour);
            // adding a bit of noise so less straight when drawn
            
            ofDrawEllipse(x + ofRandom(-1, 1), y + ofRandom(-1, 1), 5, 5);
        }
        
        this->crescents[i]->currentFrame++;
        
        if (this->crescents[i]->currentFrame >= this->crescents[i]->totalFrames){
            this->killCresc(i);
        }
    }
    
}

void LineGen::draw(int x, int y){
    // standard draw function, can be overwritten by inherited classes
    ofDrawEllipse(x, y, 10, 10);
}

xyPoint LineGen::calcLinePoint(xyPoint start, xyPoint end, xyPoint control1, xyPoint control2, int totalFrames, int currentFrame){
    // calculate where should be drawn for each line in its given frame
    float currentPoint = (float)currentFrame / (float)totalFrames;
    int currentX = ofInterpolateCatmullRom(control1.x, start.x, end.x, control2.x, currentPoint);
    int currentY = ofInterpolateCatmullRom(control1.y, start.y, end.y, control2.y, currentPoint);
    
    return {currentX, currentY};
}

void LineGen::changeLine(int index, xyPoint end, xyPoint control1, xyPoint control2){
    //change to:
    this->lines[index]->start = this->lines[index]->currentPos;
    this->lines[index]->end = end;
    this->lines[index]->control1 = control1;
    this->lines[index]->control2 = control2;
    this->lines[index]->currentFrame = 0;
}

void LineGen::increasing(int index){
    // there is an increase in pitch, so adjust line accordingly
    xyPoint currentXY = this->lines[index]->currentPos;
    xyPoint control1 = this->lines[index]->control1;
    xyPoint control2 = this->lines[index]->control2;
    this->changeLine(index, {(int)ofRandom(ofGetWidth()), (int)ofRandom(currentXY.y)}, control1, control2);
}

void LineGen::decreasing(int index){
    // there is a decrease in pitch, so adjust line accordingly
    xyPoint currentXY = this->lines[index]->currentPos;
    xyPoint control1 = this->lines[index]->control1;
    xyPoint control2 = this->lines[index]->control2;
    this->changeLine(index, {(int)ofRandom(ofGetWidth()), (int)ofRandom(currentXY.y, ofGetHeight())}, control1, control2);
}

int LineGen::returnTotalLines(){
    // return the total number of lines currently being drawn
    return this->lines.size();
}
