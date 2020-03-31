#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    for(int i = 0; i < tileCountX; i++){
        ofFill();
        ofColor colour;
        int index = i;
        cout << index << endl;
        colour.setHsb(hueValues[i], saturationValues[i], brightnessValues[i]);
        ofSetColor(colour);
        ofDrawRectangle(i * 20, 20, 20, 100);
        
        index = (i + 1) % tileCountX;
        cout << index << endl;
        colour.setHsb(hueValues[index], saturationValues[index], brightnessValues[index]);
        ofSetColor(colour);
        ofDrawRectangle(i * 20, 120, 20, 150);
        
        index = (i + 2) % tileCountX;
        colour.setHsb(hueValues[index], saturationValues[index], brightnessValues[index]);
        ofSetColor(colour);
        ofDrawRectangle(i * 20, 270, 20, 100);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1'){
        for(int i = 0; i < tileCountX; i++){
            hueValues[i] = rand() % 360;
            saturationValues[i] = rand() % 100;
            brightnessValues[i] = rand() % 100;
        }
    } if (key == '2'){
        for(int i = 0; i < tileCountX; i++){
            hueValues[i] = rand() % 360;
            saturationValues[i] = rand() % 100;
            brightnessValues[i] = 100;
        }
    } if (key == '3'){
        for(int i = 0; i < tileCountX; i++){
            hueValues[i] = rand() % 360;
            saturationValues[i] = 100;
            brightnessValues[i] = rand() % 100;
        }
    } if (key == '7'){
        for(int i = 0; i < tileCountX; i++){
            hueValues[i] = rand() % 100;
            saturationValues[i] = (rand() % 100) + 80;
            brightnessValues[i] = (rand() % 90) + 50;
        } 
    } if (key == '9'){
        for(int i = 0; i < tileCountX; i++){
            if (i % 2 == 0){
                hueValues[i] = rand() % 360;
                saturationValues[i] = 100;
                brightnessValues[i] = rand() % 100;
            } else{
                hueValues[i] = 195;
                saturationValues[i] = rand() % 100;
                brightnessValues[i] = 100;
            }
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
