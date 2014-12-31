#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    othersPanel.setup("others");
    othersPanel.add(m_pAdress.set("adress", "192.168.3.101"));
    othersPanel.add(m_pWidth.set("width", 640));
    othersPanel.add(m_pHeight.set("height", 480));
    othersPanel.setPosition(10, 300);
    othersPanel.loadFromFile("settings.xml");
    
    bluestrokGrabber = ofPtr<ofxBluestorkGrabber>(new ofxBluestorkGrabber);
	bluestrokGrabber->setCameraAddress(m_pAdress);
    
	grabber.setGrabber(bluestrokGrabber);
	grabber.initGrabber(m_pWidth,m_pHeight);

    grabberPanel.setup(bluestrokGrabber->parameters);
    
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	grabber.draw(ofGetWidth() - m_pWidth,0);

    grabberPanel.draw();
    othersPanel.draw();
    
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
