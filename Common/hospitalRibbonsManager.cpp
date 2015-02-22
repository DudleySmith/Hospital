//
//  hospitalRibbonsManager.cpp
//  RibbonRenderer
//
//  Created by Dudley Smith on 11/02/2015.
//
//

#include "hospitalRibbonsManager.h"

// -------------------------------------------------------------------------------------------
hospitalRibbonsManager::hospitalRibbonsManager(){
    parameters.add(pcBackGround.set("background",ofColor(0,0),ofColor(0,0),ofColor(255,255)));
    parameters.add(pcForeGround.set("foreground",ofColor(255,255),ofColor(0,0),ofColor(255,255)));
    
    parameters.add(pbDrawCurves.set("drawCurves", false));
    parameters.add(pbDrawMeshes.set("drawMeshes", false));
    parameters.add(pbDrawDebug.set("drawDebug", false));
    parameters.add(pbDrawCircles.set("drawCircles", false));
    
    parameters.add(pfFadeTime.set("fade", 3, 0, 10));
    parameters.add(pfDownSpeed.set("downSpeed", 0.5, 0, 10));
    parameters.add(pfMinThickness.set("minThickness", 2, 0, 50));
    parameters.add(pfMaxThickness.set("maxThickness", 20, 0, 50));
    parameters.add(pfMatchingDistance.set("matchingDistance", 100, 0, 500));
    
    
    parameters.add(piNbCircles.set("nbCircles", 3, 0, 10));
    parameters.add(pfCirclesRadius.set("circlesRadius", 50, 0, 250));
    parameters.add(pfCirclesFreqDivider.set("circlesFreqDivider", 0.8, 0, 5));
    parameters.add(pfCirclesRadiusDephaser.set("circlesRadiusDephaser", 0.25, 0, 3));
    
    pgRibbon1.add(pbDrawRibbon1.set("drawRibbon1", true));
    pgRibbon1.add(pfRibbon1GlobalPointsDivider.set("ribbon1globalPointDiv", 5, 1, 10));
    pgRibbon1.add(pfRibbon1EffectLevel.set("ribbon1FxLevel", 50, 0, 200));
    pgRibbon1.add(pfRibbon1IdxPointsDivider.set("ribbon1IdxDivider", 20, 1, 150));
    pgRibbon1.add(pfRibbon1TimeDivider.set("ribbon1TimeDivide", 2, 1, 10));
    parametersFx.add(pgRibbon1);
    
    pgRibbon2.add(pbDrawRibbon2.set("drawRibbon2", true));
    pgRibbon2.add(pfRibbon2GlobalPointsDivider.set("ribbon2GlobalPointDiv", 5, 1, 10));
    pgRibbon2.add(pfRibbon2EffectLevel.set("ribbon2FxLevel", 50, 0, 200));
    pgRibbon2.add(pfRibbon2IdxPointsDivider.set("ribbon2IdxDivider", 20, 1, 150));
    pgRibbon2.add(pfRibbon2TimeDivider.set("ribbon2TimeDivide", 2, 1, 10));
    parametersFx.add(pgRibbon2);
    
    pgRibbon3.add(pbDrawRibbon3.set("drawRibbon3", true));
    pgRibbon3.add(pfRibbon3GlobalPointsDivider.set("ribbon3GlobalPointDiv", 5, 1, 10));
    pgRibbon3.add(pfRibbon3EffectLevel.set("ribbon3FxLevel", 50, 0, 200));
    pgRibbon3.add(pfRibbon3IdxPointsDivider.set("ribbon3IdxDivider", 20, 1, 150));
    pgRibbon3.add(pfRibbon3TimeDivider.set("ribbon3TimeDivide", 2, 1, 10));
    parametersFx.add(pgRibbon3);
    
    pgRibbon4.add(pbDrawRibbon4.set("drawRibbon4", true));
    pgRibbon4.add(pfRibbon4GlobalPointsDivider.set("ribbon4GlobalPointDiv", 5, 1, 10));
    pgRibbon4.add(pfRibbon4EffectLevel.set("ribbon4FxLevel", 50, 0, 200));
    pgRibbon4.add(pfRibbon4IdxPointsDivider.set("ribbon4IdxDivider", 20, 1, 150));
    pgRibbon4.add(pfRibbon4TimeDivider.set("ribbon4TimeDivide", 2, 1, 10));
    parametersFx.add(pgRibbon4);
    
    pgOsc.add(psPort.set("Port", 1551, 999, 9999));
    
}

// -------------------------------------------------------------------------------------------
void hospitalRibbonsManager::setup(){
    // listen on the given port
    cout << "listening for osc messages on port " << psPort << "\n";
    oscReceiver.setup(psPort);
}

// -------------------------------------------------------------------------------------------
void hospitalRibbonsManager::newPoint(hospitalPoint _hp){
    
    string keyId = _hp.getCam() + "_" + _hp.getBlobId();
    map<string, hospitalRibbon>::iterator ribbon = mRibbons.find(keyId);
    
    if(ribbon!=mRibbons.end()){
        // Found, update
        ribbon->second.addPoint(_hp.getPosition(), _hp.getCam(), _hp.getBlobId());
    }else{
        // Not Found, create and add
        hospitalRibbon newRibbon;
        newRibbon.addPoint(_hp.getPosition(), _hp.getCam(), _hp.getBlobId());
        mRibbons[keyId] = newRibbon;
    }
    
    // Check in other positions
    map<string, hospitalRibbon>::iterator meetings;
    for(meetings = mRibbons.begin(); meetings != mRibbons.end(); meetings++){
        
        // Not the same cam !
        if(meetings->second.getBlobId() != _hp.getBlobId() || (meetings->second.getBlobId()==_hp.getBlobId() && meetings->second.getCam() != _hp.getCam())){
            
            // Get something if we meet some other
            if (meetings->second.IsThereAPointNearToMe(_hp.getPosition(), pfMatchingDistance)) {
                
                ofLogVerbose() << "Ribbon Cam:" << meetings->second.getCam() << " Id:" << meetings->second.getBlobId();
                ofLogVerbose() << "Point Cam:" << _hp.getCam() << " Id:" << _hp.getBlobId();
                
                hospitalMeetingPoint p;
                p.setPos(_hp.getPosition());
                p.setId(_hp.getBlobId() + "_" + meetings->second.getBlobId());
                mMeetings[p.getId()] = p;
                
            }
            
        }
    }
    
}

