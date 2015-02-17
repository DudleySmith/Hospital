//
//  hospitalPictureManager.cpp
//  RibbonRenderer
//
//  Created by Dudley Smith on 17/02/2015.
//
//

#include "hospitalPictureManager.h"

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::update(){
    //mPicturePositions.clear();
}
// -------------------------------------------------------------------------------------------
void hospitalPictureManager::draw(){
    
    vector<ofPoint>::iterator pos;
    
    for (pos = mPicturePositions.begin(); pos != mPicturePositions.end(); pos++) {
        ofCircle(*pos, 25);
    }
    
}
