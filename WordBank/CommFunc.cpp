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
AnsiString IniName(void)
{
     AnsiString strINIFileName;
     strINIFileName=Application->ExeName;
     strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
      				 (AnsiString)".ini";
     return strINIFileName;
}

bool SetIniInt (AnsiString Section, AnsiString Key, int value)
{
	 return WritePrivateProfileStringA(Section.c_str(),Key.c_str(),
				AnsiString(IntToStr(value)).c_str(),IniName().c_str());
}

int GetIniInt (AnsiString Section, AnsiString Key, int DefValue)
{
    return GetPrivateProfileInt(Section.c_str(),Key.c_str(),
                DefValue,IniName().c_str());
}

void SetIniFormPos(AnsiString Section, TForm* form)
{
    WINDOWPLACEMENT w1;

	if ((int)form->WindowState<2)
		{
		SetIniInt(Section,"State",0);
        SetIniInt(Section,"Top",form->Top);
        SetIniInt(Section,"Height",form->Height);
        SetIniInt(Section,"Left",form->Left);
        SetIniInt(Section,"Width",form->Width);
        }
    else{
        w1.length=sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(form->Handle,&w1);

        SetIniInt(Section,"State",2);
        SetIniInt(Section,"Top",w1.rcNormalPosition.top);
        SetIniInt(Section,"Height",w1.rcNormalPosition.bottom-
                                    w1.rcNormalPosition.top+1);
        SetIniInt(Section,"Left",w1.rcNormalPosition.left);
        SetIniInt(Section,"Width",w1.rcNormalPosition.right-
                                    w1.rcNormalPosition.left+1);
        }
}

void GetIniFormPos(AnsiString Section, TForm* form)
{
    int ws=GetIniInt(Section,"State",0);
    if (ws!=2)
        {
        form->WindowState=wsNormal;
        form->Top=GetIniInt(Section,"Top",form->Top);
        form->Height=GetIniInt(Section,"Height",form->Height);
        form->Left=GetIniInt(Section,"Left",form->Left);
        form->Width=GetIniInt(Section,"Width",form->Width);
        }
    else{
        form->WindowState=wsNormal;
        form->Top=GetIniInt(Section,"Top",form->Top);
        form->Height=GetIniInt(Section,"Height",form->Height);
        form->Left=GetIniInt(Section,"Left",form->Left);
        form->Width=GetIniInt(Section,"Width",form->Width);
        form->WindowState=wsMaximized;
        }
}

