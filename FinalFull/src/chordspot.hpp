#ifndef chordspot_hpp
#define chordspot_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
using namespace std;

class ChordSpot{
    struct chord{
        string chordName;
        vector<int> notes;
    };
    
    public:
        ChordSpot();
        void analyse(int fftSize, int sampleRate, int octaves, vector<float> bins);
        string returnChord();
        vector<int> returnTopNotes();
        
    private:
        vector<int> top3(vector<float> chroma);
        vector<float> fftFreqs(int fftSize, int sampleRate);
        float octaveSum(float baseFreq, int numOctaves, int smapleRate, vector<float> bins, int fftSize);

        string chordPred;
        vector<int> topNotes;
        vector<chord> chords = {
            // Chord 'Database'
            /*        C  C# D  D# E  F  F# G  G# A  A# B      */
            {"CMaj", {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0}},
            {"DMaj", {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0}},
            {"EMaj", {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1}},
            {"FMaj", {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}},
            {"GMaj", {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1}},
            {"AMaj", {0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0}},
            {"AMin", {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0}},
            {"BMaj", {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}}
        };
};

#endif /* chordspot_hpp */
