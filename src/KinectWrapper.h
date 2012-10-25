/*
 *  KinectDummy.h
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
#pragma once

#include "ofxKinect.h"

namespace itg
{
    namespace vision
    {
        class KinectWrapper
        {
        public:
            enum State
            {
                PREINIT,
                LIVE,
                DUMMY
            };
            
            enum DummyMedia
            {
                NONE,
                VIDEO,
                IMAGE
            };
            
            KinectWrapper();
            
            void init(const string& depthVideo, const string& rgb = "");
            
            void init(bool infrared=false, bool video=true, bool texture=true);
            
            void setRegistration(bool bUseRegistration=false);
            
            void open();
            
            void close();
            
            virtual void update();
            
            virtual bool isFrameNew();
                        
            bool setCameraTiltAngle(float angleInDegrees);
            
            void enableDepthNearValueWhite(bool bEnabled);
            
            bool isDepthNearValueWhite();
            
            /// get the pixels of the most recent depth frame
            virtual unsigned char* getDepthPixels();
            
            /// get the pixels of the most recent depth frame
            ofPixels& getDepthPixelsRef();       	///< grayscale values
            
            /// get the distance in millimeters to a given point as a float array
            virtual float* getDistancePixels();
            
            /// get the grayscale depth texture
            virtual ofTexture& getDepthTextureReference();
            
            unsigned char * 	getPixels();
            
            ofPixelsRef			getPixelsRef();
            
            ofTexture& getTextureReference();
            
            State getState() const { return state; }
            
            ofVec3f getWorldCoordinateAt(int cx, int cy);
            
            ofxKinect& getKinect() { return kinect; }
        
        protected:
            State state;
        
        private:
            ofVideoPlayer depthVideo;
            
            DummyMedia rgbMedia;
            ofVideoPlayer rgbVideo;
            ofImage rgbImage;
            
            ofxKinect kinect;
        };        
    }
}