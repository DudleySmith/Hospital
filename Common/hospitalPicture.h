//
//  hospitalPicture.h
//  RibbonRenderer
//
//  Created by Dudley Smith on 21/02/2015.
//
//

#pragma once

#include "ofMain.h"
#include "hospitalMeetingPoint.h"

class hospitalPicture : public hospitalMeetingPoint{

private:
    float   mTimeStamp;
    
    float  mFadeInSec;
    float  mFadeOutSec;
    
    
public:
    
    ofImage mImage;
    
    void setup(ofImage _image);
    void draw(bool _drawDebug, bool _drawImage);
    
    void refresh(){mTimeStamp = ofGetElapsedTimef();};
    float getAscTimeRatio();
    float getDescTimeRatio();
    
    void setFadeIn(float _fadeIn){mFadeInSec = _fadeIn;};
    void setFadeOut(float _fadeOut){mFadeOutSec = _fadeOut;};
    
};
