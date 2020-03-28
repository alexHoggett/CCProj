#ifndef chordspot_hpp
#define chordspot_hpp

#include <stdio.h>
#include <vector>
#include <cstdlib>
using namespace std;

class ChordSpot{
public:
    ChordSpot();
    string returnChord(int fftSize, int sampleRate, int octaves, vector<float> bins);
private:
    // Private functions
    vector<int> top3(vector<float> chroma);
    vector<float> fftFreqs(int fftSize, int sampleRate);
    float octaveSum(float baseFreq, int numOctaves, int smapleRate, vector<float> bins, int fftSize);
    
    
    // Chord 'Database'
    /*                       C  C# D  D# E  F  F# G  G# A  A# B      */
    vector<int> CMaj = {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0}; // checked
    vector<int> DMaj = {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0}; // checked
    vector<int> EMaj = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1}; // not working for open E *fixed
    vector<int> FMaj = {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}; // checked
    vector<int> GMaj = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1}; // checked
    vector<int> AMaj = {0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0}; // checked
    vector<int> BMaj = {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}; // checked
    
    vector<float> currentFreqs;
    
};

#endif /* chordspot_hpp */
