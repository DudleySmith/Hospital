#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // Setup panels
    setupPanelOpenCv();
    setupPanelSource();
    
    switch (m_pSource) {
        case 0:
            // Ip camera -----------
            setupIpCamera();
            setupPanelGrabber();
            break;
        case 1:
            // Movei file
            vidPlayer.loadMovie(m_pMoviePath);
            vidPlayer.play();
            break;
        case 2:
            // Kinect -----
            setupKinect();
            break;
            
        default:
            break;
    }

    setupOpenCv();
    setupOsc();
    
}

//--------------------------------------------------------------
void ofApp::setupPanelGrabber(){
    bluestrokGrabber = ofPtr<ofxBluestorkGrabber>(new ofxBluestorkGrabber);
    bluestrokGrabber->setCameraAddress(m_pAdress);
    
    grabberPanel.setup(bluestrokGrabber->parameters);
    grabberPanel.setPosition(20, 550);
}

//--------------------------------------------------------------
void ofApp::setupIpCamera(){
    videoGrabber.setGrabber(bluestrokGrabber);
    videoGrabber.initGrabber(m_pGrabWidth,m_pGrabHeight);
    
}

//--------------------------------------------------------------
void ofApp::setupPanelSource(){
    cameraPanel.setup("grabber", "grabber.xml");
    
    cameraPanel.add(m_pSource.set("source", 0, 0, 2));
    cameraPanel.add(m_pSourceDisplay.set("sourceDisplay", ""));
    cameraPanel.add(m_pMoviePath.set("moviePath", "fingers.mov"));
    cameraPanel.add(m_pAdress.set("adress", "192.168.3.101"));
    cameraPanel.add(m_pGrabWidth.set("grabWidth", 320, 320, 1280));
    cameraPanel.add(m_pGrabHeight.set("grabHeight", 240, 240, 768));
    cameraPanel.add(m_pDrawWidth.set("drawWidth", 320, 320, 1280));
    cameraPanel.add(m_pDrawHeight.set("drawHeight", 240, 240, 768));
    
    cameraPanel.setPosition(260, 550);
    cameraPanel.loadFromFile("grabber.xml");
    
}

//--------------------------------------------------------------
void ofApp::setupPanelOpenCv(){
    // Panel setup
    m_pSetBg.addListener(this,&ofApp::setBgPressed);
    
    openCvPanel.setup("openCv", "openCv.xml");
    openCvPanel.add(m_pTwoThreshold.set("twoThreshold", true));
    openCvPanel.add(m_pThreshold.set("threshold", 80, 0, 255));
    openCvPanel.add(m_pNearThreshold.set("nearThreshold", 80, 0, 255));
    openCvPanel.add(m_pFarThreshold.set("farRhreshold", 80, 0, 255));
    openCvPanel.add(m_pBrightness.set("brightness", 0, -1, 1));
    openCvPanel.add(m_pContrast.set("contrast", 0, -1, 1));
    openCvPanel.add(m_pBlur.set("blur", 0, 0, 100));
    openCvPanel.add(m_pSetBg.setup("Set Background"));
    openCvPanel.add(m_pDraw.setup("Draw", true));
    openCvPanel.loadFromFile("openCv.xml");
    openCvPanel.setPosition(500, 550);
}

//--------------------------------------------------------------
void ofApp::setupOpenCv(){
    colorImg.allocate(m_pGrabWidth,m_pGrabHeight);
    grayImage.allocate(m_pGrabWidth,m_pGrabHeight);
    grayThreshNear.allocate(m_pGrabWidth,m_pGrabHeight);
    grayThreshFar.allocate(m_pGrabWidth,m_pGrabHeight);
    grayBg.allocate(m_pGrabWidth,m_pGrabHeight);
    grayDiff.allocate(m_pGrabWidth,m_pGrabHeight);
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
    
    oscPanel.setPosition(750, 550);
    oscPanel.loadFromFile("osc.xml");
    
    oscSender.setup(m_pHost, m_pPort);

}

