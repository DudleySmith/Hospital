#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_ERROR);
    
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
    
    openCvPanel.setPosition(700, 400);
    oscPanel.setPosition(1000, 400);
    grabberPanel.setPosition(1000, 550);
    cameraPanel.setPosition(1000, 550);
    
}

//--------------------------------------------------------------
void ofApp::setupIpCamera(){
    videoGrabber.setGrabber(bluestrokGrabber);
    videoGrabber.initGrabber(m_pGrabWidth,m_pGrabHeight);
    
}

//--------------------------------------------------------------
void ofApp::setupPanelGrabber(){
    bluestrokGrabber = ofPtr<ofxBluestorkGrabber>(new ofxBluestorkGrabber);
    bluestrokGrabber->setCameraAddress(m_pAdress);
    
    grabberPanel.setup(bluestrokGrabber->parameters);
}

//--------------------------------------------------------------
void ofApp::setupPanelOpenCv(){
    // Panel setup
    m_pSetBg.addListener(this,&ofApp::setBgPressed);
    
    openCvPanel.setup("openCv", "openCv.xml");
    openCvPanel.add(m_pMinArea.set("minArea", 1, 0, 255));
    openCvPanel.add(m_pMaxArea.set("maxArea", 100, 0, 255));
    openCvPanel.add(m_pTwoThreshold.set("twoThreshold", true));
    openCvPanel.add(m_pThreshold.set("threshold", 80, 0, 255));
    openCvPanel.add(m_pNearThreshold.set("nearThreshold", 80, 0, 255));
    openCvPanel.add(m_pFarThreshold.set("farRhreshold", 80, 0, 255));
    openCvPanel.add(m_pPersistence.set("persistence", 15, 0, 255));
    openCvPanel.add(m_pMaxDistance.set("maxDistance", 32, 0, 255));
    
    openCvPanel.add(m_pBrightness.set("brightness", 0, -1, 1));
    openCvPanel.add(m_pContrast.set("contrast", 0, -1, 1));
    openCvPanel.add(m_pBlur.set("blur", false));
    openCvPanel.add(m_pResultBrightness.set("ResultBrightness", 0, -1, 1));
    openCvPanel.add(m_pResultContrast.set("ResultContrast", 0, -1, 1));
    
    openCvPanel.add(m_pSetBg.setup("Set Background"));
    openCvPanel.add(m_pDraw.setup("Draw", true));
    openCvPanel.add(m_pShowLabels.setup("labels", true));
    
    openCvPanel.loadFromFile("openCv.xml");
    
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
    
    cameraPanel.loadFromFile("grabber.xml");
    
}

//--------------------------------------------------------------
void ofApp::setupOpenCv(){
    colorImg.allocate(m_pGrabWidth,m_pGrabHeight);
    grayImage.allocate(m_pGrabWidth,m_pGrabHeight);
    grayThreshNear.allocate(m_pGrabWidth,m_pGrabHeight);
    grayThreshFar.allocate(m_pGrabWidth,m_pGrabHeight);
    grayBg.allocate(m_pGrabWidth,m_pGrabHeight);
    grayDiff.allocate(m_pGrabWidth,m_pGrabHeight);
}


//--------------------------------------------------------------
void ofApp::setupOsc(){
    
    oscPanel.setup("osc", "osc.xml");
    oscPanel.add(m_pReady.set("ready", false));
    oscPanel.add(m_pSend.set("send", true));
    oscPanel.add(m_pHost.set("host", "127.0.0.1"));
    oscPanel.add(m_pPort.set("port", 9001));
    oscPanel.add(m_pPrefix.set("prefix", "cam1"));
    
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
    
    bNewFrame = false;
    
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
    
    contourFinder.setMinAreaRadius(m_pMinArea);
    contourFinder.setMaxAreaRadius(m_pMaxArea);
    contourFinder.setThreshold(m_pThreshold);
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(m_pPersistence);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(m_pMaxDistance);
    
    //colorImg.setFromPixels(grabber.getPixels(), 320,240);
    grayImage = colorImg;
    
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
    grayDiff.brightnessContrast(m_pResultBrightness, m_pResultContrast);
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    contourFinder.findContours(grayDiff);	// find holes

}


//--------------------------------------------------------------
void ofApp::updateOsc(){
    
    //if (reportOsc.length() > 1000) {
    reportOsc.str("");
    reportOsc.clear();
    //}
    
    for(int i = 0; i < contourFinder.size(); i++) {
        
        if(m_pRandomId==true){
            m_pIdxCam = 1 + contourFinder.getLabel(i)%3;
        }
        
        ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
        ofPoint velocity = ofxCv::toOf(contourFinder.getVelocity(i));
        float angle = velocity.angle(ofPoint(1,0));
        float radius = sqrt(contourFinder.getContourArea(i)/PI);
        /*
         
         old way
         
        ofxOscMessage m = getMessage(contourFinder.getLabel(i), center, velocity, angle, radius);
        */
        
        // new way
        velocity.x = (float)(velocity.x)/(float)m_pGrabWidth;
        velocity.y = (float)(velocity.y)/(float)m_pGrabHeight;
        
        center.x = (float)(center.x)/(float)m_pGrabWidth;
        center.y = (float)(center.y)/(float)m_pGrabHeight;
        
        ofxOscMessage m = hospitalMessage::getMessage(m_pPrefix, contourFinder.getLabel(i), center, velocity, angle, radius);
        
        oscSender.sendMessage(m);
        
        reportOsc << "Message Osc : " << endl;
        reportOsc << m.getAddress() + ":" ;
        
        vector<ofxOscArg>::iterator arg;
        for (int nbArgs=0; nbArgs<m.getNumArgs(); nbArgs++) {
            reportOsc << m.getArgAsString(nbArgs);
            if(nbArgs<(m.getNumArgs()-1)){
                reportOsc << " : " << endl;
            }
        }
        
        ofLogVerbose() << reportOsc.str();
        
    }
    
}

