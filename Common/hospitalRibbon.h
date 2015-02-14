//
//  hospitalRibbon.h
//  RibbonRenderer
//
//  Created by Dudley Smith on 10/02/2015.
//
//

#pragma once

#include "ofMain.h"

class hospitalRibbon {

    vector<ofPoint> points;
    string Id;
    
    float mTimeStampf;
    
public:
    float getTimeStampf(){return mTimeStampf;};
    
public:
    void addPoint(ofPoint _point);
    void update();
    void draw(ofColor _color);
    void drawShape(ofColor _color);
    void drawMesh(ofColor _color);
    
};
