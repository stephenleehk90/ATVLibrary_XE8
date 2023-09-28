//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ArchiveThread.h"
#include "ArchiveDialog.h"
#include <inifiles.hpp>
#include "CommFunc.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TThdArchive::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
TThdArchive *thdArchive;
//---------------------------------------------------------------------------
__fastcall TThdArchive::TThdArchive(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TThdArchive::Execute()
{
    //---- Place thread code here ----
  AnsiString strSQLClause;

  try{
    InitDBConnect();

    strSQLClause=(AnsiString)"UpdateStoryLibrary";
	qryArchive->Close();
    qryArchive->SQL->Clear();
    qryArchive->SQL->Add(strSQLClause);

    qryArchive->ExecSQL();

    fArchiveSucceed=true;
  }
  catch(...)
  {
	// failed
	fArchiveSucceed=false;
  }
  DBDisconnect();
  Synchronize(ArchiveFinish);
}
//--------------------------------------------------------------------------
void __fastcall TThdArchive::ArchiveFinish()
{
	frmArchive->tmrIndexInfo->Enabled=false;
	frmArchive->prgIndexInfo->Position=frmArchive->prgIndexInfo->Max;
	if (fArchiveSucceed)
		frmArchive->Label1->Caption="Archive finished successfully !";
	else
		frmArchive->Label1->Caption="There is something wrong with archiving !";
	Screen->Cursor = (TCursor)0;

	frmArchive->btnOK->Enabled=true;
}
//---------------------------------------------------------------------------
bool TThdArchive::InitDBConnect()
{
   TIniFile* IniFile;
   AnsiString strLoginID, strPassword, strDataBaseName,
			strDataBaseServer;

   try{
	 IniFile=new TIniFile(IniName());
	 strLoginID=IniFile->ReadString("Database Connection","UserName","U_TDvn[l|cx");
	 strPassword=IniFile->ReadString("Database Connection","PassWord","dqymbibaeuo{}d");
//	 strDataBaseName=IniFile->ReadString("Database Connection","DatabaseName","");
//	 strDataBaseServer=IniFile->ReadString("Database Connection","ServerName","");
	 delete IniFile;

//	 if ((strDataBaseServer.IsEmpty())||(strDataBaseName.IsEmpty()))
//		return false;
//	 DecodeString(strLoginID);
//	 DecodeString(strPassword);

	 snArchive=new TSession(Application);
	 snArchive->SessionName="ArchiveSession";

	 dbArchive=new TDatabase(Application);
	 dbArchive->SessionName=snArchive->SessionName;
	 dbArchive->LoginPrompt=false;
//	 dbArchive->DriverName="MSSQL";
	 dbArchive->DriverName="";
	 dbArchive->DatabaseName="Library";
   dbArchive->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dbArchive->Params->Add((AnsiString)"PASSWORD="+strPassword);
	 //	 dbArchive->Params->Add((AnsiString)"DATABASE NAME="+strDataBaseName);
//	 dbArchive->Params->Add((AnsiString)"SERVER NAME="+strDataBaseServer);
//	 dbArchive->Params->Add((AnsiString)"USER NAME="+strLoginID);
//	 dbArchive->Params->Add((AnsiString)"PASSWORD="+strPassword);
//	 dbArchive->Params->Add((AnsiString)"LANGDRIVER=DBWINUS0");
	 dbArchive->Open();

	 qryArchive=new TQuery(Application);
	 qryArchive->SessionName=snArchive->SessionName;
	 qryArchive->DatabaseName="Library";
	 qryArchive->ParamCheck=false;
	 }
   catch(...) {
	 return false;
	 }
   return true;
}
//---------------------------------------------------------------------------
void TThdArchive::DBDisconnect()
{
  try{
	delete qryArchive;
	}
  catch(...){}
  try{
	delete dbArchive;
	}
  catch(...){}
  try{
    delete snArchive;
  }
  catch(...) {}
}
//---------------------------------------------------------------------------

