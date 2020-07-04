#pragma once

#include "ofMain.h"
#include "linegen.hpp"
#include "mistyBrush.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
        void mouseReleased(int x, int y, int button);
		
        bool refresh;
        xyPoint firstPos = {0, 0};
        bool settingPos;
        LineGen* lineGen;
        LineGen* mistyGen;
        ofImage image;
        ofShader gaussianBlurX;
        ofShader gaussianBlurY;
        ofFbo fboBlurOnePass;
        ofFbo fboBlurTwoPass;
};
