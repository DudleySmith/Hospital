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
    
    parameters.add(pfFadeTime.set("fade", 3, 0, 10));
    parameters.add(pfDownSpeed.set("downSpeed", 0.5, 0, 10));
    parameters.add(pfMinThickness.set("minThickness", 2, 0, 50));
    parameters.add(pfMaxThickness.set("maxThickness", 20, 0, 50));
    
    parameters.add(pfGlobalPointsDivider.set("globalPointDiv", 5, 1, 10));
    parameters.add(pfRibbonEffectLevel.set("ribbonFxLevel", 50, 0, 200));
    parameters.add(pfRibbonIdxPointsDivider.set("ribbonIdxDivider", 20, 1, 150));
    parameters.add(pfRibbonTimeDivider.set("ribbonTimeDivide", 2, 1, 10));
}

// -------------------------------------------------------------------------------------------
void hospitalRibbonsManager::newPoint(hospitalPoint _hp){
    
    string keyId = _hp.getId();
    map<string, hospitalRibbon>::iterator ribbon = mRibbons.find(keyId);
    
    if(ribbon!=mRibbons.end()){
        // Found, update
        ribbon->second.addPoint(_hp.getPosition());
    }else{
        // Not Found, create and add
        hospitalRibbon newRibbon;
        newRibbon.addPoint(_hp.getPosition());
        mRibbons[keyId] = newRibbon;
    }
    
}

// -------------------------------------------------------------------------------------------
void hospitalRibbonsManager::update(){
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
    
    ofBackground(pcBackGround);
    
    if(pbDrawCurves==false && pbDrawMeshes==false){
        return;
    }
    
    map<string, hospitalRibbon>::iterator oneRibbon;
    for (oneRibbon=mRibbons.begin(); oneRibbon!=mRibbons.end(); oneRibbon++) {
        // Calculating ratio based on last move time
        float ratio = ofClamp((ofGetElapsedTimef() - oneRibbon->second.getTimeStampf())/pfFadeTime, 0, 1);
        // Fade to background
        ofColor drawColor = fadingColors(ratio, pcForeGround, pcBackGround);

        // Draw some meshes if you can
        if (pbDrawMeshes==true) {
            oneRibbon->second.drawMesh(drawColor
                                       , pfMinThickness, pfMaxThickness
                                       , pfGlobalPointsDivider
                                       , pfRibbonEffectLevel, pfRibbonIdxPointsDivider, pfRibbonTimeDivider);
        }
        
        // Draw some curves in opposite color
        if (pbDrawCurves==true) {
            ofColor oppositColor = drawColor;
            oppositColor.setHueAngle(drawColor.getHueAngle() + 180);
            oneRibbon->second.drawShape(oppositColor
                                        , pfMinThickness, pfMaxThickness
                                        , pfGlobalPointsDivider
                                        , pfRibbonEffectLevel, pfRibbonIdxPointsDivider, pfRibbonTimeDivider);
        }
        
    }
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