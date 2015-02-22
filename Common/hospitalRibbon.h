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
    string mBlobId;
    string mCam;
    
    float mTimeStampf;
    
public:
    float getTimeStampf(){return mTimeStampf;};
    string getCam(){return mCam;};
    string getBlobId(){return mBlobId;};
    
public:
    void addPoint(ofPoint _point, string _cam, string _blobId);
    void update(float _dnSpeed);
    
    void drawCamText();
    
    void drawCircles(ofColor _color, float _maxThickness
                     ,int _nbCircles, float _circlesRadius
                     ,float _radiusDephaser, float _freqDivider);
    
    void drawShape(ofColor _color
                , float _minThickness, float _maxThickness
                , int globalPointsDiv
                , float _ribbonEffectLevel, float _ribbonIdxPointsDivider, float _ribbonTimeDivider);
    
    void drawMesh(ofColor _color
                  , float _minThickness, float _maxThickness
                  , int globalPointsDiv
                  , float _ribbonEffectLevel, float _ribbonIdxPointsDivider, float _ribbonTimeDivider);
    
    bool IsThereAPointNearToMe(ofPoint _p, float _matchingDistance);
    
};
