#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include <math.h>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        ofVec2f polarToCart(float angle, int radius);
        ofVec2f cartToPolar(float x, float y);
        ofVec2f whichBlock(float x, float y);
    
        void audioReceived     (float * input, int bufferSize, int nChannels); /* input method */
        void audioRequested     (float * input, int bufferSize, int nChannels); /* output method */
    
        int height, width;
        int whichSectorIndex(float x, float y, int radius);
    
        float angles [12] = {PI/6, PI/3, PI/2, PI*2/3, PI*5/6, PI, PI*7/6, PI*4/3, PI*3/2, PI*5/3, PI*11/6, 2*PI};
    
        int rows;
        int columns;
        int blocks [16][12];
    
        //  Maxim stuff
        float   * lAudioOut; /* outputs */
        float   * rAudioOut;
    
        float * lAudioIn; /* inputs */
        float * rAudioIn;
    
        int        initialBufferSize; /* buffer size */
        int        sampleRate;
    
        double wave,sample,outputs[2], ifftVal;
        maxiMix mymix;
        maxiOsc osc;
    
        ofxMaxiFFTOctaveAnalyzer oct;
        int nAverages;
        float *ifftOutput;
        int ifftSize;
    
        float peakFreq = 0;
        float centroid = 0;
        float RMS = 0;
    
        ofxMaxiIFFT ifft;
        ofxMaxiFFT mfft;
        int fftSize;
        int bins, dataSize;
    
        float callTime;
        timeval callTS, callEndTS;
    
        maxiMFCC mfcc;
        double *mfccs;
    
        maxiSample samp;
};
