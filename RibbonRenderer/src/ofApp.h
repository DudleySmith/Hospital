/**
 *
 * OFDevCon Example Code Sprint
 * Camera Ribbon example
 * This example generates ribbons along the mouse trail that descend in space
 * When you click space bar, you can
 *
 * Created by James George for openFrameworks workshop at Waves Festival Vienna sponsored by Lichterloh and Pratersauna
 * Adapted during ofDevCon on 2/23/2012
 */

#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "hospitalRibbonsManager.h"
#include "hospitalPoint.h"

class ofApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
	//this holds all of our points
    hospitalRibbonsManager ribbonManager;

    ofxOscReceiver receiver;
    
    ofxPanel plOsc;
    ofParameter<int> psPort;

    ofxPanel plRibbons;
    ofxPanel plRibbonsFx;
    
    bool mDrawGui;
    
};
