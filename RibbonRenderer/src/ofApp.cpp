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


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
	//just set up the openFrameworks stuff
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    
    // Setup Panels -------------------------
    // Osc panel
    plOsc.setup("osc", "main.xml");
    plOsc.add(psPort.set("Port", 1551, 999, 9999));
    plOsc.loadFromFile("main.xml");
    // Ribbon panel
    ribbonManager.parameters.setName("ribbons");
    plRibbons.setup(ribbonManager.parameters, "ribbons.xml");
    plRibbons.loadFromFile("ribbons.xml");
    // Ribbon panel
    ribbonManager.parametersFx.setName("ribbonsFx");
    plRibbonsFx.setup(ribbonManager.parametersFx, "ribbonsFx.xml");
    plRibbonsFx.loadFromFile("ribbonsFx.xml");
    
    // Positions
    plRibbons.setPosition(25, 25);
    plRibbonsFx.setPosition(275, 25);
    plOsc.setPosition(525, 25);
    
    mDrawGui = false;
    
    // listen on the given port
    cout << "listening for osc messages on port " << psPort << "\n";
    receiver.setup(psPort);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if (hospitalMessage::isOk(m)) {
            hospitalPoint point;
            point.update(m);
            ribbonManager.newPoint(point);
        }
    }
        
    ribbonManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ribbonManager.draw();
    
    
    if(mDrawGui){
        plOsc.draw();
        plRibbons.draw();
        plRibbonsFx.draw();
        ofDrawBitmapString("Ribbons count=" + ribbonManager.countMessage(), ofPoint(25, ofGetHeight() - 25));
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'h':
            if (mDrawGui==false) {
                mDrawGui=true;
            }else{
                mDrawGui=false;
            }
            break;
            
        default:
            break;
    }
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
