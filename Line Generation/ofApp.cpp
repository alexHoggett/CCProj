#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    refresh = true;
    ofBackground(0);
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(refresh);
    
    //initialize our points
    keyFrame1.x = 50;
    keyFrame1.y = 50;
    
    //This syntax works, too:
    keyFrame2 = { 400, 400 };
    controlPoint1 = { 300, 50 };
    controlPoint2 = { 400, 100 };
    
    lineGen = new LineGen();
    
    //start from frame 0
    frameCount = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    counter++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // settings for brush effect
    int maxRadius = 100;  // Increase for a wider brush
    int radiusStepSize = 3;  // Decrease for more circles (i.e. a more opaque brush)
    int alpha = 3;  // Increase for a more opaque brush
    int maxOffsetDistance = 100;  // Increase for a larger spread of circles
    // draw smaller and smaller circles and layering (increasing) opaqueness
    
    // draw the line & keyframes initially
    ofSetColor(0, 128, 255);
    ofFill();
    // draw key frames
    ofDrawEllipse(keyFrame1.x, keyFrame1.y, 20, 20);
    ofDrawEllipse(keyFrame2.x, keyFrame2.y, 20, 20);
    // draw control points
    ofDrawEllipse(controlPoint1.x, controlPoint1.y, 10, 10);
    ofDrawEllipse(controlPoint2.x, controlPoint2.y, 10, 10);
    // draw curve
    ofSetColor(0, 64, 128);
    ofNoFill();
    ofDrawCurve(controlPoint1.x, controlPoint1.y, keyFrame1.x, keyFrame1.y, keyFrame2.x, keyFrame2.y, controlPoint2.x, controlPoint2.y);
    
    ofFill();
    float currentPoint = frameCount * 0.01; //scale to 0. - 1.
    int currentX = ofInterpolateCatmullRom(controlPoint1.x, keyFrame1.x, keyFrame2.x, controlPoint2.x, currentPoint);
    int currentY = ofInterpolateCatmullRom(controlPoint1.y, keyFrame1.y, keyFrame2.y, controlPoint2.y, currentPoint);
    float noiseX = ofNoise(currentX*0.01, currentY*0.01)*2. - 1.;
    // float noiseY = ofNoise(currentY)*2. - 1.;
    currentX += noiseX * 5;
    //currentY += noiseY*100;
    
    ofDrawEllipse(currentX, currentY, 10, 10);
    
//    for (int radius=maxRadius; radius>0; radius-=radiusStepSize) {
//        float angle = ofRandom(ofDegToRad(360.0));
//        float distance = ofRandom(maxOffsetDistance);
//        float xOffset = cos(angle) * distance;
//        float yOffset = sin(angle) * distance;
//
//        ofColor myOrange(255, 132, 0, alpha);
//        ofColor myRed(255, 6, 0, alpha);
//        ofColor inBetween = myOrange.getLerped(myRed, ofRandom(1.0));
//        ofSetColor(inBetween);
//
//        ofDrawCircle(currentX+xOffset, currentY+yOffset, radius);
//    }
    
    //increment frameCount, and wrap around 1000
    frameCount = (frameCount + 1) % 100;
    
    if (counter == 100){
        lineGen->addLine({60, 60}, {120, 400}, {400, 70}, {500, 90}, 100);
        cout << "FIRST" << endl;
    }
    if (counter == 300){
        lineGen->addLine({120, 300}, {120, 500}, {120, 70}, {150, 90}, 100);
        cout << "SECOND" << endl;
    }
    if (counter == 600){
        lineGen->addLine({200, 200}, {100, 100}, {200, 70}, {250, 90}, 100);
        cout << "THIRD" << endl;
    }
    
    lineGen->run();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ' && !refresh){
        refresh = true;
        ofSetBackgroundAuto(refresh);
    } else if (key == ' ' && refresh){
        refresh = false;
        ofSetBackgroundAuto(refresh);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
