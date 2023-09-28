//version 1.02  last update:98.05.11
// VTRBetacam.cpp: implementation of the VTRBetacam class.
//
//////////////////////////////////////////////////////////////////////
#include <vcl.h>
//#include "stdafx.h"
#include "VTR.h"

CVTR::CVTR()
{
	//set the default value;for SONY Betacam VTR serias
	VTR_TYPE[0] = 0x00;
	VTR_TYPE[1] = 0x00;
	Status.CassetteOut   = false;
	Status.RefVideoError = false;
	Status.TapeError     = false;
	Status.Local         = false;       //Local or Remote Control 
	Status.Standby       = false;	    //StandBy is On.
	Status.Stop          = true;        
	Status.Eject         = false;
	Status.Rew           = false;
	Status.FFwd          = false;
	Status.Rec           = false;
	Status.Play          = false;
	Status.Shuttle       = false;
	Status.Jog           = false;
	Status.Var           = false;         //VAR mode or 
	Status.Still         = false;
	Status.Cueupcomplete = false;
	Status.PrerollCueup  = false;
	Status.EOT           = false;         //End of the Tape
	Status.Timer[0]='\0';		
	Status.LTC[0]='\0';		
	Status.VITC[0]='\0';		
	C422();
}
//////////////////////////////////////////////////////////////////////

CVTR::~CVTR()
{
//	~C422();
}
//////////////////////////////////////////////////////////////////////


