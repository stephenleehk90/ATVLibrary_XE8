//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LogFrm.h"
#include "ConfigFrm.h"
#include "RestypeFrm.h"
#include "LogData.h"
#include "CommFunc.h"
#include "DataProcess.h"
#include "frm_Main.h"
#include "StoryData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
bool RefreshLogs(TDateTime tBegin,TDateTime tEnd,AnsiString strID)
{
   int j;
   TListItem *pListItem;
   AnsiString strSQLClause,strBeginDate,strEndDate;
   TDateTime ResTime;
   AnsiString strDateTime;
   strBeginDate=tBegin.FormatString("yyyy-mm-dd");
   strEndDate=tEnd.FormatString("yyyy-mm-dd");
   if (strID.Trim()=="everyone")
	   strSQLClause=(AnsiString)"Select * from dbo.lib_Log where cast(OpTime As Date)>='"
					+(AnsiString)strBeginDate+(AnsiString)"'and cast(OpTime As Date)<='"
					+(AnsiString)strEndDate+(AnsiString)"'";
   else
	   strSQLClause=(AnsiString)"Select * from dbo.lib_Log where cast(OpTime As Date)>='"
					+(AnsiString)strBeginDate+(AnsiString)"'and cast(OpTime As Date)<='"
                    +(AnsiString)strEndDate
					+(AnsiString)"'and OpID='"
                    +(AnsiString)SQLStr(strID.Trim())+(AnsiString)"'";
   dmdLog->daqLogs->SQL->Clear();
   dmdLog->daqLogs->SQL->Add(strSQLClause);
   try
   {
	 dmdLog->daqLogs->Open();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 return false;
   }
   dmdLog->daqLogs->First();
   j=0;
//   frmLogs->lstvwLogs->BeginUpdate();
   frmLogs->lstvwLogs->Items->Clear();
   while (!dmdLog->daqLogs->Eof)
   {
	  if (j>=MAXROWS)
	  {
//         Application->MessageBox("The Browsing list is too long,please change query condition to reduce the number of records being browsed!","TBS Library",MB_OK|MB_ICONINFORMATION);
		 dmdLog->stbLog->Panels->Items[2]->Text = "The Browsing list is too long!";
		// frmLogs->lstvwLogs->EndUpdate();
		 dmdLog->stbLog->Panels->Items[1]->Text="Log numbers: "+(AnsiString)dmdLog->daqLogs->RecordCount;
		 return true;
	  }
	  else
	  {
		pListItem=frmLogs->lstvwLogs->Items->Add();
		pListItem->Caption=OrigStr(dmdLog->daqLogs->FieldByName("LogID")->AsString);
		pListItem->SubItems->Add(OrigStr(dmdLog->daqLogs->FieldByName("OpID")->AsString));
		ResTime=dmdLog->daqLogs->FieldByName("OpTime")->AsDateTime;
		strDateTime=ResTime.FormatString("yyyy-mm-dd hh:nn:ss");
		pListItem->SubItems->Add(strDateTime);
		pListItem->SubItems->Add(OrigStr(dmdLog->daqLogs->FieldByName("Operation")->AsString));
		pListItem->SubItems->Add(OrigStr(dmdLog->daqLogs->FieldByName("ObjStr")->AsString));
		pListItem->SubItems->Add(OrigStr(dmdLog->daqLogs->FieldByName("Module")->AsString));
		dmdLog->daqLogs->Next();
		j++;
	  }
	}
 //	frmLogs->lstvwLogs->EndUpdate();
	frmMain->stbStory->Panels->Items[1]->Text="Log numbers: "+(AnsiString)frmLogs->lstvwLogs->Items->Count;
	return true;
}
bool LogDelete(int nDays)
{
  AnsiString strSQLClause,strDate;
  TDateTime time;
  time=time.CurrentDateTime()-nDays+1;
  strDate=time.FormatString("yyyy-mm-dd");
  frmLogs->lstvwLogs->Items->Clear();
  strSQLClause=(AnsiString)"delete dbo.lib_Log where cast(OpTime As Date)<='"
			   +(AnsiString)strDate+(AnsiString)"'";
  dmdLog->daqLogs->SQL->Clear();
  dmdLog->daqLogs->SQL->Add(strSQLClause);
  try
  {
	dmdLog->daqLogs->ExecSQL();
	CreateLog("Delete Logs","Logs","","Event Log");
	RefreshLogs(frmLogs->LogBegin,frmLogs->LogEnd,frmLogs->strOpID);
	frmMain->stbStory->Panels->Items[2]->Text="The expired logs have been deleted.";
	return true;
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	return false;
  }
}
bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module)
{
  AnsiString strSQLClause,strFileID;
  strSQLClause =(AnsiString)"exec GetNewFileID";
  dmdLog->daqLogs->SQL->Clear();
  dmdLog->daqLogs->SQL->Add(strSQLClause);
  try
  {
	dmdLog->daqLogs->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Failed update table lib_Log.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	return false;
  }
  dmdLog->daqLogs->First();
  strFileID=dmdLog->daqLogs->FieldValues["FileID"];
  strSQLClause = (AnsiString)"Insert into dbo.lib_Log (LogID,OpID,OpTime,Operation,ObjID,ObjStr,Module) values('"
				  +(AnsiString)SQLStr(strFileID)
				  +(AnsiString)"','"
				  //+SQLStr(frmLogs->strUserID)
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
 dmdLog->daqLogs->SQL->Clear();
 dmdLog->daqLogs->SQL->Add(strSQLClause);
 try
 {
  dmdLog->daqLogs->ExecSQL();
  }
 catch(...)
 {
  Application->MessageBox(L"Failed update table lib_Log.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  return false;
 }
 return true;
}
bool CommitValues(AnsiString Oldkey,AnsiString Newkey)
{
  AnsiString strSQLClause;
  strSQLClause=(AnsiString)"delete dbo.lib_Keyword where Keyname='"
			   +SQLStr(Oldkey)+(AnsiString)"'";
  dmdLog->daqKeywords->SQL->Clear();
  dmdLog->daqKeywords->SQL->Add(strSQLClause);
  for(int i=0;i<frmConfig->lstvwValues->Items->Count;i++)
  {
	if ((!frmConfig->lstvwValues->Items->Item[i]->Caption.Trim().IsEmpty())
	   &&(CheckValues(frmConfig->lstvwValues->Items->Item[i]->Caption)<=1))
	{
	  strSQLClause=(AnsiString)"insert dbo.lib_Keyword (Keyname,Value)values('"
				   +SQLStr(Newkey)+(AnsiString)"','"
				   +SQLStr(frmConfig->lstvwValues->Items->Item[i]->Caption.Trim())+(AnsiString)"')";
	  dmdLog->daqKeywords->SQL->Add(strSQLClause);
	}
  }
  try
  {
	dmdLog->daqKeywords->ExecSQL();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	return false;
  }

  return true;

}
int CheckValues(AnsiString Caption)
{
  int nCount;
  nCount=0;
  for (int i=0;i<frmConfig->lstvwValues->Items->Count;i++)
  {
	if (Caption.Trim()==frmConfig->lstvwValues->Items->Item[i]->Caption.Trim())
	   nCount++;
  }
  return nCount;
}
bool CommitTypes()
{
  AnsiString strSQLClause;
  AnsiString strTypeID,strTypeName,strDays;
  dmdLog->daqResType->SQL->Clear();
  strSQLClause=(AnsiString)"delete dbo.lib_ResTypes";
  dmdLog->daqResType->SQL->Add(strSQLClause);
  for(int i=1;i<frmResType->strgrdResType->RowCount;i++)
  {
    strTypeID=frmResType->strgrdResType->Cells[0][i].Trim();
    strTypeName=frmResType->strgrdResType->Cells[1][i].Trim();
    strDays=frmResType->strgrdResType->Cells[2][i].Trim();
    if ((!strTypeID.IsEmpty())&&(!strTypeName.IsEmpty())&&(!strDays.IsEmpty())
       &&(CheckTypes(strTypeID,strTypeName)==0))
    {
      strSQLClause=(AnsiString)"insert dbo.lib_ResTypes (TypeID,TypeName,BorrowDays)values("
                   +frmResType->strgrdResType->Cells[0][i].ToInt()+(AnsiString)",'"
                   +SQLStr(frmResType->strgrdResType->Cells[1][i])+(AnsiString)"',"
                   +frmResType->strgrdResType->Cells[2][i].ToInt()+(AnsiString)")";
      dmdLog->daqResType->SQL->Add(strSQLClause);
    }
  }
  try
  {
    dmdLog->daqResType->ExecSQL();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    return false;
  }
  return true;
}
int CheckTypes(AnsiString strTypeID,AnsiString strTypeName)
{
  int j,k;
  j=0;
  k=0;
  for (int i=1;i<=frmResType->strgrdResType->RowCount;i++)
  {
    if (strTypeID.Trim()==frmResType->strgrdResType->Cells[0][i].Trim())
        j++;
    if (strTypeName.Trim()==frmResType->strgrdResType->Cells[1][i].Trim())
        k++;
  }
  if (j>1)
    return 1;
  if (k>1)
    return 2;
  return 0;  
}
bool CheckUnique(AnsiString strSQLClause)
{
   dmdLog->daqCheck->SQL->Clear();
   dmdLog->daqCheck->SQL->Add(strSQLClause);
   try
   {
     dmdLog->daqCheck->Open();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return false;
   }
   if (dmdLog->daqCheck->RecordCount>0)
       return false;
   return true;
}

