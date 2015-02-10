//
//  hospitalPoint.cpp
//  DummyGrabberReceiver
//
//  Created by Dudley Smith on 10/02/2015.
//
//

#include "hospitalPoint.h"

// -------------------------------------------------------------------------
void hospitalPoint::update(ofxOscMessage m){
    
    if(hospitalMessage::isOk(m) == true){
        
        mID = hospitalMessage::getID(m);
        
        mPosition = hospitalMessage::getPosition(m);
        mPosition *= ofPoint(ofGetWidth(), ofGetHeight());
        
        mVelocity = hospitalMessage::getVelocity(m);
        mAngle = hospitalMessage::getAngle(m);
        mRadius = hospitalMessage::getRadius(m);
        
        mTimeStampf = ofGetElapsedTimef();
        
    }
    
}

// -------------------------------------------------------------------------
void hospitalPoint::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(mPosition);
    
    ofNoFill();
    ofSetColor(ofColor::white);
    
    ofCircle(ofPoint::zero(), mRadius);
    ofLine(ofPoint::zero(), mVelocity);
    ofDrawBitmapString(mID, ofPoint::zero());
    
    ofPopStyle();
    
}