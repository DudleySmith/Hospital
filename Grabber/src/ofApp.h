#pragma once

#include "ofMain.h"
#include "ofxBluestorkGrabber.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // Grabber -------------------------------------
    ofPtr<ofxBluestorkGrabber> bluestrokGrabber;
    ofVideoGrabber grabber;
    
    ofxPanel grabberPanel;
    
    ofParameter<string> m_pAdress;
    ofParameter<int>    m_pWidth;
    ofParameter<int>    m_pHeight;
    ofxPanel othersPanel;
    
    // OpenCv -----------------------------------------
    
};
