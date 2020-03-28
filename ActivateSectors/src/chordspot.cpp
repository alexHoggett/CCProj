#include "chordspot.hpp"
#include <string>
#include <iostream>
using namespace std;

ChordSpot::ChordSpot(){
    
}
//--------------------------------------------------------------
// Function with 2 parameters: a base frequency & the number of octaves,
// the function will sum the bin magnitudes of the bins that correspond to the given
// base frequency and it's octaves
float ChordSpot::octaveSum(float baseFreq, int numOctaves, int sampleRate, vector<float> bins, int fftSize){
    // Generate the octave frequencies
    std::vector<float> octaves;
    octaves.push_back(baseFreq);
    for(int i = 1; i < numOctaves + 1; i++){
        octaves.push_back(octaves[i - 1]*2);
    }
    
    // Summate the bin magnitudes that correspond to the octave frequencies
    float freqRes = (float)sampleRate / fftSize;
    float chromaBin = 0;
    for(int i = 0; i < octaves.size(); i++){
        int targetBin = octaves[i]/freqRes;
        chromaBin += bins[targetBin];
    }
    return chromaBin;
}

//--------------------------------------------------------------
// Function that returns 3 index positions that correspond to the 3 greatest elements
vector<int> ChordSpot::top3(vector<float> chroma){
    float first, second, third = -99999999;
    int index1, index2, index3 = 0;
    
    
    for (int i = 0; i < chroma.size(); i++){
        if(chroma[i] > first){
            third = second;
            index3 = index2;
            second = first;
            index2 = index1;
            first = chroma[i];
            index1 = i;
        }
        else if(chroma[i] > second){
            third = second;
            index3 = index2;
            second = chroma[i];
            index2 = i;
        }
        else if(chroma[i] > third){
            third = chroma[i];
            index3 = i;
        }
    }
    std::vector<int> top3 = {index1, index2, index3};
    return top3;
}

//--------------------------------------------------------------
// Generate the frequencies for the FFT
vector<float> ChordSpot::fftFreqs(int fftSize, int sampleRate){
    std::vector<float> theFreqs;
    for (int i = 0; i < fftSize; ++i){
        theFreqs.push_back((sampleRate/float(fftSize)) * i);
    }
    return theFreqs;
}
//--------------------------------------------------------------
// Calculate what chord this corresponds to
string ChordSpot::returnChord(int fftSize, int sampleRate, int octaves, vector<float> bins){
    int maxBin = 0;
    float maxMag = 0;
    float maxFreq = 0;
    currentFreqs = fftFreqs(fftSize, sampleRate);
    
    //find max frequency bin
    for(int i = 0; i < bins.size(); ++i){
        if(bins[i] > maxMag){
            maxMag = bins[i];
            maxFreq = currentFreqs[i];
            maxBin = i;
        }
    }
    
    // print to console for debugging
    cout << "max mag: " << maxMag << endl;
    cout << "max freq: " << maxFreq  << endl;
    cout << "max bin: " << maxBin << endl;
    
    // lowest octaves (in Hz)
    float baseC = 16.35;
    float baseDb = 17.325;
    float baseD = 18.355;
    float baseEb = 19.445;
    float baseE = 20.6;
    float baseF = 21.83;
    float baseGb = 23.125;
    float baseG = 24.5;
    float baseAb = 25.955;
    float baseA = 27.5;
    float baseBb = 29.135;
    float baseB = 30.87;
    
    // calculate chroma bins
    float C = octaveSum(baseC, octaves, sampleRate, bins, fftSize);
    float Db = octaveSum(baseDb, octaves, sampleRate, bins, fftSize);
    float D = octaveSum(baseD, octaves, sampleRate, bins, fftSize);
    float Eb = octaveSum(baseEb, octaves, sampleRate, bins, fftSize);
    float E = octaveSum(baseE, octaves, sampleRate, bins, fftSize);
    float F = octaveSum(baseF, octaves, sampleRate, bins, fftSize);
    float Gb = octaveSum(baseGb, octaves, sampleRate, bins, fftSize);
    float G = octaveSum(baseG, octaves, sampleRate, bins, fftSize);
    float Ab = octaveSum(baseAb, octaves, sampleRate, bins, fftSize);
    float A = octaveSum(baseA, octaves, sampleRate, bins, fftSize);
    float Bb = octaveSum(baseBb, octaves, sampleRate, bins, fftSize);
    float B = octaveSum(baseB, octaves, sampleRate, bins, fftSize);
    
    // create a vector to store the results and find the top 3 occuring notes
    vector<float> chroma = {C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B};
    vector<int> notes = top3(chroma);
    
    // create a prediction vector for the chord
    std::vector<int> prediction = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i = 0; i < notes.size(); i++){
        prediction[notes[i]] =  1;
    }
        
    for(int i = 0; i < prediction.size(); i++){
        cout << prediction[i] << " ";
    }
    cout << endl;
    
    string result;
    
    if(prediction == CMaj){
        result = "C";
    } else if(prediction == DMaj){
        result = "D";
    } else if(prediction == EMaj){
        result = "E";
    } else if(prediction == FMaj){
        result = "F";
    } else if(prediction == GMaj){
        result = "G";
    } else if(prediction == AMaj){
        result = "A";
    } else if(prediction == BMaj){
        result = "B";
    } else{
        result =  "Sorry that chord could not be found";
    }
    
    return result;
}
