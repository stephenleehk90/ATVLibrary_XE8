//version 1.02  last update:98.06.26
// VTR.h: interface for the CVTR class.
//
//////////////////////////////////////////////////////////////////////
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "rs422.h"

//VTR_COMMAND means error occured within VTR function call. (the top layer,return value)
#define VTR_COMMAND_FAIL        -20 
#define VTR_COMMAND_NOTSUPPORT  -21
#define VTR_COMMAND_BADDEVICE   -22

class CVTR : public C422
{
//define the private variables
protected:
	char VTR_TYPE[2];
	virtual int VTRGetDeviceData();
	virtual int VTRGetTimeData(int nWhich);
public:
	struct VTR_STATUS
	{
		bool CassetteOut; //the cassette is out
		bool RefVideoError;  //the Reference Video is missing
		bool TapeError;   //the Tape is sticking to the drum
						    //or the slackening
		bool Local;       //Local or Remote Control 
		bool Standby;	  //StandBy is On.
		bool Stop;        
		bool Eject;
		bool Rew;
		bool FFwd;
		bool Play;
		bool Rec;
		bool Cueupcomplete;
		bool Still;
		bool Shuttle;
		bool Jog;
		bool Var;         //VAR mode or 
		bool RevFwd;
		bool PrerollCueup;
		bool EOT;         //End of the Tape
		char Timer[12];
		char LTC[12];
		char VITC[12];
	} Status;
//define the interface
public:
	virtual int VTRServoModeSel(int nMode);
	virtual int VTRGetStatus();
	virtual int VTRGetVITC();
	virtual int VTRGetLTC();
	virtual int VTRGetTimer1();
	virtual int VTRGetType(char *sType);
	CVTR();
	virtual ~CVTR();

	virtual int VTRLocalEnable();          //local control
	virtual int VTRLocalDisable();          //remote control
	virtual int VTRStandbyOn();          //StandbyOn
	virtual int VTRStandbyOff();          //StandbyOff
	virtual int VTRPlay();          //play
	virtual int VTRSyncPlay();
	virtual int VTRStop();          //stop
	virtual int VTRRecord();        //record
	virtual int VTRPauseOn();         //pause on
	virtual int VTRPauseOff();         //pause off
	virtual int VTRStill();         //still
	virtual int VTRFFwd();          //FastForward 
	virtual int VTRRewind();        //Rewind 
	virtual int VTRHome();          //Rew to Home   
	virtual int VTREnd();           //FF  to End
	virtual int VTREject();         //ecject the cassette
	virtual int VTRStepF();         //step forward
	virtual int VTRStepR();         //step rewind 
	virtual int VTRCue(char sHour,char sMinute,char sSecond,char sFrame);           //cue 

	virtual int VTRInit();          //open the port,VTR.
	virtual int VTRRelease();       //close the port. 

	virtual void VTRGetLastError(int nError);
};
extern CVTR *pVtr;
