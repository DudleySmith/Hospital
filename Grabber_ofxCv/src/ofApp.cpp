#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    setupCamera();
    setupGrabber();
    setupOpenCv();
    setupOsc();
    
    int idxPanels = 0;
    grabberPanel    .setPosition(20 + (idxPanels++)*250, 550);
    cameraPanel     .setPosition(20 + (idxPanels++)*250, 550);
    oscPanel        .setPosition(20 + (idxPanels++)*250, 550);
    openCvPanel     .setPosition(20 + (idxPanels++)*250, 450);
    
    setBgPressed();
    
}

//--------------------------------------------------------------
void ofApp::setupGrabber(){
    
    grabber.setGrabber(bluestrokGrabber);
    grabber.initGrabber(m_pWidth,m_pHeight);
    
    grabberPanel.setup(bluestrokGrabber->parameters);
}

//--------------------------------------------------------------
void ofApp::setupCamera(){
    cameraPanel.setup("grabber", "grabber.xml");
    cameraPanel.add(m_pAdress.set("adress", "192.168.3.101"));
    cameraPanel.add(m_pWidth.set("width", 320));
    cameraPanel.add(m_pHeight.set("height", 240));
    cameraPanel.loadFromFile("grabber.xml");
    
    bluestrokGrabber = ofPtr<ofxBluestorkGrabber>(new ofxBluestorkGrabber);
    bluestrokGrabber->setCameraAddress(m_pAdress);
}

//--------------------------------------------------------------
void ofApp::setupOpenCv(){
    
    // Panel setup
    m_pSetBg.addListener(this,&ofApp::setBgPressed);
    
    openCvPanel.setup("openCv", "openCv.xml");
    openCvPanel.add(m_pMinArea.set("minArea", 1, 0, 255));
    openCvPanel.add(m_pMaxArea.set("maxArea", 100, 0, 255));
    openCvPanel.add(m_pThreshold.set("threshold", 15, 0, 255));
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
    oscPanel.add(m_pPrefix.set("prefix", "cam"));
    oscPanel.add(m_pIdxCam.set("idxCam", 1, 1, 3));
    oscPanel.add(m_pRandomId.set("randomId", false));
    oscPanel.add(m_pIdSent.set("IdSent", ""));
    oscPanel.add(m_pRevertX.set("revertX", false));
    oscPanel.add(m_pRevertY.set("revertY", false));
    
    oscPanel.loadFromFile("osc.xml");
    
    oscSender.setup(m_pHost, m_pPort);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    string idSent = "";
    idSent += m_pPrefix;
    idSent += ofToString(m_pIdxCam, 1, '0');
    
    m_pIdSent = idSent;
    
    
    grabber.update();
    
    if (grabber.isFrameNew()==true) {
        updateOpenCv();
        updateOsc();
    }
    
    
    int millis = ofGetElapsedTimeMillis() % 5000;
    if(millis == 0){
        // Alive !!!!!!
        ofxOscMessage aliveMess;
        string prefix = m_pPrefix;
        
        aliveMess.setAddress("/" + prefix + "/Alive");
        aliveMess.addStringArg("yes");
        aliveMess.addFloatArg(0.5);
        aliveMess.addIntArg(millis);
        oscSender.sendMessage(aliveMess);
        
    }
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
    
    colorImg.setFromPixels(grabber.getPixels(), 320,240);
    
    grayImage = colorImg;
    grayImage.brightnessContrast(m_pBrightness, m_pContrast);
    
    // take the abs value of the difference between background and incoming and then threshold:
    grayDiff.absDiff(grayBg, grayImage);
    
    if(m_pBlur)
        grayDiff.blurHeavily();
    grayDiff.brightnessContrast(m_pResultBrightness, m_pResultContrast);
    //grayDiff.threshold(m_pThreshold);
    
    // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    // also, find holes is set to true so we will get interior contours as well....
    
    contourFinder.findContours(grayDiff);	// find holes
    
}


//--------------------------------------------------------------
void ofApp::updateOsc(){
    
    //if (reportOsc.length() > 1000) {
    reportOsc = "";
    //}
    
   for(int i = 0; i < contourFinder.size(); i++) {
       
       if(m_pRandomId==true){
           m_pIdxCam = 1 + contourFinder.getLabel(i)%3;
       }
        
        ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
        float radius = sqrt(contourFinder.getContourArea(i)/PI);
        
        ofxOscMessage m = getMessage(contourFinder.getLabel(i), center, radius);
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
    string prefix = m_pIdSent;
    
    m.setAddress("/" + prefix + "/blobs");
    m.addIntArg(_blobIndex);
    
    // Map between -1 , +1
    _center.x = ofMap(_center.x, 0, m_pWidth, -1, 1);
    _center.y = ofMap(_center.y, 0, m_pHeight, -1, 1);
    
    ofPoint realCenter;
    realCenter.x = m_pRevertX == true ? -1 * _center.x : _center.x;
    realCenter.y = m_pRevertY == true ? -1 * _center.y : _center.y;
    

    m.addFloatArg(realCenter.x);
    m.addFloatArg(realCenter.y);
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
    << "num blobs found " << contourFinder.size() << endl
    << "fps: " << ofGetFrameRate();
    
    //ofDrawBitmapString(reportStr.str(), 750, 360);
    ofLogVerbose() << reportStr.str();
    
    // Report OSC
    //ofDrawBitmapString(reportOsc, 750, 500);
    
}

//--------------------------------------------------------------
void ofApp::drawOpenCv(){
    
    //ofSetBackgroundAuto(m_pShowLabels);
    ofxCv::RectTracker& tracker = contourFinder.getTracker();
    
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);
    colorImg.draw(20,20);
    grayImage.draw(360,20);
    grayBg.draw(20,280);
    grayDiff.draw(360,280);
    
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
    /*
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofPoint center = contourFinder.blobs[i].boundingRect.getCenter();
        float radius = sqrt(contourFinder.blobs[i].area/PI);
        ofCircle(center, radius);
    }
    */
    
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
