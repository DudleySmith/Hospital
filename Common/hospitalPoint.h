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
    
    string mBlobId;
    ofPoint mPosition;
    ofPoint mVelocity;
    float mAngle;
    float mRadius;
    string mCam;
    
public:
    ofPoint getPosition(){return mPosition;};
    string getBlobId(){return mBlobId;};
    string getCam(){return mCam;};
    
public:
    void update(ofxOscMessage m);
    void draw();
    
};
