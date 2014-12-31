#pragma once

#include "ofMain.h"
#include "ofxBluestorkGrabber.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void setupOpenCv();
    void setupCamera();
    void setupGrabber();
    void setupOsc();
    
    void update();
    void updateOpenCv();
    void updateOsc();
    void setBgPressed();
    
    void draw();
    void drawOpenCv();
    
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
    ofxToggle           m_pDraw;
    ofxPanel            cameraPanel;
    
    // OpenCv -----------------------------------------
    ofxCvColorImage			colorImg;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    ofxCvContourFinder 	contourFinder;
    
    ofParameter<int>    m_pThreshold;
    ofParameter<float>  m_pBrightness;
    ofParameter<float>  m_pContrast;
    ofParameter<bool>   m_pBlur;
    ofxButton           m_pSetBg;
    ofxPanel            openCvPanel;
    
    // OSC -----------------------------------------
    ofxOscSender        oscSender;
    string              reportOsc;
    
    ofxPanel            oscPanel;
    ofParameter<bool>       m_pReady;
    ofParameter<bool>       m_pSend;
    ofParameter<string>     m_pHost;
    ofParameter<int>        m_pPort;
    ofParameter<string>     m_pPrefix;
    
    ofxOscMessage getMessage(int _blobIndex, ofPoint _center, float _radius);
    
};

