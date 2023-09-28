//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Borrower.h"
#include "CommFunc.h"
#include "lendmain.h"
#include "dmData.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmBorrower *frmBorrower;
//---------------------------------------------------------------------------
__fastcall TfrmBorrower::TfrmBorrower(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmBorrower::btnOkClick(TObject *Sender)
{
    AnsiString strSQLClause,strUser;
    strUser=edtUser->Text.Trim();
    if (strUser.IsEmpty())
    {
      ModalResult=mrNone;
	  Application->MessageBox(L"Please input the user's id,code or name.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      return;
    }
    if (strUser=="everyone")
    {
	   frmLendSys->lblUserID->Caption=(AnsiString)"everyone";
       return;
    }
    switch (cboMode->ItemIndex)
    {
      case 0:
		  strSQLClause=(AnsiString)"select UserID from lib_User where code='"
                       +SQLStr(strUser)+(AnsiString)"'";
          break;
      case 1:
          strSQLClause=(AnsiString)"select UserID from lib_User where UserID='"
                       +SQLStr(strUser)+(AnsiString)"'";
          break;
      case 2:
          strSQLClause=(AnsiString)"select UserID from lib_User where name='"
                       +SQLStr(strUser)+(AnsiString)"'";
          break;
    }
    dmLendDM->daqUser->SQL->Clear();
    dmLendDM->daqUser->SQL->Add(strSQLClause);
    try
    {
       dmLendDM->daqUser->Open();
       dmLendDM->daqUser->First();
    }
    catch(...)
    {
       ModalResult=mrNone;
	   Application->MessageBox(L"Can not open table lib_User",L"TBS Library",MB_OK|MB_ICONINFORMATION);
       return;
    }
	if (dmLendDM->daqUser->RecordCount<1)
    {
       ModalResult=mrNone;
	   Application->MessageBox(L"The user does not exist,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	   edtUser->SetFocus();
	   edtUser->SelectAll();
	   return;
	}
	frmLendSys->lblUserID->Caption=OrigStr(dmLendDM->daqUser->FieldByName("UserID")->AsString);

}
//---------------------------------------------------------------------------
void __fastcall TfrmBorrower::FormShow(TObject *Sender)
{
  if (edtUser->Text.Trim().IsEmpty())
	 cboMode->ItemIndex=0;
  edtUser->SetFocus();
  edtUser->SelectAll();
}
//---------------------------------------------------------------------------


void __fastcall TfrmBorrower::chkEveryoneClick(TObject *Sender)
{
	if (chkEveryone->Checked)
		{
		edtUser->Text="everyone";
		}
	edtUser->Enabled=!chkEveryone->Checked;
    cboMode->Enabled=!chkEveryone->Checked;
    ActiveControl=btnOk;
}
//---------------------------------------------------------------------------

