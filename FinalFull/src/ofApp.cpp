#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp(){
    
}

//--------------------------------------------------------------
void ofApp::setup(){
    refresh = false;
    ofSetBackgroundAuto(refresh);
    misty = new MistyBrush();
    
    // Audio set-up
    sampleRate = 44100;
    initialBufferSize = 512;
    lAudioIn = new float[initialBufferSize];
    rAudioIn = new float[initialBufferSize];
    memset(lAudioIn, 0, initialBufferSize * sizeof(float));
    memset(rAudioIn, 0, initialBufferSize * sizeof(float));
    
    fftSize = 1024*16;
    myFFT.setup(fftSize, 1024*8, 256);
    
    nAverages = 12;
    oct.setup(sampleRate, fftSize/2, nAverages);
    
    mfccs = (double*) malloc(sizeof(double) * 13);
    mfcc.setup(512, 42, 13, 20, 20000, sampleRate);
    
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
    analyse = false;
    triggerFFT = false;
    snippetBufferOffset = 0;
    playingBufferOffset = 0;
    columns = sizeof(blocks) / sizeof(blocks[0]);
    rows = sizeof(blocks[0]) / sizeof(blocks[0][0]);
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
    if(playingBufferOffset * initialBufferSize >= SNIPPET_LENGTH - initialBufferSize){
        isPlaying = false;
    }
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
    
    // FFT IMPLEMENTATION
    // we only run the FFT when 'recording' has stopped
    if (triggerFFT){
        for (int i = 0; i < SNIPPET_LENGTH - initialBufferSize; i++){
            wave = snippet[i]; //* blackManWin.operator()(SNIPPET_LENGTH, i);
            // get fft
            if (myFFT.process(wave)){
                myFFT.magsToDB();
                oct.calculate(myFFT.magnitudesDB);
                float sum = 0;
                float maxFreq = 0;
                int maxBin = 0;
                
                // calc centroid and bin/freq
                for (int i = 0; i < fftSize/2; i++) {
                    sum += myFFT.magnitudes[i];
                    if (myFFT.magnitudes[i] > maxFreq) {
                        maxFreq=myFFT.magnitudes[i];
                        maxBin = i;
                    }
                }
                centroid = sum / (fftSize / 2);
                peakFreq = (float)maxBin/fftSize * 44100;
                mfcc.mfcc(myFFT.magnitudes, mfccs);
            }
        }
        
        // convert the array of bins to a vector
        vector <float> bins;
        for (int i = 0; i < myFFT.bins; i++){
            bins.push_back(myFFT.magnitudes[i]);
        }
        chordSpotter.analyse(fftSize, sampleRate, 8, bins);
        
        string pred = chordSpotter.returnChord();
        cout << pred << endl;
        
        // Adding a line to draw
        // starting position
        int startX = (int)peakFreq % width;
        int endX = width - startX;
        int startY = ofLerp(0, height, peakFreq/100);
        int endY = height - startY;
        
        misty->addLine({startX, startY}, {endX, endY}, {300 + ((int)peakFreq % 50), 400}, {200, 700}, 500);
        triggerFFT = false;
    }
    
    // Draw fft output
    float xinc = horizWidth / fftSize * 2.0;
    int drawFreq = 0;
    for(int i=0; i < fftSize / 2; i++) {
        // scale the values so they're more visible
        float height = myFFT.magnitudes[i] * 100;
        if (i % 10 == 0){
            ofDrawRectangle(horizOffset + (i*xinc),ofGetHeight() - height,1, height);
        }
    }
    
    // Draw octave analyser
    ofSetColor(255, 0, 255, 200);
    xinc = horizWidth / oct.nAverages;
    for (int i = 0; i < oct.nAverages; i++) {
        float height = oct.averages[i] / 20.0 * 100;
        ofDrawRectangle(horizOffset + (i * xinc), chromagramTop - height, 2, height);
    }
    
    misty->run();
}
//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input){
    
    float sum = 0;
    
    if (recording){
        // begin filling the audio snippet
        for (int i = 0; i < input.getNumFrames(); i++){
            snippet[i + snippetBufferOffset*input.getNumFrames()] = input[i];
        }
        snippetBufferOffset++;
    }
    
    for (int i = 0; i < input.getNumFrames(); i++){
        lAudioIn[i] = input[i*2];
        rAudioIn[i] = input[i*2+1];
        
        sum += input[i*2] * input[i*2];
        RMS = sqrt(sum);
    }
    
    // We can use the RMS to trigger an analysis of what was just played
    if (RMS > 3 && !recording){
        recording = true;
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output){
    std::size_t outChannels = output.getNumChannels();
    if (isPlaying){
        for (int i = 0; i < output.getNumFrames(); i++){
            output[i * outChannels + 0] = snippet[i + playingBufferOffset * output.getNumFrames()] * 2;
            output[i * outChannels + 1] = snippet[i + playingBufferOffset * output.getNumFrames()] * 2;
        }
        playingBufferOffset++;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1'){
        if(isPlaying) {
            isPlaying = false;
        }
        else{
            isPlaying = true;
            playingBufferOffset = 0;
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
    
    polar.x = atan(y/x);
    
    if (x < 0) {
        polar.x += PI/2;
    } else {
        polar.x += 3 * PI / 2;
    }
    
    polar.y = sqrt((x*x) + (y*y));
    return polar;
}
