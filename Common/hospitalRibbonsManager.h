//
//  hospitalRibbonsManager.h
//  RibbonRenderer
//
//  Created by Dudley Smith on 11/02/2015.
//
//

#pragma once

#include "ofMain.h"
#include "hospitalRibbon.h"
#include "hospitalPoint.h"

class hospitalRibbonsManager {
public:
    hospitalRibbonsManager();
    
private:
    map<string, hospitalRibbon> mRibbons;
    vector<ofPoint> mMeetings;
    
    void addRibbon(hospitalRibbon _r);
    void addPointToRibbon(hospitalRibbon _r, ofPoint _p);
    
    ofColor fadingColors(float _ratio, ofColor _c1, ofColor _c2);
    
public:
    void newPoint(hospitalPoint _hp);
    
    void setup();
    void update();
    void draw();
    
    string countMessage(){return ofToString(mRibbons.size());};
    vector<ofPoint> getMeetingPoints(){return mMeetings;};
    void emptyMeetingPoints(){mMeetings.clear();};
    
private:
    // Parameters
    ofParameter<ofColor> pcForeGround;
    ofParameter<ofColor> pcBackGround;
    
    ofParameter<bool>    pbDrawCurves;
    ofParameter<bool>    pbDrawMeshes;
    
    ofParameter<float>   pfFadeTime;
    ofParameter<float>   pfDownSpeed;
    ofParameter<float>   pfMinThickness;
    ofParameter<float>   pfMaxThickness;
    ofParameter<float>   pfMatchingDistance;
    
    ofParameterGroup   pgRibbon1;
    ofParameter<bool>  pbDrawRibbon1;
    ofParameter<int>   pfRibbon1GlobalPointsDivider;
    ofParameter<float> pfRibbon1EffectLevel;
    ofParameter<float> pfRibbon1IdxPointsDivider;
    ofParameter<float> pfRibbon1TimeDivider;
    
    ofParameterGroup   pgRibbon2;
    ofParameter<bool>  pbDrawRibbon2;
    ofParameter<int>   pfRibbon2GlobalPointsDivider;
    ofParameter<float> pfRibbon2EffectLevel;
    ofParameter<float> pfRibbon2IdxPointsDivider;
    ofParameter<float> pfRibbon2TimeDivider;
    
    ofParameterGroup   pgRibbon3;
    ofParameter<bool>  pbDrawRibbon3;
    ofParameter<int>   pfRibbon3GlobalPointsDivider;
    ofParameter<float> pfRibbon3EffectLevel;
    ofParameter<float> pfRibbon3IdxPointsDivider;
    ofParameter<float> pfRibbon3TimeDivider;
    
    ofParameterGroup   pgRibbon4;
    ofParameter<bool>  pbDrawRibbon4;
    ofParameter<int>   pfRibbon4GlobalPointsDivider;
    ofParameter<float> pfRibbon4EffectLevel;
    ofParameter<float> pfRibbon4IdxPointsDivider;
    ofParameter<float> pfRibbon4TimeDivider;
    
    
public:
    ofParameterGroup parameters;
    ofParameterGroup parametersFx;
    
    // OSC --------------------------------------
    ofxOscReceiver oscReceiver;
    
    ofParameterGroup pgOsc;
    ofParameter<int> psPort;


};
