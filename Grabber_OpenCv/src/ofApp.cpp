#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    setupCamera();
    setupGrabber();
    setupOpenCv();
    setupOsc();
    
}

//--------------------------------------------------------------
void ofApp::setupGrabber(){
    
    grabber.setGrabber(bluestrokGrabber);
    grabber.initGrabber(m_pWidth,m_pHeight);
    
    grabberPanel.setup(bluestrokGrabber->parameters);
    grabberPanel.setPosition(20, 550);
}

//--------------------------------------------------------------
void ofApp::setupCamera(){
    cameraPanel.setup("grabber", "grabber.xml");
    cameraPanel.add(m_pAdress.set("adress", "192.168.3.101"));
    cameraPanel.add(m_pWidth.set("width", 320));
    cameraPanel.add(m_pHeight.set("height", 240));
    cameraPanel.setPosition(260, 550);
    cameraPanel.loadFromFile("grabber.xml");
    
    bluestrokGrabber = ofPtr<ofxBluestorkGrabber>(new ofxBluestorkGrabber);
    bluestrokGrabber->setCameraAddress(m_pAdress);
}

//--------------------------------------------------------------
void ofApp::setupOpenCv(){
    
    // Panel setup
    m_pSetBg.addListener(this,&ofApp::setBgPressed);
    
    openCvPanel.setup("openCv", "openCv.xml");
    openCvPanel.add(m_pThreshold.set("threshold", 80, 0, 255));
    openCvPanel.add(m_pBrightness.set("brightness", 0, -1, 1));
    openCvPanel.add(m_pContrast.set("contrast", 0, -1, 1));
    openCvPanel.add(m_pBlur.set("blur", false));
    openCvPanel.add(m_pSetBg.setup("Set Background"));
    openCvPanel.add(m_pDraw.setup("Draw", true));
    openCvPanel.loadFromFile("openCv.xml");
    openCvPanel.setPosition(500, 550);
    
    colorImg.allocate(m_pWidth,m_pHeight);
    grayImage.allocate(m_pWidth,m_pHeight);
    grayBg.allocate(m_pWidth,m_pHeight);
    grayDiff.allocate(m_pWidth,m_pHeight);
    //grayToCompute.allocate(320, 240);
}


//--------------------------------------------------------------
void ofApp::setupOsc(){
    
    oscPanel.setup("osc", "osc.xml");
    oscPanel.add(m_pReady.set("ready", false));
    oscPanel.add(m_pSend.set("send", true));
    oscPanel.add(m_pHost.set("host", "127.0.0.1"));
    oscPanel.add(m_pPort.set("port", 9001));
    oscPanel.add(m_pPrefix.set("prefix", "cam1"));
    
    oscPanel.setPosition(400, 550);
    oscPanel.loadFromFile("osc.xml");
    
    oscSender.setup(m_pHost, m_pPort);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    grabber.update();
    
    if (grabber.isFrameNew()==true) {
        updateOpenCv();
        updateOsc();
    }
    

    
}

//--------------------------------------------------------------
void ofApp::updateOpenCv(){
    
    colorImg.setFromPixels(grabber.getPixels(), 320,240);
    grayImage = colorImg;
    grayImage.brightnessContrast(m_pBrightness, m_pContrast);
    
    // take the abs value of the difference between background and incoming and then threshold:
    grayDiff.absDiff(grayBg, grayImage);
    
    if(m_pBlur)
        grayDiff.blurHeavily();
    grayDiff.threshold(m_pThreshold);
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
    
}


//--------------------------------------------------------------
void ofApp::updateOsc(){
    
    
    if (reportOsc.length() > 1000) {
        reportOsc = "";
    }

   for (int i = 0; i < contourFinder.nBlobs; i++){
        
        ofPoint center = contourFinder.blobs[i].boundingRect.getCenter();
        float radius = sqrt(contourFinder.blobs[i].area/PI);
        
        ofxOscMessage m = getMessage(i, center, radius);
        oscSender.sendMessage(m);
        
        string message = "Message Osc : " + m.getAddress() + ":" ;
        
        vector<ofxOscArg>::iterator arg;
        for (int nbArgs=0; nbArgs<m.getNumArgs(); nbArgs++) {
            message += m.getArgAsString(nbArgs) + " - ";
        }
        
        reportOsc.append(message);
        ofLogVerbose() << message;
        
    }
    
    
}

//-----------------------------------------------------------------------------
ofxOscMessage ofApp::getMessage(int _blobIndex, ofPoint _center, float _radius){
    
    ofxOscMessage m;
    string prefix = m_pPrefix;
    
    m.setAddress("/" + prefix + "/blob" + ofToString(_blobIndex, 3, '0'));
    m.addFloatArg((float)(_center.x)/m_pWidth);
    m.addFloatArg((float)(_center.y)/m_pHeight);
    m.addFloatArg(_radius);
    
    return m;
    
}

//--------------------------------------------------------------
void ofApp::setBgPressed(){
    grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(m_pDraw){
        drawOpenCv();
    }
    
    grabberPanel.draw();
    cameraPanel.draw();
    openCvPanel.draw();
    oscPanel.draw();
    
    // Report Blob
    ofSetColor(ofColor::white);
    stringstream reportStr;
    
    reportStr << "bg subtraction and blob detection" << endl
    << "threshold " << m_pThreshold << endl
    << "num blobs found " << contourFinder.nBlobs << endl
    << "fps: " << ofGetFrameRate();
    
    ofDrawBitmapString(reportStr.str(), 750, 360);
    
    // Report OSC
    ofDrawBitmapString(reportOsc, 750, 500);
    
}

//--------------------------------------------------------------
void ofApp::drawOpenCv(){
    
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);
    colorImg.draw(20,20);
    grayImage.draw(360,20);
    grayBg.draw(20,280);
    grayDiff.draw(360,280);
    //grayToCompute.draw(20, 540);
    
    // Draw background of blobs ---
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(700, 20);
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::darkGrey);
    ofRect(0, 0,320,240);
    ofPopStyle();
    
    ofNoFill();
    ofSetColor(ofColor::white);
    ofSetLineWidth(2);
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofPoint center = contourFinder.blobs[i].boundingRect.getCenter();
        float radius = sqrt(contourFinder.blobs[i].area/PI);
        ofCircle(center, radius);
    }
    ofPopMatrix();
    ofPopStyle();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
