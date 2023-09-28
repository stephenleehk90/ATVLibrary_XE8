// $Header :  $
//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutFrm.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
  AnsiString str1="$Revision: 40 $";
  str1.SetLength(str1.Length()-2);
  Version->Caption=(AnsiString)"Version : 2.0  (Build No. "+
       str1.SubString(12,str1.Length()-11)+(AnsiString)")";
}
//---------------------------------------------------------------------------

