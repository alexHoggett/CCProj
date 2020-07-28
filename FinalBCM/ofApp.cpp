#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp(){
    // destructor
}

//--------------------------------------------------------------
void ofApp::setup(){
    // Start the BCM2835 Library to access GPIO.
    if (!bcm2835_init()){
      printf("bcm2835_init failed. Are you running as root??\n");
    }
	// Start the SPI BUS.
	if (!bcm2835_spi_begin()){
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
    }
    
	//define SPI bus configuration
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); 	  // 4MHz clock with _64
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
	
    //Define GPIO pins configuration
	bcm2835_gpio_fsel(TOGGLE_SWITCH, BCM2835_GPIO_FSEL_INPT);	//TOGGLE_SWITCH as input
	bcm2835_gpio_fsel(FOOT_SWITCH, BCM2835_GPIO_FSEL_INPT); 	//FOOT_SWITCH as input
	bcm2835_gpio_fsel(LED, BCM2835_GPIO_FSEL_OUTP);				//LED as output

	// bcm2835_gpio_set_pud(TOGGLE_SWITCH, BCM2835_GPIO_PUD_UP);   //TOGGLE_SWITCH pull-up enabled 
	bcm2835_gpio_set_pud(FOOT_SWITCH, BCM2835_GPIO_PUD_UP);     //FOOT_SWITCH pull-up enabled 
    
    // Audio set-up
    sampleRate = 44100;
    initialBufferSize = 4;
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    ofSoundStreamSettings settings;
    
    auto devices = soundStream.getMatchingDevices("default");
    if (!devices.empty()) {
        settings.setInDevice(devices[0]);
    }
    
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 1;
    settings.numInputChannels = 0;
    settings.bufferSize = initialBufferSize;
    soundStream.setup(settings);
    soundStream.start();
    
    // Initialising values
    triggerFFT = false;
    drawing = false;
    stopDrawing = false;
    toggleSwitch = bcm2835_gpio_lev(TOGGLE_SWITCH);
    tempToggleSwitch = toggleSwitch;
    footSwitch = bcm2835_gpio_lev(FOOT_SWITCH);
    tempFootSwitch = footSwitch;
    snippetBufferOffset = 0;
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
    mistyGen = new MistyBrush;
    cleanGen = new CleanBrush;
    largeGen = new LargeBrush;
    snippetsCounter = 0;
    recordingCounter = 0;
    //ofEnableAlphaBlending();
    
    if (toggleSwitch == 0){
        for (int i = 0; i < 14; i++){
            ofColor c;
            c.set(ofRandom(255), ofRandom(255), ofRandom(255));
            mistyGen->addLine({(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, 1000, true, 0.0, c);
        }    
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    toggleSwitch = bcm2835_gpio_lev(TOGGLE_SWITCH);
	footSwitch = bcm2835_gpio_lev(FOOT_SWITCH);
	bcm2835_gpio_write(LED,1); //light the effect when running.
    
     // cout << "toggle switch: " << toggleSwitch << endl;
    // cout << "foot switch: " << footSwitch << endl;
    
    
    if (toggleSwitch != tempToggleSwitch){
        // toggleswitch been switched
        // cout << "have it" << endl;
        drawing = false;
        lineGen->clearAll();
        mistyGen->clearAll();
        stairGen.clearAll();
        cleanGen->begin();
        
        if (!toggleSwitch){
            // switching to state 2
            frameCount = 0;
            frameCountBegin = true;
        } else {
            // switching to state 1
            frameCountBegin = false;
        }
        
        tempToggleSwitch = toggleSwitch;
    }
    
    if (footSwitch != tempFootSwitch){
        // footswitch been switched
        drawing = false;
        lineGen->clearAll();
        mistyGen->clearAll();
        stairGen.clearAll();
        cleanGen->begin();
        
        if (toggleSwitch == false){
            frameCountBegin = true;
        }
        
        tempFootSwitch = footSwitch;
    }
    
    
    if (frameCountBegin){
        frameCount++;
        if (frameCount >= 200){
            frameCount = 0;
            frameCountBegin = false;
            for (int i = 0; i < 14; i++){
                ofColor c;
                c.set(ofRandom(255), ofRandom(255), ofRandom(255));
                mistyGen->addLine({(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, {(int)ofRandom(ofGetWidth()), (int)ofRandom(ofGetHeight())}, 1000, true, 0.0, c);
            }
        }
    }
    
    if (frameCountBegin == false && snippetsCounter % 4 == 0 && toggleSwitch == false){
        // update position of mist
        for (int i = 0; i < mistyGen->returnTotalLines(); i++){
            if (i % 2 == 0){
                mistyGen->increasing(i);
            } else {
                mistyGen->decreasing(i);
            }
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 0, 0, 255);
    
    float rmsThresh;
    
    if (toggleSwitch){
        rmsThresh = 15.0;
    } else {
        rmsThresh = 15.0;
    }
    
    // I use the RMS to trigger an analysis of what was just played
    // thresh is different for the 2 states
    if (RMS > rmsThresh && !drawing){
        drawing = true;
    }
        
    // FFT IMPLEMENTATION
    // only run the FFT at the end of the recording snippet
    if (triggerFFT){
        // divide the sum of the rms's by the amount of buffers used to fill the snippet
        
        snippets.push_back({ofRandom(1), ofRandom(1000), RMS, drawing});
        snippetsCounter++;
        
        // if snippets vector reaches a certain size it is trimmed
        int maxLength = 10;
        if (snippets.size() > maxLength){
            snippets.erase(snippets.begin());
        }
        
        if (drawing && snippets.size() > 2){
            
            if (toggleSwitch){
                // state 1
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
                    if (prob <= 30){
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
                    lineGen->addLine(startPoint, endPoint, control1, control2, totalFrames * 0.7, squiggle, orient, colour);
                    if(cresc){
                        lineGen->addCrescent(startPoint, crescEnd, colour);
                    }
                }
                
                if (snippets[snippets.size() - 2].drawing == true && lineGen->returnTotalLines() > 0){
                    // already drawing a line
                    if (snippetsCounter > snippets.size() / 2){
                        // cout << "updating" << endl;
                        // check previous snippets, i dont do this after every snippet
                        vector <int> peakFreqs;
                        float avgCentroid = 0;
                        for (int i = 0; i < snippets.size(); i++){
                            peakFreqs.push_back(snippets[i].peakFreq);
                            avgCentroid += snippets[i].centroid;
                        }
                        
                        if (increaseDecrease()){
                            // pitch is increasing
                            lineGen->increasing(0);
                        } else{
                            // pitch is decreasing
                            lineGen->decreasing(0);
                        }
                        
                        // calc avg centroid across snippets
                        avgCentroid /= snippets.size();
                        
                        // check if peak freq has been consistent & centroid is averaging high
                        if (occurenceCheck(peakFreqs, 4) == true && avgCentroid > 0.1){
                            lineGen->squiggleLine(0);
                            
                        }
                    }
                }
                if (RMS < rmsThresh * 0.8){
                    stopDrawing = true;
                }
            }
            else {
                // state 2
                if (snippets[snippets.size() - 2].drawing == false){
                    // start drawing something new
                    // draw first blob
                    xyPoint control1;
                    xyPoint control2;
                    control1 = {(int)ofRandom(width), (int)ofRandom(height)};
                    control2= {(int)ofRandom(width), (int)ofRandom(height)};
                    
                    ofColor colour;
                    int colourIndex = ofRandom(colourQuantity);
                    colour.setHsb(ofRandom(hueValues[colourIndex]), saturationValues[colourIndex], brightnessValues[colourIndex]);
                    xyPoint startPoint = {(int)(snippets[snippets.size() - 1].peakFreq) % width, (int)ofRandom(height)};
                    xyPoint endPoint = {(int)ofRandom(width), (int)ofRandom(height)};
                    
                    int totalFrames = ofDist(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                    
                    largeGen->addLine(startPoint, endPoint, control1, control2, totalFrames, 0, ofRandom(1), colour);
                    
                    // add a random amount of other blobs, roughly spaced evenly apart
                    for (int i = 0; i < ofRandom(3); i++){
                        
                        colourIndex = ofRandom(colourQuantity);
                        colour.setHsb(ofRandom(hueValues[colourIndex]), saturationValues[colourIndex], brightnessValues[colourIndex]);
                        
                        control1 = {(int)ofRandom(width), (int)ofRandom(height)};
                        control2 = {(int)ofRandom(width), (int)ofRandom(height)};
                        
                        xyPoint next;
                        if (i == 0){
                            next = {(int)ofRandom(width), (int)ofRandom(height)};
                        } else {
                            next = {next.x * i % width, next.y * i % height};
                        }
                        endPoint = {next.x + (int)ofRandom(-300, 300), next.y + (int)ofRandom(-300, 300)};
                        totalFrames = ofDist(next.x, next.y, endPoint.x, endPoint.y);
                        largeGen->addLine(next,
                                          endPoint,
                                          control1,
                                          control2,
                                          totalFrames,
                                          0,
                                          ofRandom(1),
                                          colour);
                    }
                }
                
                if(snippets[snippets.size() - 2].drawing == true && largeGen->returnTotalLines() > 0){
                    // already drawing
                    if (snippetsCounter > snippets.size() / 2){
                        float freqSum;
                        for (int i = 0; i < snippets.size(); i++){
                            freqSum += snippets[i].peakFreq;
                        }
                        // take average freq
                        float avgFreq = freqSum / snippets.size();
                        
                        for (int i = 0; i < largeGen->returnTotalLines(); i++){
                            if (i % 2 == 0){
                                largeGen->changeLine(i, {i * (int)avgFreq, (i * (int)avgFreq) - height}, {(int)ofRandom(width), (int)ofRandom(height)}, {(int)ofRandom(width), (int)ofRandom(height)});
                            } else {
                                if (increaseDecrease()){
                                    largeGen->increasing(i);
                                } else{
                                    largeGen->decreasing(i);
                                }
                            }
                        }
                    }
                    
                    // prob of adding stairs
                    int prob = ofRandom(100);
                    if (prob < 10){
                        // draw some stairs
                        xyPoint startPoint = {(int)ofRandom(width), (int)ofRandom(height)};
                        xyPoint endPoint = {};
                        int totalFrames;
                        
                        ofColor colour;
                        int colourIndex = ofRandom(colourQuantity);
                        colour.setHsb(ofRandom(hueValues[colourIndex]), saturationValues[colourIndex], brightnessValues[colourIndex]);
                        
                        if (fiftyFifty()){
                            endPoint.x = startPoint.x + (ofRandom(100, 200));
                        } else {
                            endPoint.x = startPoint.x - (ofRandom(100, 200));
                        }
                        
                        if (increaseDecrease()){
                            // increasing pitch so ascending stairs
                            endPoint.y = startPoint.y + 150;
                            totalFrames = ofDist(startPoint.x, startPoint.y, endPoint.x, endPoint.y) * 1.5;
                            stairGen.add(startPoint, endPoint, totalFrames, ofRandom(4, 10), ofRandom(0.4, 0.6), colour);
                        } else {
                            // decreasing so descending stairs
                            endPoint.y = startPoint.y - 150;
                            totalFrames = ofDist(startPoint.x, startPoint.y, endPoint.x, endPoint.y) * 3;
                            stairGen.add(startPoint,endPoint, totalFrames, ofRandom(4, 10), ofRandom(0.4, 0.6), colour);
                        }
                        
                    }
                }
                if (RMS < rmsThresh){
                    stopDrawing = true;
                }
                
                
            }
            
            if (stopDrawing){
                // audio has dropped low enough to cease drawing
                drawing = false;
                lineGen->clearAll();
                // mistyGen->clearAll();
                largeGen->clearAll();
                stairGen.clearAll();
                stopDrawing = false;
            }
        }
        
        triggerFFT = false;
        rmsSum = 0;
    }
    mistyGen->run();
    lineGen->run();
    stairGen.run();
    cleanGen->run();
    largeGen->run();
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
    
    for (int i = 0; i < 100; i++){
        bcm2835_spi_transfernb((char*)mosi, (char*)miso, 3);
        input_signal = miso[2] + ((miso[1] & 0x0F) << 8);
        // cout << input_signal << endl;
        
        int rest = 2047;
        
        int min = rest - 150;
        int max = rest + 150;
        
        float norm = ofClamp(input_signal, min, max);
        norm -= min;
        norm /= (max - min);
        norm = (norm * 2) - 1;
        
        rmsSum += norm * norm;
        
        if(recordingCounter >= SNIPPET_LENGTH){
            // cout << "full" << endl;
            RMS = sqrt(rmsSum);
            // cout << "RMS: " << RMS << endl;
            triggerFFT = true;
            recordingCounter = 0;
            rmsSum = 0;
        } else{
            audioRecording[recordingCounter] = norm;
            recordingCounter++;
        }
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
    else if (key == '2'){
        drawing = false;
        lineGen->clearAll();
        mistyGen->clearAll();
        stairGen.clearAll();
        cleanGen->begin();
        
        if (toggleSwitch){
            frameCount = 0;
            frameCountBegin = true;
        } else {
            mistyGen->clearAll();
        }
        
        toggleSwitch = !toggleSwitch;
    }
    else if (key == ' '){
        drawing = false;
        lineGen->clearAll();
        mistyGen->clearAll();
        stairGen.clearAll();
        cleanGen->begin();
        
        if (toggleSwitch == false){
            frameCountBegin = true;
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

//--------------------------------------------------------------
bool ofApp::increaseDecrease(){
    int increase = 0;
    int decrease = 0;
    
    for (int i = 0; i < snippets.size(); i++){
        if (i != 0){
            if (snippets[i].centroid > snippets[i - 1].centroid){
                increase++;
            } else{
                decrease++;
            }
        }
    }
    
    if (increase > decrease){
        // increasing pitch
        return 1;
    } else {
        // decreasing pitch
        return 0;
    }
}

//--------------------------------------------------------------
bool ofApp::fiftyFifty(){
    float prob = ofRandom(100);
    if (prob > 50){
        return true;
    } else{
        return false;
    }
}

