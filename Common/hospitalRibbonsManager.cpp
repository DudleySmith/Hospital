//
//  hospitalRibbonsManager.cpp
//  RibbonRenderer
//
//  Created by Dudley Smith on 11/02/2015.
//
//

#include "hospitalRibbonsManager.h"

// -------------------------------------------------------------------------------------------
void hospitalRibbonsManager::addRibbon(hospitalRibbon _r){
    
}
void hospitalRibbonsManager::addPointToRibbon(hospitalRibbon _r, ofPoint _p){
    
}

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
void hospitalRibbonsManager::update(){
    map<string, hospitalRibbon>::iterator oneRibbon;
    for (oneRibbon=mRibbons.begin(); oneRibbon!=mRibbons.end(); oneRibbon++) {
        oneRibbon->second.update();
    }
}
void hospitalRibbonsManager::draw(){
    map<string, hospitalRibbon>::iterator oneRibbon;
    for (oneRibbon=mRibbons.begin(); oneRibbon!=mRibbons.end(); oneRibbon++) {
        oneRibbon->second.draw();
    }
}