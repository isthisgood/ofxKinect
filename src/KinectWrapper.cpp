/*
 *  KinectDummy.cpp
 *
 *  Copyright (c) 2012, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#include "KinectWrapper.h"

namespace itg
{
    namespace vision
    {
        KinectWrapper::KinectWrapper() : state(PREINIT), rgbMedia(NONE) {}
                
        void KinectWrapper::init(const string& depthVideo, const string& rgb)
        {
            if (state == PREINIT)
            {
                this->depthVideo.loadMovie(depthVideo);
                if (rgb.find(".jpg") != string::npos || rgb.find(".png") != string::npos)
                {
                    rgbImage.loadImage(rgb);
                    rgbMedia = IMAGE;
                }
                else
                {
                    rgbVideo.loadMovie(rgb);
                    rgbMedia = VIDEO;
                }
                state = DUMMY;
            }
            else ofLogError() << "Kinect wrapper already inited.";
        }
        
        void KinectWrapper::init(bool infrared, bool video, bool texture)
        {
            if (state == PREINIT)
            {
                kinect.init(infrared, video, texture);
                state = LIVE;
            }
            else ofLogError() << "Kinect wrapper already inited.";    
        }
        
        bool KinectWrapper::setCameraTiltAngle(float angleInDegrees)
        {
            if (state == LIVE) return kinect.setCameraTiltAngle(angleInDegrees);
            else return false;
        }
        
        void KinectWrapper::enableDepthNearValueWhite(bool bEnabled)
        {
            if (state == LIVE) return kinect.enableDepthNearValueWhite(bEnabled);
        }
        
        bool KinectWrapper::isDepthNearValueWhite()
        {
            if (state == LIVE) return kinect.isDepthNearValueWhite();
            else return false;
        }
        
        void KinectWrapper::setRegistration(bool bUseRegistration)
        {
            if (state == LIVE) kinect.setRegistration(bUseRegistration);
        }
        
        void KinectWrapper::update()
        {
            switch (state)
            {
                case DUMMY:
                    depthVideo.update();
                    if (rgbMedia == VIDEO) rgbVideo.update();
                    break;
                    
                case LIVE:
                    kinect.update();
                    break;
                    
                default:
                    ofLogError() << "Open called before init.";
                    break;
            }
        }
        
        bool KinectWrapper::isFrameNew()
        {
            switch (state)
            {
                case DUMMY:
                    return depthVideo.isFrameNew();
                    break;
                    
                case LIVE:
                    return kinect.isFrameNew();
                    break;
                    
                default:
                    ofLogError() << "Open called before init.";
                    return false;
                    break;
            }
        }
        
        void KinectWrapper::open()
        {
            switch (state)
            {
                case DUMMY:
                    depthVideo.play();
                    if (rgbMedia == VIDEO) rgbVideo.play();
                    break;
                    
                case LIVE:
                    kinect.open();
                    break;
                    
                default:
                    ofLogError() << "Open called before init.";
                    break;
            }
        }
        
        void KinectWrapper::close()
        {
            switch (state)
            {
                case DUMMY:
                    depthVideo.stop();
                    if (rgbMedia == VIDEO) rgbVideo.stop();
                    break;
                    
                case LIVE:
                    kinect.close();
                    break;
                    
                default:
                    ofLogError() << "Close called before init.";
                    break;
            }
            
        }
        
        /// get the pixels of the most recent depth frame
        unsigned char* KinectWrapper::getDepthPixels()
        {
            switch (state)
            {
                case DUMMY:
                    return depthVideo.getPixels();
                    break;
                    
                case LIVE:
                    return kinect.getDepthPixels();
                    break;
                    
                default:
                    ofLogError() << "Not initialised.";
                    return NULL;
                    break;
            }
        }
        
        /// get the pixels of the most recent depth frame
        ofPixels& KinectWrapper::getDepthPixelsRef()
        {
            switch (state)
            {
                case DUMMY:
                    return depthVideo.getPixelsRef();
                    break;
                    
                case LIVE:
                    return kinect.getDepthPixelsRef();
                    break;
                    
                default:
                    ofLogError() << "Not initialised.";
                    break;
            }
        }
        
        unsigned char* KinectWrapper::getPixels()
        {
            switch (state)
            {
                case DUMMY:
                    if (rgbMedia == IMAGE) return rgbImage.getPixels();
                    else return rgbVideo.getPixels();
                    break;
                    
                case LIVE:
                    return kinect.getPixels();
                    break;
                    
                default:
                    ofLogError() << "Not initialised.";
                    return NULL;
                    break;
            }
        }
        
		ofPixelsRef KinectWrapper::getPixelsRef()
        {
            switch (state)
            {
                case DUMMY:
                    if (rgbMedia == IMAGE) return rgbImage.getPixelsRef();
                    else return rgbVideo.getPixelsRef();
                    break;
                    
                case LIVE:
                    return kinect.getPixelsRef();
                    break;
                    
                default:
                    ofLogError() << "Not initialised.";
                    break;
            }
        }
        
        /// get the distance in millimeters to a given point as a float array
        float* KinectWrapper::getDistancePixels()
        {
            switch (state)
            {
                case LIVE:
                    return kinect.getDistancePixels();
                    break;
                    
                default:
                    ofLogError() << "Not supported.";
                    return NULL;
                    break;
            }
        }
        
        /// get the grayscale depth texture
        ofTexture& KinectWrapper::getDepthTextureReference()
        {
            switch (state)
            {
                case DUMMY:
                    return depthVideo.getTextureReference();
                    break;
                    
                case LIVE:
                    return kinect.getDepthTextureReference();
                    break;
                    
                default:
                    ofLogError() << "Not initialised.";
                    break;
            }
        }
        
        ofColor KinectWrapper::getColorAt(int cx, int cy)
        {
            switch (state)
            {
                case DUMMY:
                {
                    switch (rgbMedia)
                    {
                        case IMAGE:
                            return rgbImage.getColor(cx, cy);
                            break;
                            
                        case VIDEO:
                        {
                            unsigned char* pixels = rgbVideo.getPixels();
                            unsigned idx = 3 * (cx + cy * (int)depthVideo.getWidth());
                            return ofColor((int)pixels[idx], (int)pixels[idx + 1], (int)pixels[idx + 2]);
                            break;
                        }
                                
                        default:
                            ofLogError() << "rgb media not set";
                            break;

                        break;
                    }
                }
                    
                case LIVE:
                    return kinect.getColorAt(cx, cy);
                    break;
                    
                default:
                    ofLogError() << "Not initialised.";
                    break;
            }
        }
        
        ofVec3f KinectWrapper::getWorldCoordinateAt(int cx, int cy)
        {
            switch (state)
            {
                case DUMMY:
                {
                    unsigned char depthPixel = depthVideo.getPixels()[3 * (cx + cy * (int)depthVideo.getWidth())];
                    float depth = ofMap(depthPixel, 0, 255, 1000, 0);
                    return ofVec3f(cx, cy, depth);
                    break;
                }
                    
                case LIVE:
                    return kinect.getWorldCoordinateAt(cx, cy);
                    break;
                    
                default:
                    ofLogError() << "Not initialised.";
                    break;
            }
        }
        
        ofTexture& KinectWrapper::getTextureReference()
        {
            switch (rgbMedia)
            {
                case IMAGE:
                    return rgbImage.getTextureReference();
                    break;
                    
                case VIDEO:
                    return rgbVideo.getTextureReference();
                    break;
                    
                default:
                    ofLogError() << "rgb media not set";
                    break;
            }
            
        }
    }
}