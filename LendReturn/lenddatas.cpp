//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "lenddatas.h"
#include "dmData.h"
#include "commfunc.h"
//---------------------------------------------------------------------------
TLdSystemData *CLdSystemData;
__fastcall TLdSystemData::TLdSystemData()
{
 nLendOrReturnCount = 0;
 nMaxRecord = MAXROWS;
}
bool __fastcall TLdSystemData::SearchInListBox(AnsiString strBeSearched,TObject *SearchedBox)
{
  int i;
  if  (((AnsiString)SearchedBox->ClassName())=="TListView")
  {
   for (i=0;i<((TListView *)SearchedBox)->Items->Count;i++)
	{
	 if (strBeSearched == ((TListView *)SearchedBox)->Items->Item[i]->Caption)
	  return true;
	}
   return false;
  }

  if  (((AnsiString)SearchedBox->ClassName())=="TComboBox")
  {
   for (i=0;i<((TComboBox *)SearchedBox)->Items->Count;i++)
	{
	 if (strBeSearched == ((TComboBox *)SearchedBox)->Items->Strings[i])
	  return true;
	}
   return false;
  }
  return true;
}

bool __fastcall TLdSystemData::CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module,AnsiString Remark)
{
 AnsiString l_strSQLClause;
 try
 {
  dmLendDM->daqNewID->Close();
  dmLendDM->daqNewID->Open();
 }
 catch(...)
 {
  Application->MessageBox(L"Failed execute SQL Clause:exec GetNewFileID",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return false;
 }

 l_strSQLClause = (AnsiString)"Insert into lib_Log values(\'"
				  +dmLendDM->daqNewID->FieldValues["FileID"]
				  +(AnsiString)"\',\'"
				  +SQLStr(this->strCurrentUser)
				  +(AnsiString)"\',GetDate(),\'"
				  +SQLStr(Operation)
				  +(AnsiString)"\',\'"
				  +SQLStr(ObjID)
				  +(AnsiString)"\',\'"
				  +SQLStr(ObjStr)
				  +(AnsiString)"\',\'"
				  +SQLStr(Module)
				  +(AnsiString)"\',\'"
				  +SQLStr(Remark)
				  +(AnsiString)"\')";
 dmLendDM->daqLog->SQL->Clear();
 dmLendDM->daqLog->SQL->Add(l_strSQLClause);
 try
 {
  dmLendDM->daqLog->ExecSQL();
  }
 catch(...)
 {
  Application->MessageBox(L"Failed update table lib_Log",UnicodeString(CLdSystemData->strCaption).c_str(),MB_OK|MB_ICONINFORMATION);
  return false;
 }
 return true;
}
