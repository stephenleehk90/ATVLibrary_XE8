//last modified:skiff 26,June,1998
//notice:include lib:winmm.lib in the directory DevStudio\VC\Lib
//C422 implementation method. 
//#include "Stdafx.h"
#include "Api232c.h"
#include "mmsystem.h"
#include "rs422.h"

//note: all the self-defined function returns 0 if success.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
C422 *p422;
C422::C422()
{
	//set the default value;
	COM_PORT = 5;						   //the port
	COM_BAUD = B38400; 			   //the baud rate
	COM_DATABITS = BIT_8;         //data bits
	COM_STOPBITS = STOP_1;         //stop bits
	COM_PARITY = P_ODD;           //0:none 1:even 2:odd
	COM_FLOWCONTROL = 0;			       //0:
	m_hMutex = ::CreateMutex ( NULL, FALSE, "AccessShare");
	if(NULL == m_hMutex)
	{
		MessageBox(NULL,"Error creating mutex:422","TBS Library",MB_OK);
	}
//	m_pLock = new CSingleLock(&m_nMutex);
}

C422::~C422()
{

}

int C422::COMOpen(void)
{
	int nResult,nReturnCode;
	nResult = sio_open(COM_PORT);
	if(SIO_OK != nResult) //does not have the port;
	{
		COM_ERROR_CODE = COM_ERROR_OPENFAIL;
		nReturnCode = nResult;
	}
	else //
	{
		int nMode = COM_DATABITS+COM_STOPBITS+COM_PARITY;  //
		sio_ioctl(COM_PORT,COM_BAUD,nMode);
		nReturnCode=0;
		m_nError = 0;
	}
	return nReturnCode;
}

int C422::COMClose(void)
{
	int nReturnCode;
	nReturnCode = sio_close(COM_PORT);
	COM_ERROR_CODE = COM_ERROR_CLOSEFAIL;
	m_nError = 0;
	return nReturnCode;
}

int C422::COMGetlstatus(void)
{
	int nReturnCode;
	nReturnCode = sio_lstatus(COM_PORT);
	return nReturnCode;
}

int  C422::COMAccess(int nOutLen,int& nInLen)
{
	int  nResult,i,j,nReturnCode;
	bool blShouldResend=false;
	DWORD tmBegin;

	
	//COMAccess generate the checksum for Output Buffer.
	nResult=0;
	for(j=0;j<nOutLen;j++)
		nResult += COM_OUTBUFFER[j];
	COM_OUTBUFFER[nOutLen]=(char)nResult;
	nOutLen++;

	for(i=0;i<1;i++)
	{
		nInLen = 0;
		//first,clear all the buffers.
		::WaitForSingleObject(m_hMutex, INFINITE); 
		nResult = sio_flush(COM_PORT,2);
		if(nResult != SIO_OK)
		{
			nReturnCode = nResult;
			::ReleaseMutex(m_hMutex);
			return nReturnCode;
		}

		sio_write(COM_PORT,COM_OUTBUFFER,nOutLen);
//		::Sleep(15);
		tmBegin = timeGetTime();
		while(timeGetTime() < tmBegin+11);
		nInLen = sio_iqueue(COM_PORT);
		//upper and down limit check. nInLen between 0 and 18
		if(nInLen > 2 && nInLen < 19)
		{
			//get the data to inbuffer.length is nInLen;
			sio_read(COM_PORT,COM_INBUFFER,nInLen);

			//check chechsum.			
			nResult = 0;
			for(j=0;j<nInLen-1;j++)
				nResult += COM_INBUFFER[j];
			if(0 != (nResult - COM_INBUFFER[nInLen-1])%256)
			{
				COM_ERROR_CODE = COM_ERROR_CHECKSUMFAIL;
				blShouldResend = true;
			}
			else
			{
				//valid check,see if data_count+3 = nInLen
				nResult = 0x0F & COM_INBUFFER[0];
				nResult += 3;
				if(nResult != nInLen)
				{
					COM_ERROR_CODE = COM_ERROR_DATAMISSED;
					blShouldResend = true;
				}
				else
					blShouldResend = false;
			}
		}
		else
		{
			if(nInLen > 18)
				COM_ERROR_CODE = COM_ERROR_DATATOOLONG;
			else
				COM_ERROR_CODE = COM_ERROR_NODATA;
			blShouldResend = true;
		}

		::ReleaseMutex(m_hMutex);
		
		if(!blShouldResend)
			break;
	}

	if(blShouldResend)
	{
		m_nError++;
		nReturnCode = -10;
	}
	else
	{
		nInLen--;
		nReturnCode = 0;
	}

	return nReturnCode;
}

