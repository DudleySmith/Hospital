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
    float  mTimeStamp;
    
public:
    void refresh(){mTimeStamp = ofGetElapsedTimef();};
    float getTimeStamp(){return mTimeStamp;};
    
};
