#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    refresh = false;
    ofBackground(0);
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(refresh);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int maxRadius = 50;  // Increase for a wider brush
    int radiusStepSize = 5;  // Decrease for more circles (i.e. a more opaque brush)
    int alpha = 3;  // Increase for a more opaque brush
    int maxOffsetDistance = 100;  // Increase for a larger spread of circles
    // draw smaller and smaller circles and layering (increasing) opaqueness
    for (int radius=maxRadius; radius>0; radius-=radiusStepSize) {
        float angle = ofRandom(ofDegToRad(360.0));
        float distance = ofRandom(maxOffsetDistance);
        float xOffset = cos(angle) * distance;
        float yOffset = sin(angle) * distance;
        
        ofColor myOrange(255, 132, 0, alpha);
        ofColor myRed(255, 6, 0, alpha);
        ofColor inBetween = myOrange.getLerped(myRed, ofRandom(1.0));
        ofSetColor(inBetween);
        
        ofDrawCircle(ofGetMouseX()+xOffset, ofGetMouseY()+yOffset, radius);
    }
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