//////////////////////////////////////////////////////////////////////
int  C422::COMBVSAccess(int nOutLen,int& nInLen)
{ //modified by dingjm
int  nResult=0,j,nReturnCode;
//bool blShouldResend=false;
DWORD tmBegin;
//COMAccess generate the checksum for Output Buffer.
char sum=0x00;

for(j=2;j<nOutLen;j++)
sum += COM_OUTBUFFER[j];
COM_OUTBUFFER[nOutLen]=~sum + 0x01;

nOutLen++;

// for(i=0;i<2;i++)
{
nInLen = 0;
//first,clear all the buffers.
nResult = sio_flush(COM_PORT,2);
if(nResult != SIO_OK)
{
nReturnCode = nResult;
return nReturnCode;
}

nResult=sio_write(COM_PORT,COM_OUTBUFFER,nOutLen);
if(nResult != nOutLen)
{
nReturnCode = nResult;
return nReturnCode;
}

//::Sleep(20);
		tmBegin = timeGetTime();
		while(timeGetTime() < tmBegin+11);

nInLen = sio_iqueue(COM_PORT);
if(nInLen<0)
{
nReturnCode=nInLen;
return nReturnCode;
}

//get the data to inbuffer.length is nInLen;
nResult=sio_read(COM_PORT,COM_INBUFFER,nInLen);
if(nResult != nInLen)
{
nReturnCode = nResult;
return nReturnCode;
}


//check chechsum. 
// nResult = 0;
// for(j=0;j<nInLen-1;j++)
// nResult += COM_INBUFFER[j];

//valid check,see if data_count+3 = nInLen
/* nResult = COM_INBUFFER[1];
nResult += 3;
if(nResult != nInLen)
{
COM_ERROR_CODE = COM_ERROR_DATAMISSED;
blShouldResend = true;
}
else
blShouldResend = false;
*/ 
// if(!blShouldResend)
// break;
}

/* if(blShouldResend)
nReturnCode = COM_RETURN_FAIL;
else
{
nInLen--;
nReturnCode = 0;
}
*/

nReturnCode = 0;
return nReturnCode;
}

//////////////////////////////////////////////////////////////////////
void C422::COMSetPort(int nPort) 
{
	if((nPort >= 0) && (nPort < 256))
		COM_PORT = nPort;
}
//////////////////////////////////////////////////////////////////////

int  C422::COMGetPort() 
{
	return COM_PORT;
}
//////////////////////////////////////////////////////////////////////

void C422::COMSetDataBits(int nDataBits) 
{
	if(nDataBits >= BIT_5 && nDataBits <= BIT_8)
		COM_DATABITS = nDataBits;
}
//////////////////////////////////////////////////////////////////////

int  C422::COMGetDataBits() 
{
	return COM_DATABITS;
}
//////////////////////////////////////////////////////////////////////

void C422::COMSetStopBits(int nStopBits) 
{
	if(nStopBits == STOP_1 || nStopBits == STOP_2)
		COM_STOPBITS = nStopBits;
}
//////////////////////////////////////////////////////////////////////

int  C422::COMGetStopBits() 
{
	return COM_STOPBITS;
}
//////////////////////////////////////////////////////////////////////

void C422::COMSetParity(int nParity) 
{
	if(nParity == 0x00 || nParity == 0x08 || nParity == 0x18 
			|| nParity == 0x28 || nParity == 0x38)
		COM_PARITY = nParity;
}
//////////////////////////////////////////////////////////////////////

int C422::COMGetParity() 
{
	return COM_PARITY;
}
//////////////////////////////////////////////////////////////////////

void C422::COMSetFlowControl(int nFlowControl) 
{
	COM_FLOWCONTROL = nFlowControl;
}
//////////////////////////////////////////////////////////////////////

int C422::COMGetFlowControl() 
{
	return COM_FLOWCONTROL;
}
//////////////////////////////////////////////////////////////////////