void ofApp::setupKinect(){
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();		// opens first available kinect
    //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    m_pGrabWidth = (int)kinect.width;
    m_pGrabHeight = (int)kinect.height;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    bool bNewFrame = false;
    
    switch (m_pSource) {
        case 0:
            // Ip camera -----------
            updateIpCam();
            bNewFrame = videoGrabber.isFrameNew();
            if (bNewFrame==true) {
                colorImg.setFromPixels(videoGrabber.getPixels(), m_pGrabWidth, m_pGrabHeight);
                grayImage = colorImg;
            }
            break;
        case 1:
            // Movei file
            vidPlayer.update();
            bNewFrame = vidPlayer.isFrameNew();
            if (bNewFrame==true) {
                colorImg.setFromPixels(vidPlayer.getPixels(), m_pGrabWidth, m_pGrabHeight);
                grayImage = colorImg;
            }
            break;
        case 2:
            // Kinect -----
            updateKinect();
            bNewFrame = kinect.isFrameNew();
            if (bNewFrame==true) {
                grayImage.setFromPixels(kinect.getDepthPixels(), m_pGrabWidth, m_pGrabHeight);
            }
            break;
            
        default:
            break;
    }
    
    if (bNewFrame==true) {
        updateOpenCv();
        updateOsc();
    }
}

//--------------------------------------------------------------
void ofApp::updateIpCam(){
    videoGrabber.update();
}

//--------------------------------------------------------------
void ofApp::updateKinect(){
    kinect.update();
}


//--------------------------------------------------------------
void ofApp::updateOpenCv(){
    
    grayImage.brightnessContrast(m_pBrightness, m_pContrast);
    
    // take the abs value of the difference between background and incoming and then threshold:
    
    if(m_pTwoThreshold == true){
        // New way
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(m_pNearThreshold, true);
        grayThreshFar.threshold(m_pFarThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayDiff.getCvImage(), NULL);
    }else{
        // Old way : one threshold
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(m_pThreshold);
    }

    if(m_pBlur > 0){
        //grayDiff.blurHeavily();
        grayDiff.blurGaussian(m_pBlur);
    }
    
    
    
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
    m.addFloatArg((float)(_center.x)/m_pGrabWidth);
    m.addFloatArg((float)(_center.y)/m_pGrabHeight);
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
    
    switch (m_pSource) {
        case 0:
            m_pSourceDisplay = "IP camera";
            break;
        case 1:
            m_pSourceDisplay = "movie file";
            break;
        case 2:
            m_pSourceDisplay = "kinect";
            break;
            
        default:
            break;
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
    
    // Nove all drawings ----------------------------
    ofPushMatrix();
    ofTranslate(20, 20);
    
    // ---------------------------------------------
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);
    switch (m_pSource) {
        case 0:
        case 1:
            colorImg.draw(0, 0, m_pDrawWidth, m_pDrawHeight);
            ofDrawBitmapString("colorImg", 0.5*m_pDrawWidth, 0.5*m_pDrawWidth);
            break;
            // Draw movie here
            //break;
        case 2:
            kinect.drawDepth(0, 0, m_pDrawWidth, m_pDrawHeight);
            ofDrawBitmapString("kinectDepth", 0.5*m_pDrawWidth, 0.5*m_pDrawWidth);
            break;
        default:
            break;
    }
    
    // ---------------------------------------------
    ofPushMatrix();
    ofTranslate(320, 0);
    grayImage.draw(0,0,m_pDrawWidth, m_pDrawHeight);
    ofDrawBitmapString("grayImage", 0.5*m_pDrawWidth, 0.5*m_pDrawWidth);
    ofPopMatrix();
    
    // ---------------------------------------------
    ofPushMatrix();
    ofTranslate(0, 240);
    grayBg.draw(0,0,m_pDrawWidth, m_pDrawHeight);
    ofDrawBitmapString("grayBg", 0.5*m_pDrawWidth, 0.5*m_pDrawWidth);
    ofPopMatrix();
    
    // ---------------------------------------------
    ofPushMatrix();
    ofTranslate(320,240);
    grayDiff.draw(0,0,m_pDrawWidth, m_pDrawHeight);
    ofDrawBitmapString("grayDiff", 0.5*m_pDrawWidth, 0.5*m_pDrawWidth);
    ofPopMatrix();
    
    // ---------------------------------------------
    ofPushMatrix();
    ofTranslate(0,480);
    grayThreshNear.draw(0,0,m_pDrawWidth, m_pDrawHeight);
    ofDrawBitmapString("grayThreshNear", 0.5*m_pDrawWidth, 0.5*m_pDrawWidth);
    ofPopMatrix();
    
    // ---------------------------------------------
    ofPushMatrix();
    ofTranslate(320,480);
    grayThreshFar.draw(0,0,m_pDrawWidth, m_pDrawHeight);
    ofDrawBitmapString("grayThreshFar", 0.5*m_pDrawWidth, 0.5*m_pDrawWidth);
    ofPopMatrix();
    
    
    // ---------------------------------------------
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
    // ---------------------------------------------
    
    ofPopMatrix();
    
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