int CVTR::VTRLocalEnable()          //local control
{
	int nLen=0,nRes,nReturnCode;
	//localenable sends 00.1D
	COM_OUTBUFFER[0]=0x00;
	COM_OUTBUFFER[1]=0x1D;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////
int CVTR::VTRLocalDisable()          //remote control
{
	int nLen=0,nRes,nReturnCode;
	//Local Disable sends 00.0C
	COM_OUTBUFFER[0]=0x00;
	COM_OUTBUFFER[1]=0x0C;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////
int CVTR::VTRStandbyOn()          //StandbyON
{
	int nLen=0,nRes,nReturnCode;
	//StandbyOn sends 20.05
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x05;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////
int CVTR::VTRStandbyOff()          //StandbyOff
{
	int nLen=0,nRes,nReturnCode;
	//StandbyOff sends 20.04
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x04;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////
int CVTR::VTRPlay()          //play
{
	int nLen=0,nRes,nReturnCode;
	//play sends 20.01
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x01;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRStop()           //stop
{
	int nLen=0,nRes,nReturnCode;
	//stop sends 20.00
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x00;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRRecord()         //record
{
	int nLen=0,nReturnCode,nRes;
	// record sends 20.02
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x02;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRPauseOn()          //pause on
{
	int nLen=0,nReturnCode,nRes;
	// pauseon sends 20.05
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x05;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRPauseOff()          //pause on
{
	int nLen=0,nReturnCode,nRes;
	// pauseoff sends 20.04
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x04;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRStill()          //still
{
	/*
	int nLen=0,nReturnCode,nRes;
	// sends 20.00
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x00;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;*/
	return VTR_COMMAND_NOTSUPPORT;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRFFwd()           //FastForward
{
	int nLen=0,nReturnCode,nRes;
	// ff sends 20.10
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x10;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRRewind()         //Rewind 
{
	int nLen=0,nReturnCode,nRes;
	// rew sends 20.20
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x20;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRHome()           //Rew to Home   
{
/*	int nLen=0,nReturnCode,nRes;
	// home sends 20.00
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x00;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
*/
	return VTR_COMMAND_NOTSUPPORT;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTREnd()            //FF  to End
{
/*	int nLen=0,nReturnCode,nRes;
	// sends 20.00
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x00;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
*/
	return VTR_COMMAND_NOTSUPPORT;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTREject()          //ecject the cassette
{
	int nLen=0,nReturnCode,nRes;
	// eject sends 20.0f
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x0F;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRStepF()          //step forward
{
	int nLen=0,nReturnCode,nRes;
	// Program + sends 21.38
	COM_OUTBUFFER[0]=0x21;
	COM_OUTBUFFER[1]=0x11;
	COM_OUTBUFFER[2]=0x20;     //deviation % =0.1*SpeedData
	nRes = COMAccess(3,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRStepR()          //step rewind
{
	int nLen=0,nReturnCode,nRes;
	// Program - sends 21.39
	COM_OUTBUFFER[0]=0x21;
	COM_OUTBUFFER[1]=0x21;
	COM_OUTBUFFER[2]=0x10;     //deviation % =0.1*SpeedData
	nRes = COMAccess(3,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRCue(char sHour,char sMinute,char sSecond,char sFrame)            //cue 
{
	int nLen=0,nReturnCode,nRes;
	// sends 24.31.XX.XX.XX.XX
	COM_OUTBUFFER[0]=0x24;
	COM_OUTBUFFER[1]=0x31;
	COM_OUTBUFFER[2]=sFrame;
	COM_OUTBUFFER[3]=sSecond;
	COM_OUTBUFFER[4]=sMinute;
	COM_OUTBUFFER[5]=sHour;
	nRes = COMAccess(6,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRInit()           //open the port,VTR.
{
	int nResult,nReturnCode;
	nResult = COMOpen();
	if(SIO_OK != nResult) //does not have the port;
		nReturnCode = nResult;
	else //
	{
		nReturnCode=0;
		VTRGetStatus();
//		if(Status.Local) //Now is local control,turn it.
//			VTRLocalDisable();
		if(0 == VTRGetDeviceData())
		{
			nReturnCode = 0;
		}
		else
			nReturnCode = VTR_COMMAND_FAIL;
	}

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRRelease()        //close the port. 
{
	int nReturnCode;
	nReturnCode = COMClose();
	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

void CVTR::VTRGetLastError(int nError) 
{
	AnsiString str;

	if(nError >= 0)
		return ;

	if(COM_ERROR_CODE < COM_ERROR_BASE)
	{
		switch(COM_ERROR_CODE)
		{
		case COM_ERROR_CHECKSUMFAIL:
			str = "422:chechsum failed,";
			break;
		case COM_ERROR_DATAMISSED:
			str = "422:some data is missing,";
			break;
		case COM_ERROR_NODATA:
			str = "422:device no response,";
			break;
		case COM_ERROR_DATATOOLONG:
			str = "422:device return too much data,";
			break;
		case COM_ERROR_UNKNOWN:
			str = "422:error,";
			break;
		case COM_ERROR_OPENFAIL:
			str = "422:Cannot open the port,";
			break;
		case COM_ERROR_CLOSEFAIL:
			str = "422:Cannot close the port,";
			break;
		default:
			str = "422:error,";
		}
	}
	else
	{
		int nii = COM_ERROR_CODE - COM_ERROR_BASE;
		if(0x01 & nii)
			str = "Device:unsurport command,";
		if(0x04 & nii)
			str = "Device:checksum failed,";
		if(0x10 & nii)
			str = "Device:parity error,";
		if(0x40 & nii)
			str = "Device:generate frame error,";
		if(0x80 & nii)
			str = "Device:tick too long between bytes,";

	}
//	AnsiString ss;
//	ss.Format("line status:\n  CTS|DSR|RI|CD:%d.",COMGetlstatus);
//	str += ss;
//	::MessageBox(NULL,str,"Error occured",MB_OK|MB_ICONERROR);
	return;
}
//////////////////////////////////////////////////////////////////////



int  CVTR::VTRGetDeviceData()
{
	int nLen=0,nRes,nReturnCode;

	COM_OUTBUFFER[0]=0x00;
	COM_OUTBUFFER[1]=0x11;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 12.11.XX.XX
		if( (COM_INBUFFER[0] == 0x12) && (COM_INBUFFER[1] == 0x11) )
		{
			VTR_TYPE[0] = COM_INBUFFER[2];
			VTR_TYPE[1] = COM_INBUFFER[3];
			nReturnCode = 0; //OK
		}
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRGetType(char *sType)
{
	switch(VTR_TYPE[0])
	{
		case 0x20:
		case 0x21:
			switch(VTR_TYPE[1])
			{
				case 0x00 : strcpy(sType," BVW-10         ");
					break;
				case 0x02 : strcpy(sType," BVW-11         ");
					break;
				case 0x03 : strcpy(sType," BVW-15         ");
				     break;
				case 0x10 : strcpy(sType," BVW-35         ");
				     break;
				case 0x01 : strcpy(sType," BVW-40         ");
 				     break;
				case 0x30 : strcpy(sType," BVW-50         ");
				    break;
				case 0x20 : strcpy(sType," BVW-60         ");
				     break;
				case 0x21 : strcpy(sType," BVW-65         ");
 				     break;
				case 0x22 : strcpy(sType," BVW-95         ");
 				     break;
				case 0x23 : strcpy(sType," BVW-96         ");
 				     break;
				case 0x24 : strcpy(sType," BVW-70         ");
 				     break;
				case 0x25 : strcpy(sType," BVW-75         ");
				     break;
				case 0x46 : strcpy(sType," BVW-D75        ");
 				     break;
				case 0x45 : strcpy(sType," BVW-D256       ");
 				     break;
				case 0x47 : strcpy(sType," BVW-9000       ");
 				     break;
				case 0x18 : strcpy(sType," BVW-35PM       ");
 				     break;
				case 0x29 : strcpy(sType," BVW-65PM       ");
 				     break;
				case 0x2D : strcpy(sType," BVW-95PM/75S   ");
				     break;
				case 0x26 : strcpy(sType," BVW-85PM       ");
 				     break;
				case 0x2C : strcpy(sType," BVW-70S        ");
 				     break;
				case 0x2F : strcpy(sType," WBR-700        ");
 				     break;
			}
			break;
		case 0xb0:
		case 0xb1:
			switch(VTR_TYPE[1])
		    {
				case 0x00 : strcpy(sType," DVW-A500       ");
				     break;
				case 0x01 : strcpy(sType," DVW-A510       ");
					 break;
				case 0x03 : strcpy(sType," DVW-CA510      ");
				     break;
				case 0x10 : strcpy(sType," DVW-500        ");
 				     break;
				case 0x11 : strcpy(sType," DVW-510        ");
 				     break;
			}
			break;
       default:
			strcpy(sType," UNKOWN TYPE    ");
	}            
	return 0;
}

int CVTR::VTRGetTimeData(int nWhich)
{
	int nLen=0,nRes,nReturnCode;
	//Time query sends 61.0C
	COM_OUTBUFFER[0]=0x61;
	COM_OUTBUFFER[1]=0x0C;
	COM_OUTBUFFER[2]=nWhich;
	nRes = COMAccess(3,nLen);
	if( 0 == nRes )
	{
		//if it is Data 7X.XX.xx.xx...
		if( (COM_INBUFFER[0] & 0xF0) == 0x70 )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}

int CVTR::VTRGetTimer1()
{
	//the Timer stored in Status.Timer[12]
	int nReturnCode;
	nReturnCode = VTRGetTimeData(0x04);
	if(0 == nReturnCode)
	{
		Status.Timer[0] = ((COM_INBUFFER[5] & 0x7f)  / 16)   + 0x30;
		Status.Timer[1] = (COM_INBUFFER[5] & 0x0f) + 0x30;
		Status.Timer[2] = ':';
		Status.Timer[3] = ((COM_INBUFFER[4] & 0x7f)  / 16)   + 0x30;
		Status.Timer[4] = (COM_INBUFFER[4] & 0x0f) + 0x30;
		Status.Timer[5] = ':';
        Status.Timer[6] = ((COM_INBUFFER[3] & 0x7f)  / 16)   + 0x30;
        Status.Timer[7] = (COM_INBUFFER[3] & 0x0f) + 0x30;
        Status.Timer[8] = ':';
        Status.Timer[9] = ((COM_INBUFFER[2] & 0x7f)  / 16)   + 0x30;
        Status.Timer[10] = (COM_INBUFFER[2] & 0x0f) + 0x30;
        Status.Timer[11] = 0x00;
	}
	else
		strcpy(Status.Timer,"00:00:00:00");
	return nReturnCode;
}

int CVTR::VTRGetLTC()
{
	//the LTC stored in Status.LTC[12]
	int nReturnCode;
	nReturnCode = VTRGetTimeData(0x01);
	if(0 == nReturnCode)
	{
	        Status.LTC[0] = ((COM_INBUFFER[5] & 0x7f) / 16)   + 0x30;
	        Status.LTC[1] = (COM_INBUFFER[5] & 0x0f) + 0x30;
	        Status.LTC[2] = ':';
	        Status.LTC[3] = ((COM_INBUFFER[4] & 0x7f) / 16)   + 0x30;
	        Status.LTC[4] = (COM_INBUFFER[4] & 0x0f) + 0x30;
	        Status.LTC[5] = ':';
	        Status.LTC[6] = ((COM_INBUFFER[3] & 0x7f) / 16)   + 0x30;
	        Status.LTC[7] = (COM_INBUFFER[3] & 0x0f) + 0x30;
	        Status.LTC[8] = ':';
	        Status.LTC[9] = ((COM_INBUFFER[2] & 0x7f) / 16)   + 0x30;
	        Status.LTC[10] = (COM_INBUFFER[2] & 0x0f) + 0x30;
	        Status.LTC[11] = 0x00;
	}
	else
		strcpy(Status.LTC,"00:00:00:00");
	return nReturnCode;
}

int CVTR::VTRGetVITC()
{
	//the VITC stored in Status.VITC[12]
	int nReturnCode;
	nReturnCode = VTRGetTimeData(0x02);
	if(0 == nReturnCode)
	{
	   if(0x70 == COM_INBUFFER[0] && 0x0D == COM_INBUFFER[1])
		   strcpy(Status.VITC,"No TimeCode");
	   else
	   {
	        Status.VITC[0] = ((COM_INBUFFER[5] & 0x7f)  / 16)   + 0x30;
	        Status.VITC[1] = (COM_INBUFFER[5] & 0x0f) + 0x30;
	        Status.VITC[2] = ':';
	        Status.VITC[3] = ((COM_INBUFFER[4] & 0x7f)  / 16)   + 0x30;
	        Status.VITC[4] = (COM_INBUFFER[4] & 0x0f) + 0x30;
	        Status.VITC[5] = ':';
	        Status.VITC[6] = ((COM_INBUFFER[3] & 0x7f)  / 16)   + 0x30;
	        Status.VITC[7] = (COM_INBUFFER[3] & 0x0f) + 0x30;
	        Status.VITC[8] = ':';
	        Status.VITC[9] = ((COM_INBUFFER[2] & 0x7f)  / 16)   + 0x30;
	        Status.VITC[10] = (COM_INBUFFER[2] & 0x0f) + 0x30;
	        Status.VITC[11] = 0x00;
	   }
	}
	else
		strcpy(Status.VITC,"00:00:00:00");
	return nReturnCode;
}

int CVTR::VTRGetStatus()
{
	int nLen=0,nReturnCode,nRes;
	// StatusSense - sends 61.20.XX
	COM_OUTBUFFER[0]=0x61;
	COM_OUTBUFFER[1]=0x20;
	COM_OUTBUFFER[2]=0x09;     //from 0 number:9 the sony only defines 9.
	nRes = COMAccess(3,nLen);
	if( 0 == nRes )
	{
		//if it is Data 7X.20
		if((0x70 == (COM_INBUFFER[0] & 0xF0)) && 0x20 == COM_INBUFFER[1]) 
		{
//			if(0x10 == (COM_INBUFFER[2] & 0x10)) //bit5
//				CassetteOut = true;
			Status.CassetteOut   = (0x20 == (COM_INBUFFER[2] & 0x20))?true:false;
			Status.RefVideoError = (0x10 == (COM_INBUFFER[2] & 0x10))?true:false;
			Status.TapeError     = (0x00 <  (COM_INBUFFER[2] & 0xA0))?true:false;
			Status.Local         = (0x01 == (COM_INBUFFER[2] & 0x01))?true:false;       //Local or Remote Control 
			Status.Standby       = (0x80 == (COM_INBUFFER[3] & 0x80))?true:false;	  //StandBy is On.
			Status.Stop          = (0x20 == (COM_INBUFFER[3] & 0x20))?true:false;        
			Status.Eject         = (0x10 == (COM_INBUFFER[3] & 0x10))?true:false;
			Status.Rew           = (0x08 == (COM_INBUFFER[3] & 0x08))?true:false;
			Status.FFwd          = (0x04 == (COM_INBUFFER[3] & 0x04))?true:false;
			Status.Rec           = (0x02 == (COM_INBUFFER[3] & 0x02))?true:false;
			Status.Play          = (0x01 == (COM_INBUFFER[3] & 0x01))?true:false;
			Status.Shuttle       = (0x20 == (COM_INBUFFER[4] & 0x20))?true:false;
			Status.Jog           = (0x10 == (COM_INBUFFER[4] & 0x10))?true:false;
			Status.Var           = (0x08 == (COM_INBUFFER[4] & 0x08))?true:false;         //VAR mode or 
			Status.Still         = (0x02 == (COM_INBUFFER[4] & 0x02))?true:false;
			Status.Cueupcomplete = (0x01 == (COM_INBUFFER[4] & 0x01))?true:false;
			Status.PrerollCueup  = (0x01 == (COM_INBUFFER[6] & 0x01))?true:false;
			Status.EOT           = (0x10 == (COM_INBUFFER[10] & 0x10))?true:false;         //End of the Tape
			nReturnCode = 0;
		}
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}

int CVTR::VTRSyncPlay()
{
	int nLen=0,nRes,nReturnCode;
	//sync play sends 20.34
	COM_OUTBUFFER[0]=0x20;
	COM_OUTBUFFER[1]=0x34;
	nRes = COMAccess(2,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
//////////////////////////////////////////////////////////////////////

int CVTR::VTRServoModeSel(int nMode)
{
	int nLen=0,nRes,nReturnCode;
	//SERVO Mode Select sends 41.33.XX
	COM_OUTBUFFER[0]=0x41;
	COM_OUTBUFFER[1]=0x33;
	COM_OUTBUFFER[2]=nMode;
	//nMode = 0x00 AUTO
	//nMode = 0x01 External
	//nMode = 0xFF Depends
	nRes = COMAccess(3,nLen);
	if( 0 == nRes )
	{
		//if it is ACK 10.01
		if( (COM_INBUFFER[0] == 0x10) && (COM_INBUFFER[1] == 0x01) )
			nReturnCode = 0; //OK
		else
		{
			nReturnCode = VTR_COMMAND_FAIL;
			//NAK 11.12.XX
			if((COM_INBUFFER[0] == 0x11) && (COM_INBUFFER[1] == 0x12))
				COM_ERROR_CODE = COM_ERROR_BASE - COM_INBUFFER[nLen-1];
			else
				COM_ERROR_CODE = COM_ERROR_UNKNOWN;
		}
	}
	else
		nReturnCode = nRes;

	return nReturnCode;
}