//-----------------------------------------------------------------------------
void ofApp::updatePoints(){
    if (bNewFrame==true) {
        updateOpenCv();
        updateOsc();
    }
}

//-----------------------------------------------------------------------------
ofxOscMessage ofApp::getMessage(int _blobIndex, ofPoint _pos, ofPoint _vel, float _angle, float _radius){
    
    ofxOscMessage m;
    string prefix = m_pPrefix;
    
    m.setAddress("/" + prefix + "/blobs");
    
    m.addStringArg(ofToString(_blobIndex, 4, '0'));
    m.addFloatArg((float)(_pos.x)/(float)m_pGrabWidth);
    m.addFloatArg((float)(_pos.y)/(float)m_pGrabHeight);
    m.addFloatArg(_radius);
    m.addFloatArg((float)(_vel.x)/(float)m_pGrabWidth);
    m.addFloatArg((float)(_vel.y)/(float)m_pGrabHeight);
    m.addFloatArg(_angle);
    
    return m;
    
}

//--------------------------------------------------------------
void ofApp::setBgPressed(){
    grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    updatePoints();
    
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
    << "num blobs found " << contourFinder.size() << endl
    << "fps: " << ofGetFrameRate();
    
    if (reportStr.str().size() > 0) {
        ofDrawBitmapString(reportStr.str(), 700, 275);
    }
    if (reportOsc.str().size() > 0) {
        ofDrawBitmapString(reportOsc.str(), 700, 350);
    }

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
    
    ofxCv::RectTracker& tracker = contourFinder.getTracker();
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(650, 0);
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::darkGrey);
    ofRect(0, 0,m_pDrawWidth,m_pDrawHeight);
    ofPopStyle();
    
    ofPushMatrix();
    float xScale = (float)m_pDrawWidth/m_pGrabWidth;
    float yScale = (float)m_pDrawHeight/m_pGrabHeight;
    
    //ofLogVerbose() << "Scale : " << ofToString(xScale) << ":" << ofToString(yScale);
    
    ofScale(xScale, yScale);
    
    ofNoFill();
    ofSetColor(ofColor::white);
    ofSetLineWidth(1);
    
    if(m_pShowLabels) {
        ofSetColor(255);
        //movie.draw(0, 0);
        contourFinder.draw();
        for(int i = 0; i < contourFinder.size(); i++) {
            ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
            
            ofPushMatrix();
            ofTranslate(center.x, center.y);
            
            int label = contourFinder.getLabel(i);
            string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
            ofDrawBitmapString(msg, 0, 0);
            
            float radius = sqrt(contourFinder.getContourArea(i)/PI);
            ofCircle(0, 0, radius);
            
            ofPushMatrix();
            ofVec2f velocity = ofxCv::toOf(contourFinder.getVelocity(i));
            ofScale(5, 5);
            ofLine(0, 0, velocity.x, velocity.y);
            ofPopMatrix();
            
            ofPopMatrix();
            
        }
    } else {
        for(int i = 0; i < contourFinder.size(); i++) {
            unsigned int label = contourFinder.getLabel(i);
            // only draw a line if this is not a new label
            if(tracker.existsPrevious(label)) {
                // use the label to pick a random color
                ofSeedRandom(label << 24);
                ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
                // get the tracked object (cv::Rect) at current and previous position
                const cv::Rect& previous = tracker.getPrevious(label);
                const cv::Rect& current = tracker.getCurrent(label);
                // get the centers of the rectangles
                ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
                ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
                ofLine(previousPosition, currentPosition);
            }
        }
    }
    
    // this chunk of code visualizes the creation and destruction of labels
    const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
    const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
    const vector<unsigned int>& newLabels = tracker.getNewLabels();
    const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
    ofSetColor(ofxCv::cyanPrint);
    for(int i = 0; i < currentLabels.size(); i++) {
        int j = currentLabels[i];
        ofLine(j, 0, j, 4);
    }
    ofSetColor(ofxCv::magentaPrint);
    for(int i = 0; i < previousLabels.size(); i++) {
        int j = previousLabels[i];
        ofLine(j, 4, j, 8);
    }
    ofSetColor(ofxCv::yellowPrint);
    for(int i = 0; i < newLabels.size(); i++) {
        int j = newLabels[i];
        ofLine(j, 8, j, 12);
    }
    ofSetColor(ofColor::white);
    for(int i = 0; i < deadLabels.size(); i++) {
        int j = deadLabels[i];
        ofLine(j, 12, j, 16);
    }
    
    ofPopMatrix();
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
