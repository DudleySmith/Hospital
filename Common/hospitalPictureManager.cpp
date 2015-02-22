//
//  hospitalPictureManager.cpp
//  RibbonRenderer
//
//  Created by Dudley Smith on 17/02/2015.
//
//

#include "hospitalPictureManager.h"

// -------------------------------------------------------------------------------------------
hospitalPictureManager::hospitalPictureManager(){
    
    pgPictures.add(pbDrawDebug.set("drawDebug", true));
    pgPictures.add(pbDrawImage.set("drawImage", true));
    pgPictures.add(psPicturesPath.set("path", "/pictures"));
    pgPictures.add(pfFadeInInSec.set("fadeIn", 5, 0, 20));
    pgPictures.add(pfFadeOutInSec.set("fadeOur", 5, 0, 20));
    pgPictures.add(pfResizeRatio.set("resizeRatio", 0.25, 0, 1));
    pgPictures.add(piNbImages.set("nbImages", 3, 0, 50));
    
}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::setup(){
    
}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::update(){
    
    map<string, hospitalPicture>::iterator pic;
    
    for (pic = mPicturePositions.begin(); pic != mPicturePositions.end(); pic++) {
        if(pic->second.getAscTimeRatio() >= 1.0f){
            mPicturePositions.erase(pic);
        }
    }

}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::draw(){
    
    ofPushStyle();
    ofNoFill();
    
    map<string, hospitalPicture>::iterator pic;
    
    for (pic = mPicturePositions.begin(); pic != mPicturePositions.end(); pic++) {
        pic->second.draw(pbDrawDebug, pbDrawImage, "pictures/HiCulture_00.jpg", pfResizeRatio);
    }
    
    ofPopStyle();
    
}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::setPicturePositions(map<string, hospitalMeetingPoint> _pp){
    
    map<string, hospitalMeetingPoint>::iterator meetingPoint;
    
    for(meetingPoint = _pp.begin(); meetingPoint != _pp.end(); meetingPoint++){
        
        map<string, hospitalPicture>::iterator foundPicture = mPicturePositions.find(meetingPoint->first);
        
        if (foundPicture != mPicturePositions.end()) {
            // This picture exists
            //foundPicture->second.setPos(meetingPoint->second.getPos());
            //foundPicture->second.refresh();
        }else{
            
            if (mPicturePositions.size() < piNbImages) {
                // No Pictures like this -> add it
                hospitalPicture newPicture;
                newPicture.setFadeIn(pfFadeInInSec);
                newPicture.setFadeOut(pfFadeOutInSec);
                newPicture.setId(meetingPoint->second.getId());
                newPicture.setPos(meetingPoint->second.getPos());
                newPicture.refresh();
                
                mPicturePositions[meetingPoint->second.getId()] = newPicture;
            }
            
        }
        
    }
    
    
}
