//
//  hospitalMessage.h
//  
//
//  Created by Dudley Smith on 09/02/2015.
//
//

#pragma once

#include "ofMain.h"

#include "ofxOsc.h"

class hospitalMessage {
    
public:
    // --------
    // Send side
    static ofxOscMessage getMessage(string _prefix, int _blobLabel, ofPoint _pos, ofPoint _vel, float _angle, float _radius);
    
    // ---------
    // Receive side
    static bool isOk(ofxOscMessage m);
    static string getID(ofxOscMessage m);
    static ofPoint getPosition(ofxOscMessage m);
    static ofPoint getVelocity(ofxOscMessage m);
    static float getAngle(ofxOscMessage m);
    static float getRadius(ofxOscMessage m);
    
};