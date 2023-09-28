//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>

#pragma hdrstop

#include "DataProcess.h"
#include "MaterialData.h"
#include "MaterialFrm.h"
#include "PreviewFrm.h"
#include "CommFunc.h"
#include "frm_Main.h"
#include "StoryData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
bool BrowseMaterialType(TStringList *strResType)
{
  AnsiString strSQLClause;
  int nMaterialIndex,nResIndex,nInputIndex;
  nMaterialIndex=frmMaterials->cboMaterialType->ItemIndex;
  nResIndex=frmMaterials->cboResType->ItemIndex;
  nInputIndex=frmMaterials->cboInputType->ItemIndex;
  frmMaterials->cboMaterialType->Items->Clear();
  frmMaterials->cboResType->Items->Clear();
  frmMaterials->cboInputType->Items->Clear();
  frmMaterials->cboInputType->Items->Add("All Materials");
  strSQLClause=(AnsiString)"select TypeName,TypeID from lib_ResTypes";

  dmdMaterials->daqCheck->SQL->Clear();
  dmdMaterials->daqCheck->SQL->Add(strSQLClause);
  try
  {
	dmdMaterials->daqCheck->Open();
	dmdMaterials->daqCheck->First();
	for (int i=0;i<dmdMaterials->daqCheck->RecordCount;i++)
	{
	  strResType->Add(OrigStr(dmdMaterials->daqCheck->FieldValues["TypeName"])
			+(AnsiString)"="+dmdMaterials->daqCheck->FieldValues["TypeID"]);
	  frmMaterials->cboMaterialType->Items->Add(OrigStr(dmdMaterials->daqCheck->FieldValues["TypeName"]));
	  frmMaterials->cboResType->Items->Add(OrigStr(dmdMaterials->daqCheck->FieldValues["TypeName"]));
	  frmMaterials->cboInputType->Items->Add(OrigStr(dmdMaterials->daqCheck->FieldValues["TypeName"]));
	  dmdMaterials->daqCheck->Next();
	}
//	dmdMaterials->daqCheck->Close();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	strResType->Clear();
	strResType->Add("Tapes=1");
	strResType->Add("Books=2");
	strResType->Add("Magazines=3");
	strResType->Add("Files=4");
	frmMaterials->cboMaterialType->Items->Add("Tapes");
	frmMaterials->cboMaterialType->Items->Add("Books");
	frmMaterials->cboMaterialType->Items->Add("Magazines");
	frmMaterials->cboMaterialType->Items->Add("Files");
	frmMaterials->cboResType->Items->Add("Tapes");
	frmMaterials->cboResType->Items->Add("Books");
	frmMaterials->cboResType->Items->Add("Magazines");
	frmMaterials->cboResType->Items->Add("Files");
	frmMaterials->cboInputType->Items->Add("Tapes");
	frmMaterials->cboInputType->Items->Add("Books");
	frmMaterials->cboInputType->Items->Add("Magazines");
	frmMaterials->cboInputType->Items->Add("Files");
  }
  frmMaterials->cboMaterialType->ItemIndex=nMaterialIndex;
  frmMaterials->cboResType->ItemIndex=nResIndex;
  frmMaterials->cboInputType->ItemIndex=nInputIndex;
  return true;
}

 // added 2002.7 by huchen
int GetLimitNum(int &imaxpage,int &ipage, int &istart,int &iend)
{
  imaxpage = 0;
  imaxpage = dmdMaterials->daqMaterials->RecordCount / MAXROWS;
  if ( (dmdMaterials->daqMaterials->RecordCount % MAXROWS) > 0 )
	imaxpage ++;
  ipage = atoi(AnsiString(frmMaterials->edpage->Text).c_str()) ;

  if  ( ipage > imaxpage )
	 ipage = imaxpage;

  if ( ipage < 1 )
	ipage = 1;

  istart = (ipage -1 )* MAXROWS;
  iend = ipage *  MAXROWS;

  if ( iend > dmdMaterials->daqMaterials->RecordCount )
	iend = dmdMaterials->daqMaterials->RecordCount;

  return 1;
}

