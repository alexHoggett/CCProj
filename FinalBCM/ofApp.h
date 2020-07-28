#pragma once

// software
#include "ofMain.h"
#include "ofxMaxim.h"
#include <math.h>
#include "maxiMFCC.h"
#include "mistyBrush.hpp"
#include "linegen.hpp"
#include "chordspot.hpp"
#include "stairs.hpp"
#include "cleanBrush.hpp"
#include "largeBrush.hpp"
#include <algorithm>
#define SNIPPET_LENGTH 3000
#define colourQuantity 10

// hardware
#include <bcm2835.h>
#include <stdio.h>
#include <cstdint>

// Define Input Pins
#define TOGGLE_SWITCH 	        RPI_V2_GPIO_P1_32 	//GPIO12
#define FOOT_SWITCH 	        RPI_GPIO_P1_10 		//GPIO15
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
        void audioOut(ofSoundBuffer & buffer); //callback method
        void keyPressed(int key);
        void shiftSnippets(vector<snippet> snippets);
    
        ofSoundStream soundStream;
        int testCounter = 0;
		
        // For generating colours
        float hueValues [colourQuantity] = {0};
        float saturationValues [colourQuantity] = {0};
        float brightnessValues [colourQuantity] = {0};
        
        //  maths functions
        ofVec2f whichBlock(float x, float y);
        bool occurenceCheck(vector <int> &freqs, int thresh); // returns true if there are over a certain number of occurences of any element, roughly checking for consistency
        bool increaseDecrease();
        bool fiftyFifty();
    
        float angles [12] = {PI/6, PI/3, PI/2, PI*2/3, PI*5/6, PI, PI*7/6, PI*4/3, PI*3/2, PI*5/3, PI*11/6, 2*PI};
        int height, width;
        int rows;
        int columns;
        int blocks [16][12];
        
        LineGen * lineGen;
        LineGen * mistyGen;
        LineGen * cleanGen;
        LineGen * largeGen;
        Stairs stairGen;
        
        vector<snippet> snippets;
        int snippetsCounter; // to count how many snippets are being stored
        int recordingCounter; // to track index position of sample
        double frameCount;
        bool frameCountBegin;
        bool drawing;
        bool footSwitch;
        bool tempFootSwitch;
        bool toggleSwitch;
        bool tempToggleSwitch;
        bool stopDrawing;
    
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
        double wave,sample,outputs[2], ifftVal;
        float audioRecording [SNIPPET_LENGTH] = {0};
        int snippetBufferOffset;
        int playingBufferOffset;
    
        float peakFreq = 0;
        float centroid = 0;
        float RMS = 0; // to hold current rms
        float rmsSum = 0; // to calc the average rms of an audio snippet
    
        ofxMaxiFFT myFFT;
        int fftSize;
        int bins, dataSize;
        
        // for hardware
        uint32_t read_timer = 0;
        uint32_t input_signal = 0;
        uint8_t mosi[10] = { 0x01, 0x00, 0x00 }; //12 bit ADC read 0x08 ch0, - 0c for ch1 
	uint8_t miso[10] = { 0 };

        uint8_t FOOT_SWITCH_val;
        uint8_t TOGGLE_SWITCH_val;
};
