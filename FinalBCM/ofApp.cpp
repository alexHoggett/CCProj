#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp(){
    
}

//--------------------------------------------------------------
void ofApp::setup(){
    // Audio set-up
    sampleRate = 64;
    initialBufferSize = 1;
    
    fftSize = 64;
    myFFT.setup(fftSize, 32, 60);
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
    // Initialising values
    triggerFFT = false;
    drawing = false;
    snippetBufferOffset = 0;
    playingBufferOffset = 0;
    columns = sizeof(blocks) / sizeof(blocks[0]);
    rows = sizeof(blocks[0]) / sizeof(blocks[0][0]);
    ofSetBackgroundAuto(false);
    ofSetBackgroundColor(0, 0, 0);
    
    for(int i = 0; i < colourQuantity; i++){
        if (i % 2 == 0){
            hueValues[i] = rand() % 360;
            saturationValues[i] = 100;
            brightnessValues[i] = ofRandom(50, 100);
        } else{
            hueValues[i] = 195;
            saturationValues[i] = rand() % 100;
            brightnessValues[i] = 100;
        }
    }
    
    lineGen = new LineGen;
    snippetsCounter = 0;
    recordingCounter = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    // for testing, i will just randomly generate a number between -1 & 1
    float inputSignal = ofRandom(-1, 1);
    
    audioRecording[recordingCounter] = inputSignal;
    rmsSum += inputSignal * inputSignal;
    
    if(recordingCounter >= SNIPPET_LENGTH){
        cout << "hmm" << endl;
        RMS = sqrt(rmsSum);
        triggerFFT = true;
        recordingCounter = 0;
        rmsSum = 0;
    } else{
        recordingCounter++;
    }
    
    cout << snippets.size() << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw sectors
//    ofNoFill();
//    ofSetColor(0, 255, 0);
//    int radius = 700;
//    ofDrawCircle(width/2, height/2, radius);
//    for (int i = 0; i < 12; i++){
//        ofSetColor(0, 255, 0);
//        ofVec2f edge = polarToCart(angles[i] - PI/2, radius);
//
//        // x & y values from the centre of circle
//        float x = edge.x + width/2;
//        float y = edge.y + height/2;
//
//        ofDrawLine(width/2, height/2, x, y);
//    }

    // Draw rects
//    ofVec2f mouse = whichBlock(ofGetMouseX(), ofGetMouseY());
//    for (int i = 0; i < 16; i++){
//        for (int j = 0; j < 12; j++){
//            // Highlight box where mouse is
//            if (mouse.x == i && mouse.y == j){
//                ofFill();
//            } else {
//                ofNoFill();
//            }
//            ofDrawRectangle(width/16 * i, height/12 * j, width/16, height/12);
//        }
//    }
    
    // Draw spectrum
    float horizWidth = width;
    float horizOffset = 100;
    float fftTop = 250;
    float mfccTop = 350;
    float chromagramTop = 450;
    
    ofSetColor(255, 0, 0, 255);
    
    float rmsThresh = 0.8;
    // I use the RMS to trigger an analysis of what was just played
    cout << "RMS: " << RMS << endl;
    if (RMS > rmsThresh && !drawing && snippets.size() > 1){
        drawing = true;
    }
    
    // FFT IMPLEMENTATION
    // only run the FFT at the end of the recording snippet
    if (triggerFFT){
//        for (int i = 0; i < SNIPPET_LENGTH - initialBufferSize; i++){
//            wave = audioRecording[i]; //* blackManWin.operator()(SNIPPET_LENGTH, i);
//            // get fft
//            if (myFFT.process(wave)){
//                myFFT.magsToDB();
//                oct.calculate(myFFT.magnitudesDB);
//                float sum = 0;
//                float maxFreq = 0;
//                int maxBin = 0;
//
//                // calc centroid and bin/freq
//                for (int i = 0; i < fftSize/2; i++) {
//                    sum += myFFT.magnitudes[i];
//                    if (myFFT.magnitudes[i] > maxFreq) {
//                        maxFreq = myFFT.magnitudes[i];
//                        maxBin = i;
//                    }
//                }
//                centroid = sum / (fftSize / 2);
//                peakFreq = (float)maxBin/fftSize * 44100;
//                mfcc.mfcc(myFFT.magnitudes, mfccs);
//            }
//        }
          // cout << centroid << endl;
//        // convert the array of bins to a vector
//        vector <float> bins;
//        for (int i = 0; i < myFFT.bins; i++){
//            bins.push_back(myFFT.magnitudes[i]);
//        }
//        chordSpotter.analyse(fftSize, sampleRate, 8, bins);
//
//        string pred = chordSpotter.returnChord();
//        cout << pred << endl;
        
        snippets.push_back({ofRandom(1), ofRandom(750), RMS, drawing});
        snippetsCounter++;
        
        // if snippets vector reaches a certain size it is trimmed
        int maxLength = 10;
        if (snippets.size() > maxLength){
            snippets.erase(snippets.begin());
        }
        
        if (drawing && snippets.size() > 2){
            cout << "is it sdrawing" << snippets[snippets.size() - 2].drawing << endl;
            if (snippets[snippets.size() - 2].drawing == false){
                // start drawing a new line
                xyPoint startPoint = {};
                if (centroid < 0.1){
                    startPoint = {(int)ofRandom(width), (int)ofRandom(height / 2)};
                } else {
                    startPoint = {(int)ofRandom(width), (int)ofRandom(height / 2, height)};
                }
                
                xyPoint endPoint = {};
                if (centroid < 0.1){
                    endPoint = {(int)ofRandom(width), (int)ofRandom(height / 2)};
                } else {
                    endPoint = {(int)ofRandom(width), (int)ofRandom(height / 2, height)};
                }
                
                xyPoint control1 = {(int)ofRandom(width), (int)ofRandom(height)};
                xyPoint control2= {(int)ofRandom(width), (int)ofRandom(height)};
                
                int totalFrames = ofDist(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                bool squiggle = false;
                float orient = ofRandom(1);
                ofColor colour;
                int colourIndex = ofRandom(colourQuantity);
                colour.setHsb(ofRandom(hueValues[colourIndex]), saturationValues[colourIndex], brightnessValues[colourIndex]);
                cout << "adding" << endl;
                lineGen->addLine(startPoint, endPoint, control1, control2, totalFrames, squiggle, orient, colour);
            }
            
            if (snippets[snippets.size() - 2].drawing == true){
                // already drawing a line
                if (snippetsCounter > snippets.size() / 2){
                    // check previous snippets, i don't do this after every snippet
                    int increase = 0;
                    int decrease = 0;
                    vector <int> peakFreqs;
                    float avgCentroid = 0;
                    for (int i = 0; i < snippets.size(); i++){
                        if (i != 0){
                            if (snippets[i].centroid > snippets[i - 1].centroid){
                                decrease++;
                            } else{
                                increase++;
                            }
                        }
                        peakFreqs.push_back(snippets[i].peakFreq);
                        avgCentroid += snippets[i].centroid;
                    }
                    
                    if (increase > decrease){
                        // pitch is increasing
                        lineGen->changeLine(0, {600, 20}, {(int)ofRandom(width), (int)ofRandom(height)}, {(int)ofRandom(width), (int)ofRandom(height)});
                    } else{
                        // pitch is decreasing
                        lineGen->changeLine(0, {250, height}, {(int)ofRandom(width), (int)ofRandom(height)}, {(int)ofRandom(width), (int)ofRandom(height)});
                    }
                    
                    // calc avg centroid across snippets
                    avgCentroid /= snippets.size();
                    cout << avgCentroid << endl;
                    
                    // check if peak freq has been consistent & centroid is averaging high
                    if (occurenceCheck(peakFreqs, 4) == true && avgCentroid > 0.1){
                        cout << "gotcha" << endl;
                        lineGen->squiggleLine(0);
                        
                    }
                }
            }
            
            if (RMS < rmsThresh * 0.4){
                // stop drawing as the signal is too low
                drawing = false;
                lineGen->clearAll();
            }
        }
        
        triggerFFT = false;
    }
    
    // Draw fft output
//    float xinc = horizWidth / fftSize * 2.0;
//    int drawFreq = 0;
//    for(int i=0; i < fftSize / 2; i++) {
//        // scale the values so they're more visible
//        float height = myFFT.magnitudes[i] * 100;
//        if (i % 10 == 0){
//            ofDrawRectangle(horizOffset + (i*xinc),ofGetHeight() - height,1, height);
//        }
//    }
//
//    // Draw octave analyser
//    ofSetColor(255, 0, 255, 200);
//    xinc = horizWidth / oct.nAverages;
//    for (int i = 0; i < oct.nAverages; i++) {
//        float height = oct.averages[i] / 20.0 * 100;
//        ofDrawRectangle(horizOffset + (i * xinc), chromagramTop - height, 2, height);
//    }
    
    lineGen->run();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1'){
        for(int i = 0; i < colourQuantity; i++){
            hueValues[i] = rand() % 360;
            saturationValues[i] = rand() % 100;
            brightnessValues[i] = rand() % 100;
        }
    }
}

//--------------------------------------------------------------
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
    
    polar.x = atan(y / x);
    
    if (x < 0) {
        polar.x += PI / 2;
    } else {
        polar.x += 3 * PI / 2;
    }
    
    polar.y = sqrt((x * x) + (y * y));
    return polar;
}

//--------------------------------------------------------------
bool ofApp::occurenceCheck(vector<int> &freqs, int thresh){
    sort(freqs.begin(), freqs.end());
    int currentFreq = freqs[0];
    int currentCount = 0, maxFreq = 0, maxCount = 0;
    for (int i = 0; i < freqs.size(); i++){
        if (freqs[i] == currentFreq){
            currentCount++;
        }
        else{
            if (currentCount > maxCount){
                maxCount = currentCount;
                maxFreq = freqs[i - 1];
            }
            currentCount = 0;
            currentFreq = freqs[i];
        }
    }
    
    if (maxCount >= thresh){
       return true;
    } else{
        return false;
    }
}

