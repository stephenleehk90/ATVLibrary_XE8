//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PwdFrm.h"
#include "StoryData.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmPassword *frmPassword;
//---------------------------------------------------------------------------
__fastcall TfrmPassword::TfrmPassword(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPassword::btnOkClick(TObject *Sender)
{
  AnsiString strSQLClause,strPassword;
  strSQLClause=(AnsiString)"select passwd from dbo.lib_User where UserID='"
			   +SQLStr(dmdStory->strCurrentUserID)+(AnsiString)"'";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
	dmdStory->daqCheck->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	ModalResult=mrNone;
	btnCancel->SetFocus();
	return;
  }
  strPassword=OrigStr(dmdStory->daqCheck->FieldByName("passwd")->AsString);
  DecodeString(strPassword);
  if (edtOld->Text!=strPassword)
  {
	Application->MessageBox(L"The password is not right,please confirm your old password!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	ModalResult=mrNone;
	edtOld->SetFocus();
	return;
  }
  if (edtNew->Text!=edtConfirm->Text)
  {
	Application->MessageBox(L"Please confirm your new password!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	ModalResult=mrNone;
	edtConfirm->SetFocus();
	return;
  }
  strPassword=edtNew->Text.Trim();
  EncodeString(strPassword);
  strSQLClause=(AnsiString)"update dbo.lib_User set passwd='"
			   +SQLStr(strPassword)+(AnsiString)"'where UserID='"
			   +SQLStr(dmdStory->strCurrentUserID)+(AnsiString)"'";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
	dmdStory->daqCheck->ExecSQL();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	ModalResult=mrNone;
	btnCancel->SetFocus();
	return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPassword::FormShow(TObject *Sender)
{
  edtOld->Clear();
  edtNew->Clear();
  edtConfirm->Clear();
  edtOld->SetFocus();
}
//---------------------------------------------------------------------------
