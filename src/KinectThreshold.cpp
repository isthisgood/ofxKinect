/*
 *  KinectThreshold.cpp
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
#include "KinectThreshold.h"

namespace itg
{
    namespace vision
    {
        using namespace ofxCv;
        
        KinectThreshold::KinectThreshold() : nearThresh(1.f), farThresh(0.f), KinectWrapper()
        {
        }
        
        void KinectThreshold::update()
        {
            KinectWrapper::update();
            frameNewFlag = KinectWrapper::isFrameNew();
            // there is a new frame and we are connected
            if (frameNewFlag)
            {
                switch (state)
                {
                    case LIVE:
                        depthMat = toCv(getDepthPixelsRef());
                        break;
                        
                    case DUMMY:
                    {
                        Mat colMat = toCv(getDepthPixelsRef());
                        convertColor(colMat, depthMat, CV_RGB2GRAY);
                        break;
                    }
                        
                    default:
                        ofLogError() << "Not initialised";
                        break;
                }
                
                ofxCv::threshold(depthMat, nearMat, 255 * nearThresh, true);
                ofxCv::threshold(depthMat, farMat, 255 * farThresh);
                threshMat = nearMat & farMat;
            }
        }
        
        bool KinectThreshold::isFrameNew()
        {
            if (frameNewFlag)
            {
                frameNewFlag = false;
                return true;
            }
            return false;
        }
    }
}