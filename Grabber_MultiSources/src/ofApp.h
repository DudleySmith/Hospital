#pragma once

#include "ofMain.h"
#include "ofxBluestorkGrabber.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    
    void setupPanelGrabber();
    void setupPanelSource();
    void setupPanelOpenCv();
    
    void setupOpenCv();
    void setupOsc();
    
    void setupIpCamera();
    void setupKinect();
    
    void update();
    void updateIpCam();
    void updateKinect();
    
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
    ofVideoGrabber videoGrabber;
    
    ofxPanel grabberPanel;
    
    ofParameter<string> m_pAdress;
    ofParameter<string> m_pMoviePath;
    ofParameter<int>    m_pSource;
    ofParameter<string> m_pSourceDisplay;
    ofParameter<int>    m_pGrabWidth;
    ofParameter<int>    m_pGrabHeight;
    ofParameter<int>    m_pDrawWidth;
    ofParameter<int>    m_pDrawHeight;
    ofxToggle           m_pDraw;
    ofxPanel            cameraPanel;
    
    // OpenCv -----------------------------------------
    ofxCvColorImage			colorImg;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    ofxCvGrayscaleImage     grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage     grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder 	contourFinder;
    
    ofParameter<bool>   m_pTwoThreshold;
    ofParameter<int>    m_pThreshold;
    ofParameter<int>    m_pNearThreshold;
    ofParameter<int>    m_pFarThreshold;
    ofParameter<float>  m_pBrightness;
    ofParameter<float>  m_pContrast;
    ofParameter<float>  m_pBlur;
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
    
    // Kinect --------------------------------------
    ofxKinect kinect;
    
    // Movie file -----------------------------------
    ofVideoPlayer vidPlayer;
    
};

