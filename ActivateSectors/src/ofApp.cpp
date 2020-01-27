#include "ofApp.h"

ofVec2f ofApp::whichBlock(float x, float y){
    ofVec2f correctBlock;
    int blockWidth = width / columns;
    int blockHeight = height / rows;
    
    for (int i = 0; i < columns; i++){
        for (int j = 0; j < rows; j++){
            if (x >= blockWidth * i && x < blockWidth * (i + 1) && y >= blockHeight * j && y < blockHeight * (j + 1)){
                correctBlock.x = i;
                correctBlock.y = j;
            }
        }
    }
    return correctBlock;
}

//--------------------------------------------------------------
int ofApp::whichSectorIndex(float x, float y, int radius){
    // given a point, this function can deduce which sector of the circle the point lies
    // return the index position of that sector (for angles[] array)
    
    int correctSector = 0;
    
    // get polar co-ordinates
    ofVec2f polar = cartToPolar(x - width/2, y - height/2);
    float angle = polar.x;
    float mag = polar.y;
    
    angle = 2 * PI - angle;
    
    for (int i = 0; i < 12; i++){
        float endAngle = angles[i];
        float startAngle = angles[i] - (PI/6);
        
        if (angle < endAngle && angle > startAngle && mag < radius){
            correctSector = i;
        }
    }
    
    return correctSector;
}

//--------------------------------------------------------------
ofVec2f ofApp::polarToCart(float angle, int radius){
    // converts polar co-ordinates to cartesian
    ofVec2f cartesian;
    
    cartesian.x = radius * cos(angle);
    cartesian.y = radius * sin(angle);
    
    return cartesian;
}

//--------------------------------------------------------------
ofVec2f ofApp::cartToPolar(float x, float y){
    // converts cartesian points to polar
    // storing angle in x & magnitude in y
    ofVec2f polar;
    
    // As y is taken from the top of the screen it must be inverted
    y *= -1;
    
    polar.x = atan(y/x);
    
    if (x < 0) {
        polar.x += PI/2;
    } else {
        polar.x += 3 * PI / 2;
    }
    
    polar.y = sqrt((x*x) + (y*y));
    return polar;
}

//--------------------------------------------------------------
void ofApp::setup(){
    x = y = 0;
    
    columns = sizeof(blocks) / sizeof(blocks[0]);
    rows = sizeof(blocks[0]) / sizeof(blocks[0][0]);
}

//--------------------------------------------------------------
void ofApp::update(){
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofNoFill();
    ofSetColor(0, 255, 0);
    
    int radius = 700;
    
    ofDrawCircle(width/2, height/2, radius);
    
    // draw sectors
    for (int i = 0; i < 12; i++){
        ofSetColor(0, 255, 0);
        ofVec2f edge = polarToCart(angles[i] - PI/2, radius);
        
        // x & y values from the centre of circle
        float x = edge.x + width/2;
        float y = edge.y + height/2;
    
        ofDrawLine(width/2, height/2, x, y);
    }
    
    ofVec2f mouse = whichBlock(ofGetMouseX(), ofGetMouseY());
    
    // draw rects
    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 12; j++){
            if (mouse.x == i && mouse.y == j){
                ofFill();
            } else {
                ofNoFill();
            }
            ofDrawRectangle(width/16 * i, height/12 * j, width/16, height/12);
        }
    }
    
    ofDrawEllipse(ofGetMouseX(), ofGetMouseY(), 5, 5);
    
    cout << "Sector: " << whichSectorIndex(ofGetMouseX(), ofGetMouseY(), radius) << endl;
    cout << whichBlock(ofGetMouseX(), ofGetMouseY()).x << " " << whichBlock(ofGetMouseX(), ofGetMouseY()).y << endl;
    
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