bool GetOrderColumn(AnsiString &SQLClause,AnsiString &strOrderList)
{

 AnsiString fieldlist[] = {"ResID","ResType","remark","RegTime","Position","state","BorrowerID"};
 // lstvQuery->GetSubItemText(lstvQuery->LastColumnClicked);

// if ( frmMaterials->lstvwMaterials->LastColumnClicked > 6  || frmMaterials->lstvwMaterials->LastColumnClicked < 0 )
  //	 frmMaterials->lstvwMaterials->LastColumnClicked = 3;

// SQLClause += AnsiString(" order by " +  fieldlist[frmMaterials->lstvwMaterials->LastColumnClicked]);
 SQLClause += AnsiString(" order by " +  fieldlist[3]);
 strOrderList = AnsiString(" sorted by " +  frmMaterials->lstvwMaterials->Columns->Items[3]->Caption);
	 SQLClause += AnsiString(" asc ");
	 strOrderList += AnsiString(" asc ");
/*
 if ( frmMaterials->lstvwMaterials->CurrentSortAscending )
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

bool BrowseMaterials()
{
  AnsiString strSQLClause,strBeginDate,strEndDate;
  AnsiString strResType,strResState;
  TDateTime ResTime;
  AnsiString strDateTime;
  int j,nResType,nResState;
  int istart,iend,ipage,imaxpage;
  char cmessage[255];

  j=0;
  TListItem *pListItem;
  BrowseMaterialType(frmMaterials->strlstResType);
  strBeginDate=frmMaterials->BeginDate.FormatString("yyyy-mm-dd");
  strEndDate=frmMaterials->EndDate.FormatString("yyyy-mm-dd");
  if (frmMaterials->cboInputType->ItemIndex==0)
	  strSQLClause=(AnsiString)"select * from lib_Res where cast(RegTime As Date)>='"+
					  (AnsiString)strBeginDate+(AnsiString)"'"+
					  (AnsiString)"and cast(RegTime As Date) <='"+
					  (AnsiString)strEndDate+(AnsiString)"'";
  else
	  strSQLClause=(AnsiString)"Select * from lib_Res Where ResType="
					   +(AnsiString)frmMaterials->cboInputType->ItemIndex
					   +(AnsiString)"and cast(RegTime As Date)>='"
					   +(AnsiString)strBeginDate+(AnsiString)"'"
					   +(AnsiString)"and cast(RegTime As Date) <='"
					   +(AnsiString)strEndDate+(AnsiString)"'";

  AnsiString strOrderList;
  GetOrderColumn(strSQLClause,strOrderList);

  dmdMaterials->daqMaterials->SQL->Clear();
  dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
  try
  {
	dmdMaterials->daqMaterials->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	return false;
  }
  dmdMaterials->daqMaterials->First();
  //frmMaterials->lstvwMaterials->BeginUpdate();
  frmMaterials->lstvwMaterials->Items->Clear();

  GetLimitNum(imaxpage,ipage,istart,iend);

  while (!dmdMaterials->daqMaterials->Eof)
  {
	// huchen 2002.7.
	 if ( j < istart )
	 {
		dmdMaterials->daqMaterials->Next();
		j ++;
		continue;
	 }
	 if ( j > iend )
	 //if (j>=MAXROWS)
	 {
		break;
	 }
	 else
	 {
	   pListItem=frmMaterials->lstvwMaterials->Items->Add();
	   pListItem->Caption=OrigStr(dmdMaterials->daqMaterials->FieldByName("ResID")->AsString);
	   nResType=dmdMaterials->daqMaterials->FieldByName("ResType")->AsInteger;
	   for (int i=0;i<frmMaterials->strlstResType->Count;i++)
	   {
		 if (nResType==frmMaterials->strlstResType->Values[frmMaterials->strlstResType->Names[i]].ToInt())
		 {
		   strResType=frmMaterials->strlstResType->Names[i];
		   break;
		 }
	   }
	   pListItem->SubItems->Add(strResType);
	   pListItem->SubItems->Add(OrigStr(dmdMaterials->daqMaterials->FieldByName("remark")->AsString));
//	   ResTime=(TDateTime)dmdMaterials->daqMaterials->FieldValues["RegTime"];
	   ResTime=(TDateTime)dmdMaterials->daqMaterials->FieldByName("RegTime")->AsDateTime;

	   strDateTime=ResTime.FormatString("yyyy-mm-dd hh:nn");
	   pListItem->SubItems->Add(strDateTime);
	   pListItem->SubItems->Add(OrigStr(dmdMaterials->daqMaterials->FieldByName("Position")->AsString));
	   nResState=dmdMaterials->daqMaterials->FieldByName("state")->AsInteger;
	   switch (nResState)
	   {
		 case -1:
			 strResState="Lost";
			 break;
		 case 0:
			 strResState="On Shelf";
			 break;
		 case 1:
			 strResState="Borrowed";
			 break;
	   }
	   pListItem->SubItems->Add(strResState);
	   pListItem->SubItems->Add(OrigStr(dmdMaterials->daqMaterials->FieldByName("BorrowerID")->AsString));
	   dmdMaterials->daqMaterials->Next();
	   j++;
	 }
   }

 //  frmMaterials->lstvwMaterials->EndUpdate();
   sprintf(cmessage,"The current record number is from %d to %d!" , istart,iend );
   frmMain->stbStory->Panels->Items[2]->Text=(AnsiString) cmessage;
   sprintf(cmessage," %d records / %d pages" , dmdMaterials->daqMaterials->RecordCount, imaxpage );
   frmMain->stbStory->Panels->Items[1]->Text=(AnsiString)cmessage;

   return true;
}
bool ShelfMaterials()
{
  AnsiString strSQLClause;
  AnsiString strResType,strResState;
  AnsiString strDateTime;
  TDateTime ResTime;
  int j,nResType,nResState;
  int istart,iend,ipage,imaxpage;
  char cmessage[255];


  j=0;
  TListItem *pListItem;
  BrowseMaterialType(frmMaterials->strlstResType);
  if (frmMaterials->cboInputType->ItemIndex==0)
  {
	 strSQLClause=(AnsiString)"select * from lib_Res where Position='"+
				  (AnsiString)SQLStr(frmMaterials->lblShelfID->Caption.Trim())+
				  (AnsiString)"'";
  }
  else
  {
	 strSQLClause=(AnsiString)"Select * from lib_Res Where ResType="
				 +(AnsiString)frmMaterials->cboInputType->ItemIndex
				 +(AnsiString)"and Position='"
				 +(AnsiString)SQLStr(frmMaterials->lblShelfID->Caption)
				 +(AnsiString)"'";
  }

  AnsiString strOrderList;
  GetOrderColumn(strSQLClause,strOrderList);

  dmdMaterials->daqMaterials->SQL->Clear();
  dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
  try
  {
	dmdMaterials->daqMaterials->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	return false;
  }
  dmdMaterials->daqMaterials->First();
 // frmMaterials->lstvwMaterials->BeginUpdate();
  frmMaterials->lstvwMaterials->Items->Clear();

  GetLimitNum(imaxpage,ipage,istart,iend);

  j = 0;
  while (!dmdMaterials->daqMaterials->Eof)
  {
			 // updated 2002.7
	 if ( j < istart )
	 {
	   dmdMaterials->daqMaterials->Next();
	   j++;
	   continue;
	 }

	 if ( j>= iend ) //j>=MAXROWS)
	 {
//        Application->MessageBox("The Browsing list is too long,please change query condition to reduce the number of records being browsed!","TBS Library",MB_OK|MB_ICONINFORMATION);
		break;
	 }
	 else
	 {
	   pListItem=frmMaterials->lstvwMaterials->Items->Add();
	   pListItem->Caption=OrigStr(dmdMaterials->daqMaterials->FieldByName("ResID")->AsString);
	   nResType=dmdMaterials->daqMaterials->FieldByName("ResType")->AsInteger;
	   for (int i=0;i<frmMaterials->strlstResType->Count;i++)
	   {
		 if (nResType==frmMaterials->strlstResType->Values[frmMaterials->strlstResType->Names[i]].ToInt())
		 {
		   strResType=frmMaterials->strlstResType->Names[i];
		   break;
		 }
	   }
	   pListItem->SubItems->Add(strResType);
	   pListItem->SubItems->Add(OrigStr(dmdMaterials->daqMaterials->FieldByName("remark")->AsString));
//	   ResTime=(TDateTime)dmdMaterials->daqMaterials->FieldValues["RegTime"];
	   ResTime=(TDateTime)dmdMaterials->daqMaterials->FieldByName("RegTime")->AsDateTime;
	   strDateTime=ResTime.FormatString("yyyy-mm-dd hh:nn");
	   pListItem->SubItems->Add(strDateTime);
	   pListItem->SubItems->Add(OrigStr(dmdMaterials->daqMaterials->FieldByName("Position")->AsString));
	   nResState=dmdMaterials->daqMaterials->FieldByName("state")->AsInteger;
	   switch (nResState)
	   {
		 case -1:
			 strResState="Lost";
			 break;
		 case 0:
			 strResState="On Shelf";
			 break;
		 case 1:
			 strResState="Borrowed";
			 break;
	   }
	   pListItem->SubItems->Add(strResState);
	   pListItem->SubItems->Add(OrigStr(dmdMaterials->daqMaterials->FieldByName("BorrowerID")->AsString));
	   dmdMaterials->daqMaterials->Next();
	   j++;
	 }
   }

 //  frmMaterials->lstvwMaterials->EndUpdate();

   sprintf(cmessage,"The current record number is from %d to %d!" , istart,iend );
   frmMain->stbStory->Panels->Items[2]->Text=(AnsiString) cmessage;
   sprintf(cmessage," %d records / %d pages" , dmdMaterials->daqMaterials->RecordCount, imaxpage );
   frmMain->stbStory->Panels->Items[1]->Text=(AnsiString)cmessage;

   return true;
}
bool MaterialsAdd()
{
  AnsiString strSQLClause;
  AnsiString ResTime;
  int nResType;
  nResType=frmMaterials->cboResType->ItemIndex+1;
//  nResState=frmMaterials->cboState->ItemIndex-1;
  strSQLClause = (AnsiString)"Insert into lib_Res(ResID,ResType,RegTime,Position,state,remark) values('"
				  +(AnsiString)SQLStr(frmMaterials->edtResID->Text.Trim())
				  +(AnsiString)"',"
				  +nResType
				  +(AnsiString)",GetDate(),'"
				  +frmMaterials->edtPosition->Text.Trim()
				  +(AnsiString)"',"
				  +0
				  +(AnsiString)",'"
				  +SQLStr(frmMaterials->edtResRemark->Text.Trim())
				  +(AnsiString)"')";
  dmdMaterials->daqMaterials->SQL->Clear();
  dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
  try
  {
	dmdMaterials->daqMaterials->ExecSQL();
	CreateLog("Add Materials",frmMaterials->edtResID->Text.Trim(),frmMaterials->edtResRemark->Text.Trim(),"Materials Input");
  }
  catch(...)
  {
	Application->MessageBox(L"Database Error",L"TBS Library",MB_OK|MB_ICONERROR);
	return false;
  }
  TListItem *pListItem;
  strSQLClause=(AnsiString)"select getdate()";
  dmdMaterials->daqMaterials->SQL->Clear();
  dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
  try
  {
	dmdMaterials->daqMaterials->Open();
	dmdMaterials->daqMaterials->First();
	ResTime=dmdMaterials->daqMaterials->FieldValues["column1"];
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	return false;
  }
  pListItem=frmMaterials->lstvwMaterials->Items->Add();
  pListItem->Caption=frmMaterials->edtResID->Text.Trim();
  pListItem->SubItems->Add(frmMaterials->cboResType->Text.Trim());
  pListItem->SubItems->Add(frmMaterials->edtResRemark->Text.Trim());
  pListItem->SubItems->Add(ResTime.SubString(0,ResTime.Length()-3));
  pListItem->SubItems->Add(frmMaterials->edtPosition->Text.Trim());
  pListItem->SubItems->Add("On Shelf");
  pListItem=frmMaterials->lstvwMaterialPos->Items->Add();
  pListItem->Caption=frmMaterials->edtResID->Text.Trim();
  pListItem->SubItems->Add(frmMaterials->cboResType->Text.Trim());
  pListItem->SubItems->Add(frmMaterials->edtResRemark->Text.Trim());
  pListItem->SubItems->Add(ResTime.SubString(0,ResTime.Length()-3));
  pListItem->SubItems->Add(frmMaterials->edtPosition->Text.Trim());
  pListItem->SubItems->Add("On Shelf");

  return true;
}
bool MaterialsDelete()
{
   AnsiString strSQLClause,strResID,strRemark;
   AnsiString strBeginDate,strEndDate;
   TListItem *pListItem,*pNextItem;
   TItemStates ItemState;
   int nSelected,nState;


   ItemState<<isSelected;
   //nBorrowed=0;
   pListItem=frmMaterials->lstvwMaterials->Selected;
   nSelected=frmMaterials->lstvwMaterials->SelCount;
   try
   {
	 for (int i=0;i<nSelected;i++)
	 {
	   pNextItem=frmMaterials->lstvwMaterials->GetNextItem(pListItem,sdBelow,ItemState);
	   strResID=pListItem->Caption;
	   strRemark=pListItem->SubItems->Strings[1];
	   dmdMaterials->daqMaterials->SQL->Clear();
	   strSQLClause=(AnsiString)"delete lib_Res where ResID='"
				  +(AnsiString)SQLStr(strResID)+(AnsiString)"'";
	   dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
	   strSQLClause=(AnsiString)"select state from lib_Res where ResID='"
					+SQLStr(strResID)+(AnsiString)"'";
	   dmdMaterials->daqCheck->SQL->Clear();
	   dmdMaterials->daqCheck->SQL->Add(strSQLClause);
	   dmdMaterials->daqCheck->Open();
	   if (dmdMaterials->daqCheck->RecordCount<1)
	   {
		 frmMaterials->lstvwMaterials->Items->
					  Delete(frmMaterials->lstvwMaterials->Items->IndexOf(pListItem));
	   }
	   else
	   {
		 nState=dmdMaterials->daqCheck->FieldByName("state")->AsInteger;
		 if(nState==1)
		 {
		 Application->MessageBox(L"It is borrowed, and it can not be deleted.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
		// nBorrowed++;
		 }
		 else
		 {
		   dmdMaterials->daqMaterials->ExecSQL();
		   CreateLog("Delete Materials",strResID,strRemark,"Material Input");
		   frmMaterials->lstvwMaterials->Items->
					  Delete(frmMaterials->lstvwMaterials->Items->IndexOf(pListItem));
		 }
	   }
	   pListItem=pNextItem;
	 }

	 dmdMaterials->daqMaterials->SQL->Clear();
	 strBeginDate=frmMaterials->BeginDate.FormatString("yyyy-mm-dd");
	 strEndDate=frmMaterials->EndDate.FormatString("yyyy-mm-dd");

	 if (frmMaterials->muiBrowse->Checked)
	 {
	   if (frmMaterials->cboInputType->ItemIndex==0)
		  strSQLClause=(AnsiString)"select * from lib_Res where cast(RegTime As Date) >='"+
						  (AnsiString)strBeginDate+(AnsiString)"'"+
						  (AnsiString)"and cast(RegTime As Date)<='"+
						  (AnsiString)strEndDate+(AnsiString)"'";
	   else
		  strSQLClause=(AnsiString)"Select * from lib_Res Where ResType="
						   +(AnsiString)frmMaterials->cboInputType->ItemIndex
						   +(AnsiString)"and cast(RegTime As Date)>='"
						   +(AnsiString)strBeginDate+(AnsiString)"'"
						   +(AnsiString)"and cast(RegTime As Date)<='"
						   +(AnsiString)strEndDate+(AnsiString)"'";
	 }
	 else
	 {
	   if (frmMaterials->cboInputType->ItemIndex==0)
	   {
		  strSQLClause=(AnsiString)"select * from lib_Res where Position='"+
					   (AnsiString)SQLStr(frmMaterials->lblShelfID->Caption.Trim())+
					   (AnsiString)"'";
	   }
	   else
	   {
		  strSQLClause=(AnsiString)"Select * from lib_Res Where ResType="
					  +(AnsiString)frmMaterials->cboInputType->ItemIndex
					  +(AnsiString)"and Position='"
					  +(AnsiString)SQLStr(frmMaterials->lblShelfID->Caption)
					  +(AnsiString)"'";
	   }
	 }
	 dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
	 dmdMaterials->daqMaterials->Open();
	 frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)dmdMaterials->daqMaterials->RecordCount;
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 return false;
   }

/*
   if (nBorrowed<1)
	 frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"A material has been deleted.";
*/
   return true;
}
bool MaterialsModify()
{
   AnsiString strSQLClause;
   int nResType,ResState;
   nResType=frmMaterials->strlstResType->Values[frmMaterials->cboResType->Text].ToInt();
   ResState=frmMaterials->cboState->ItemIndex-1;
   dmdMaterials->daqMaterials->SQL->Clear();
   strSQLClause=(AnsiString)"update lib_Res set ResID="+
                (AnsiString)"'"+
                (AnsiString)SQLStr(frmMaterials->edtResID->Text.Trim())+
                (AnsiString)"',ResType="+
                nResType+
                (AnsiString)",remark='"+
                (AnsiString)SQLStr(frmMaterials->edtResRemark->Text.Trim())+
                (AnsiString)"',Position='"+
                (AnsiString)SQLStr(frmMaterials->edtPosition->Text.Trim())+
                (AnsiString)"',state="+
                ResState+
                (AnsiString)"where ResID='"+
                (AnsiString)SQLStr(frmMaterials->strResID)+(AnsiString)"'";
   dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
   try
   {
      dmdMaterials->daqMaterials->Prepare();
      dmdMaterials->daqMaterials->ExecSQL();
      CreateLog("Modify Material",frmMaterials->strResID,frmMaterials->strRemark,"Material Input");
      return true;
   }
   catch(...)
   {
	  Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  return false;
   }
}
void ClearMaterialInput()
{
  frmMaterials->edtResID->Clear();
  frmMaterials->cboResType->ItemIndex=-1;
  frmMaterials->edtResRemark->Clear();
  frmMaterials->cboState->ItemIndex=-1;
  frmMaterials->edtPosition->Clear();
}
bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module)
{
  AnsiString strSQLClause,strFileID;
  strSQLClause =(AnsiString)"exec GetNewFileID";
  dmdMaterials->daqLogs->Close();
  dmdMaterials->daqLogs->SQL->Clear();
  dmdMaterials->daqLogs->SQL->Add(strSQLClause);
  try
  {
	dmdMaterials->daqLogs->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Failed update table lib_Log",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	return false;
  }
  dmdMaterials->daqLogs->First();
  strFileID=dmdMaterials->daqLogs->FieldValues["FileID"];
  strSQLClause = (AnsiString)"Insert into lib_Log (LogID,OpID,OpTime,Operation,ObjID,ObjStr,Module) values('"
				  +(AnsiString)SQLStr(strFileID)
				  +(AnsiString)"','"
				  +SQLStr(dmdStory->strCurrentUserID)
				  +(AnsiString)"',GetDate(),'"
				  +SQLStr(Operation)
				  +(AnsiString)"','"
				  +SQLStr(ObjID)
				  +(AnsiString)"','"
				  +SQLStr(ObjStr)
				  +(AnsiString)"','"
				  +SQLStr(Module)
				  +(AnsiString)"')";
 dmdMaterials->daqLogs->SQL->Clear();
 dmdMaterials->daqLogs->SQL->Add(strSQLClause);
 try
 {
  dmdMaterials->daqLogs->ExecSQL();
  }
 catch(...)
 {
  Application->MessageBox(L"Failed update table lib_Log",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  return false;
 }
 return true;
}

bool MaterialReport(bool bpreview,TObject *sender)
{
}

bool CheckUnique(AnsiString strSQLClause)
{
   dmdMaterials->daqCheck->SQL->Clear();
   dmdMaterials->daqCheck->SQL->Add(strSQLClause);
   try
   {
	 dmdMaterials->daqCheck->Open();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 return false;
   }
   if (dmdMaterials->daqCheck->RecordCount>0)
	   return false;
   return true;
}
