//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FindUser.h"
#include "CommFunc.h"
#include "dmdata.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmFindUser *frmFindUser;
//---------------------------------------------------------------------------
__fastcall TfrmFindUser::TfrmFindUser(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmFindUser::btnClearClick(TObject *Sender)
{
  lstvwUser->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFindUser::FormShow(TObject *Sender)
{
 edtUser->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmFindUser::btnFindClick(TObject *Sender)
{

    TListItem* pListItem;
    AnsiString strSQLClause,strUser;

    strUser=edtUser->Text.Trim();
    if (strUser.IsEmpty())
    {
	  Application->MessageBox(L"Please input the user's id.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      edtUser->SetFocus();
      edtUser->SelectAll();
      return;
    }

    pListItem=lstvwUser->FindCaption(0,strUser,false,true,true);
    if (pListItem!=NULL)
    {
       pListItem->MakeVisible(false);
       lstvwUser->ItemFocused=pListItem;
       lstvwUser->Selected=pListItem;
       lstvwUser->SetFocus();
       return;
    }
    strSQLClause=(AnsiString)"select UserID,name,code,phone,department from lib_User where UserID='"
                 +SQLStr(strUser)+(AnsiString)"'";
    dmLendDM->daqUser->SQL->Clear();
    dmLendDM->daqUser->SQL->Add(strSQLClause);
    dmLendDM->daqUser->Open();
    if (dmLendDM->daqUser->RecordCount<1)
    {
	   Application->MessageBox(L"There is no this user.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
       return;
    }
    pListItem=lstvwUser->Items->Add();
	pListItem->Caption=OrigStr(dmLendDM->daqUser->FieldByName("UserID")->AsString);
    pListItem->SubItems->Add(OrigStr(dmLendDM->daqUser->FieldByName("name")->AsString));
    pListItem->SubItems->Add(OrigStr(dmLendDM->daqUser->FieldByName("code")->AsString));
    pListItem->SubItems->Add(OrigStr(dmLendDM->daqUser->FieldByName("phone")->AsString));
    pListItem->SubItems->Add(OrigStr(dmLendDM->daqUser->FieldByName("department")->AsString));
    lstvwUser->ItemFocused=pListItem;
    lstvwUser->Selected=pListItem;
    lstvwUser->SetFocus();

}
//---------------------------------------------------------------------------

