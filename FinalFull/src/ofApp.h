#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include <math.h>
#include "maxiMFCC.h"
#include "mistyBrush.hpp"
#include "linegen.hpp"
#include "chordspot.hpp"
#include "stairs.hpp"
#include <algorithm>
#define SNIPPET_LENGTH 10752 // ~1/2s, must be multiple of buffer size
#define colourQuantity 10

struct snippet{
    float centroid;
    float peakFreq;
    float rms;
    bool drawing;
};

class ofApp : public ofBaseApp{
	public:
        ~ofApp();
		void setup();
		void update();
		void draw();
        void audioIn(ofSoundBuffer& input); //input method
		void keyPressed(int key);
        void shiftSnippets(vector<snippet> snippets);
    
        ofSoundStream soundStream;
		
        // For generating colours
        float hueValues [colourQuantity] = {0};
        float saturationValues [colourQuantity] = {0};
        float brightnessValues [colourQuantity] = {0};
        
        //  maths functions
        ofVec2f polarToCart(float angle, int radius);
        ofVec2f cartToPolar(float x, float y);
        ofVec2f whichBlock(float x, float y);
        bool occurenceCheck(vector <int> &freqs, int thresh); // returns true if there are over a certain number of occurences of any element, roughly checking for consistency
    
        float angles [12] = {PI/6, PI/3, PI/2, PI*2/3, PI*5/6, PI, PI*7/6, PI*4/3, PI*3/2, PI*5/3, PI*11/6, 2*PI};
        int height, width;
        int whichSectorIndex(float x, float y, int radius);
        int rows;
        int columns;
        int blocks [16][12];
    
        Stairs stairGen;
        MistyBrush * mistyGen;
        LineGen * lineGen;
        
        vector<snippet> snippets;
        int snippetsCounter;
    
        // for chord recognition
        ChordSpot chordSpotter;
    
        // audio / maximilian
        float * lAudioIn;
        float * rAudioIn;
        int initialBufferSize;
        int sampleRate;
        bool triggerFFT;
        bool recording;
        bool drawing;
        double wave,sample,outputs[2], ifftVal;
        float snippet [SNIPPET_LENGTH] = {0};
        int snippetBufferOffset;
        float peakFreq = 0;
        float centroid = 0;
        float RMS = 0; // to hold current rms
        float rmsSum = 0; // to calc the average rms of an audio snippet
        ofxMaxiFFT myFFT;
        int fftSize;
        int bins, dataSize;
};
