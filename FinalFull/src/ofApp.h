#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include <math.h>
#include "maxiMFCC.h"
#include "mistyBrush.hpp"
#include "linegen.hpp"
#include "chordspot.hpp"
#define SNIPPET_LENGTH 44032 * 1// ~1, must be multiple of buffer size
#define colourQuantity 10

struct xyPoint {
    int x;
    int y;
};

class ofApp : public ofBaseApp{

	public:
        ~ofApp();
		void setup();
		void update();
		void draw();
        void audioOut(ofSoundBuffer& output); //output method
        void audioIn(ofSoundBuffer& input); //input method
        ofSoundStream soundStream;
		void keyPressed(int key);
		
        // For generating colours
        float hueValues [colourQuantity] = {0};
        float saturationValues [colourQuantity] = {0};
        float brightnessValues [colourQuantity] = {0};
        
        //  maths functions
        ofVec2f polarToCart(float angle, int radius);
        ofVec2f cartToPolar(float x, float y);
        ofVec2f whichBlock(float x, float y);
    
        float angles [12] = {PI/6, PI/3, PI/2, PI*2/3, PI*5/6, PI, PI*7/6, PI*4/3, PI*3/2, PI*5/3, PI*11/6, 2*PI};
        int height, width;
        int whichSectorIndex(float x, float y, int radius);
        int rows;
        int columns;
        int blocks [16][12];
    
        MistyBrush * misty;
        bool refresh;
        
        // for chord recognition
        ChordSpot chordSpotter;
    
        // audio / maximilian
        float * lAudioOut;
        float * rAudioOut;
        float * lAudioIn;
        float * rAudioIn;
        int initialBufferSize;
        int sampleRate;
        bool triggerFFT;
        bool recording;
        bool analyse;
        double wave,sample,outputs[2], ifftVal;
        float snippet [SNIPPET_LENGTH] = {0};
        int snippetBufferOffset;
        int playingBufferOffset;
        bool isPlaying = false;
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
    
        blackmanHarrisWinFunctor blackManWin;
        
};