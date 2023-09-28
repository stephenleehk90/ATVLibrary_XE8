//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SaveFrm.h"
#include "LogFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TfrmSave *frmSave;
//---------------------------------------------------------------------------
__fastcall TfrmSave::TfrmSave(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmSave::spnedtDaysKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_RETURN)
  {
     frmLogs->nSaveDays=spnedtDays->Value;
     ModalResult=true;
     Close();
  }
  else if (Key==VK_ESCAPE)
     Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSave::FormShow(TObject *Sender)
{
  spnedtDays->Value=frmLogs->nSaveDays;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSave::btnOkClick(TObject *Sender)
{
  frmLogs->nSaveDays=spnedtDays->Value;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSave::spnedtDaysKeyPress(TObject *Sender, char &Key)
{
    if ((Key!=VK_RETURN)&&(Key!=VK_ESCAPE)&&(Key!=VK_BACK))
    {
      if ((Key!=0x30)&&(Key!=0x31)&&(Key!=0x32)&&(Key!=0x33)&&(Key!=0x34)
      &&(Key!=0x35)&&(Key!=0x36)&&(Key!=0x37)&&(Key!=0x38)&&(Key!=0x39))
        Key = NULL;
      if (((spnedtDays->Text.IsEmpty())||(spnedtDays->SelStart==0))
          &&(Key==0x30))
        Key = NULL;
      if (spnedtDays->Text.Length()>=3)
        Key = NULL;

    }
}
//---------------------------------------------------------------------------

