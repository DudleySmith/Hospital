//
//  hospitalRibbonsManager.h
//  RibbonRenderer
//
//  Created by Dudley Smith on 11/02/2015.
//
//

#pragma once

#include "ofMain.h"
#include "hospitalRibbon.h"
#include "hospitalPoint.h"

class hospitalRibbonsManager {
    
    map<string, hospitalRibbon> mRibbons;
    
    void addRibbon(hospitalRibbon _r);
    void addPointToRibbon(hospitalRibbon _r, ofPoint _p);
    
public:
    void newPoint(hospitalPoint _hp);
    
    void update();
    void draw();
    
};
