#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp(){
    // destructor
}

//--------------------------------------------------------------
void ofApp::setup(){
    // Audio set-up
    sampleRate = 44100;
    initialBufferSize = 512;
    lAudioIn = new float[initialBufferSize];
    rAudioIn = new float[initialBufferSize];
    memset(lAudioIn, 0, initialBufferSize * sizeof(float));
    memset(rAudioIn, 0, initialBufferSize * sizeof(float));
    
    fftSize = 1024*4;
    myFFT.setup(fftSize, 1024*2, 512);
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    ofSoundStreamSettings settings;
    
    auto devices = soundStream.getMatchingDevices("default");
    if (!devices.empty()) {
        settings.setInDevice(devices[0]);
    }
    
    settings.setInListener(this);
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 1;
    settings.bufferSize = initialBufferSize;
    soundStream.setup(settings);
    soundStream.start();
    
    // Initialising values
    triggerFFT = false;
    drawing = false;
    snippetBufferOffset = 0;
    columns = sizeof(blocks) / sizeof(blocks[0]);
    rows = sizeof(blocks[0]) / sizeof(blocks[0][0]);
    ofSetBackgroundAuto(false);
    ofSetBackgroundColor(0, 0, 0);
    
    // randomising colour values within a range
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
    mistyGen = new MistyBrush;
    snippetsCounter = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    if(snippetBufferOffset * initialBufferSize >= SNIPPET_LENGTH - initialBufferSize){
        triggerFFT = true;
        recording = false;
        snippetBufferOffset = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 0, 0, 255);
    
    float rmsThresh = 0.8;
    // I use the RMS to trigger an analysis of what was just played
    if (RMS > rmsThresh && !drawing){
        drawing = true;
    }
    
    // FFT IMPLEMENTATION
    // only run the FFT at the end of the recording snippet
    if (triggerFFT){
        for (int i = 0; i < SNIPPET_LENGTH - initialBufferSize; i++){
            wave = snippet[i]; //* blackManWin.operator()(SNIPPET_LENGTH, i);
            // get fft
            if (myFFT.process(wave)){
                myFFT.magsToDB();
                float sum = 0;
                float maxFreq = 0;
                int maxBin = 0;
                
                // calc centroid and bin/freq
                for (int i = 0; i < fftSize/2; i++) {
                    sum += myFFT.magnitudes[i];
                    if (myFFT.magnitudes[i] > maxFreq) {
                        maxFreq = myFFT.magnitudes[i];
                        maxBin = i;
                    }
                }
                centroid = sum / (fftSize / 2);
                peakFreq = (float)maxBin/fftSize * 44100;
            }
        }
                
        // divide the sum of the rms's by the amount of buffers used to fill the snippet
        float avgRMS = rmsSum / (SNIPPET_LENGTH/initialBufferSize);
        
        snippets.push_back({centroid, peakFreq, avgRMS, drawing});
        snippetsCounter++;
        
        // if snippets vector reaches a certain size it is trimmed
        int maxLength = 10;
        if (snippets.size() > maxLength){
            snippets.erase(snippets.begin());
        }
        
        if (drawing && snippets.size() > 2){
            // send bins to chordSpotter to detect a chord
            if (peakFreq != 0){
                // convert the array of bins to a vector
                vector <float> bins;
                for (int i = 0; i < myFFT.bins; i++){
                    bins.push_back(myFFT.magnitudes[i]);
                }
                chordSpotter.analyse(fftSize, sampleRate, 8, bins);

                string pred = chordSpotter.returnChord();
                cout << pred << endl;
            }
            
            if (snippets[snippets.size() - 2].drawing == false){
                // start drawing a new line
                xyPoint startPoint = {};
                xyPoint endPoint = {};
                xyPoint control1 = {};
                xyPoint control2 = {};
                xyPoint crescEnd = {};
                
                // possibility of drawing line or shape
                int prob = ofRandom(100);
                bool cresc = false;
                if (prob <= 40){
                    cresc = true;
                }
                if (centroid < 0.1){
                    // draw in lower region
                    startPoint = {(int)ofRandom(width), (int)ofRandom(height / 2, height)};
                    endPoint = {(int)ofRandom(width), (int)ofRandom(height / 2, height)};
                } else {
                    // draw in upper region
                    startPoint = {(int)ofRandom(width), (int)ofRandom(height / 2)};
                    endPoint = {(int)ofRandom(width), (int)ofRandom(height / 2)};
                }
                control1 = {(int)ofRandom(width), (int)ofRandom(height)};
                control2= {(int)ofRandom(width), (int)ofRandom(height)};
                if (cresc){
                    // drawing a crescent
                    // currently keeping them small
                    crescEnd = {startPoint.x + (int)ofRandom(-50, 50), startPoint.y + (int)ofRandom(-50, 50)};
                }
                
                int totalFrames = ofDist(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                bool squiggle = false;
                float orient = ofRandom(1);
                ofColor colour;
                int colourIndex = ofRandom(colourQuantity);
                colour.setHsb(ofRandom(hueValues[colourIndex]), saturationValues[colourIndex], brightnessValues[colourIndex]);
                lineGen->addLine(startPoint, endPoint, control1, control2, totalFrames, squiggle, orient, colour);
                if(cresc){
                    lineGen->addCrescent(startPoint, crescEnd, colour);
                }
            }
            
            if (snippets[snippets.size() - 2].drawing == true && lineGen->returnTotalLines() > 0){
                // already drawing a line
                if (snippetsCounter > snippets.size() / 2){
                    // check previous snippets, i dont do this after every snippet
                    int increase = 0;
                    int decrease = 0;
                    vector <int> peakFreqs;
                    float avgCentroid = 0;
                    for (int i = 0; i < snippets.size(); i++){
                        if (i != 0){
                            cout << snippets[i].centroid << endl;
                            if (snippets[i].centroid > snippets[i - 1].centroid){
                                increase++;
                            } else{
                                decrease++;
                            }
                        }
                        peakFreqs.push_back(snippets[i].peakFreq);
                        avgCentroid += snippets[i].centroid;
                    }
                    
                    if (increase > decrease){
                        // pitch is increasing
                        cout << "increasing" << endl;
                        lineGen->increasing(0);
                    } else{
                        cout << "decreasing" << endl;
                        // pitch is decreasing
                        lineGen->decreasing(0);
                    }
                    
                    // calc avg centroid across snippets
                    avgCentroid /= snippets.size();
                    
                    // check if peak freq has been consistent & centroid is averaging high
                    if (occurenceCheck(peakFreqs, 4) == true && avgCentroid > 0.1){
                        cout << "gotcha" << endl;
                        lineGen->squiggleLine(0);
                        
                    }
                }
            }
            
            if (rmsSum < rmsThresh * 0.4){
                // audio has dropped low enough to cease drawing
                drawing = false;
                lineGen->clearAll();
            }
        }
        
        triggerFFT = false;
        rmsSum = 0;
    }
    mistyGen->run();
    lineGen->run();
}
//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input){
    float sum = 0;
    // fill the audio snippet
    for (int i = 0; i < input.getNumFrames(); i++){
        snippet[i + snippetBufferOffset * input.getNumFrames()] = input[i];
        sum += input[i] * input[i];
    }
    snippetBufferOffset++;
    // calc RMS for this buffer
    RMS = sqrt(sum);
    rmsSum += RMS; // to calc RMS for whole snippet

    if(snippetBufferOffset * initialBufferSize >= SNIPPET_LENGTH - initialBufferSize){
        triggerFFT = true;
        recording = false;
        snippetBufferOffset = 0;
    }
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

