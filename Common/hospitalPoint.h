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
    string mCam;
    
public:
    ofPoint getPosition(){return mPosition;};
    string getId(){return mID;};
    string getCam(){return mCam;};
    
public:
    void update(ofxOscMessage m);
    void draw();
    
};
