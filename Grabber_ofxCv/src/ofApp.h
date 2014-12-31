#pragma once

#include "ofMain.h"
#include "ofxBluestorkGrabber.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxCv.h"

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
    ofxCv::ContourFinder    contourFinder;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    ofParameter<float>    m_pMinArea;
    ofParameter<float>    m_pMaxArea;
    ofParameter<float>    m_pThreshold;
    ofParameter<float>    m_pPersistence;
    ofParameter<float>    m_pMaxDistance;
    
    ofParameter<float>  m_pBrightness;
    ofParameter<float>  m_pContrast;
    ofParameter<bool>   m_pBlur;
    ofParameter<float>  m_pResultBrightness;
    ofParameter<float>  m_pResultContrast;
    ofxButton           m_pSetBg;
    ofxToggle           m_pShowLabels;
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
    ofParameter<int>        m_pIdxCam;
    ofParameter<bool>       m_pRandomId;
    ofParameter<string>     m_pIdSent;
    ofParameter<bool>       m_pRevertX;
    ofParameter<bool>       m_pRevertY;
    
    ofxOscMessage getMessage(int _blobIndex, ofPoint _center, float _radius);
    
};

