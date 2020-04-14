//
//  linegen.cpp
//  DrawingExamples
//
//  Created by Alex Hoggett on 08/05/2020.
//

#include "linegen.hpp"
LineGen::LineGen(){
    
}

void LineGen::addLine(xyPoint start, xyPoint end, xyPoint control1, xyPoint control2, int totalFrames){
    line * newLine = new line;
    newLine->start = start;
    newLine->end = end;
    newLine->control1 = control1;
    newLine->control2 = control2;
    newLine->currentFrame = 0;
    newLine->totalFrames = totalFrames;
    this->lines.push_back(newLine);
}

void LineGen::killLine(int index){
    this->lines.erase(lines.begin() + (index - 1));
}

void LineGen::run(){
    for(int i = 0; i < lines.size(); i++){
        ofFill();
        line * currentLine = this->lines[i];
        float currentPoint = currentLine->currentFrame / currentLine->totalFrames;
        int currentX = ofInterpolateCatmullRom(currentLine->control1.x, currentLine->start.x, currentLine->end.x, currentLine->control2.x, currentPoint);
        int currentY = ofInterpolateCatmullRom(currentLine->control1.y, currentLine->start.y, currentLine->end.y, currentLine->control2.y, currentPoint);
        float noiseX = ofNoise(currentX, currentY)*2. - 1.;
        // float noiseY = ofNoise(currentY)*2. - 1.;
        currentX += noiseX * 5; // change the int to be at a ratio of the total number of frames
        // currentY += noiseY*100;
        ofSetColor(255, 0, 0);
        
        cout << currentX << " " << currentY << endl;
        ofDrawEllipse(currentX, currentY, 10, 10);
        
        this->lines[i]->currentFrame++;
        
        if (this->lines[i]->currentFrame >= this->lines[i]->totalFrames){
            this->killLine(i);
        }
    }
}
