//---------------------------------------------------------------------------
#pragma once
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "LoginFrm.h"
#include "StoryData.h"
#include "dmData.h"
#include "MaterialData.h"
#include "UserData.h"
#include "LogData.h"
#include "utGlobDm.h"
#include "InitForm.h"

#include "frm_Main.h"
//#include "InitForm.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmLogin *frmLogin;
//extern AnsiString strCurrentUserID;
//extern int nGroupType;
//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormShow(TObject *Sender)
{
   TIniFile* IniFile=new TIniFile(IniName());

   edtLoginID->Text=IniFile->ReadString("Users","LoginID","");
   if (edtLoginID->Text!="") edtPassword->SetFocus();

   strLoginID=IniFile->ReadString("Database Connection","UserName","U_TDvn[l|cx");
   strPassword=IniFile->ReadString("Database Connection","PassWord","dqymbibaeuo{}d");
   strDataBaseName=IniFile->ReadString("Database Connection","DatabaseName","");
   strDataBaseServer=IniFile->ReadString("Database Connection","ServerName","");
   delete IniFile;


   dmdStory->dbMain->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dmdStory->dbMain->Params->Add((AnsiString)"PASSWORD="+strPassword);

   dmdLog->dbMain->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dmdLog->dbMain->Params->Add((AnsiString)"PASSWORD="+strPassword);

//   dmdLog->dbOldData->Params->Add((AnsiString)"USER NAME="+strLoginIDOld);
 //  dmdLog->dbOldData->Params->Add((AnsiString)"PASSWORD="+strPasswordOld);


   dmLendDM->dbMain->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dmLendDM->dbMain->Params->Add((AnsiString)"PASSWORD="+strPassword);

   dmdMaterials->dbMain1->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dmdMaterials->dbMain1->Params->Add((AnsiString)"PASSWORD="+strPassword);

   dmdUserAdmin->dbMain->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dmdUserAdmin->dbMain->Params->Add((AnsiString)"PASSWORD="+strPassword);

   dmTemplate->dbMain->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dmTemplate->dbMain->Params->Add((AnsiString)"PASSWORD="+strPassword);

   /*
   if ((strDataBaseServer.IsEmpty())||(strDataBaseName.IsEmpty()))
		{
		frmInit->ShowModal();
		ModalResult=mrCancel;
		Application->Terminate();
		return;
		}

   nLoginTimes=1;
   DecodeString(strLoginID);
   DecodeString(strPassword);
   dmdMain->dadbLibrary->Params->Add((AnsiString)"DATABASE NAME="+strDataBaseName);
   dmdMain->dadbLibrary->Params->Add((AnsiString)"SERVER NAME="+strDataBaseServer);
   dmdMain->dadbLibrary->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dmdMain->dadbLibrary->Params->Add((AnsiString)"PASSWORD="+strPassword);
   dmdMain->dadbLibrary->Params->Add((AnsiString)"LANGDRIVER=DBWINUS0");
*/
   try
   {
	 dmdStory->dbMain->Open();
	 dmdLog->dbMain->Open();
	   try
	   {
//		 dmdLog->dbOldData->Open();
	   }
	   catch(...)
	   {

       }
	 dmLendDM->dbMain->Open();
	 dmdMaterials->dbMain1->Open();
	 dmdUserAdmin->dbMain->Open();
	 dmTemplate->dbMain->Open();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database can not be connected,please input database information and retry login!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
 //	 frmInit->ShowModal();

	   TIniFile* IniFile=new TIniFile(IniName());
	   frmInit->strODBC="Library";
	   frmInit->strUser=IniFile->ReadString("Database Connection","UserName","");
	   frmInit->strPass=IniFile->ReadString("Database Connection","PassWord","");
	   delete IniFile;
	   frmInit->Caption = "Database Login Information";
		if (frmInit->ShowModal()==mrOk)
		{
		   dmdStory->dbTBSNews->Params->Add((AnsiString)"USER NAME="+frmInit->strUser);
		   dmdStory->dbTBSNews->Params->Add((AnsiString)"PASSWORD="+frmInit->strPass);

			WritePrivateProfileString("Database Connection","UserName",
						AnsiString(frmInit->strUser).c_str(),IniName().c_str());
			WritePrivateProfileString("Database Connection","Password",
						AnsiString(frmInit->strPass).c_str(),IniName().c_str());

		}

		 ModalResult=mrCancel;
		 Application->Terminate();
	   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::btnOkClick(TObject *Sender)
{
   AnsiString strSQLClause;
   AnsiString strPassword;
   int nPrivilege;
   if(nLoginTimes<=3)
   {
	 if (edtLoginID->Text.Trim().IsEmpty())
	 {
		Application->MessageBox(L"Please input UserID.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
		edtLoginID->SetFocus();
		ModalResult=mrNone;
		return;
	 }
	 strSQLClause=(AnsiString)"Select UserID, passwd from dbo.lib_User Where UserID='"
				   +(AnsiString)SQLStr(edtLoginID->Text)+(AnsiString)"'";
	 dmdStory->daqCheck->SQL->Clear();
	 dmdStory->daqCheck->SQL->Add(strSQLClause);
	 try
	 {
	   dmdStory->daqCheck->Open();
	 }
	 catch(...)
	 {
	   Application->MessageBox(L"Database can not be connected!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	   ModalResult=mrCancel;
	   return;
	 }
	 dmdStory->daqCheck->First();
	 if (dmdStory->daqCheck->RecordCount<1)
	 {
	   if (nLoginTimes>=3)
	   {
		 Application->MessageBox(L"Please contact with the administrator!",L"Login Fail",MB_OK|MB_ICONINFORMATION);
		 ModalResult=mrCancel;
		 return;
	   }
	   ModalResult=mrNone;
	   Application->MessageBox(L"It is a illegal UserID!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	   edtLoginID->SetFocus();
	   nLoginTimes++;
	   return;
	 }
	 else
	 {
	   dmdStory->daqCheck->First();
	   strPassword=OrigStr(dmdStory->daqCheck->FieldValues["passwd"]);
	   nPrivilege=GetPrivilege(edtLoginID->Text);
	   if (nPrivilege>=3)
	   {
		  if (nLoginTimes==3)
		  {
			Application->MessageBox(L"Please contact with the administrator!",L"Login Fail",MB_OK|MB_ICONINFORMATION);
			ModalResult=mrCancel;
			return;
		  }
		  ModalResult=mrNone;
		  Application->MessageBox(L"It is a illegal UserID!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		  edtLoginID->SetFocus();
		  nLoginTimes++;
		  return;
	   }

	   if (nPrivilege==-1)
	   {
		  Application->MessageBox(L"Database can not be connected!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		  ModalResult=mrNone;
		  return;
	   }
	   if (nPrivilege>2)
	   {
		  ModalResult=mrNone;
		  Application->MessageBox(L"It is a illegal UserID!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		  edtLoginID->SetFocus();
		  nLoginTimes++;
		  return;
	   }
	   DecodeString(strPassword);
	   if (edtPassword->Text==strPassword)
	   {
		  dmdStory->nGroupType=nPrivilege;
		  dmdStory->strCurrentUserID=edtLoginID->Text;
		  ModalResult=mrOk;
	   }
	   else
	   {
		 if (nLoginTimes==3)
		 {
		   Application->MessageBox(L"Please contact with the administrator!",L"Login Fail",MB_OK|MB_ICONINFORMATION);
		   ModalResult=mrCancel;
		   return;
		 }
		 ModalResult=mrNone;
		 Application->MessageBox(L"Please confirm your password!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		 edtPassword->SetFocus();
		 edtPassword->SelectAll();
		 nLoginTimes++;
		 return;
	   }
     }
   }
   else
   {
	 Application->MessageBox(L"Please contact with the administrator!",L"Login Fail",MB_OK|MB_ICONINFORMATION);
	 ModalResult=mrCancel;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormClose(TObject *Sender, TCloseAction &Action)
{
   if (ModalResult==mrOk)
   {
	 TIniFile* IniFile=new TIniFile(IniName());
	 IniFile->WriteString("Users","LoginID",edtLoginID->Text);
	 delete IniFile;
   }
   else if (ModalResult==mrCancel)
   {
  //	 if (dmdStory->dadbLibrary->Connected)
  //	   dmdStory->dadbLibrary->Close();
   }
}
//---------------------------------------------------------------------------
int TfrmLogin::GetPrivilege(AnsiString strUserID)
{
  AnsiString strSQLClause;
  int nType,nNextType,nMinType;
  strSQLClause=(AnsiString)"Select * from dbo.lib_Group Where GroupID In(Select GroupID from dbo.lib_GroupUser Where UserID='"
					 +(AnsiString)SQLStr(strUserID)+(AnsiString)"')";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
    dmdStory->daqCheck->Open();
  }
  catch(...)
  {
    return -1;
  }
  if (dmdStory->daqCheck->RecordCount<1)
      return 3;
  dmdStory->daqCheck->First();
  nType=dmdStory->daqCheck->FieldByName("GroupType")->AsInteger;
  while (!dmdStory->daqCheck->Eof)
  {
	  dmdStory->daqCheck->Next();
	  nNextType=dmdStory->daqCheck->FieldByName("GroupType")->AsInteger;
	  if (nType<=nNextType)
		  nMinType=nType;
	  else
	  {
		  nMinType=nNextType;
		  nType=nNextType;
	  }
  }
  return nMinType;
}
//---------------------------------------------------------------------------

