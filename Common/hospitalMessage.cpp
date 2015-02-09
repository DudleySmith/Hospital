//
//  hospitalMessage.cpp
//  
//
//  Created by Dudley Smith on 09/02/2015.
//
//

#include "hospitalMessage.h"

//-----------------------------------------------------------------------------
ofxOscMessage hospitalMessage::getMessage(string _prefix, int _blobIndex, ofPoint _pos, ofPoint _vel, float _angle, float _radius){
    
    ofxOscMessage m;
    
    m.setAddress("/" + _prefix + "/blobs");
    
    m.addStringArg(ofToString(_blobIndex, 4, '0')); // ID : 0
    m.addFloatArg(_pos.x);                          // Position : x:1 y:2
    m.addFloatArg(_pos.y);
    m.addFloatArg(_radius);                         // Radius : 3
    m.addFloatArg(_vel.x);                          // Velocity : x:4 y:5
    m.addFloatArg(_vel.y);
    m.addFloatArg(_angle);                          // Angle : 6
    
    return m;
    
}

// ------------------------------------------------------------------------------------
string hospitalMessage::getID(ofxOscMessage m){
    
    string id = "";
    
    if (m.getNumArgs() >= 0) {
        id =  m.getArgAsString(0);
    }
    return id;
}

ofPoint hospitalMessage::getPosition(ofxOscMessage m){

    ofPoint position;
    
    if (m.getNumArgs() >= 2) {
        position.x = m.getArgAsFloat(1);
        position.y = m.getArgAsFloat(2);
    }
    return position;
}

ofPoint hospitalMessage::getVelocity(ofxOscMessage m){
    
    ofPoint velocity;
    
    if (m.getNumArgs() >= 5) {
        velocity.x = m.getArgAsFloat(4);
        velocity.y = m.getArgAsFloat(5);
    }
    return velocity;
}

float hospitalMessage::getAngle(ofxOscMessage m){

    float angle = 0;
    
    if (m.getNumArgs() >= 6) {
        angle =  m.getArgAsFloat(6);
    }
    return angle;
}

float hospitalMessage::getRadius(ofxOscMessage m){
    
    float radius = 0;
    
    if (m.getNumArgs() >= 3) {
        radius =  m.getArgAsFloat(3);
    }
    return radius;
}
