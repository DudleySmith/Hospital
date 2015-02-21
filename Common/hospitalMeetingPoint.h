#pragma once

#include "ofMain.h"

class hospitalMeetingPoint {
    
    string mId;
    ofPoint mPos;
    
public:
    ofPoint getPos(){return mPos;};
    void setPos(ofPoint _p){mPos = _p;};
    
    string getId(){return mId;};
    void setId(string _id){mId = _id;};
    
};