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
/*
    map<string, hospitalPicture>::iterator pic;
    
    for (pic = mPicturePositions.begin(); pic != mPicturePositions.end(); pic++) {
        if(ofGetElapsedTimef() > (pic->second.getTimeStamp() + 5.0f)){
            mPicturePositions.erase(pic);
        }
    }*/
    
}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::draw(){
    
    ofPushStyle();
    ofNoFill();
    
    map<string, hospitalPicture>::iterator pic;
    
    for (pic = mPicturePositions.begin(); pic != mPicturePositions.end(); pic++) {
        
        float timeRatio = (ofGetElapsedTimef() - pic->second.getTimeStamp()) / LIFE_TIME;
        
        if(timeRatio > 1.0f){
            mPicturePositions.erase(pic);
        }else{
            ofCircle(pic->second.getPos(), 25 * (1.0f -timeRatio));
            ofDrawBitmapString(ofToString(timeRatio), pic->second.getPos());
        }
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
            foundPicture->second.setPos(meetingPoint->second.getPos());
            foundPicture->second.refresh();
        }else{
            // No Pictures like this -> add it
            hospitalPicture newPicture;
            newPicture.setId(meetingPoint->second.getId());
            newPicture.setPos(meetingPoint->second.getPos());
            newPicture.refresh();
            
            mPicturePositions[meetingPoint->second.getId()] = newPicture;
            
        }
        
    }
    
    
}
