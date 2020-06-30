#pragma once

// software
#include "ofMain.h"
#include "ofxMaxim.h"
#include <math.h>
#include "maxiMFCC.h"
#include "linegen.hpp"
#include "chordspot.hpp"
#include <algorithm>
#define SNIPPET_LENGTH 100
#define colourQuantity 10

// hardware
#include <bcm2835.h>
#include <stdio.h>
#include <cstdint>

// Define Input Pins
#define PUSH1 			RPI_GPIO_P1_08  	//GPIO14
#define PUSH2 			RPI_V2_GPIO_P1_38  	//GPIO20 
#define TOGGLE_SWITCH 	RPI_V2_GPIO_P1_32 	//GPIO12
#define FOOT_SWITCH 	RPI_GPIO_P1_10 		//GPIO15
#define LED   			RPI_V2_GPIO_P1_36 	//GPIO16

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
        void keyPressed(int key);
        void audioOut(ofSoundBuffer & buffer);
        void shiftSnippets(vector<snippet> snippets);
        
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
        
        ofSoundStream soundStream; // used to establish the callback function
        LineGen * lineGen;
        vector<snippet> snippets;
        int snippetsCounter;
        int recordingCounter;
    
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
        bool drawing;
        double wave,sample,outputs[2], ifftVal;
        float audioRecording [SNIPPET_LENGTH] = {0};
        int snippetBufferOffset;
        int playingBufferOffset;
        maxiOsc osc;
    
        ofxMaxiFFTOctaveAnalyzer oct;
        int nAverages;
    
        float peakFreq = 0;
        float centroid = 0;
        float RMS = 0; // to hold current rms
        float rmsSum = 0; // to calc the average rms of an audio snippet
    
        ofxMaxiFFT myFFT;
        int fftSize;
        int bins, dataSize;
    
        float callTime;
        timeval callTS, callEndTS;
    
        maxiMFCC mfcc;
        double *mfccs;
        
        // for hardware
        uint32_t read_timer=0;
        uint32_t input_signal=0;
        uint8_t mosi[10] = { 0x01, 0x00, 0x00 }; //12 bit ADC read 0x08 ch0, - 0c for ch1 
	uint8_t miso[10] = { 0 };

        uint8_t FOOT_SWITCH_val;
        uint8_t TOGGLE_SWITCH_val;
        uint8_t PUSH1_val;
        uint8_t PUSH2_val;
};
