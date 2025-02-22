/*
 *  RN4020.h
 * 
 *  Created on: 20/02/2025
 *  Author: 	Ross Underhill
 * 
 *  Description: Header file for RN4020 class
 */

 #ifndef RN4020_H_
 #define RN4020_H_
 
 #include <Arduino.h>
 #include <AltSoftSerial.h>

 class RN4020 {

    public:
        RN4020(AltSoftSerial *bt, int wake);
        ~RN4020();

        void wake();
        void sleep();
        void init();
        String getResponse();
        bool printResponse();
        void factoryReset(int resetType);
        void baudrateReset();
        void setName(String name);

        void pair();
        void unpair();

        void talk();
        
    private:
        int wakeHW;
        AltSoftSerial* bluetooth;
        String response;
        long int baudrates[8];
 };

 #endif /* RN4020_H_ */