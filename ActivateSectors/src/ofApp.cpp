#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    columns = sizeof(blocks) / sizeof(blocks[0]);
    rows = sizeof(blocks[0]) / sizeof(blocks[0][0]);
    
    sampleRate = 44100; // Sampling Rate
    initialBufferSize = 512; // Buffer Size. you have to fill this buffer with sound
    lAudioOut = new float[initialBufferSize];
    rAudioOut = new float[initialBufferSize];
    lAudioIn = new float[initialBufferSize];
    rAudioIn = new float[initialBufferSize];
    
    // This is a nice safe piece of code
    memset(lAudioOut, 0, initialBufferSize * sizeof(float));
    memset(rAudioOut, 0, initialBufferSize * sizeof(float));
    memset(lAudioIn, 0, initialBufferSize * sizeof(float));
    memset(rAudioIn, 0, initialBufferSize * sizeof(float));
    
    fftSize = 1024*4;
    myFFT.setup(fftSize, 512*4, 256);
    
    nAverages = 12;
    oct.setup(sampleRate, fftSize/2, nAverages);
    
    mfccs = (double*) malloc(sizeof(double) * 13);
    mfcc.setup(512, 42, 13, 20, 20000, sampleRate);
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    ofSoundStreamSetup(2,2, this, sampleRate, initialBufferSize, 4);
    
    analyse = false;
    triggerFFT = false;
    snippetBufferOffset = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    if(snippetBufferOffset * initialBufferSize >= SNIPPET_LENGTH - initialBufferSize){
        triggerFFT = false;
        snippetBufferOffset = 0;
    }
    
    cout << triggerFFT << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw sectors
    ofNoFill();
    ofSetColor(0, 255, 0);
    int radius = 700;
    ofDrawCircle(width/2, height/2, radius);
    for (int i = 0; i < 12; i++){
        ofSetColor(0, 255, 0);
        ofVec2f edge = polarToCart(angles[i] - PI/2, radius);

        // x & y values from the centre of circle
        float x = edge.x + width/2;
        float y = edge.y + height/2;

        ofDrawLine(width/2, height/2, x, y);
    }

    // Draw rects
    ofVec2f mouse = whichBlock(ofGetMouseX(), ofGetMouseY());
    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 12; j++){
            // Highlight box where mouse is
            if (mouse.x == i && mouse.y == j){
                ofFill();
            } else {
                ofNoFill();
            }
            ofDrawRectangle(width/16 * i, height/12 * j, width/16, height/12);
        }
    }

    // Draw spectrum
    float horizWidth = 500.;
    float horizOffset = 100;
    float fftTop = 250;
    float mfccTop = 350;
    float chromagramTop = 450;
    
    ofSetColor(255, 0, 0, 255);
    vector <float> bins;
    
    // FFT IMPLEMENTATION
    if (!triggerFFT){
        for (int i = 0; i < SNIPPET_LENGTH - initialBufferSize; i++){
            wave = snippet[i];
            // get fft
            if (myFFT.process(wave)){
                myFFT.magsToDB();
                oct.calculate(myFFT.magnitudesDB);
                float sum = 0;
                float maxFreq = 0;
                int maxBin = 0;
                
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
    }
    
    // Draw fft output
    float xinc = horizWidth / fftSize * 2.0;
    for(int i=0; i < fftSize / 2; i++) {
        // scale the values so they're more visible
        float height = myFFT.magnitudes[i] * 100;
        ofRect(horizOffset + (i*xinc),250 - height,2, height);
        
        // We will have to convert the magnitudes values, which are pointers, into a vector
        // to be used later
        bins.push_back(myFFT.magnitudes[i]);
    }
    
    //chordSpotter.analyse(fftSize, sampleRate, 6, bins);
    
    //string pred = chordSpotter.returnChord();
    
    //cout << pred << endl;
    
    // We can use the RMS to trigger an analysis of what was just played
    if (RMS > 2 && !triggerFFT){
        triggerFFT = true;
    }
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    // You can just grab this input and stick it in a double
    // then use it above to create output
    
    float sum = 0;
    
    if (triggerFFT){
        // begin filling the audio snippet
        for (int i = 0; i < bufferSize; i++){
            snippet[i + snippetBufferOffset*bufferSize] = input[i];
        }
        snippetBufferOffset++;
    }
    
    for (int i = 0; i < bufferSize; i++){
        lAudioIn[i] = input[i*2];
        rAudioIn[i] = input[i*2+1];
        
        sum += input[i*2] * input[i*2];
        
        RMS = sqrt(sum);
    }
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    /*
    
    for (int i = 0; i < SNIPPET_LENGTH - bufferSize; i++){
        wave = snippet[i];
        // get fft
        if (!triggerFFT){
            if (myFFT.process(wave)) {
                // int bins = fftSize / 2.0;
                // do some manipulation
                // int hpCutoff = floor(((mouseX + ofGetWindowPositionX()) / (float) ofGetScreenWidth()) * fftSize / 2.0);
                // highpass
                // memset(myFFT.magnitudes, 0, sizeof(float) * hpCutoff);
                // memset(myFFT.phases, 0, sizeof(float) * hpCutoff);
                // lowpass
                // memset(myFFT.magnitudes + hpCutoff, 0, sizeof(float) * (bins - hpCutoff));
                // memset(myFFT.phases + hpCutoff, 0, sizeof(float) * (bins - hpCutoff));
                myFFT.magsToDB();
                // for(int z=0; z < 512; z++) cout << myFFT.magnitudesDB[z] << ",";
                // cout << "---------\n";
                oct.calculate(myFFT.magnitudesDB);
                
                

                // for (int j = 0; j < 12; j++) {
                // chromagram[j] = 0;
                // }
                // int j = 0;
                // for (int i = 0; i < oct.nAverages; i++) {
                // chromagram[j] += oct.averages[i];
                // j++;
                // j = j % 12;
                // }

                
                float sum = 0;
                float maxFreq = 0;
                int maxBin = 0;
                
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
                //cout << myFFT.spectralFlatness() << ", " << myFFT.spectralCentroid() << endl;
            }
            //inverse fft
            gettimeofday(&callTS,NULL);
            //        ifftVal = ifft.process(myFFT.magnitudes, myFFT.phases);
            gettimeofday(&callEndTS,NULL);
            callTime = (float)(callEndTS.tv_usec - callTS.tv_usec) / 1000000.0;
            callTime += (float)(callEndTS.tv_sec - callTS.tv_sec);
            //play result
            mymix.stereo(wave, outputs, 0.5);
            //        float mix = ((mouseX + ofGetWindowPositionX()) / (float) ofGetScreenWidth());
            //        mymix.stereo((wave * mix) + ((1.0-mix) * ifftVal), outputs, 0.5);
            // lAudioOut[i] = output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
            //rAudioOut[i] = output[i*nChannels + 1] = outputs[1];
       //     lAudioOut[i] = 0;
  //          rAudioOut[i] = 0;
  //      }
 //   }
        
    

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1'){
        analyse = false;
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

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
