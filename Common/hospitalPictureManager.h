//
//  hospitalPictureManager.h
//  RibbonRenderer
//
//  Created by Dudley Smith on 17/02/2015.
//
//

#pragma once

#define LIFE_TIME 5.0f

#include "ofMain.h"
#include "hospitalPicture.h"

class hospitalPictureManager {
    
    bool mIsPictureActive;
    map<string, hospitalPicture> mPicturePositions;
    
public:
    void update();
    void draw();
    
    void setPicturePositions(map<string, hospitalMeetingPoint> _pp);
    
};
