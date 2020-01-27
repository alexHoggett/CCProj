#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    myImage.load("image.jpg");
    pix = myImage.getPixels();
    
    height = pix.getHeight();
    width = pix.getWidth();
    numChannels = pix.getNumChannels();

    for (int i = 0; i < width * height * numChannels; i++){
        float norm = ((float)(pix[i])) / 255;
        norm = (norm * 2) - 1;
        pixelNoise.push_back(norm);
    }
    
    float redBin = 0;
    float greenBin = 0;
    float blueBin = 0;
    
    // sum the values of the RGB channels for every pixel
    for(int i = 0; i < height * width * numChannels; i += numChannels){
        redBin += pix[i + 0];
        greenBin += pix[i + 1];
        blueBin += pix[i + 2];
    }
    
    // normalize the colour values
    float max = height * width * 255;
    redBin /= max;
    greenBin /= max;
    blueBin /= max;
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
