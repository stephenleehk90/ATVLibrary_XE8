/*	api232-c.h	  ver 1.02	10/22/1996
 *	API232 define file for Win32 on C/C++
 *	History:
 *	Date	    Author	Comment
 *	10/22/1996  Victor	wrote it.
 *	03/28/1997  Victor	modified. add sio_term_irq function.
 *				add sio_linput function.
 *  06/04/1998  skiff   modified. add COM VTR Error define.
 */


#include "windows.h"
extern "C" int  WINAPI sio_ioctl(int port, int baud, int mode);
extern "C" int  WINAPI sio_getch(int port);
extern "C" int  WINAPI sio_read(int port, char *buf, int len);
extern "C" int  WINAPI sio_putch(int port, int term);
extern "C" int  WINAPI sio_write(int port, char *buf, int len);
extern "C" int  WINAPI sio_flush(int port, int func);
extern "C" long WINAPI sio_iqueue(int port);
extern "C" long WINAPI sio_oqueue(int port);
extern "C" int  WINAPI sio_lstatus(int port);
extern "C" int  WINAPI sio_lctrl(int port, int mode);
extern "C" int  WINAPI sio_cnt_irq(int port, VOID (*func)(int port), int count);
extern "C" int  WINAPI sio_modem_irq(int port, VOID (*func)(int port));
extern "C" int  WINAPI sio_break_irq(int port, VOID (*func)(int port));
extern "C" int  WINAPI sio_Tx_empty_irq(int port, VOID (*func)(int port));
extern "C" int  WINAPI sio_break(int port, int time);
extern "C" int  WINAPI sio_flowctrl(int port, int mode);
extern "C" int  WINAPI sio_Tx_hold(int port);
extern "C" int  WINAPI sio_close(int port);
extern "C" int  WINAPI sio_open(int port);
extern "C" long WINAPI sio_getbaud(int port);
extern "C" int  WINAPI sio_getmode(int port);
extern "C" int  WINAPI sio_getflow(int port);
extern "C" int  WINAPI sio_DTR(int port, int mode);
extern "C" int  WINAPI sio_RTS(int port, int mode);
extern "C" int  WINAPI sio_baud(int port, long speed);
extern "C" int  WINAPI sio_data_status(int port);
extern "C" int  WINAPI sio_term_irq(int port, VOID (*func)(int port), char code);
extern "C" int  WINAPI sio_linput(int port, char *buf, int lne, int term);

#define sio_putb(x, y, z) sio_write(x, y, z)