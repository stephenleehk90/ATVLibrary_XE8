//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>

#pragma hdrstop

#include "lendmain.h"
#include "lenddatas.h"
#include "inforeport.h"
#include "dmData.h"
#include "options.h"
#include "find.h"
#include "Finduser.h"
#include "Borrower.h"
#include "commfunc.h"
#include "findmaterail.h"
#include "frm_Main.h"
#include "StoryData.h"
#include "stdlib.h"
//---------------------------------------------------------------------------
#pragma link "EditBtn"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "AdvListV"
#pragma resource "*.dfm"
TfrmLendSys *frmLendSys;
int ColumnToSort = 0;
//----------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TfrmLendSys::TfrmLendSys(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnLendAddClick(TObject *Sender)
{
 AnsiString l_strSQLClause,l_strBorrowerID;
 TListItem* pListItem;
 if (edtLendMaterial->Text.Trim().IsEmpty())
  {
   Application->MessageBox(L"MaterialID missing",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtLendMaterial->SetFocus();
   return;
  }

 if (edtLendUser->Text.Trim().IsEmpty())
  {
   Application->MessageBox(L"User name missing",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtLendUser->SetFocus();
   return;
  }
  l_strSQLClause = (AnsiString)"Select  ResType,state,Position from dbo.lib_Res where ResID = \'" + SQLStr(edtLendMaterial->Text) +(AnsiString)"\'";

  dmLendDM->daqResource->SQL->Clear();
  dmLendDM->daqResource->SQL->Add(l_strSQLClause);
  try
  {
   dmLendDM->daqResource->Open();
  }
  catch(...)
  {
   Application->MessageBox(L"Can not open table dbo.lib_Res",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtLendMaterial->SetFocus();
   edtLendMaterial->SelectAll();
   return;
  }

  if (dmLendDM->daqResource->RecordCount==0)
   {
	Application->MessageBox(L"Can not find the material in the database",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	edtLendMaterial->SetFocus();
	edtLendMaterial->SelectAll();
	return;
   }
  if ((int)(dmLendDM->daqResource->FieldValues["state"])==1)
   {
	Application->MessageBox(L"The resource has been lent",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	edtLendMaterial->SetFocus();
	edtLendMaterial->SelectAll();
	return;
   }

  if ((int)(dmLendDM->daqResource->FieldValues["state"])==-1)
   {
	Application->MessageBox(L"The Material has lost",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	edtLendMaterial->SetFocus();
	edtLendMaterial->SelectAll();
	return;
   }
  switch (cmbMode->ItemIndex)
  {
   case 0:l_strSQLClause = (AnsiString)"Select UserID from  dbo.lib_User where code = \'"+SQLStr(edtLendUser->Text)+(AnsiString)"\'";
		   break;
   case 1:l_strSQLClause = (AnsiString)"Select UserID from  dbo.lib_User where UserID = \'"+SQLStr(edtLendUser->Text)+(AnsiString)"\'";
		   break;
   case 2:l_strSQLClause = (AnsiString)"Select UserID from  dbo.lib_User where name = \'"+SQLStr(edtLendUser->Text)+(AnsiString)"\'";
		   break;
  }


  dmLendDM->daqUser->SQL->Clear();
  dmLendDM->daqUser->SQL->Add(l_strSQLClause);
  try
  {
   dmLendDM->daqUser->Open();
  }
  catch(...)
  {
   Application->MessageBox(L"Can not open table dbo.lib_User",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtLendUser->SetFocus();
   edtLendUser->SelectAll();
   return;
  }
  if (dmLendDM->daqUser->RecordCount==0)
   {
	Application->MessageBox(L"Can not find the User in the database",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	edtLendUser->SetFocus();
	edtLendUser->SelectAll();
	return;
   }

 if (dmLendDM->daqUser->RecordCount>1)
   {
	Application->MessageBox(L"The user name is not unique",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	edtLendUser->SetFocus();
	edtLendUser->SelectAll();
	return;
   }

  l_strBorrowerID = OrigStr(dmLendDM->daqUser->FieldValues["UserID"]);
  if (!CLdSystemData->SearchInListBox(edtLendMaterial->Text,lstvLend))
   {
	//Add the record into listbox
//    if (!CLdSystemData->SearchInListBox(cmbLendUser->Text,cmbLendUser))
//       cmbLendUser->Items->Add(cmbLendUser->Text);
	pListItem=lstvLend->Items->Add();
	lstvLend->ItemFocused = pListItem;
	lstvLend->Selected = pListItem;

	pListItem->Caption = edtLendMaterial->Text;
	pListItem->SubItems->Add(l_strBorrowerID);
	pListItem->SubItems->Add(CLdSystemData->strCurrentUser);
	switch ((int)(dmLendDM->daqResource->FieldValues["ResType"]))
	 {case 1:
		  pListItem->SubItems->Add((AnsiString)"Tapes");
		  break;
	  case 2:
		  pListItem->SubItems->Add((AnsiString)"Books");
		  break;
	  case 3:
		  pListItem->SubItems->Add((AnsiString)"Magazines");
		  break;
	  case 4:
		  pListItem->SubItems->Add((AnsiString)"Files");
		  break;
	 }
	try
	{
	 dmLendDM->daqTime->Open();
	}
	catch(...)
	{
	 Application->MessageBox(L"Failed execute the SQL Clause:select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	 return;
	}
	//m_dtTimer = dmLendDM->daqTime->FieldValues["Column1"];
	m_dtTimer = dmLendDM->daqTime->FieldByName("Column1")->AsDateTime;
	m_strCurrentTime = m_dtTimer.DateTimeString();
//    lstvLend->Items->Item[CLdSystemData.nLendOrReturnCount]->SubItems->Add(m_strCurrentTime);
//    if  (!(dmLendDM->daqResource->FieldValues["remark"].IsNull()))
//    lstvLend->Items->Item[CLdSystemData->nLendOrReturnCount]->SubItems->Add((AnsiString)dmLendDM->daqResource->FieldValues["remark"]);
//    else
	pListItem->SubItems->Add(edtLendRemark->Text);
//    AnsiString strTemp = OrigStr((AnsiString)dmLendDM->daqResource->FieldValues["Position"]);
	pListItem->SubItems->Add(OrigStr(dmLendDM->daqResource->FieldValues["Position"]));
	CLdSystemData->nLendOrReturnCount++;
	frmMain->stbStory->Panels->Items[1]->Text =(AnsiString)"Material Number: "+IntToStr(CLdSystemData->nLendOrReturnCount);
	btnLendConfirm->Enabled = true;
	btnLendDelete->Enabled = true;
	btnLendDiscard->Enabled = true;
	muiLendConfirm->Enabled = true;
	muiLendDelete->Enabled = true;
	muiLendDiscard->Enabled = true;
  }
  edtLendMaterial->SetFocus();
  edtLendMaterial->SelectAll();
 }
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::btnLendDiscardClick(TObject *Sender)
{
 if (Application->MessageBox(L"Are you sure to discard the operation",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
{
 lstvLend->Items->Clear();
// edtLendMaterial->Text = "";
// cmbLendUser ->Text = "";
 CLdSystemData->nLendOrReturnCount = 0;
 frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: 0";
 edtLendMaterial->SetFocus();
 btnLendConfirm->Enabled = false;
 btnLendDelete ->Enabled = false;
 btnLendDiscard->Enabled = false;
 muiLendDelete->Enabled = false ;
 muiLendConfirm->Enabled = false;
 muiLendDiscard->Enabled = false;
 }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnLendDeleteClick(TObject *Sender)
{
 LendDelete();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnLendConfirmClick(TObject *Sender)
{
 AnsiString l_strSQLClause,l_strTemp;
 //Update  the database
 dmLendDM->dbMain->StartTransaction();
 for (int i=0;i<lstvLend->Items->Count;i++)
  {
   l_strTemp = lstvLend->Items->Item[i]->SubItems->Strings[4];
   if ((l_strTemp.Length()==0)||((l_strTemp[1]!='(')&&(l_strTemp[l_strTemp.Length()]!=')')))
   l_strTemp = (AnsiString)"("+l_strTemp+(AnsiString)")";
   l_strSQLClause = "Update dbo.lib_Res set state = 1,BorrowerID = \'"+SQLStr(lstvLend->Items->Item[i]->SubItems->Strings[0])
                    +(AnsiString)"\',Position = \'"
                    +SQLStr(l_strTemp)
                    +"\' where ResID = \'"+SQLStr(lstvLend->Items->Item[i]->Caption)+(AnsiString)"\'";
   dmLendDM->daqResource->SQL->Clear();
   dmLendDM->daqResource->SQL->Add(l_strSQLClause);
   try
   {
   dmLendDM->daqResource->ExecSQL();
   }
   catch(...)
   {
	Application->MessageBox(L"Failed update table lib_ResID",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	dmLendDM->dbMain->Rollback();
	return;
   }
   l_strSQLClause = (AnsiString)"Select BorrowDays from lib_ResTypes where TypeName =\'"
                    +SQLStr(lstvLend->Items->Item[i]->SubItems->Strings[2])
                    +(AnsiString)"\'";
   dmLendDM->daqLendDays->SQL->Clear();
   dmLendDM->daqLendDays->SQL->Add(l_strSQLClause);
   try
   {
    dmLendDM->daqLendDays->Open();
   }
   catch(...)
   {
	Application->MessageBox(L"Failed update table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	dmLendDM->dbMain->Rollback();
	return;
   }
   if (dmLendDM->daqLendDays->RecordCount==0)
   {
	Application->MessageBox(L"Can not find the ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	dmLendDM->dbMain->Rollback();
	return;
   }
   l_strSQLClause = (AnsiString)"Insert into lib_LendInfo values(\'"
					+SQLStr(lstvLend->Items->Item[i]->SubItems->Strings[0])
					+(AnsiString)"\',"
					+(AnsiString)"\'"
					+SQLStr(lstvLend->Items->Item[i]->Caption)
					+(AnsiString)"\',GetDate(),DateAdd(day,"
					+dmLendDM->daqLendDays->FieldValues["BorrowDays"]
					+(AnsiString)",GetDate()),\'"
					+SQLStr(lstvLend->Items->Item[i]->SubItems->Strings[1])
					+(AnsiString)"\',NULL,NULL,\'"
					+SQLStr(lstvLend->Items->Item[i]->SubItems->Strings[3])
					+(AnsiString)"\',0)";
   dmLendDM->daqLendInfo->SQL->Clear();
   dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
   try
   {
	dmLendDM->daqLendInfo->ExecSQL();
   }
   catch(...)
   {
	Application->MessageBox(L"Failed update table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	dmLendDM->dbMain->Rollback();
	return;
   }
/*   if (!CLdSystemData->CreateLog("LendOperation",lstvLend->Items->Item[i]->Caption,"","LendModule",lstvLend->Items->Item[i]->SubItems->Strings[3]))
	{
	 dmLendDM->dbLendDB->Rollback();
	 return;
	}*/
  }
 dmLendDM->dbMain->Commit();
 lstvLend->Items->Clear();
 CLdSystemData->nLendOrReturnCount = 0;
 frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: 0";
// edtLendMaterial->Text = (AnsiString)"";
// cmbLendUser->Text = (AnsiString)"";
// edtLendRemark->Text = (AnsiString)"";
 edtLendMaterial->Clear();
 edtLendUser->Clear();
 edtLendUser->SetFocus();
 btnLendDelete->Enabled = false;
 btnLendConfirm->Enabled = false;
 btnLendDiscard->Enabled = false;
 muiLendDelete->Enabled = false;
 muiLendConfirm->Enabled = false;
 muiLendDiscard->Enabled = false;
 }
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnReturnAddClick(TObject *Sender)
{
 AnsiString l_strSQLClause,l_strIsOutOfTime,l_strRemark,l_strLendTime,l_strLendOpID;
 TDateTime  l_dtLendTimer;
 TListItem *pListItem;
 if (edtReturnMaterial->Text.Trim().IsEmpty())
  {
   Application->MessageBox(L"MaterialID missing",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   edtReturnMaterial->SetFocus();
   return;
  }
 l_strSQLClause = (AnsiString)"Select ResID,LendTime,DueTime,LendOpID,remark from dbo.lib_LendInfo where ReturnTime is NULL and ResID = \'"
				 +SQLStr(edtReturnMaterial->Text)
				 +(AnsiString)"\'";
 dmLendDM->daqLendInfo->SQL->Clear();
 dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
 try
 {
 dmLendDM->daqLendInfo->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open the table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  edtReturnMaterial->SetFocus();
  edtReturnMaterial->SelectAll();
  return;
 }
 if (dmLendDM->daqLendInfo->RecordCount==0)
 {
  Application->MessageBox(L"No such record in database",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  edtReturnMaterial->SetFocus();
  edtReturnMaterial->SelectAll();
  return;
 }
 try
 {
 dmLendDM->daqTime->Open();
 }
 catch(...)
 {
 Application->MessageBox(L"Failed execute SQL Clause:Select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
 edtReturnMaterial->SetFocus();
 edtReturnMaterial->SelectAll();
 return;
 }
// m_dtTimer     = (TDateTime)dmLendDM->daqTime->FieldValues["Column1"];
 m_dtTimer = dmLendDM->daqTime->FieldByName("Column1")->AsDateTime;
// l_dtLendTimer =  (TDateTime)(dmLendDM->daqLendInfo->FieldValues["LendTime"]);
 l_dtLendTimer = dmLendDM->daqLendInfo->FieldByName("LendTime")->AsDateTime;
// l_strLendTime = ((TDateTime)(dmLendDM->daqLendInfo->FieldValues["LendTime"])).FormatString("yyyy/mm/dd hh:nn");
 l_strLendTime = (dmLendDM->daqLendInfo->FieldByName("LendTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");

 l_strLendOpID = OrigStr((dmLendDM->daqLendInfo->FieldValues["LendOpID"]));
 l_strRemark   =  edtReturnRemark->Text;
// if (!(dmLendDM->daqLendInfo->FieldValues["Remark"].IsNull()))
// l_strRemark   =  (AnsiString)(dmLendDM->daqLendInfo->FieldValues["Remark"]);
// l_nLendDays   =  (int)(m_dtTimer-l_dtLendTimer);
// if (m_dtTimer>(TDateTime)(dmLendDM->daqLendInfo->FieldValues["DueTime"]))
 if (m_dtTimer>dmLendDM->daqLendInfo->FieldByName("DueTime")->AsDateTime)
	l_strIsOutOfTime = (AnsiString)"Over Time";
 else
	l_strIsOutOfTime = (AnsiString)"";


 if (!CLdSystemData->SearchInListBox(edtReturnMaterial->Text,lstvReturn))
 {
   pListItem = lstvReturn->Items->Add();
   lstvReturn->ItemFocused = pListItem;
   lstvReturn->Selected = pListItem;

   pListItem->Caption = edtReturnMaterial->Text;
   try
   {
	dmLendDM->daqTime->Open();
   }
   catch(...)
   {
	Application->MessageBox(L"Failed execute the SQL Clause:select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	return;
   }
   //m_dtTimer = (TDateTime)dmLendDM->daqTime->FieldValues["Column1"];
	m_dtTimer = dmLendDM->daqTime->FieldByName("Column1")->AsDateTime;
	   //CLdSystemData.dtTimer.CurrentDateTime();
   m_strCurrentTime = m_dtTimer.DateTimeString();

  // lstvReturn->Items->Item[CLdSystemData.nLendOrReturnCount]->SubItems->Add(m_strCurrentTime);
   pListItem->SubItems->Add(CLdSystemData->strCurrentUser);
   pListItem->SubItems->Add(l_strIsOutOfTime);
   pListItem->SubItems->Add(l_strLendOpID);
   pListItem->SubItems->Add(l_strLendTime);
   pListItem->SubItems->Add(l_strRemark);
   CLdSystemData->nLendOrReturnCount++;
   frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: "+IntToStr(CLdSystemData->nLendOrReturnCount);
   btnReturnConfirm->Enabled = true;
   btnReturnDelete->Enabled  = true;
   btnReturnDiscard->Enabled = true;
   muiReturnConfirm->Enabled = true;
   muiReturnDelete->Enabled  = true;
   muiReturnDiscard->Enabled = true;

}
 edtReturnMaterial->SetFocus();
 edtReturnMaterial->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnReturnDeleteClick(TObject *Sender)
{
 ReturnDelete();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnReturnDiscardClick(TObject *Sender)
{
 if (Application->MessageBox(L"Are you sure to discard the operation",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
{
 lstvReturn->Items->Clear();
// edtReturnMaterial->Text = "";
 CLdSystemData->nLendOrReturnCount = 0;
 frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: 0";
 btnReturnConfirm->Enabled = false;
 btnReturnDelete ->Enabled = false;
 btnReturnDiscard->Enabled = false;
 muiReturnConfirm->Enabled = false;
 muiReturnDelete ->Enabled = false;
 muiReturnDiscard->Enabled = false;
 edtReturnMaterial->SetFocus();
 edtReturnMaterial->SelectAll();
}
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::btnReturnConfirmClick(TObject *Sender)
{
 AnsiString l_strSQLClause;
 dmLendDM->dbMain->StartTransaction();
 for (int i=0;i<lstvReturn->Items->Count;i++)
  {
  l_strSQLClause = (AnsiString)"Update lib_LendInfo set ReturnTime = GetDate(),ReturnOpID=\'"
				  +SQLStr(CLdSystemData->strCurrentUser)
                  +(AnsiString)"\',remark=\'"
				  +SQLStr(lstvReturn->Items->Item[i]->SubItems->Strings[4])
                  +(AnsiString)"\'  where ReturnTime is NULL and ResID=\'"
                  +SQLStr(lstvReturn->Items->Item[i]->Caption)
				  +(AnsiString)"\'";
   dmLendDM->daqLendInfo->SQL->Clear();
   dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
   try
   {
   dmLendDM->daqLendInfo->ExecSQL();
   }
   catch(...)
   {
    Application->MessageBox(L"Update lib_LendInfo failed",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	dmLendDM->dbMain->Rollback();
    return;
   }
   l_strSQLClause = (AnsiString)"Update lib_Res set state=0,BorrowerID=NULL where ResID = \'"
                    +SQLStr(lstvReturn->Items->Item[i]->Caption)
                    +(AnsiString)("\'");
   dmLendDM->daqResource->SQL->Clear();
   dmLendDM->daqResource->SQL->Add(l_strSQLClause);
   try
   {
   dmLendDM->daqResource->ExecSQL();
   }
   catch(...)
   {
    Application->MessageBox(L"Update lib_Res failed",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
    dmLendDM->dbMain->Rollback();
    return;
   }
/*  if (!CLdSystemData->CreateLog("ReturnOperation",lstvReturn->Items->Item[i]->Caption,(AnsiString)"","LendModule",lstvReturn->Items->Item[i]->SubItems->Strings[2]))
    {
     dmLendDM->dbMain->Rollback();
     return;
    }*/
  }
  dmLendDM->dbMain->Commit();
  lstvReturn->Items->Clear();
  CLdSystemData->nLendOrReturnCount = 0;
  frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: 0";
//  edtReturnMaterial->Text = (AnsiString)"";
//  edtReturnRemark->Text = (AnsiString)"";
  edtReturnMaterial->SetFocus();
  btnReturnConfirm->Enabled = false;
  btnReturnDelete ->Enabled = false;
  btnReturnDiscard->Enabled = false;
  muiReturnConfirm->Enabled = false;
  muiReturnDelete ->Enabled = false;
  muiReturnDiscard->Enabled = false;
 }
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::cmbQueryModeChange(TObject *Sender)
{
 AnsiString strUserID;
 strUserID=lblUserID->Caption;
 switch (cmbQueryMode->ItemIndex)
    {
     case 0:
         NormalQuery(strUserID);
		 CLdSystemData->nQueryMode = 0;
		 break;
	 case 1:
		 BrowseOutOfTime(strUserID);
		 CLdSystemData->nQueryMode = 1;
		 break;
	 case 2:
		 BrowseNotReturn(strUserID);
		 CLdSystemData->nQueryMode = 2;
		 break;
	 case 3:
		 LostQuery(strUserID);
		 CLdSystemData->nQueryMode = 3;
		 break;
	 case 4:
		 muiBrowseMIDClick(Sender);
		 CLdSystemData->nQueryMode = 4;
		 break;
	}
}

//*********updated 2002.07
// strOrderList -- used to print
// strClause - sql query
bool TfrmLendSys::GetOrderColumn(AnsiString &SQLClause,AnsiString &strOrderList)
{

 AnsiString fieldlist[8] = {"remark","ResID","UserID","LendTime","ReturnTime","LendOpID","ReturnOpID","remark"};
 // lstvQuery->GetSubItemText(lstvQuery->LastColumnClicked);

// SQLClause += AnsiString(" order by " +  fieldlist[lstvQuery->LastColumnClicked]);
 SQLClause += AnsiString(" order by " +  fieldlist[3]);
// strOrderList = AnsiString(" sorted by " +  lstvQuery->Columns->Items[lstvQuery->LastColumnClicked]->Caption);
 strOrderList = AnsiString(" sorted by " +  lstvQuery->Columns->Items[3]->Caption);
	 SQLClause += AnsiString(" asc ");
	 strOrderList += AnsiString(" asc ");
/* if ( lstvQuery->CurrentSortAscending )
 {
	 SQLClause += AnsiString(" asc ");
	 strOrderList += AnsiString(" asc ");
 }
 else
 {
	  SQLClause += AnsiString(" desc ");
	  strOrderList += AnsiString(" desc ");
 }
*/
 return true;
}


//--------updated in 2002.7 by huchen--------------------------------------------------------
bool TfrmLendSys::GetLostQuery(AnsiString &SQLClause, AnsiString strUserID)
{

 AnsiString l_strSQLClause;
 try
 {
   dmLendDM->daqTime->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not execute SQL:Select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return false;
 }


 if (strUserID=="everyone")
   l_strSQLClause = (AnsiString)"select * from lib_LendInfo where IsLost = 1 ";
 else
   l_strSQLClause = (AnsiString)"select * from lib_LendInfo where IsLost = 1 and UserID='"
				   +SQLStr(strUserID)
				   +(AnsiString)"'";

 AnsiString l_strOrderList;
 GetOrderColumn(l_strSQLClause,l_strOrderList);

 SQLClause=l_strSQLClause;
 return true;
}

bool TfrmLendSys::GetMaterialQuery(AnsiString &SQLClause, AnsiString strUserID)
{

 AnsiString l_strSQLClause;
 try
 {
   dmLendDM->daqTime->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not execute SQL:Select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return false;
 }

 if (strUserID=="everyone")
   l_strSQLClause = (AnsiString)"select * from lib_LendInfo where ResID ='" + frmMaterial->edtMaterialID->Text.c_str() +".'";
 else
   l_strSQLClause = (AnsiString)"select * from lib_LendInfo where ResID = '"
					+ frmMaterial->edtMaterialID->Text.c_str()
					+".' and UserID = ' "
					+SQLStr(strUserID)
					+(AnsiString)"'";
 SQLClause=l_strSQLClause;
 return true;
}

//----------------------------------------------------------------
void TfrmLendSys::LostQuery(AnsiString strUserID)
{
 AnsiString l_strSQLClause,l_strMaterial,l_strBorrowID,l_strLendTime,
			l_strReturnTime,l_strLendOpID,l_strReturnOpID,l_strRemark;
 int nCount;
 bool bIsReturn;
 int istart,iend,ipage,imaxpage;


 nCount = 0;
 //if (CLdSystemData->nQueryMode == 0)
 //return;
 l_strReturnTime = (AnsiString)"";
 l_strReturnOpID = (AnsiString)"";
 l_strRemark     = (AnsiString)"";

 TDateTime dtTemp;
 try
 {
   dmLendDM->daqTime->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not execute SQL:Select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return ;
 }
// dtTemp = (TDateTime)dmLendDM->daqTime->FieldValues["Column1"];
 dtTemp = dmLendDM->daqTime->FieldByName("Column1")->AsDateTime;


 if (  cmbQueryMode->ItemIndex == 3 )
 {
	if (!GetLostQuery(l_strSQLClause,strUserID))
		return;
 }
 else if ( cmbQueryMode->ItemIndex == 4 )
 {
	if (!GetMaterialQuery(l_strSQLClause,strUserID))
		return;
 }

 dmLendDM->daqLendInfo->SQL->Clear();
 dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);

 try
 {
  dmLendDM->daqLendInfo->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
 dmLendDM->daqLendInfo->First();
// lstvQuery->BeginUpdate();
 lstvQuery->Items->Clear();

 GetLimitNum(imaxpage,ipage,istart,iend);

 while ((!dmLendDM->daqLendInfo->Eof)&&(nCount<CLdSystemData->nMaxRecord))
  {
   nCount++;
   if (!(dmLendDM->daqLendInfo->FieldValues["ReturnTime"].IsNull()))
	bIsReturn = true;
   else
	bIsReturn = false;
   l_strMaterial = OrigStr(dmLendDM->daqLendInfo->FieldValues["ResID"]);
   l_strBorrowID = OrigStr(dmLendDM->daqLendInfo->FieldValues["UserID"]);
   //l_strLendTime = ((TDateTime)dmLendDM->daqLendInfo->FieldValues["LendTime"]).FormatString("yyyy/mm/dd hh:nn");
	l_strLendTime = (dmLendDM->daqLendInfo->FieldByName("LendTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");

   if (!(dmLendDM->daqLendInfo->FieldValues["ReturnTime"].IsNull()))
//	  l_strReturnTime = ((TDateTime)dmLendDM->daqLendInfo->FieldValues["ReturnTime"]).FormatString("yyyy/mm/dd hh:nn");
	  l_strReturnTime = (dmLendDM->daqLendInfo->FieldByName("ReturnTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");
   else
	  l_strReturnTime = (AnsiString)"";
   l_strLendOpID   = OrigStr(dmLendDM->daqLendInfo->FieldValues["LendOpID"]);
   if (!(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"].IsNull()))
	  l_strReturnOpID = OrigStr(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"]);
   else
	  l_strReturnOpID = (AnsiString)"";
   if (!(dmLendDM->daqLendInfo->FieldValues["remark"].IsNull()))
	  l_strRemark = OrigStr(dmLendDM->daqLendInfo->FieldValues["remark"]);
   else
	  l_strRemark = (AnsiString)"";

   lstvQuery->Items->Add();

   if ((int)dmLendDM->daqLendInfo->FieldValues["IsLost"]==1)
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 3;
   else
//	if  (((TDateTime)dmLendDM->daqLendInfo->FieldValues["DueTime"]>dtTemp)&&(!bIsReturn))
	if  ((dmLendDM->daqLendInfo->FieldByName("DueTime")->AsDateTime >dtTemp)&&(!bIsReturn))
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 0;
	else
	  if (bIsReturn)
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 1;
	  else
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 2;

   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strMaterial);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strBorrowID);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendTime);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnTime);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendOpID);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnOpID);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strRemark);
   dmLendDM->daqLendInfo->Next();
  }
// lstvQuery->EndUpdate();

 DisplayResultHint(imaxpage,ipage,istart,iend);
 
/* if (nCount==CLdSystemData->nMaxRecord)
 {
  frmMain->stbStory->Panels->Items[2]->Text = (AnsiString)"The browsing list is too long.";
 }
 else
  frmMain->stbStory->Panels->Items[2]->Text = (AnsiString)"";
 frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: "+IntToStr(dmLendDM->daqLendInfo->RecordCount);

 */
 if (dmLendDM->daqLendInfo->Active)
    dmLendDM->daqLendInfo->Close();

}

//----------------------------------------------------------------
bool TfrmLendSys::GetNormalQuery(AnsiString &SQLClause, AnsiString strUserID)
{
 TDateTime dtTemp;
 AnsiString l_strSQLClause;
 try
 {
   dmLendDM->daqTime->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not execute SQL:Select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return false;
 }
// dtTemp = (TDateTime)dmLendDM->daqTime->FieldValues["Column1"];
dtTemp = dmLendDM->daqTime->FieldByName("Column1")->AsDateTime;

 if (strUserID=="everyone")
   l_strSQLClause = (AnsiString)"select * from lib_LendInfo where cast(LendTime As Date)>=DATEADD(day,"
				   +String(-CLdSystemData->nNormalQueryDays)
				   +(AnsiString)",GetDate()) or cast(ReturnTime As Date)>=DATEADD(day,"
				   +String(-CLdSystemData->nNormalQueryDays)
				   +(AnsiString)",GetDate())";
 else
   l_strSQLClause = (AnsiString)"select * from lib_LendInfo where (cast(LendTime As Date)>=DATEADD(day,"
				   +String(-CLdSystemData->nNormalQueryDays)
				   +(AnsiString)",GetDate()) or cast(ReturnTime As Date)>=DATEADD(day,"
				   +String(-CLdSystemData->nNormalQueryDays)
				   +(AnsiString)",GetDate())"
				   +(AnsiString)") and UserID='"
				   +SQLStr(strUserID)
				   +(AnsiString)"'";

 AnsiString l_strOrderList;
 GetOrderColumn(l_strSQLClause,l_strOrderList);

 SQLClause=l_strSQLClause;


 return true;
}

//----------------------------------------------------------------
void TfrmLendSys::NormalQuery(AnsiString strUserID)
{
 AnsiString l_strSQLClause,l_strMaterial,l_strBorrowID,l_strLendTime,
			l_strReturnTime,l_strLendOpID,l_strReturnOpID,l_strRemark;
 int nCount;
 bool bIsReturn;


// if (CLdSystemData->nQueryMode == 0)
  //   return;

 l_strReturnTime = (AnsiString)"";
 l_strReturnOpID = (AnsiString)"";
 l_strRemark     = (AnsiString)"";

 TDateTime dtTemp;
 try
 {
   dmLendDM->daqTime->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not execute SQL:Select GetDate()",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return ;
 }
 //dtTemp = (TDateTime)dmLendDM->daqTime->FieldValues["Column1"];
 dtTemp = dmLendDM->daqTime->FieldByName("Column1")->AsDateTime;
 if (!GetNormalQuery(l_strSQLClause,strUserID))
	return;

 dmLendDM->daqLendInfo->SQL->Clear();
 dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
 try
 {
  dmLendDM->daqLendInfo->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }

 dmLendDM->daqLendInfo->First();

 //lstvQuery->BeginUpdate();
 lstvQuery->Items->Clear();

  int imaxpage,ipage,istart,iend;
  GetLimitNum(imaxpage,ipage,istart,iend);

  nCount = 0;
  while ((!dmLendDM->daqLendInfo->Eof) )
  {

	if  ( nCount < istart )
	{
	   nCount++;
	   dmLendDM->daqLendInfo->Next();
	   continue;
	}
	if ( nCount >= iend )
		break;
	nCount ++;
	// continue;


   if (!(dmLendDM->daqLendInfo->FieldValues["ReturnTime"].IsNull()))
	bIsReturn = true;
   else
	bIsReturn = false;

   l_strMaterial = OrigStr(dmLendDM->daqLendInfo->FieldValues["ResID"]);
   l_strBorrowID = OrigStr(dmLendDM->daqLendInfo->FieldValues["UserID"]);
//   l_strLendTime = ((TDateTime)dmLendDM->daqLendInfo->FieldValues["LendTime"]).FormatString("yyyy/mm/dd hh:nn");
	l_strLendTime = (dmLendDM->daqLendInfo->FieldByName("LendTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");

   if (!(dmLendDM->daqLendInfo->FieldValues["ReturnTime"].IsNull()))
	//  l_strReturnTime = ((TDateTime)dmLendDM->daqLendInfo->FieldValues["ReturnTime"]).FormatString("yyyy/mm/dd hh:nn");
		l_strReturnTime = (dmLendDM->daqLendInfo->FieldByName("ReturnTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");
   else
	  l_strReturnTime = (AnsiString)"";
   l_strLendOpID   = OrigStr(dmLendDM->daqLendInfo->FieldValues["LendOpID"]);
   if (!(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"].IsNull()))
	  l_strReturnOpID = OrigStr(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"]);
   else
	  l_strReturnOpID = (AnsiString)"";
   if (!(dmLendDM->daqLendInfo->FieldValues["remark"].IsNull()))
	  l_strRemark = OrigStr(dmLendDM->daqLendInfo->FieldValues["remark"]);
   else
	  l_strRemark = (AnsiString)"";

   lstvQuery->Items->Add();

   if ((int)dmLendDM->daqLendInfo->FieldValues["IsLost"]==1)
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 3;
   else
//	if  (((TDateTime)dmLendDM->daqLendInfo->FieldValues["DueTime"]>dtTemp)&&(!bIsReturn))
	if  ((dmLendDM->daqLendInfo->FieldByName("DueTime")->AsDateTime>dtTemp)&&(!bIsReturn))
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 0;
	else
	  if (bIsReturn)
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 1;
	  else
	   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 2;

   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strMaterial);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strBorrowID);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendTime);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnTime);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendOpID);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnOpID);
   lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strRemark);
   dmLendDM->daqLendInfo->Next();
 }
 //lstvQuery->EndUpdate();

 DisplayResultHint(imaxpage,ipage,istart,iend);

 if (dmLendDM->daqLendInfo->Active)
	dmLendDM->daqLendInfo->Close();
}

//---------------------------------------------------------------------------
bool TfrmLendSys::GetOutofTime(AnsiString &SQLClause, AnsiString strUserID)
{
 AnsiString l_strSQLClause;
 if (strUserID=="everyone")
	 l_strSQLClause = (AnsiString)"select * from lib_LendInfo where ReturnTime is NULL and GetDate()>DueTime";
 else
	 l_strSQLClause = (AnsiString)"select * from lib_LendInfo where ReturnTime is NULL and GetDate()>DueTime and UserID='"
					  +(AnsiString)SQLStr(strUserID)
					  +(AnsiString)"'";
 SQLClause=l_strSQLClause;

 return true;
}
//---------------------------------------------------------------------------
void TfrmLendSys::BrowseOutOfTime(AnsiString strUserID)
{
 AnsiString l_strSQLClause,l_strMaterial,l_strBorrowID,l_strLendTime,
			l_strReturnTime,l_strLendOpID,l_strReturnOpID,l_strRemark;
 int nCount;

 //if (CLdSystemData->nQueryMode==1)
 //return;
 l_strReturnTime = (AnsiString)"";
 l_strReturnOpID = (AnsiString)"";
 l_strRemark     = (AnsiString)"";

 GetOutofTime(l_strSQLClause,strUserID);

 AnsiString l_strOrderList;
 GetOrderColumn(l_strSQLClause,l_strOrderList);

 dmLendDM->daqLendInfo->SQL->Clear();
 dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
 try
 {
  dmLendDM->daqLendInfo->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
 dmLendDM->daqLendInfo->First();

// lstvQuery->BeginUpdate();
 lstvQuery->Items->Clear();

  int imaxpage,ipage,istart,iend;
  GetLimitNum(imaxpage,ipage,istart,iend);


  nCount = 0;
 while ((!dmLendDM->daqLendInfo->Eof)&&(nCount<CLdSystemData->nMaxRecord))
 {

	if ( nCount < istart )
	{
		dmLendDM->daqLendInfo->Next();
		nCount++;
		continue;
	}
	if ( nCount >= iend )
		break;

	nCount++;

	l_strMaterial = OrigStr(dmLendDM->daqLendInfo->FieldValues["ResID"]);
	l_strBorrowID = OrigStr(dmLendDM->daqLendInfo->FieldValues["UserID"]);
//	l_strLendTime = ((TDateTime)dmLendDM->daqLendInfo->FieldValues["LendTime"]).FormatString("yyyy/mm/dd hh:nn");
	l_strLendTime = (dmLendDM->daqLendInfo->FieldByName("LendTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");
	l_strLendOpID   = OrigStr(dmLendDM->daqLendInfo->FieldValues["LendOpID"]);

	if (!(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"].IsNull()))
	   l_strReturnOpID = OrigStr(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"]);
	else
	   l_strReturnOpID = (AnsiString)"";

	if (!(dmLendDM->daqLendInfo->FieldValues["remark"].IsNull()))
	   l_strRemark = OrigStr(dmLendDM->daqLendInfo->FieldValues["remark"]);
	else
	   l_strRemark = (AnsiString)"";

	lstvQuery->Items->Add();
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 2;
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strMaterial);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strBorrowID);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendTime);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnTime);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendOpID);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnOpID);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strRemark);
	dmLendDM->daqLendInfo->Next();
 }


// lstvQuery->EndUpdate();

 DisplayResultHint(imaxpage,ipage,istart,iend);

 if (dmLendDM->daqLendInfo->Active)
   dmLendDM->daqLendInfo->Close();
}

bool TfrmLendSys::GetNotReturn(AnsiString &SQLClause, AnsiString strUserID)
{
 AnsiString l_strSQLClause;
 if (strUserID=="everyone")
     l_strSQLClause = (AnsiString)"select * from lib_LendInfo where ReturnTime is NULL";
 else
     l_strSQLClause = (AnsiString)"select * from lib_LendInfo where ReturnTime is NULL and UserID='"
                      +(AnsiString)SQLStr(strUserID)
					  +(AnsiString)"'";

 AnsiString l_strOrderList;
 GetOrderColumn(l_strSQLClause,l_strOrderList);

 SQLClause=l_strSQLClause;
 return true;
}

void TfrmLendSys::BrowseNotReturn(AnsiString strUserID)
{
 AnsiString l_strSQLClause,l_strMaterial,l_strBorrowID,l_strLendTime,
			l_strReturnTime,l_strLendOpID,l_strReturnOpID,l_strRemark;
 int nCount;
 nCount = 0;
// if (CLdSystemData->nQueryMode==2)
// return;

 l_strReturnTime = (AnsiString)"";
 l_strReturnOpID = (AnsiString)"";
 l_strRemark     = (AnsiString)"";

 GetNotReturn(l_strSQLClause,strUserID);

 dmLendDM->daqLendInfo->SQL->Clear();
 dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
 try
 {
  dmLendDM->daqLendInfo->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
 dmLendDM->daqLendInfo->First();
// lstvQuery->BeginUpdate();
 lstvQuery->Items->Clear();

  int imaxpage,ipage,istart,iend;
  GetLimitNum(imaxpage,ipage,istart,iend);


 while ((!dmLendDM->daqLendInfo->Eof) )
 {


	if ( nCount < istart )
	{
		dmLendDM->daqLendInfo->Next();
		nCount++;
		continue;
	}
	if ( nCount >= iend )
		break;

	nCount++;


	l_strMaterial = OrigStr(dmLendDM->daqLendInfo->FieldValues["ResID"]);
	l_strBorrowID = OrigStr(dmLendDM->daqLendInfo->FieldValues["UserID"]);
   //	l_strLendTime = ((TDateTime)dmLendDM->daqLendInfo->FieldValues["LendTime"]).FormatString("yyyy/mm/dd hh:nn");
	l_strLendTime = (dmLendDM->daqLendInfo->FieldByName("LendTime")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");

	l_strLendOpID   = OrigStr(dmLendDM->daqLendInfo->FieldValues["LendOpID"]);
	if (!(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"].IsNull()))
	   l_strReturnOpID = OrigStr(dmLendDM->daqLendInfo->FieldValues["ReturnOpID"]);
	else
	   l_strReturnOpID = (AnsiString)"";
	if (!(dmLendDM->daqLendInfo->FieldValues["remark"].IsNull()))
	   l_strRemark = OrigStr(dmLendDM->daqLendInfo->FieldValues["remark"]);
	else
	   l_strRemark = (AnsiString)"";

	lstvQuery->Items->Add();
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->ImageIndex = 0;
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strMaterial);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strBorrowID);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendTime);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnTime);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strLendOpID);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strReturnOpID);
	lstvQuery->Items->Item[lstvQuery->Items->Count-1]->SubItems->Add(l_strRemark);
	dmLendDM->daqLendInfo->Next();
 }
// lstvQuery->EndUpdate();

 DisplayResultHint(imaxpage,ipage,istart,iend);

 if (dmLendDM->daqLendInfo->Active)
	dmLendDM->daqLendInfo->Close();
}
void __fastcall TfrmLendSys::muiPreviewClick(TObject *Sender)
{
	print_Report(true);

/*
 AnsiString l_strSQLClause;
 AnsiString l_strOrderList;

 TDateTime time;
 if (CLdSystemData->nQueryMode == 0)
 {
   if (!GetNormalQuery(l_strSQLClause,lblUserID->Caption))
	 return;
 }
 else if (CLdSystemData->nQueryMode == 1)
	GetOutofTime(l_strSQLClause,lblUserID->Caption);
 else if (CLdSystemData->nQueryMode == 2)
	GetNotReturn(l_strSQLClause,lblUserID->Caption);
 else if (CLdSystemData->nQueryMode == 3)
	GetLostQuery(l_strSQLClause,lblUserID->Caption);
 else if (CLdSystemData->nQueryMode == 4)
	GetMaterialQuery(l_strSQLClause,lblUserID->Caption);

  // 2002.07
 GetOrderColumn(l_strSQLClause,l_strOrderList);

 dmLendDM->daqLendInfo->SQL->Clear();
 dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);

 try
 {
  dmLendDM->daqLendInfo->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }
*/
// dmLendDM->daqLendInfo->First();
/* frmPreview->qrOrderBy->Caption  = l_strOrderList;
 frmPreview->QRDBTextMaterialID->DataField = (AnsiString)"ResID";
 frmPreview->QRDBTextBorrowID->DataField =   (AnsiString)"UserID";
 frmPreview->QRDBTextLendTime->DataField = (AnsiString)"LendTime";
 frmPreview->QRDBTextReturnTime->DataField = (AnsiString)"ReturnTime";
 frmPreview->QRDBTextLendOpID->DataField = (AnsiString)"LendOpID";
 frmPreview->QRDBTextReturnOpID->DataField = (AnsiString)"ReturnOpID";
 frmPreview->QRDBTextRemark->DataField = (AnsiString)"remark";
 frmPreview->qrlblDate->Caption=(AnsiString)"Date "+time.CurrentDate().FormatString("yyyy-mm-dd");
 frmPreview->qrlblPrinter->Caption=(AnsiString)"Printed by "+CLdSystemData->strCurrentUser;

 frmPreview->qrpLendInfo->Preview();
*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::FormShow(TObject *Sender)
{
  cmbQueryMode->ItemIndex = 0;
  CLdSystemData->nQueryMode = 0;
  CLdSystemData->strCurrentUser = dmdStory->strCurrentUserID;
  cmbMode->ItemIndex = 0;
  m_CaptionHead  = Caption;
  m_CaptionDescription = (AnsiString)" - Return";
//  Caption = m_CaptionHead + m_CaptionDescription;
  frmMain->stbStory->Panels->Items[0]->Text = (AnsiString)"Current User: "+CLdSystemData->strCurrentUser;
  frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: 0";
  AnsiString l_strINIFileName;
  l_strINIFileName = Application->ExeName;
  l_strINIFileName = l_strINIFileName.SubString(1,l_strINIFileName.Length()-4)+
					 (AnsiString)".ini";

   CLdSystemData->nNormalQueryDays = GetPrivateProfileInt("DAYOPTION","QUERYDAYS",5,l_strINIFileName.c_str());
   CLdSystemData->nQueryMode = -1;

 // if (frmLendSys->ntbLend->PageIndex==2)
 // {
	muiRefreshClick(Sender);
 // }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiOptionClick(TObject *Sender)
{
 if ((frmOption->ShowModal()==mrOk))
 {
  UpdateDays();
  if (frmLendSys->ntbLend->PageIndex==2)
  {
	muiRefreshClick(Sender);
  }
 }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::nuiFindClick(TObject *Sender)
{
 frmFind->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiNormalClick(TObject *Sender)
{
 cmbQueryMode->ItemIndex = 0;
 NormalQuery(lblUserID->Caption);
 CLdSystemData->nQueryMode = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiBrowseOutOfTimeClick(TObject *Sender)
{
 cmbQueryMode->ItemIndex = 1;
 BrowseOutOfTime(lblUserID->Caption);
 CLdSystemData->nQueryMode = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiPrintClick(TObject *Sender)
{

/* frmPreview->qrOrderBy->Caption  = l_strOrderList;
 frmPreview->QRDBTextMaterialID->DataField = (AnsiString)"ResID";
 frmPreview->QRDBTextBorrowID->DataField =   (AnsiString)"UserID";
 frmPreview->QRDBTextLendTime->DataField = (AnsiString)"LendTime";
 frmPreview->QRDBTextReturnTime->DataField = (AnsiString)"ReturnTime";
 frmPreview->QRDBTextLendOpID->DataField = (AnsiString)"LendOpID";
 frmPreview->QRDBTextReturnOpID->DataField = (AnsiString)"ReturnOpID";
 frmPreview->QRDBTextRemark->DataField = (AnsiString)"remark";
 frmPreview->qrpLendInfo->Print();
*/
print_Report(false);
}
//---------------------------------------------------------------------------

void TfrmLendSys::print_Report(bool b_Preview)
{
  AnsiString l_strSQLClause;
	AnsiString l_strOrderList;

  TDateTime time;

 if (CLdSystemData->nQueryMode == 0)
   {
   if (!GetNormalQuery(l_strSQLClause,lblUserID->Caption))
	 return;
   }
 else if (CLdSystemData->nQueryMode == 1)
	GetOutofTime(l_strSQLClause,lblUserID->Caption);
 else if (CLdSystemData->nQueryMode == 2)
	GetNotReturn(l_strSQLClause,lblUserID->Caption);
 else if (CLdSystemData->nQueryMode == 3)
	GetLostQuery(l_strSQLClause,lblUserID->Caption);
 else if (CLdSystemData->nQueryMode == 4)
	GetMaterialQuery(l_strSQLClause,lblUserID->Caption);


  // 2002.07
// GetOrderColumn(l_strSQLClause,l_strOrderList);

 dmLendDM->daqLendInfo->SQL->Clear();
 dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);

 try
 {
  dmLendDM->daqLendInfo->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not open table lib_LendInfo",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return;
 }

 dmLendDM->daqLendInfo->First();

//  frxReport1->LoadFromFile("Untitled2.fr3");
frxReport1->Clear();
TfrxDataPage * DataPage;
TfrxReportPage * Page;
TfrxBand * Band;
TfrxMasterData * DataBand;
TfrxMemoView * Memo;
TfrxPageHeader * RHBand;
TfrxLineView * RHLine;
frxDBDataset1->DataSet = dmLendDM->daqLendInfo;
frxReport1->DataSets->Add(frxDBDataset1);
// add "Data" page
DataPage = new TfrxDataPage(frxReport1);
// add page
Page = new TfrxReportPage(frxReport1);
// create unique name
Page->CreateUniqueName();
// set sizes of fields, paper and orientation to defaults
Page->SetDefaults();
// change paper orientation
Page->Orientation = poLandscape;
// add report title band

TFont* pFont;
TFontStyles style;
pFont=new TFont;
style<<fsBold;
pFont->Name=(AnsiString)"Arial";
pFont->Color=clNavy;
pFont->Style=style;
pFont->Size = 15;

Band = new TfrxReportTitle(Page);
Band->CreateUniqueName();
// only ：Top； coordinate and height of band need setting
//    both in pixels
Band->Top = 0;
Band->Height = 50;
// add object to report title band
Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = "Borrowing Information List";
Memo->Font = pFont;
Memo->Height = 40;
// this object will be stretched to band・s width
Memo->Align = baWidth;

Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = (AnsiString)"Date "+time.CurrentDate().FormatString("yyyy-mm-dd");
Memo->SetBounds(600, 0, 200, 20);
//  frmPreview->qrlblPrinter->Caption=(AnsiString)"Printed By "+dfrm->strUserID;
Memo->HAlign = haRight;

Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = (AnsiString)(AnsiString)"Printed By "+dmdStory->strCurrentUserID;
Memo->SetBounds(820, 0, 200, 20);
Memo->HAlign = haRight;


RHBand = new TfrxPageHeader(Page) ;
RHBand->CreateUniqueName();
RHBand->Top = 10 ;
RHBand->Height = 20;

RHLine = new TfrxLineView(RHBand);
RHLine->Width = Page->Width - 100;
RHLine->Left = 3;
RHLine->Top = 20;
RHLine->Visible = true;


// add masterdata band
DataBand = new TfrxMasterData(Page);
DataBand->CreateUniqueName();
DataBand->DataSet = frxDBDataset1;
// ：Top； should be greater than previously added band・s top + height
DataBand->Top = 100;
DataBand->Height = 20;
DataBand->Stretched = true;

//  TFont* pFont;
//  TFontStyles style;
  int nPosition;

  pFont=new TFont;
  style<<fsUnderline;
  style<<fsBold;
  pFont->Name=(AnsiString)"Arial";
  pFont->Color=clNavy;
  pFont->Style=style;
  pFont->Size = 10;
  nPosition=0;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Material ID";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "ResID";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

  nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Borrower";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "UserID";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

  nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Lend Time";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "LendTime";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Return Time";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "ReturnTime";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Lend Operator";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "LendOpID";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Return Operator";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "ReturnOpID";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "remark";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "Remark";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

  if (b_Preview)
	  frxReport1->ShowReport(true);
  else
	  frxReport1->Print();
  dmLendDM->daqLendInfo->Close();

if (pFont!=NULL)
	delete pFont;
}

void __fastcall TfrmLendSys::WriteINIFile()
{
 AnsiString l_strINIFileName;
 l_strINIFileName = Application->ExeName;
 l_strINIFileName = l_strINIFileName.SubString(1,l_strINIFileName.Length()-4)+
					 (AnsiString)".ini";
// WritePrivateProfileString("DAYOPTION","LENDDAYS",String(CLdSystemData->nLendDays).c_str(),l_strINIFileName.c_str());
 WritePrivateProfileString("DAYOPTION","QUERYDAYS",AnsiString(CLdSystemData->nNormalQueryDays).c_str(),AnsiString(l_strINIFileName).c_str());
 WritePrivateProfileString("USEROPTION","USER",AnsiString(CLdSystemData->strCurrentUser).c_str(),AnsiString(l_strINIFileName).c_str());
// WritePrivateProfileString("FORM","LEFT",((AnsiString)frmLendSys->Left).c_str(),l_strINIFileName.c_str());
// WritePrivateProfileString("FORM","TOP",((AnsiString)frmLendSys->Top).c_str(),l_strINIFileName.c_str());
// WritePrivateProfileString("FORM","HEIGHT",((AnsiString)frmLendSys->Height).c_str(),l_strINIFileName.c_str());
// WritePrivateProfileString("FORM","WIDTH",((AnsiString)frmLendSys->Width).c_str(),l_strINIFileName.c_str());
// WritePrivateProfileString("FORM","STATE",((AnsiString)frmLendSys->WindowState).c_str(),l_strINIFileName.c_str());
// WritePrivateProfileString("FONT","NAME", frmLendSys->fdBox->Font->Name.c_str(),l_strINIFileName.c_str());
// WritePrivateProfileString("FONT","SIZE",((AnsiString)(frmLendSys->fdBox->Font->Size)).c_str(),l_strINIFileName.c_str());
}

//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiLendFindClick(TObject *Sender)
{
 if (lstvLend->Selected!=NULL)
  frmFind->edtMaterialID->Text = lstvLend->Selected->Caption;
  frmFind->ShowModal();
 }
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiBrowseFindClick(TObject *Sender)
{
 if (lstvQuery->Selected!=NULL)
  frmFind->edtMaterialID->Text = lstvQuery->Selected->SubItems->Strings[0];
  frmFind->ActiveControl=frmFind->btnFind;
  frmFind->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiReturnFindClick(TObject *Sender)
{
 if (lstvReturn->Selected!=NULL)
   frmFind->edtMaterialID->Text = lstvReturn->Selected->Caption;
   frmFind->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::lstvLendDblClick(TObject *Sender)
{
 if (lstvLend->Selected!=NULL)
  {
   frmFind->edtMaterialID->Text = lstvLend->Selected->Caption;
   frmFind->ShowModal();
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnReturnClearClick(TObject *Sender)
{
 edtReturnMaterial->Text = "";
 edtReturnRemark->Text = "";
 edtReturnMaterial->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::btnLendClearClick(TObject *Sender)
{
 edtLendMaterial->Text = "";
 edtLendRemark->Text = "";
 edtLendUser->Text="";
 edtLendMaterial->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::lstvLendMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if (Button == mbRight)
  {
   if (lstvLend->Selected == NULL)
     muiLendDelete->Enabled = false;
   else
     muiLendDelete->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::lstvReturnMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if (Button == mbRight)
  {
   if (lstvReturn->Selected == NULL)
    muiReturnDelete->Enabled = false;
   else
    muiReturnDelete->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::lstvQueryDblClick(TObject *Sender)
{
  if (lstvQuery->Selected!=NULL)
  {
   frmFind->edtMaterialID->Text = lstvQuery->Selected->SubItems->Strings[0];
   frmFind->ShowModal();
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::lstvReturnDblClick(TObject *Sender)
{
  if (lstvReturn->Selected!=NULL)
  {
   frmFind->edtMaterialID->Text = lstvReturn->Selected->Caption;
   frmFind->ShowModal();
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::muiRefreshClick(TObject *Sender)
{
  if (frmLendSys->cmbQueryMode->ItemIndex==0)
   {
	CLdSystemData->nQueryMode = -1;
	frmLendSys->NormalQuery(lblUserID->Caption);
	CLdSystemData->nQueryMode = 0;
   }
  if (frmLendSys->cmbQueryMode->ItemIndex==1)
   {
	CLdSystemData->nQueryMode = -1;
	frmLendSys->BrowseOutOfTime(lblUserID->Caption);
	CLdSystemData->nQueryMode = 1;
   }
  if (frmLendSys->cmbQueryMode->ItemIndex==2)
   {
	CLdSystemData->nQueryMode = -1;
	frmLendSys->BrowseNotReturn(lblUserID->Caption);
	CLdSystemData->nQueryMode = 2;
   }
  if (frmLendSys->cmbQueryMode->ItemIndex==3)
   {
	CLdSystemData->nQueryMode = -1;
	frmLendSys->LostQuery(lblUserID->Caption);
	CLdSystemData->nQueryMode = 3;
   }
  if (frmLendSys->cmbQueryMode->ItemIndex==4)
   {
	CLdSystemData->nQueryMode = -1;
	frmLendSys->LostQuery(lblUserID->Caption);
	CLdSystemData->nQueryMode = 4;
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::lstvLendKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
 TShiftState State;
 State << ssCtrl;
 if ((Key == VK_DELETE)&&(Shift != State)&&(muiLendDelete->Enabled))
 LendDelete();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::LendDelete()
{
  if (lstvLend->Selected==NULL)
  {Application->MessageBox(L"No item selected",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   return;
  }
 else
  {
   while (lstvLend->Selected!=NULL)
   {
    lstvLend->Items->Delete(lstvLend->Items->IndexOf(lstvLend->Selected));
    CLdSystemData->nLendOrReturnCount--;
    frmMain->stbStory->Panels->Items[1]->Text = (AnsiString)"Material Number: "+IntToStr(CLdSystemData->nLendOrReturnCount);
   }
   if (lstvLend->Items->Count == 0)
    {
     btnLendConfirm->Enabled = false;
     btnLendDelete ->Enabled = false;
     btnLendDiscard->Enabled = false;
     muiLendConfirm->Enabled = false;
     muiLendDelete->Enabled = false;
     muiLendDiscard->Enabled = false;
    }
  }
}
//----------------------------------------------------------------
void __fastcall TfrmLendSys::ReturnDelete()
{
 if (lstvReturn->Selected==NULL)
  {Application->MessageBox(L"No item selected",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
   return;
  }
 else
  {
   while (lstvReturn->Selected!=NULL)
   {
    lstvReturn->Items->Delete(lstvReturn->Items->IndexOf(lstvReturn->Selected));
    CLdSystemData->nLendOrReturnCount--;
    frmMain->stbStory->Panels->Items[1]->Text =(AnsiString)"Material Number: "+IntToStr(CLdSystemData->nLendOrReturnCount);
   }

   if (lstvReturn->Items->Count == 0)
    {
     btnReturnConfirm->Enabled = false;
     btnReturnDelete ->Enabled = false;
     btnReturnDiscard->Enabled = false;
     muiReturnConfirm->Enabled = false;
     muiReturnDelete ->Enabled = false;
     muiReturnDiscard->Enabled = false;
    }
  }
}
void __fastcall TfrmLendSys::lstvReturnKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
 TShiftState State;
 State << ssCtrl;
 if ((Key == VK_DELETE)&&(Shift != State)&&(muiReturnDelete->Enabled))
 ReturnDelete();

}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::UpdateDays()
{
 AnsiString l_strSQLClause;
 dmLendDM->dbMain->StartTransaction();
 l_strSQLClause = (AnsiString)"update lib_ResTypes set BorrowDays="
				   +IntToStr(CLdSystemData->nTapeDays)
				   +(AnsiString)" where TypeID=1";
 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add(l_strSQLClause);
 try
 {
 dmLendDM->daqLendDays->ExecSQL();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not update table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  dmLendDM->dbMain->Rollback();
  return;
 }

 l_strSQLClause = (AnsiString)"update lib_ResTypes set BorrowDays="
                   +IntToStr(CLdSystemData->nBookDays)
                   +(AnsiString)" where TypeID=2";
 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add(l_strSQLClause);
 try
 {
 dmLendDM->daqLendDays->ExecSQL();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not update table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  dmLendDM->dbMain->Rollback();
  return;
 }
 l_strSQLClause = (AnsiString)"update lib_ResTypes set BorrowDays="
                   +IntToStr(CLdSystemData->nMagazineDays)
                   +(AnsiString)" where TypeID=3";
 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add(l_strSQLClause);
 try
 {
 dmLendDM->daqLendDays->ExecSQL();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not update table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  dmLendDM->dbMain->Rollback();
  return;
 }
 l_strSQLClause = (AnsiString)"update lib_ResTypes set BorrowDays="
                   +IntToStr(CLdSystemData->nFileDays)
                   +(AnsiString)" where TypeID=4";
 dmLendDM->daqLendDays->SQL->Clear();
 dmLendDM->daqLendDays->SQL->Add(l_strSQLClause);
 try
 {
 dmLendDM->daqLendDays->ExecSQL();
 }
 catch(...)
 {
  Application->MessageBox(L"Can not update table lib_ResTypes",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  dmLendDM->dbMain->Rollback();
  return;
 }
 dmLendDM->dbMain->Commit();
}

void __fastcall TfrmLendSys::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if ((Key == VK_RETURN))
   FindNextControl(ActiveControl,true,true,true)->SetFocus();
 }
//---------------------------------------------------------------------------


void __fastcall TfrmLendSys::edtLendMaterialKeyPress(TObject *Sender,
      char &Key)
{
 if (Key==VK_RETURN)
  {
   Key = NULL;
   btnLendAddClick(Sender);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLendSys::edtLendUserKeyPress(TObject *Sender,
      char &Key)
{
  if (Key==VK_RETURN)
  {
   Key = NULL;
   edtLendMaterial->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::edtReturnMaterialKeyPress(TObject *Sender,
      char &Key)
{
  if (Key==VK_RETURN)
  {
   Key = NULL;
   btnReturnAddClick(Sender);
  }

}
//---------------------------------------------------------------------------


void __fastcall TfrmLendSys::muiUnLostClick(TObject *Sender)
{
 AnsiString l_strSQLClause;
 AnsiString strLendTime;
 if (lstvQuery->Selected!=NULL)
  {
   if ( lstvQuery->Selected->ImageIndex != 3 )
	   {Application->MessageBox(L"Can not do this operation",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
		return;
	   }
   dmLendDM->dbMain->StartTransaction();
   l_strSQLClause = (AnsiString)"Update lib_Res set state=0,Position='.' where ResID='"
					+SQLStr(lstvQuery->Selected->SubItems->Strings[0])
					+(AnsiString)"'";
   dmLendDM->daqResource->SQL->Clear();
   dmLendDM->daqResource->SQL->Add(l_strSQLClause);
   try
	{
	 dmLendDM->daqResource->ExecSQL();
	}

   catch(...)
	{
	 Application->MessageBox(L"Update lib_Res failure",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	 dmLendDM->dbMain->Rollback();
	 return;
   }

  strLendTime=lstvQuery->Selected->SubItems->Strings[2]+(AnsiString)":00";
  l_strSQLClause = (AnsiString)"Update lib_LendInfo set IsLost=0,remark='.' "
					+(AnsiString)" where ResID='"
					+SQLStr(lstvQuery->Selected->SubItems->Strings[0])
					+(AnsiString)"'";

/*  l_strSQLClause = (AnsiString)"Update lib_LendInfo set IsLost=0,remark='.',ReturnTime=GetDate(),ReturnOpID='"
					+SQLStr(CLdSystemData->strCurrentUser)
					+(AnsiString)"' where ResID='"
					+SQLStr(lstvQuery->Selected->SubItems->Strings[0])
					+(AnsiString)"' and LendTime>='"
					+strLendTime
                    +(AnsiString)"'";
                    */
   dmLendDM->daqLendInfo->SQL->Clear();
   dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
   try
    {
     dmLendDM->daqLendInfo->ExecSQL();
    }

   catch(...)
	{
	 Application->MessageBox(L"Update lib_LendInfo failure",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	 dmLendDM->dbMain->Rollback();
	 return;
   }
   dmLendDM->dbMain->Commit();
   try
   {
	dmLendDM->daqTime->Open();
   }
   catch(...)
   {
	Application->MessageBox(L"Can not get current time",UnicodeString(CLdSystemData->strCurrentUser).c_str(),MB_OK|MB_ICONINFORMATION);
	return;
   }
   lstvQuery->Selected->ImageIndex = 3;
  // lstvQuery->Selected->SubItems->Strings[3] = (AnsiString)(((TDateTime)(dmLendDM->daqTime->FieldValues["Column1"])).FormatString("yyyy/mm/dd hh:nn"));

   lstvQuery->Selected->SubItems->Strings[3] = (dmLendDM->daqTime->FieldByName("Column1")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");
   lstvQuery->Selected->SubItems->Strings[4] =  CLdSystemData->strCurrentUser;
   lstvQuery->Selected->SubItems->Strings[6] = "Lost";
  }
}
//---------------------------------------------------------------------------



void __fastcall TfrmLendSys::BrowsePage(TObject * Sender)
{
 m_CaptionDescription = " - Browse";
// Caption = m_CaptionHead+m_CaptionDescription;

 // muiRefreshClick(Sender);
  if (cmbQueryMode->ItemIndex==0)
   {
	CLdSystemData->nQueryMode = -1;
	NormalQuery(lblUserID->Caption);
	CLdSystemData->nQueryMode = 0;
   }
 if (cmbQueryMode->ItemIndex==1)
   {
	CLdSystemData->nQueryMode = -1;
	BrowseOutOfTime(lblUserID->Caption);
	CLdSystemData->nQueryMode = 1;
   }
 if (cmbQueryMode->ItemIndex==2)
   {
	CLdSystemData->nQueryMode = -1;
	BrowseNotReturn(lblUserID->Caption);
	CLdSystemData->nQueryMode = 2;
   }


  muiRefresh->Visible = true;;
  ntbLend->ActivePage = "Browse";
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::muiBrowseNotReturnClick(TObject *Sender)
{
 cmbQueryMode->ItemIndex = 2;
 BrowseNotReturn(lblUserID->Caption);
 CLdSystemData->nQueryMode = 2;

}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::tbbtnUserClick(TObject *Sender)
{
  frmBorrower->edtUser->Enabled=true;
  frmBorrower->cboMode->Enabled=true;
  frmBorrower->chkEveryone->Checked=false;

  if (frmBorrower->ShowModal()==mrOk)
  {
	muiRefreshClick(Sender);
  }

}
//---------------------------------------------------------------------------


void __fastcall TfrmLendSys::pmQueryPopup(TObject *Sender)
{
	bool b1=false;

	muiUnLost->Visible= false;
	if (lstvQuery->Selected!=NULL)
	{
		if ( (lstvQuery->Selected->ImageIndex != 1) &&
			 (lstvQuery->Selected->ImageIndex != 3) )
		   b1=true;

		   // losted
		if ( lstvQuery->Selected->ImageIndex == 3 )
		{
			muiUnLost->Visible= true;
		}
		else
			muiUnLost->Visible= false;
	}

	N1->Visible=b1;
	muiLost->Visible=b1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::BrowsebyLosted1Click(TObject *Sender)
{
 cmbQueryMode->ItemIndex = 3;
 LostQuery(lblUserID->Caption);
 CLdSystemData->nQueryMode = 3;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::muiBrowseMIDClick(TObject *Sender)
{
 cmbQueryMode->ItemIndex = 4;
 //
 if (lstvQuery->Selected!=NULL)
	  frmMaterial->edtMaterialID->Text = lstvQuery->Selected->SubItems->Strings[0];
 frmMaterial->ActiveControl=frmMaterial->btnFind;
 frmMaterial->ShowModal();
 //
 CLdSystemData->nQueryMode = 4;
 LostQuery(lblUserID->Caption);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::itemPrintPreviewClick(TObject *Sender)
{
	muiPreviewClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::ItemPrintClick(TObject *Sender)
{
	muiPrintClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::muiLostClick(TObject *Sender)
{
 AnsiString l_strSQLClause;
 AnsiString strLendTime;
 if (lstvQuery->Selected!=NULL)
  {
   if ((lstvQuery->Selected->ImageIndex == 1) || (lstvQuery->Selected->ImageIndex == 3))
	   {Application->MessageBox(L"Can not do this operation",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
		return;
	   }
   dmLendDM->dbMain->StartTransaction();
   l_strSQLClause = (AnsiString)"Update lib_Res set state=-1,Position='Lost.' where ResID='"
					+SQLStr(lstvQuery->Selected->SubItems->Strings[0])
					+(AnsiString)"'";
   dmLendDM->daqResource->SQL->Clear();
   dmLendDM->daqResource->SQL->Add(l_strSQLClause);
   try
	{
	 dmLendDM->daqResource->ExecSQL();
	}

   catch(...)
	{
	 Application->MessageBox(L"Update lib_Res failure",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	 dmLendDM->dbMain->Rollback();
	 return;
   }
  strLendTime=lstvQuery->Selected->SubItems->Strings[2]+(AnsiString)":00";
  l_strSQLClause = (AnsiString)"Update lib_LendInfo set IsLost=1,remark='Lost.',ReturnTime=GetDate(),ReturnOpID='"
					+SQLStr(CLdSystemData->strCurrentUser)
					+(AnsiString)"' where ResID='"
					+SQLStr(lstvQuery->Selected->SubItems->Strings[0])
					+(AnsiString)"' and cast(LendTime As Date)>='"
					+strLendTime
					+(AnsiString)"'";
   dmLendDM->daqLendInfo->SQL->Clear();
   dmLendDM->daqLendInfo->SQL->Add(l_strSQLClause);
   try
	{
	 dmLendDM->daqLendInfo->ExecSQL();
	}

   catch(...)
	{
	 Application->MessageBox(L"Update lib_LendInfo failure",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
	 dmLendDM->dbMain->Rollback();
	 return;
   }
   dmLendDM->dbMain->Commit();
   try
   {
    dmLendDM->daqTime->Open();
   }
   catch(...)
   {
	Application->MessageBox(L"Can not get current time",UnicodeString(CLdSystemData->strCurrentUser).c_str(),MB_OK|MB_ICONINFORMATION);
    return;
   }
   lstvQuery->Selected->ImageIndex = 3;
  // lstvQuery->Selected->SubItems->Strings[3] = (AnsiString)(((TDateTime)(dmLendDM->daqTime->FieldValues["Column1"])).FormatString("yyyy/mm/dd hh:nn"));

   lstvQuery->Selected->SubItems->Strings[3] = (dmLendDM->daqTime->FieldByName("Column1")->AsDateTime).FormatString("yyyy/mm/dd hh:nn");

   lstvQuery->Selected->SubItems->Strings[4] =  CLdSystemData->strCurrentUser;
   lstvQuery->Selected->SubItems->Strings[6] = "Lost";
  }
    
}
//---------------------------------------------------------------------------


void __fastcall TfrmLendSys::btGoClick(TObject *Sender)
{
    muiRefreshClick(Sender);
}

 // added 2002.7 by huchen
int TfrmLendSys::GetLimitNum(int &imaxpage,int &ipage, int &istart,int &iend)
{
  imaxpage = 0;
  imaxpage = dmLendDM->daqLendInfo->RecordCount / CLdSystemData->nMaxRecord;
  if ( (dmLendDM->daqLendInfo->RecordCount % CLdSystemData->nMaxRecord) > 0 )
    imaxpage ++;
  ipage = atoi( AnsiString(edpage->Text).c_str()) ;

  if  ( ipage > imaxpage )
	 ipage = imaxpage;

  if ( ipage < 1 )
    ipage = 1;

  istart = (ipage -1 )* CLdSystemData->nMaxRecord;
  iend = ipage *  CLdSystemData->nMaxRecord;

  if ( iend > dmLendDM->daqLendInfo->RecordCount )
    iend = dmLendDM->daqLendInfo->RecordCount;

  return 1;
}

void TfrmLendSys::DisplayResultHint(int imaxpage,int ipage, int istart,int iend)
{
   char cmessage[255];
   
   sprintf(cmessage,"The current record number is from %d to %d!" , istart,iend );
   frmMain->stbStory->Panels->Items[2]->Text =(AnsiString) cmessage;
   sprintf(cmessage," %d records / %d pages" , dmLendDM->daqLendInfo->RecordCount, imaxpage );
   frmMain->stbStory->Panels->Items[1]->Text =(AnsiString)cmessage;
}
//--------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::lstvQueryColumnClick(TObject *Sender, TListColumn *Column)

{
  ColumnToSort = Column->Index;
  dynamic_cast<TCustomListView *>(Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::lstvQueryCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
		  int Data, int &Compare)
{
  if (ColumnToSort == 0)
	Compare = CompareText(Item1->Caption,Item2->Caption);
  else
  {
	int ix = ColumnToSort - 1;
	Compare =
	  CompareText(Item1->SubItems->Strings[ix], Item2->SubItems->Strings[ix]);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::ntbLendPageChanged(TObject *Sender)
{
  if (frmLendSys->ntbLend->PageIndex==2)
  {
	muiRefreshClick(Sender);
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmLendSys::UserInformation1Click(TObject *Sender)
{
 frmFindUser->ShowModal();
}
//---------------------------------------------------------------------------

