//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "options.h"
#include "lenddatas.h"
#include "dmData.h"
#include "lendmain.h"
#include "commfunc.h"
//---------------------------------------------------------------------------
#pragma link "cspin"
#pragma resource "*.dfm"
TfrmOption *frmOption;
//---------------------------------------------------------------------------
__fastcall TfrmOption::TfrmOption(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::FormShow(TObject *Sender)
{
 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add("Select * from lib_ResTypes where TypeID=1");
 try
 {
  dmLendDM->daqLendDays->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
 if (dmLendDM->daqLendDays->RecordCount==0)
 {
   Application->MessageBox(L"Error configuration",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   return;
 }
 edtTapeDays->Text = dmLendDM->daqLendDays->FieldValues["BorrowDays"];
 CLdSystemData->nTapeDays = (int)dmLendDM->daqLendDays->FieldValues["BorrowDays"];
 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add("Select * from lib_ResTypes where TypeID=2");
 try
 {
  dmLendDM->daqLendDays->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
 if (dmLendDM->daqLendDays->RecordCount==0)
 {
   Application->MessageBox(L"Error configuration",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   return;
 }
 edtBookDays->Text = dmLendDM->daqLendDays->FieldValues["BorrowDays"];
 CLdSystemData->nBookDays = (int)dmLendDM->daqLendDays->FieldValues["BorrowDays"];

 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add("Select * from lib_ResTypes where TypeID=3");
 try
 {
  dmLendDM->daqLendDays->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
 if (dmLendDM->daqLendDays->RecordCount==0)
 {
   Application->MessageBox(L"Error configuration",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   return;
 }
 edtMagazineDays->Text = dmLendDM->daqLendDays->FieldValues["BorrowDays"];
 CLdSystemData->nMagazineDays = (int)dmLendDM->daqLendDays->FieldValues["BorrowDays"];
 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add("Select * from lib_ResTypes where TypeID=4");
 try
 {
  dmLendDM->daqLendDays->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
 if (dmLendDM->daqLendDays->RecordCount==0)
 {
   Application->MessageBox(L"Error configuration",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   return;
 }
 edtFileDays->Text = dmLendDM->daqLendDays->FieldValues["BorrowDays"];
 CLdSystemData->nFileDays = (int)dmLendDM->daqLendDays->FieldValues["BorrowDays"];
 edtQueryDays->Text = IntToStr(CLdSystemData->nNormalQueryDays);
 //edtTapeDays->SetFocus();
 //edtTapeDays->SelectAll();
 if(CLdSystemData->UserRight>1)
  {
   edtTapeDays->Enabled = false;
   edtBookDays->Enabled = false;
   edtMagazineDays->Enabled = false;
   edtFileDays->Enabled = false;
   edtQueryDays->SetFocus();
  }
  else
  {
	 edtTapeDays->SetFocus();
	 edtTapeDays->SelectAll();
   }
 }
//---------------------------------------------------------------------------
void __fastcall TfrmOption::spbQueryDaysUpClick(TObject *Sender)
{
 edtQueryDays->Text = IntToStr(StrToInt(edtQueryDays->Text)+1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::spbQueryDaysDownClick(TObject *Sender)
{
 if  (StrToInt(edtQueryDays->Text)!=0)
 edtQueryDays->Text = IntToStr(StrToInt(edtQueryDays->Text)-1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::btnDaysOKClick(TObject *Sender)
{
 SetDays();
}
//---------------------------------------------------------------------------
void __fastcall TfrmOption::SetDays()
{

  if ((StrToInt(edtTapeDays->Text)<=0)||(edtTapeDays->Text.IsEmpty()))
  {
   Application->MessageBox(L"Invalid Number",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtTapeDays->Text = String(CLdSystemData->nTapeDays);
   edtTapeDays->SetFocus();
   return;
  }

  if ((StrToInt(edtBookDays->Text)<=0)||(edtBookDays->Text.IsEmpty()))
  {
   Application->MessageBox(L"Invalid Number",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtBookDays->Text = String(CLdSystemData->nBookDays);
   edtBookDays->SetFocus();
   return;
  }

  if ((StrToInt(edtMagazineDays->Text)<=0)||(edtMagazineDays->Text.IsEmpty()))
  {
   Application->MessageBox(L"Invalid Number",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtMagazineDays->Text = String(CLdSystemData->nMagazineDays);
   edtMagazineDays->SetFocus();
   return;
  }

  if ((StrToInt(edtFileDays->Text)<=0)||(edtFileDays->Text.IsEmpty()))
  {
   Application->MessageBox(L"Invalid Number",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtFileDays->Text = String(CLdSystemData->nFileDays);
   edtFileDays->SetFocus();
   return;
  }

  if ((StrToInt(edtQueryDays->Text)<=0)||(edtQueryDays->Text.IsEmpty()))
  {
   Application->MessageBox(L"Invalid Number",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtQueryDays->Text = String(CLdSystemData->nNormalQueryDays);
   edtQueryDays->SetFocus();
   return;
  }

  CLdSystemData->nTapeDays = StrToInt(edtTapeDays->Text);
  CLdSystemData->nBookDays = StrToInt(edtBookDays->Text);
  CLdSystemData->nMagazineDays = StrToInt(edtMagazineDays->Text);
  CLdSystemData->nFileDays = StrToInt(edtFileDays->Text);
  CLdSystemData->nNormalQueryDays = StrToInt(edtQueryDays->Text);

  ModalResult = mrOk;

}

//---------------------------------------------------------------------------


void __fastcall TfrmOption::edtTapeDaysKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key ==VK_RETURN)
     SetDays();
  if (Key == VK_ESCAPE)
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmOption::edtTapeDaysKeyPress(TObject *Sender, char &Key)
{
    if ((Key!=VK_RETURN)&&(Key!=VK_ESCAPE)&&(Key!=VK_BACK))
    {
      if ((Key!=0x30)&&(Key!=0x31)&&(Key!=0x32)&&(Key!=0x33)&&(Key!=0x34)
      &&(Key!=0x35)&&(Key!=0x36)&&(Key!=0x37)&&(Key!=0x38)&&(Key!=0x39))
        Key = NULL;
      if (((edtTapeDays->Text.IsEmpty())||(edtTapeDays->SelStart==0))
          &&(Key==0x30))
        Key = NULL;
      if (edtTapeDays->Text.Length()>=3)
        Key = NULL;

    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmOption::edtBookDaysKeyPress(TObject *Sender, char &Key)
{
    if ((Key!=VK_RETURN)&&(Key!=VK_ESCAPE)&&(Key!=VK_BACK))
    {
      if ((Key!=0x30)&&(Key!=0x31)&&(Key!=0x32)&&(Key!=0x33)&&(Key!=0x34)
      &&(Key!=0x35)&&(Key!=0x36)&&(Key!=0x37)&&(Key!=0x38)&&(Key!=0x39))
        Key = NULL;
      if (((edtBookDays->Text.IsEmpty())||(edtBookDays->SelStart==0))
          &&(Key==0x30))
        Key = NULL;
      if (edtBookDays->Text.Length()>=3)
        Key = NULL;

    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmOption::edtMagazineDaysKeyPress(TObject *Sender,
      char &Key)
{
    if ((Key!=VK_RETURN)&&(Key!=VK_ESCAPE)&&(Key!=VK_BACK))
    {
      if ((Key!=0x30)&&(Key!=0x31)&&(Key!=0x32)&&(Key!=0x33)&&(Key!=0x34)
      &&(Key!=0x35)&&(Key!=0x36)&&(Key!=0x37)&&(Key!=0x38)&&(Key!=0x39))
        Key = NULL;
      if (((edtMagazineDays->Text.IsEmpty())||(edtMagazineDays->SelStart==0))
          &&(Key==0x30))
        Key = NULL;
      if (edtMagazineDays->Text.Length()>=3)
        Key = NULL;

    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmOption::edtFileDaysKeyPress(TObject *Sender, char &Key)
{
    if ((Key!=VK_RETURN)&&(Key!=VK_ESCAPE)&&(Key!=VK_BACK))
    {
      if ((Key!=0x30)&&(Key!=0x31)&&(Key!=0x32)&&(Key!=0x33)&&(Key!=0x34)
      &&(Key!=0x35)&&(Key!=0x36)&&(Key!=0x37)&&(Key!=0x38)&&(Key!=0x39))
        Key = NULL;
      if (((edtFileDays->Text.IsEmpty())||(edtFileDays->SelStart==0))
          &&(Key==0x30))
        Key = NULL;
      if (edtFileDays->Text.Length()>=3)
        Key = NULL;
    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmOption::edtQueryDaysKeyPress(TObject *Sender,
      char &Key)
{
    if ((Key!=VK_RETURN)&&(Key!=VK_ESCAPE)&&(Key!=VK_BACK))
    {
      if ((Key!=0x30)&&(Key!=0x31)&&(Key!=0x32)&&(Key!=0x33)&&(Key!=0x34)
      &&(Key!=0x35)&&(Key!=0x36)&&(Key!=0x37)&&(Key!=0x38)&&(Key!=0x39))
        Key = NULL;
      if (((edtQueryDays->Text.IsEmpty())||(edtQueryDays->SelStart==0))
          &&(Key==0x30))
        Key = NULL;
      if (edtQueryDays->Text.Length()>=3)
        Key = NULL;
    }

}
//---------------------------------------------------------------------------

