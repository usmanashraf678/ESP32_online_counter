/*                          =======================
============================   C/C++ SOURCE FILE   ============================
                            =======================                       *//**
  BoardInfo.cpp

  Created by Onno Dirkzwager on 22.11.2018.
  Copyright (c) 2018 IOTAppStory. All rights reserved.

*///===========================================================================

#ifdef ESP8266

/*---------------------------------------------------------------------------*/
/*                                INCLUDES                                   */
/*---------------------------------------------------------------------------*/

#include "BoardInfo.h"

/*---------------------------------------------------------------------------*/
/*                        DEFINITIONS AND MACROS                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        TYPEDEFS AND STRUCTURES                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                PROTOTYPES                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                            LOCAL VARIABLES                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        FUNCTION IMPLEMENTATION                            */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                        BoardInfo constructor

    @param bootTimes int &
    @param boardMode char &

*///---------------------------------------------------------------------------
BoardInfo::BoardInfo(int& bootTimes, char& boardMode) {
    this->_bootTimes = &bootTimes;
    this->_boardMode = &boardMode;
}

/*-----------------------------------------------------------------------------
                        BoardInfo read

*///---------------------------------------------------------------------------
void BoardInfo::read() {
    RtcMemDef rtcMem;

    system_rtc_mem_read(RTCMEMBEGIN, &rtcMem, sizeof(rtcMem));
    if (rtcMem.markerFlag != MAGICBYTE) {
        rtcMem.markerFlag = MAGICBYTE;
        rtcMem.bootTimes = 0;
        rtcMem.boardMode = 'N';
        system_rtc_mem_write(RTCMEMBEGIN, &rtcMem, sizeof(rtcMem));
    }
    (*this->_boardMode) = rtcMem.boardMode;
    (*this->_bootTimes) = rtcMem.bootTimes;
}

/*-----------------------------------------------------------------------------
                        BoardInfo write

*///---------------------------------------------------------------------------
void BoardInfo::write() {
    RtcMemDef rtcMem;

    rtcMem.boardMode = (*this->_boardMode);
    rtcMem.bootTimes = (*this->_bootTimes);

    rtcMem.markerFlag = MAGICBYTE;
    system_rtc_mem_write(RTCMEMBEGIN, &rtcMem, sizeof(rtcMem));
}

/*---------------------------------------------------------------------------*/
/*                                    EOF                                    */
/*---------------------------------------------------------------------------*/
#endif // ESP8266
