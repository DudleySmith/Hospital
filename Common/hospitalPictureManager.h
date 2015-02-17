//
//  hospitalPictureManager.h
//  RibbonRenderer
//
//  Created by Dudley Smith on 17/02/2015.
//
//

#pragma once

#include "ofMain.h"

class hospitalPictureManager {
    
    bool mIsPictureActive;
    vector<ofPoint> mPicturePositions;
    
public:
    void update();
    void draw();
    
    void setPicturePositions(vector<ofPoint> _pp){mPicturePositions = _pp;};
    
};
