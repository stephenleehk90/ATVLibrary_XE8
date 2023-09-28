//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "find.h"
#include "dmData.h"
#include "lenddatas.h"
#include "commfunc.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmFind *frmFind;
//---------------------------------------------------------------------------
__fastcall TfrmFind::TfrmFind(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmFind::StartFind()
{
  AnsiString l_strSQLClause,l_strPosition,l_strType,l_strRemark,l_strRegTime,l_strState,l_strBorrowID;
  TListItem *pListItem;
 if (edtMaterialID->Text.Trim().IsEmpty())
 return;
 l_strSQLClause =  (AnsiString)"Select * from lib_Res where ResID = \'"
                  +SQLStr(edtMaterialID->Text)
				  +(AnsiString)"\'";
 dmLendDM->daqResource->SQL->Clear();
 dmLendDM->daqResource->SQL->Add(l_strSQLClause);
 try
 {
 dmLendDM->daqResource->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_Res",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
if (dmLendDM->daqResource->RecordCount==0)
{
 Application->MessageBox(L"Can not find the material in the database!",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
 edtMaterialID->SetFocus();
 edtMaterialID->SelectAll();
 return;
}
dmLendDM->daqResource->First();
while (!dmLendDM->daqResource->Eof)
 {
  if (dmLendDM->daqResource->FieldValues["Position"].IsNull())
  l_strPosition = (AnsiString)"";
  else
  l_strPosition = OrigStr(dmLendDM->daqResource->FieldValues["Position"]);

  switch ((int)(dmLendDM->daqResource->FieldValues["ResType"]))
  {
   case 1:l_strType = (AnsiString)"Tapes";
		  break;
   case 2:l_strType = (AnsiString)"Books";
		  break;
   case 3:l_strType = (AnsiString)"Magzines";
		  break;
   case 4:l_strType = (AnsiString)"Files";
		  break;

  }
  if (dmLendDM->daqResource->FieldValues["Remark"].IsNull())
  l_strRemark = (AnsiString)"";
  else
  l_strRemark = OrigStr(dmLendDM->daqResource->FieldValues["Remark"]);

  if (dmLendDM->daqResource->FieldValues["RegTime"].IsNull())
  l_strRegTime = (AnsiString)"";
  else
  //l_strRegTime = ((TDateTime)dmLendDM->daqResource->FieldValues["RegTime"].FormatString("yyyy/mm/dd hh:nn");
	 l_strRegTime = (dmLendDM->daqResource->FieldByName("RegTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");
  switch ((int)(dmLendDM->daqResource->FieldValues["State"]))
  {
   case 0:l_strState  = (AnsiString)"On Shelf";
		  break;
   case 1:l_strState  = (AnsiString)"Borrowed";
		  break;
   case -1:l_strState = (AnsiString)"Lost";
		  break;
  }

  if (dmLendDM->daqResource->FieldValues["BorrowerID"].IsNull())
  l_strBorrowID = (AnsiString)"";
  else
  l_strBorrowID = OrigStr(dmLendDM->daqResource->FieldValues["BorrowerID"]);
//  if (lstvFind->Items->Count == lstvFind->VisibleRowCount)
//      lstvFind->Items->Delete(lstvFind->Items->Count-1);
  pListItem = lstvFind->Items->Add();
  pListItem->Caption = edtMaterialID->Text;
  pListItem->SubItems->Add(l_strPosition);
  pListItem->SubItems->Add(l_strType);
  pListItem->SubItems->Add(l_strRemark);
  pListItem->SubItems->Add(l_strRegTime);
  pListItem->SubItems->Add(l_strState);
  pListItem->SubItems->Add(l_strBorrowID);
  dmLendDM->daqResource->Next();
 /* if (lstvFind->Items->Count == lstvFind->VisibleRowCount)
      lstvFind->Items->Delete(0);
  lstvFind->Items->Add();
  lstvFind->Items->Item[lstvFind->Items->Count-1]->Caption = edtMaterialID->Text;
  lstvFind->Items->Item[lstvFind->Items->Count-1]->SubItems->Add(l_strPosition);
  lstvFind->Items->Item[lstvFind->Items->Count-1]->SubItems->Add(l_strType);
  lstvFind->Items->Item[lstvFind->Items->Count-1]->SubItems->Add(l_strRemark);
  lstvFind->Items->Item[lstvFind->Items->Count-1]->SubItems->Add(l_strRegTime);
  lstvFind->Items->Item[lstvFind->Items->Count-1]->SubItems->Add(l_strState);
  lstvFind->Items->Item[lstvFind->Items->Count-1]->SubItems->Add(l_strBorrowID);
  dmLendDM->daqResource->Next();*/
 }
/*( dmLendDM->dbLendDB->StartTransaction();
 if (!CLdSystemData->CreateLog((AnsiString)"Find Material",(AnsiString)"lib_Res",(AnsiString)"None",(AnsiString)"LendModule",(AnsiString)""))
 dmLendDM->dbLendDB->Rollback();
 else
 dmLendDM->dbLendDB->Commit();
*/
// edtMaterialID->SetFocus();
 lstvFind->ItemFocused  = pListItem;
 lstvFind->Selected = pListItem;
 lstvFind->SetFocus(); 
}
void __fastcall TfrmFind::btnFindClick(TObject *Sender)
{
 TListItem* pListItem;
 pListItem=lstvFind->FindCaption(0,edtMaterialID->Text,false,true,false);
 if (pListItem==NULL)
   StartFind();
 else
 {
   pListItem->MakeVisible(false);
   lstvFind->ItemFocused=pListItem;   
   lstvFind->Selected=pListItem;
   lstvFind->SetFocus();
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmFind::btnClearClick(TObject *Sender)
{
  lstvFind->Items->Clear();
  edtMaterialID->SetFocus();
  edtMaterialID->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TfrmFind::FormShow(TObject *Sender)
{
 edtMaterialID->SetFocus();    
}
//---------------------------------------------------------------------------

