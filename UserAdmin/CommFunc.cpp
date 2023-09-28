#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "CommFunc.h"
//---------------------------------------------------------------------------
#define		KEYCODES		0x1f

void EncodeString(AnsiString str)
{
	int nLen, i;
	char c, cKey, cSerialKey = KEYCODES;

	nLen = str.Length();
	cKey =  (char)(nLen % KEYCODES);

	for (i=0; i < nLen; i ++)
	{
		c = str.c_str()[i];
		c ^= cKey;
		c ^= cSerialKey;
		cKey = c & (char)KEYCODES;

		str.c_str()[i]=c;

		cSerialKey --;
		if (cSerialKey == 0)
			cSerialKey = KEYCODES;
	}
}

void DecodeString(AnsiString str)
{
	int nLen, i;
	char c, cKey, cSerialKey = KEYCODES, cSaveKey;

	nLen = str.Length();
	cKey =  (char)(nLen % KEYCODES);

	for (i=0; i <nLen; i ++)
	{
		c =str.c_str()[i];
		cSaveKey = c;
		c ^= cSerialKey;
		c ^= cKey;

		str.c_str()[i]=c;

		cKey = cSaveKey & (char)KEYCODES;
		cSerialKey --;
		if (cSerialKey == 0)
			cSerialKey = KEYCODES;
	}
}

//---------------------------------------------------------------------------
AnsiString SQLStr(AnsiString strText)
{
   unsigned int nLength = strText.Length(),i;
   AnsiString str1=strText;
   unsigned char *p=(unsigned char*)strText.c_str();
   unsigned int flag=0, Count = 1;

   for (i=0;i<nLength;i++,p++)
    {
    	if (flag==0)
        {
        	if ( ((*p)>0x80)&((*p)<0xff) )
            {
            flag=1;
            }
            else if (*p == '\'')
    	    {
        	str1.Insert("'",i+Count);
            Count++;
        	}
        }
        else
        {
        	flag=0;
        }
    }
    str1+=".";
    return str1;
}

AnsiString OrigStr(AnsiString strText)
{
    AnsiString str1=strText;
    str1.SetLength(str1.Length()-1);
    return str1;
}

//---------------------------------------------------------------------------
void DisableMenu(TMainMenu *mnu1)
{
	int i;
    for(i=0;i<mnu1->Items->Count;i++)
		mnu1->Items->Items[i]->Enabled=false;
}

void EnableMenu(TMainMenu *mnu1)
{
	int i;
    for(i=0;i<mnu1->Items->Count;i++)
		mnu1->Items->Items[i]->Enabled=true;
}

//---------------------------------------------------------------------------

