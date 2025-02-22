/*
 *  EMSChannel.h
 * 
 *  Created on: 20/02/2025
 *  Author: 	Ross Underhill
 * 
 *  Description: Header file for EMSChannel class
 */

#ifndef EMSCHANNEL_H_
#define EMSCHANNEL_H_

#include <Arduino.h>
#include <Wire.h>
#include <AD5252.h>

class EMSChannel {

    public:
        EMSChannel(int switch1, int switch2, int channelLED, AD5252 *digiPot, int wiperIndex);
        ~EMSChannel();
        void activateChannel(int I, int D);
        bool emsDebug;

    private:
        int sw1;
        int sw2;
        int LED;
        AD5252* pot;
        int wiper;

        int intensity;
};

#endif /* EMSCHANNEL_H_ */
