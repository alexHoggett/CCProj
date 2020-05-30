#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    settingPos = false;
    refresh = true;
    ofBackground(0);
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(refresh);
    lineGen = new LineGen();
    mistyGen = new MistyBrush();

    if(ofIsGLProgrammableRenderer()){
        gaussianBlurX.load("shadersGL3/shaderBlurX");
        gaussianBlurY.load("shadersGL3/shaderBlurY");
    }else{
        gaussianBlurX.load("shadersGL2/shaderBlurX");
        gaussianBlurY.load("shadersGL2/shaderBlurY");
    }
    
    fboBlurOnePass.allocate(ofGetWidth(), ofGetHeight());
    fboBlurTwoPass.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor(255);
    ofDrawBitmapString("Press '1' to generate misty lines", 10, 12);
    ofDrawBitmapString("Press '2' for standard brush", 10, 25);
    ofDrawBitmapString("Press space to toggle refresh", 10, 38);
    
    lineGen->run();
    mistyGen->run();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofColor c;
    if (key == ' ' && !refresh){
        refresh = true;
        ofSetBackgroundAuto(refresh);
    } else if (key == ' ' && refresh){
        refresh = false;
        ofSetBackgroundAuto(refresh);
    } else if (key == '1'){
        for(int i = 0; i < 10; i++){
            c.set(ofRandom(255), ofRandom(255), ofRandom(255));
            mistyGen->addLine({(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, 1000, true, 0.0, c);
        }
    } else if (key == '2'){
        for(int i = 0; i < 1; i++){
            c.set(ofRandom(255), ofRandom(255), ofRandom(255));
            lineGen->addLine({(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, 1000, true, 1.0, c);
        } 
    } else if (key == '3'){
        c.set(ofRandom(255), ofRandom(255), ofRandom(255));
        lineGen->addLine({(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, 1000, true, 0.5, c);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (!settingPos){
        firstPos = {x, y};
        settingPos = true;
    } else{
        ofColor c;
        c.set(ofRandom(255), ofRandom(255), ofRandom(255));
        lineGen->addCrescent(firstPos, {x, y}, c);
        settingPos = false;
    }
}
