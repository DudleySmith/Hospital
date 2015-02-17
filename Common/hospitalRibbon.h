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

    vector<ofPoint> mPoints;
    string mId;
    string mCam;
    
    float mTimeStampf;
    
public:
    float getTimeStampf(){return mTimeStampf;};
    string getCam(){return mCam;};
    
public:
    void addPoint(ofPoint _point, string _cam);
    void update(float _dnSpeed);
    
    void drawCamText();
    
    void drawShape(ofColor _color
                , float _minThickness, float _maxThickness
                , int globalPointsDiv
                , float _ribbonEffectLevel, float _ribbonIdxPointsDivider, float _ribbonTimeDivider);
    
    void drawMesh(ofColor _color
                  , float _minThickness, float _maxThickness
                  , int globalPointsDiv
                  , float _ribbonEffectLevel, float _ribbonIdxPointsDivider, float _ribbonTimeDivider);
    
    bool IsThereAPointNearToMe(ofPoint _p);
    
};
