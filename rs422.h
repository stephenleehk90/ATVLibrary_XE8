//C422 base class.
//COM_ERROR means error within 422 protocol. (the lowest layer,variable is int COM_ERROR_CODE)
#ifndef rs422H
#define rs422H
#define COM_ERROR_BASE         -100
#define COM_ERROR_CHECKSUMFAIL -90
#define COM_ERROR_DATAMISSED   -91
#define COM_ERROR_NODATA       -92
#define COM_ERROR_DATATOOLONG  -93
#define COM_ERROR_OPENFAIL	   -94
#define COM_ERROR_CLOSEFAIL	   -95
#define COM_ERROR_UNKNOWN      -99

/*	BAUD rate setting	*/
#define B50			0x00
#define B75			0x01
#define B110		0x02
#define B134		0x03
#define B150		0x04
#define B300		0x05
#define B600		0x06
#define B1200		0x07
#define B1800		0x08
#define B2400		0x09
#define B4800		0x0A
#define B7200		0x0B
#define B9600		0x0C
#define B19200		0x0D
#define B38400		0x0E
#define B57600		0x0F
#define B115200 	0x10
#define B230400 	0x11
#define B460800 	0x12
#define B921600 	0x13

/*	MODE setting		*/
#define BIT_5		0x00			/* Word length define	*/
#define BIT_6		0x01
#define BIT_7		0x02
#define BIT_8		0x03

#define STOP_1		0x00			/* Stop bits define	*/
#define STOP_2		0x04

#define P_EVEN		0x18			/* Parity define	*/
#define P_ODD		0x08
#define P_SPC		0x38
#define P_MRK		0x28
#define P_NONE		0x00

/*	MODEM CONTROL setting	*/
#define C_DTR		0x01
#define C_RTS		0x02

/*	MODEM LINE STATUS	*/
#define S_CTS		0x01
#define S_DSR		0x02
#define S_RI		0x04
#define S_CD		0x08

/* error code */
#define SIO_OK		0
#define SIO_BADPORT	-1	/* no such port or port not opened */
#define SIO_OUTCONTROL	-2	/* can't control MOXA board */
#define SIO_NODATA	-4	/* no data to read or no buffer to write */
#define SIO_BADPARM	-7	/* bad parameter */
#define SIO_WIN32FAIL	-8	/* call win32 function fail, please call */
				/* GetLastError to get the error code */
#define SIO_NOTMOXABOARD    -9	/* not moxa board */

class C422
{
protected:
//	CMutex m_nMutex;
//	CSingleLock* m_pLock;
	HANDLE m_hMutex; 
	char COM_INBUFFER[100];
	char COM_OUTBUFFER[100];
	int  COM_PORT;             //the port
	int  COM_BAUD;			   //the baud rate
	int  COM_DATABITS;         //data bits
	int  COM_STOPBITS;         //stop bits
	int  COM_PARITY;           //0:none 1:even 2:odd
	int  COM_FLOWCONTROL;      //0:

	int  COM_ERROR_CODE;       //the internal error code
public:
	long int m_nError;
	C422();
	virtual ~C422();
	int  COMAccess(int nOutLen,int& nInLen);
	int  COMBVSAccess(int nOutLen,int& nInLen);
	int  COMOpen(void);
	int  COMClose(void);
	int  COMGetlstatus(void); //return value:

	void COMSetPort(int nPort);
	int  COMGetPort();
	void COMSetDataBits(int nDataBits);
	int  COMGetDataBits();
	void COMSetStopBits(int nStopBits);
	int  COMGetStopBits();
	void COMSetParity(int nParity);
	int  COMGetParity();
	void COMSetFlowControl(int nFlowControl);
	int  COMGetFlowControl();
};
extern C422 *p422;
#endif
