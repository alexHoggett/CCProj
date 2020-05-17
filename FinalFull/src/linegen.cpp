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

void LineGen::killLine(int &index){
    // remove a dead line from the vector
    this->lines.erase(lines.begin() + (index - 1));
}

void LineGen::squiggleLine(int &index){
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
        float currentPoint = (float)currentLine->currentFrame / (float)currentLine->totalFrames;
        int currentX = ofInterpolateCatmullRom(currentLine->control1.x, currentLine->start.x, currentLine->end.x, currentLine->control2.x, currentPoint);
        int currentY = ofInterpolateCatmullRom(currentLine->control1.y, currentLine->start.y, currentLine->end.y, currentLine->control2.y, currentPoint);
        // add perlin noise in the X axis to slightly offset the line but with continuity
        if (currentLine->currentFrame >= 5){
            float noiseX = 0;
            float noiseY = 0;
            
            if (!currentLine->squiggle){
                // draw as normal
                noiseX = ofNoise(currentX * 0.01, currentY * 0.01) * 2. - 1.;
                currentX += noiseX * 0.1;
            } else {
                // line is 'squiggly' so add more noise on a given axis
                // orient is 0 - 1, 0 is vertical, 1 is horizontal
                noiseX = ofNoise(currentX * 0.01, currentY * 0.01) * 2. - 1.;
                noiseX *= currentLine->orient;
                currentX += noiseX * 100;
                
                noiseY = ofNoise(currentX * 0.01, currentY * 0.01) * 2. - 1.;
                noiseY *= 1 - currentLine->orient;
                currentY += noiseY * 100;
            }
        }
        
        ofSetColor(currentLine->colour);
        cout << currentLine->colour << endl;
        
        // stored current position so that we can change direction from last drawn point
        this->lines[i]->currentPos = {currentX, currentY};
        
        // draw at currernt point
        this->draw(currentX, currentY);
        // upadte the frame number
        this->lines[i]->currentFrame++;
        
        // delete if at their final frame
        if (this->lines[i]->currentFrame >= this->lines[i]->totalFrames){
            this->killLine(i);
        }
    }
}

void LineGen::draw(int x, int y){
    // standard draw function, can be overwritten by inherited classes
    ofDrawEllipse(x, y, 5, 5);
}

void LineGen::changeLine(int index, xyPoint end, xyPoint control1, xyPoint control2){
    //change to:
    this->lines[index]->start = this->lines[index]->currentPos;
    this->lines[index]->end = end;
    this->lines[index]->control1 = control1;
    this->lines[index]->control2 = control2;
    this->lines[index]->currentFrame = 0;
}
