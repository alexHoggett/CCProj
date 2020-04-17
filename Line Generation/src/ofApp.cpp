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
    controlPoint1 = { 600, 50 };
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
    ofColor(255);
    ofDrawBitmapString("Press '1' to generate lines", 10, 12);
    ofDrawBitmapString("Press space to disable refresh", 10, 25);
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
    } else if (key == '1'){
        for(int i = 0; i < 100; i++){
            lineGen->addLine({(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, 1000);
        }
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
