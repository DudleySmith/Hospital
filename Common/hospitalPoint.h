//
//  hospitalPoint.h
//  DummyGrabberReceiver
//
//  Created by Dudley Smith on 10/02/2015.
//
//

#pragma once   

#include "ofMain.h"
#include "hospitalMessage.h"

class hospitalPoint {
    
    string mID;
    ofPoint mPosition;
    ofPoint mVelocity;
    float mAngle;
    float mRadius;
    float mTimeStampf;
    
public:
    float getTimeStampf(){return mTimeStampf;};
    ofPoint getPosition(){return mPosition;};
    string getId(){return mID;};
    
public:
    void update(ofxOscMessage m);
    void draw();
    
};
