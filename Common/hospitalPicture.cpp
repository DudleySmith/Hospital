//
//  hospitalPicture.cpp
//  RibbonRenderer
//
//  Created by Dudley Smith on 21/02/2015.
//
//

#include "hospitalPicture.h"

//---------------------------------------------------------------------------
void hospitalPicture::setup(ofImage _image){
    
    if(_image.isAllocated()){
        mImage = _image;
    }
    
}

//---------------------------------------------------------------------------
void hospitalPicture::draw(bool _drawDebug, bool _drawImage){
    
    ofPushStyle();
    
    if (_drawDebug == true) {
        ofCircle(mPos, 25 * getAscTimeRatio());
        ofDrawBitmapString(ofToString(getAscTimeRatio()), mPos);
    }
    
    if (_drawImage && mImage.isAllocated()) {
        ofSetColor(ofColor::white, 255*getAscTimeRatio());
        ofEnableAlphaBlending();
        
        ofPoint centerPos;
        centerPos.set(mPos.x - 0.5f * (float)mImage.width, mPos.y - 0.5f * (float)mImage.height);
        mImage.draw(centerPos);
        
        ofDisableAlphaBlending();
    }
    
    ofPopStyle();
    
}

//---------------------------------------------------------------------------
float hospitalPicture::getAscTimeRatio(){
    
    float timeRatio = 1.0f;
    float myTimeElapsed = ofGetElapsedTimef() - mTimeStamp;
    
    if(mFadeInSec>0 && mFadeOutSec>0){
       
        if(myTimeElapsed < mFadeInSec){
            timeRatio = myTimeElapsed / mFadeInSec;
            
        }else if (myTimeElapsed < (mFadeInSec+mFadeOutSec)){
            timeRatio = 1 - ((myTimeElapsed-mFadeInSec) / mFadeOutSec);
            
        }
        
    }else{
        timeRatio = 1.0f;
    }
    
    //ofLogVerbose() << "TimeRatio = " << ofToString(timeRatio) << " Fade In=" << ofToString(mFadeInSec) << " Fade Out=" << ofToString(mFadeOutSec);
    
    return timeRatio;
    
}
//---------------------------------------------------------------------------
float hospitalPicture::getDescTimeRatio(){
    return 1 - getAscTimeRatio();
}
