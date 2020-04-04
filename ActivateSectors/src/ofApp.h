#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include <math.h>
#include "maxiMFCC.h"
#include "chordspot.hpp"
#define SNIPPET_LENGTH 44032 // ~1, must be multiple of buffer size

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void audioReceived(float * input, int bufferSize, int nChannels); // input method
        void audioRequested(float * input, int bufferSize, int nChannels); // output method
		
        ofVec2f polarToCart(float angle, int radius);
        ofVec2f cartToPolar(float x, float y);
        ofVec2f whichBlock(float x, float y);
    
        float * lAudioOut; /* outputs */
        float * rAudioOut;
        float * lAudioIn; /* inputs */
        float * rAudioIn;
        int initialBufferSize; /* buffer size */
        int sampleRate;
        bool triggerFFT;
    
        int height, width;
        int whichSectorIndex(float x, float y, int radius);
    
        float angles [12] = {PI/6, PI/3, PI/2, PI*2/3, PI*5/6, PI, PI*7/6, PI*4/3, PI*3/2, PI*5/3, PI*11/6, 2*PI};
    
        int rows;
        int columns;
        int blocks [16][12];
        bool analyse;
        ChordSpot chordSpotter;
    
    
    
    
        //MAXIMILIAN STUFF:
        double wave,sample,outputs[2], ifftVal;
        float snippet [SNIPPET_LENGTH] = {0};
        int snippetBufferOffset;
        maxiMix mymix;
        maxiOsc osc;
    
        ofxMaxiFFTOctaveAnalyzer oct;
        int nAverages;
    
        float peakFreq = 0;
        float centroid = 0;
        float RMS = 0;
    
        ofxMaxiFFT myFFT;
        int fftSize;
        int bins, dataSize;
    
        float callTime;
        timeval callTS, callEndTS;
    
        maxiMFCC mfcc;
        double *mfccs;
    
        maxiSample samp;
        ofPolyline line;
};
