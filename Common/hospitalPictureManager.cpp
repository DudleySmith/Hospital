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
    pgPictures.add(psPicturesPath.set("path", "pictures/"));
    pgPictures.add(pfFadeInInSec.set("fadeIn", 5, 0, 20));
    pgPictures.add(pfFadeOutInSec.set("fadeOur", 5, 0, 20));
    pgPictures.add(pfResizeRatio.set("resizeRatio", 0.25, 0, 1));
    pgPictures.add(piNbImages.set("nbImages", 3, 0, 50));
    
}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::setup(){
    
    ofDirectory picturesDir;
    
    // List all pictures (only *.jpg allowed)
    picturesDir.allowExt("jpg");
    picturesDir.listDir(psPicturesPath);
    picturesDir.sort();
    
    if (picturesDir.exists() && picturesDir.numFiles()>0) {
        
        // allocate the vector to have as many ofImages as files
        ofLogVerbose() << "Assigning images";
        mImages.assign(picturesDir.size(), ofImage());
        
        // you can now iterate through the files and load them into the ofImage vector
        for(int i = 0; i < (int)picturesDir.size(); i++){
            ofLogVerbose() << "Loading image : " << picturesDir.getPath(i);
            mImages[i].loadImage(picturesDir.getPath(i));
            mImages[i].resize(pfResizeRatio*mImages[i].width, pfResizeRatio*mImages[i].height);
        }
    }
}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::update(){

    map<string, hospitalPicture>::iterator pic;
    
    int idx = 0;
    ofLogVerbose() << "Size of pictures map : " << mPicturePositions.size();
    
    pic = mPicturePositions.begin();
    
    while (pic != mPicturePositions.end()){
        
        if(pic->second.getAscTimeRatio() >= 1.0f){
            //pic->second.mImage.clear();
            mPicturePositions.erase(pic++);
            ofLogVerbose() << "This image deleted : " << ofToString(idx);
            ofLogVerbose() << "Size of pictures map : " << mPicturePositions.size();
        }else{
            ++pic;
        }
        idx++;
    };
    
    ofLogVerbose() << "Size of pictures map : " << mPicturePositions.size();
 
}

// -------------------------------------------------------------------------------------------
void hospitalPictureManager::draw(){
    
    map<string, hospitalPicture>::iterator pic;
    
    ofPushStyle();
    ofNoFill();
    
    for (pic = mPicturePositions.begin(); pic != mPicturePositions.end(); pic++) {
            pic->second.draw(pbDrawDebug, pbDrawImage);
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
                
                newPicture.setup(getANewImage());
                
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

// -------------------------------------------------------------------------------------------
ofImage hospitalPictureManager::getANewImage(){
    
    if (mImages.size() > 0) {
        int rndIdx = ofRandom(0, mImages.size() - 1);
        return mImages[rndIdx];
        
    }else{
        return ofImage();
        
    }
    
}
