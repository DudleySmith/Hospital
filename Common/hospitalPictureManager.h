//
//  hospitalPictureManager.h
//  RibbonRenderer
//
//  Created by Dudley Smith on 17/02/2015.
//
//

#pragma once



#include "ofMain.h"
#include "hospitalPicture.h"

class hospitalPictureManager {

public:
    hospitalPictureManager();

private:
    bool mIsPictureActive;
    map<string, hospitalPicture> mPicturePositions;
    
public:
    void setup();
    void update();
    void draw();
    
    void setPicturePositions(map<string, hospitalMeetingPoint> _pp);
    
    ofParameterGroup     pgPictures;
    ofParameter<bool>    pbDrawDebug;
    ofParameter<bool>    pbDrawImage;
    ofParameter<string>  psPicturesPath;
    ofParameter<float>   pfFadeInInSec;
    ofParameter<float>   pfFadeOutInSec;
    ofParameter<float>   pfResizeRatio;
    ofParameter<int>     piNbImages;
    
    
};