// -------------------------------------------------------------------------------------------
void hospitalRibbonsManager::update(){
    
    // check for waiting messages
    while(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);
        
        if (hospitalMessage::isOk(m)) {
            hospitalPoint point;
            point.update(m);
            newPoint(point);
        }
    }
    
    map<string, hospitalRibbon>::iterator oneRibbon;
    for (oneRibbon=mRibbons.begin(); oneRibbon!=mRibbons.end(); oneRibbon++) {
        // if remaining movement
        if((ofGetElapsedTimef() - oneRibbon->second.getTimeStampf()) <= pfFadeTime){
            // keep alive and update
            oneRibbon->second.update(pfDownSpeed);
        }else{
            mRibbons.erase(oneRibbon->first);
        }
        
    }
}

// -------------------------------------------------------------------------------------------
void hospitalRibbonsManager::draw(){
    
    ofPopStyle();
    ofEnableAlphaBlending();
    ofSetCircleResolution(36);
    
    if(pbDrawCurves==false && pbDrawMeshes==false && pbDrawDebug==false && pbDrawCircles==false){
        return;
    }
    
    map<string, hospitalRibbon>::iterator oneRibbon;
    for (oneRibbon=mRibbons.begin(); oneRibbon!=mRibbons.end(); oneRibbon++) {
        
        if (pbDrawDebug) {
            oneRibbon->second.drawCamText();
        }
        
        // Calculating ratio based on last move time
        float ratio = ofClamp((ofGetElapsedTimef() - oneRibbon->second.getTimeStampf())/pfFadeTime, 0, 1);
        // Fade to background
        ofColor drawColor = fadingColors(ratio, pcForeGround, pcBackGround);
        
        if (pbDrawCircles) {
            oneRibbon->second.drawCircles(drawColor, pfMaxThickness
                                          , piNbCircles, pfCirclesRadius
                                          , pfCirclesRadiusDephaser, pfCirclesFreqDivider);
        }
        
        // Draw some meshes if you can
        if (pbDrawMeshes==true) {
            if (pbDrawRibbon1) {
                oneRibbon->second.drawMesh(drawColor
                                           , pfMinThickness, pfMaxThickness
                                           , pfRibbon1GlobalPointsDivider
                                           , pfRibbon1EffectLevel, pfRibbon1IdxPointsDivider, pfRibbon1TimeDivider);
            }
            if (pbDrawRibbon2) {
                oneRibbon->second.drawMesh(drawColor
                                           , pfMinThickness, pfMaxThickness
                                           , pfRibbon2GlobalPointsDivider
                                           , pfRibbon2EffectLevel, pfRibbon2IdxPointsDivider, pfRibbon2TimeDivider);
            }
            if (pbDrawRibbon3) {
                oneRibbon->second.drawMesh(drawColor
                                           , pfMinThickness, pfMaxThickness
                                           , pfRibbon3GlobalPointsDivider
                                           , pfRibbon3EffectLevel, pfRibbon3IdxPointsDivider, pfRibbon3TimeDivider);
            }
            if (pbDrawRibbon4) {
                oneRibbon->second.drawMesh(drawColor
                                           , pfMinThickness, pfMaxThickness
                                           , pfRibbon4GlobalPointsDivider
                                           , pfRibbon4EffectLevel, pfRibbon4IdxPointsDivider, pfRibbon4TimeDivider);
            }
        }
        
        
        // Draw some curves in opposite color
        if (pbDrawCurves==true) {
            ofColor oppositColor = drawColor;
            oppositColor.setHueAngle(drawColor.getHueAngle() + 180);
            oneRibbon->second.drawShape(oppositColor
                                        , pfMinThickness, pfMaxThickness
                                        , pfRibbon1GlobalPointsDivider
                                        , pfRibbon1EffectLevel, pfRibbon1IdxPointsDivider, pfRibbon1TimeDivider);
        }
        
    }
    
    ofPopStyle();
}

// -------------------------------------------------------------------------------------------
ofColor hospitalRibbonsManager::fadingColors(float _ratio, ofColor _c1, ofColor _c2){
    
    ofColor result;
    
    result.r = (1 - _ratio)*_c1.r + _ratio*_c2.r;
    result.g = (1 - _ratio)*_c1.g + _ratio*_c2.g;
    result.b = (1 - _ratio)*_c1.b + _ratio*_c2.b;
    /*
    result.setHue((1 - _ratio)*_c1.getHue() + _ratio*_c2.getHue());
    result.setSaturation((1 - _ratio)*_c1.getSaturation() + _ratio*_c2.getSaturation());
    result.setBrightness((1 - _ratio)*_c1.getBrightness() + _ratio*_c2.getBrightness());
    */
    return result;
    
}
