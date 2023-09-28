//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "dir.h"
#include "LogFrm.h"
#include "DirectoryFrm.h"
#include "SaveFrm.h"
#include "QueryFrmLog.h"
#include "SourceTbFrm.h"
#include "SelectFieldFrm.h"
#include "LogData.h"
#include "DataProcess.h"
#include "CommFunc.h"
#include "StoryData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "AdvGrid"
#pragma link "EditBtn"
#pragma link "AdvListV"
#pragma resource "*.dfm"
TfrmLogs *frmLogs;

//---------------------------------------------------------------------------
AnsiString DateOf(AnsiString str)
{
	AnsiString str1;
	str1="19"+str.SubString(7,2)+"-"+str.SubString(1,2)+"-"
		   +str.SubString(4,2);
	return str1;
}
//---------------------------------------------------------------------------
__fastcall TfrmLogs::TfrmLogs(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogs::FormShow(TObject *Sender)
{
  TDateTime time;
  AnsiString strINIFileName;
  AnsiString strSQLClause;
  char szReturned[256];

  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
				 (AnsiString)".ini";
  TIniFile* IniFile=new TIniFile(strINIFileName);
  LogEnd=IniFile->ReadInteger("Log Browse","EndDate",35912);
  LogBegin=IniFile->ReadInteger("Log Browse","BeginDate",35911);
  delete IniFile;
  time=time.CurrentDate();
  LogBegin=LogBegin-LogEnd+time;
  LogEnd=time;
  strOpID="everyone";
  lblLogBegin->Caption=LogBegin.FormatString("yyyy-mm-dd");
  lblLogEnd->Caption=LogEnd.FormatString("yyyy-mm-dd");
  lblOpID->Caption=strOpID;
  strSQLClause=(AnsiString)"select LogKeepDays from dbo.lib_SysConfig";
  dmdLog->daqLogs->SQL->Clear();
  dmdLog->daqLogs->SQL->Add(strSQLClause);
  try
  {
	dmdLog->daqLogs->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database Error",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	nSaveDays=30;
	return;
  }
  nSaveDays=dmdLog->daqLogs->FieldByName("LogKeepDays")->AsInteger;
//  nbkDatabase->PageIndex=1;
//  RefreshLogs(LogBegin,LogEnd,strOpID);

  //�ӳ�ʼ���ļ��ж�ȡ��ǰ���ݿ���
  strINIFileName = IniName();

  GetPrivateProfileString("Database Connection","DatabaseName","ERROR",
						  szReturned,256,strINIFileName.c_str());

  if (strcmp(szReturned,"ERROR") == 0)
	  m_strDatabaseName = "";
  else
	  m_strDatabaseName = (AnsiString)szReturned;

  lblDBName->Caption = m_strDatabaseName;
  try
  {
		//��ȡ������
		strSQLClause = "select CodeType from lib_SysConfig";

		//ִ��SQL���
		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->Open();

		if (dmdLog->qeGlobal->RecordCount <= 0)
			m_strCodeType = LANGUAGE_BIG5;
		else
		{
			if (dmdLog->qeGlobal->FieldValues["CodeType"].IsNull())
				m_strCodeType = LANGUAGE_BIG5;
			else
				m_strCodeType = OrigStr(dmdLog->qeGlobal->FieldValues["CodeType"]);
		}
  }
  catch(...)
  {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		m_strNodeType = LANGUAGE_BIG5;
  }

	RefreshLogs(LogBegin,LogEnd,strOpID);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogs::muiLogQueryClick(TObject *Sender)
{
/*  TDateTime time;
  AnsiString strINIFileName;

  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
				 (AnsiString)".ini";

  TIniFile* IniFile=new TIniFile(strINIFileName);
  LogEnd=IniFile->ReadInteger("Log Browse","EndDate",35912);
  LogBegin=IniFile->ReadInteger("Log Browse","BeginDate",35911);
  delete IniFile;
  time=time.CurrentDate();
  LogBegin=LogBegin-LogEnd+time+1;
  LogEnd=time+1;
*/
 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
  LogBegin = StrToDateTime(lblLogBegin->Caption, FS );
  LogEnd = StrToDateTime(lblLogEnd->Caption, FS );
 strOpID = lblOpID->Caption;

  if (frmQueryLog->ShowModal()==mrOk)
  {

 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
 strOpID = lblOpID->Caption;
  LogBegin = StrToDateTime(lblLogBegin->Caption, FS );
  LogEnd = StrToDateTime(lblLogEnd->Caption, FS );

	  RefreshLogs(LogBegin,LogEnd,strOpID);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogs::muiExpireClick(TObject *Sender)
{
  frmSave->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TfrmLogs::muiClearLogClick(TObject *Sender)
{
  LogDelete(nSaveDays);
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogs::FormDestroy(TObject *Sender)
{
 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
 strOpID = lblOpID->Caption;
  LogBegin = StrToDateTime(lblLogBegin->Caption, FS );
  LogEnd = StrToDateTime(lblLogEnd->Caption, FS );


  AnsiString strINIFileName;
  AnsiString strSQLClause;
  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
					 (AnsiString)".ini";
  TIniFile* IniFile=new TIniFile(strINIFileName);
  IniFile->WriteInteger("Log Browse","EndDate",(int)LogEnd);
  IniFile->WriteInteger("Log Browse","BeginDate",(int)LogBegin);
  delete IniFile;
  strSQLClause=(AnsiString)"update dbo.lib_SysConfig set LogKeepDays="
				+(AnsiString)nSaveDays;
  dmdLog->daqLogs->SQL->Clear();
  dmdLog->daqLogs->SQL->Add(strSQLClause);
  try
  {
	dmdLog->daqLogs->ExecSQL();
  }
  catch(...)
  {
	Application->MessageBox(L"Database Error",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  }
}
//---------------------------------------------------------------------------




void __fastcall TfrmLogs::WndProc(TMessage &Message)
{
  if ( (Message.Msg!=WM_SYSCOMMAND)||
	((Message.WParam!=SC_NEXTWINDOW)&&(Message.WParam!=SC_PREVWINDOW)) )
   TForm::WndProc(Message);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogs::btnDestTbsMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	RECT r1,r2;
	GetWindowRect(btnDestTbs->Handle, &r1);
	GetWindowRect(Handle, &r2);
	frmDirectory->Left=r1.left;
	frmDirectory->Top=r1.bottom+1;
	frmDirectory->Width=200;
    frmDirectory->Height=r2.bottom-r1.bottom;

	m_nDataType = 1;

	frmDirectory->Show();
	frmDirectory->InitForm();
	frmDirectory->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogs::btnSourceTbsMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	AnsiString strPath,strTableName;
    RECT r1,r2;

    m_nDataType = 2;

	if (chkOld->Checked)
    {
        if (frmSourceTbs->ShowModal() == mrOk)
        {
            try
            {
				if (dmdLog->dbOldData->Connected)
					dmdLog->dbOldData->Close();

				if ((frmSourceTbs->cbbSourceTable->Items->Count <= 0)||
					(frmSourceTbs->cbbSourceTable->ItemIndex < 0))
					 return;

				strPath = frmSourceTbs->edSourcePath->Text.Trim();
				strTableName =
					frmSourceTbs->cbbSourceTable->Items->Strings[frmSourceTbs->cbbSourceTable->ItemIndex];

				edtSourceTb->Text = strPath + "\\" + strTableName;

				dmdLog->dbOldData->Params->Delete(dmdLog->dbOldData->Params->Count-1);
				dmdLog->dbOldData->Params->Add("Path="+strPath);

				dmdLog->tbOldData->TableName = strTableName;

				dmdLog->dbOldData->Open();

				dmdLog->tbOldData->Open();

			}
			catch(...)
			{
				Application->MessageBox(L"Open DBF File Failed!",GLOBALMSGCAPTION,48);

				if (dmdLog->dbOldData->Connected)
					dmdLog->dbOldData->Close();

				return;
			}
		}
	}
	else
	{
	  GetWindowRect(btnSourceTbs->Handle, &r1);
	  GetWindowRect(Handle, &r2);
	  frmDirectory->Left=r1.left-200;
	  frmDirectory->Top=r1.bottom+1;
	  frmDirectory->Width=200;
	  frmDirectory->Height=r2.bottom-r1.bottom;
	  frmDirectory->Show();
	   frmDirectory->InitForm();
	  frmDirectory->BringToFront();
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmLogs::btnTransferClick(TObject *Sender)
{
	AnsiString strNodeID,strSourceNode,strDestNode,strWriteTime;
	AnsiString strSourceFieldName,strDestFieldName,strFieldType;
	AnsiString strSourceNodeID,strSQLClause,strValues;
	TBlobStream * bsSourceStream = NULL,* bsDestStream = NULL;
	char * pBuffer = NULL;
	TLocateOptions LocateOption;
	bool bSucceed;
	int i;

	strSourceNode = edtSourceTb->Text.Trim();
	if (strSourceNode.IsEmpty())
	{
		Application->MessageBox(L"You should specify a source table!",GLOBALMSGCAPTION,48);
		return;
	}

	strDestNode = edtDestTb->Text.Trim();
	if (strDestNode.IsEmpty())
	{
		Application->MessageBox(L"You should specify a target table!",GLOBALMSGCAPTION,48);
		return;
	}

	if (lstvwFields->Items->Count <= 0)
	{
		Application->MessageBox(L"No fields associated!",GLOBALMSGCAPTION,48);
		return;
	}

	try
	{
		//���Ŀ�����ݱ��Ƿ����
		strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_new"+
					   strDestNode+
					   (AnsiString)"')";

		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->Open();

		if (dmdLog->qeGlobal->RecordCount <= 0)
		{
			Application->MessageBox(L"Target table does not exist!",GLOBALMSGCAPTION,48);
			return;
		}

		//����Ŀ�����ݱ����չ�ֶ�
		strSQLClause = (AnsiString)"select * from "+
					   (AnsiString)strLIBEXTENDFIELDS+
					   (AnsiString)" where CategoryID = '"+
					   SQLStr(strDestNode)+
					   (AnsiString)"'";

		dmdLog->qeExtFields->SQL->Clear();
		dmdLog->qeExtFields->SQL->Add(strSQLClause);
		dmdLog->qeExtFields->Open();

		if (! chkOld->Checked)
		{
			//���Դ���ݱ��Ƿ����
			strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_old"+
						   strSourceNode+
						   (AnsiString)"')";

			dmdLog->qeGlobal->SQL->Clear();
			dmdLog->qeGlobal->SQL->Add(strSQLClause);
			dmdLog->qeGlobal->Open();

			if (dmdLog->qeGlobal->RecordCount <= 0)
			{
				Application->MessageBox(L"Source table does not exist!",GLOBALMSGCAPTION,48);
				return;
			}

			dmdLog->tbSource->TableName = (AnsiString)"lib_old"+strSourceNode;
			dmdLog->tbDest->TableName   = (AnsiString)"lib_new"+strDestNode;

			if (! dmdLog->tbSource->Active)
				dmdLog->tbSource->Open();

			if (! dmdLog->tbDest->Active)
				dmdLog->tbDest->Open();

			//ȷ��Locate�����Ĳ���
			LocateOption << loCaseInsensitive;

			dmdLog->tbSource->First();
			while (! dmdLog->tbSource->Eof)
			{
				//�����¼�¼
				dmdLog->tbDest->Append();

				//�Զ�����StoryID
				strSQLClause = "GetNewFileID";
				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);
				dmdLog->qeGlobal->Open();

				dmdLog->qeGlobal->First();
				strNodeID = dmdLog->qeGlobal->FieldValues["FileID"];

				dmdLog->tbDest->FieldValues[FIELDSTORYID] = SQLStr(strNodeID);

				bSucceed = false;

				for (i=0;i<lstvwFields->Items->Count;i++)
				{
					//��ȡĿ���ֶ�
					strDestFieldName = lstvwFields->Items->Item[i]->Caption.Trim();

					if (strDestFieldName.IsEmpty())
						continue;
					//��ȡ���Ӧ��Դ�ֶ�
					if (lstvwFields->Items->Item[i]->SubItems->Count <= 0)
					{
						if (strDestFieldName=="WriteTime")
						{
							strSQLClause = "select GetDate()";
							dmdLog->qeGlobal->SQL->Clear();
							dmdLog->qeGlobal->SQL->Add(strSQLClause);
							dmdLog->qeGlobal->Open();

							dmdLog->qeGlobal->First();
							strWriteTime = dmdLog->qeGlobal->FieldValues["COLUMN1"];
							dmdLog->tbDest->FieldValues[strDestFieldName] = strWriteTime;
						}
						continue;
					}
					strSourceFieldName = lstvwFields->Items->Item[i]->SubItems->Strings[0].Trim();

					if (strSourceFieldName.IsEmpty())
					{
						if (strDestFieldName == FIELDWRITETIME)
						//�Զ�����WriteTime
						{
							strSQLClause = "select GetDate()";
							dmdLog->qeGlobal->SQL->Clear();
							dmdLog->qeGlobal->SQL->Add(strSQLClause);
							dmdLog->qeGlobal->Open();

							dmdLog->qeGlobal->First();
							strWriteTime = dmdLog->qeGlobal->FieldValues["COLUMN1"];

							dmdLog->tbDest->FieldValues[strDestFieldName] = strWriteTime;
						}

						continue;
					}

					strFieldType = "";

					//ȷ���ֶε�����
					if (dmdLog->qeBaseFields->Locate("FieldName",SQLStr(strSourceFieldName),LocateOption))
						strFieldType = OrigStr(dmdLog->qeBaseFields->FieldValues["FieldType"]);
					else
					{
						if (dmdLog->qeExtFields->Locate("FieldName",SQLStr(strSourceFieldName),LocateOption))
							strFieldType = OrigStr(dmdLog->qeExtFields->FieldValues["FieldType"]);
					}

					if (strFieldType.IsEmpty())
						continue;

					if (strFieldType == typeTEXT)
					//��дText�ֶ�
					{
	/*
						  //�����Ŀ�����ݱ��Text�ֶ�
						  dmdLog->tbDest->Edit();
						  bsDestStream = new TBlobStream((TBlobField*)dmdLog->tbDest->FieldByName(strDestFieldName),bmReadWrite);
						  bsDestStream->Seek(0,soFromBeginning);
						  bsDestStream->Truncate();
						  dmdLog->tbDest->Post();
	*/

						  //����Դ���ݱ��Textֵ
						  bsSourceStream = new TBlobStream((TBlobField*)dmdLog->tbSource->FieldByName(strSourceFieldName),bmRead);
						  pBuffer = new char[bsSourceStream->Size];
						  bsSourceStream->Read(pBuffer,bsSourceStream->Size);

						  //д��Ŀ�����ݱ�
						  bsDestStream = new TBlobStream((TBlobField*)dmdLog->tbDest->FieldByName(strDestFieldName),bmReadWrite);
						  bsDestStream->Write(pBuffer,bsSourceStream->Size);

						  //�ͷſռ�
						  delete bsSourceStream;
						  bsSourceStream = NULL;
						  delete bsDestStream;
						  bsDestStream = NULL;
						  delete pBuffer;
						  pBuffer = NULL;
					}
					else
					//�������͵��ֶ�
					{
						if (dmdLog->tbSource->FieldValues[strSourceFieldName].IsNull())
							strValues = "";
						else
							strValues = dmdLog->tbSource->FieldValues[strSourceFieldName];

						dmdLog->tbDest->FieldValues[strDestFieldName] = strValues;
					}

					bSucceed = true;
				}

				if (! bSucceed)
					dmdLog->tbDest->Delete();
				else
					dmdLog->tbDest->Post();

				dmdLog->tbSource->Next();
			}

			if (dmdLog->tbSource->Active)
				dmdLog->tbSource->Close();

			if (dmdLog->tbDest->Active)
				dmdLog->tbDest->Close();
		}
		else
		{
			if (! dmdLog->dbOldData->Connected)
				dmdLog->dbOldData->Open();

			if (! dmdLog->tbOldData->Active)
				dmdLog->tbOldData->Open();

			dmdLog->tbDest->TableName   = (AnsiString)"lib_new"+strDestNode;

			if (! dmdLog->tbDest->Active)
				dmdLog->tbDest->Open();

			//ȷ��Locate�����Ĳ���
			LocateOption << loCaseInsensitive;

			dmdLog->tbOldData->First();
			while (! dmdLog->tbOldData->Eof)
			{
				//�����¼�¼
				dmdLog->tbDest->Append();

				//�Զ�����StoryID
				strSQLClause = "GetNewFileID";
				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);
				dmdLog->qeGlobal->Open();

				dmdLog->qeGlobal->First();
				strNodeID = dmdLog->qeGlobal->FieldValues["FileID"];

				dmdLog->tbDest->FieldValues[FIELDSTORYID] = SQLStr(strNodeID);

				bSucceed = false;

				for (i=0;i<lstvwFields->Items->Count;i++)
				{
					//��ȡĿ���ֶ�
					strDestFieldName = lstvwFields->Items->Item[i]->Caption.Trim();

					if (strDestFieldName.IsEmpty())
						continue;

					//��ȡ���Ӧ��Դ�ֶ�
					if (lstvwFields->Items->Item[i]->SubItems->Count <= 0)
					{
						if (strDestFieldName=="WriteTime")
						{
							strSQLClause = "select GetDate()";
							dmdLog->qeGlobal->SQL->Clear();
							dmdLog->qeGlobal->SQL->Add(strSQLClause);
							dmdLog->qeGlobal->Open();

							dmdLog->qeGlobal->First();
							strWriteTime = dmdLog->qeGlobal->FieldValues["COLUMN1"];
							dmdLog->tbDest->FieldValues[strDestFieldName] = strWriteTime;
						}

						continue;
					}

					strSourceFieldName = lstvwFields->Items->Item[i]->SubItems->Strings[0].Trim();
					if (strSourceFieldName.IsEmpty())
					{
						if (strDestFieldName == FIELDWRITETIME)
						//�Զ�����WriteTime
						{
							strSQLClause = "select GetDate()";
							dmdLog->qeGlobal->SQL->Clear();
							dmdLog->qeGlobal->SQL->Add(strSQLClause);
							dmdLog->qeGlobal->Open();

							dmdLog->qeGlobal->First();
							strWriteTime = dmdLog->qeGlobal->FieldValues["Column1"];

							dmdLog->tbDest->FieldValues[strDestFieldName] = strWriteTime;
						}

						continue;
					}

					strFieldType = "";

					//ȷ���ֶε�����
					if (dmdLog->qeBaseFields->Locate("FieldName",SQLStr(strDestFieldName),LocateOption))
						strFieldType = OrigStr(dmdLog->qeBaseFields->FieldValues["FieldType"]);
					else
					{
						if (dmdLog->qeExtFields->Locate("FieldName",SQLStr(strDestFieldName),LocateOption))
							strFieldType = OrigStr(dmdLog->qeExtFields->FieldValues["FieldType"]);
					}

					if (strFieldType.IsEmpty())
						continue;

					if (dmdLog->tbOldData->FieldValues[strSourceFieldName].IsNull())
						strValues = "";
					else
						strValues = dmdLog->tbOldData->FieldByName(strSourceFieldName)->AsString;

					if (strSourceFieldName.AnsiPos("DATE")!=0)
					{
					try{
						dmdLog->tbDest->FieldValues[strDestFieldName]=dmdLog->tbOldData->FieldValues[strSourceFieldName];
						}
					catch(...)
					   {
						 try{
						   dmdLog->tbDest->FieldValues[strDestFieldName] = DateOf(strValues);
						 }
						 catch(...){}
					   }
					}
					else
					{
						dmdLog->tbDest->FieldByName(strDestFieldName)->AsString = SQLStr(strValues);
					}

					bSucceed = true;
				}

				if (! bSucceed)
					dmdLog->tbDest->Delete();
				else
					dmdLog->tbDest->Post();

				dmdLog->tbOldData->Next();
			}

			if (dmdLog->dbOldData->Connected)
				dmdLog->dbOldData->Close();

			if (dmdLog->tbDest->Active)
				dmdLog->tbDest->Close();
		}

		Application->MessageBox(L"Transfer is sucessfully finished!",GLOBALMSGCAPTION,MB_OK|MB_ICONINFORMATION);
	}
	catch(...)
	{
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);

		if (dmdLog->tbSource->Active)
			dmdLog->tbSource->Close();

		if (dmdLog->tbDest->Active)
			dmdLog->tbDest->Close();

		if (dmdLog->dbOldData->Connected)
			dmdLog->dbOldData->Close();

		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogs::MenuItem1Click(TObject *Sender)
{
	AnsiString strAssociatedFieldName;

	if (lstvwFields->Items->Count <= 0)
		return;

	if (! lstvwFields->Selected)
	{
		Application->MessageBox(L"You should specify a field first!",GLOBALMSGCAPTION,48);
		return;
	}

	if (edtDestTb->Text.Trim().IsEmpty())
	{
		Application->MessageBox(L"You should specify target table!",GLOBALMSGCAPTION,48);
		btnDestTbs->SetFocus();
		return;
	}

    if (edtSourceTb->Text.Trim().IsEmpty())
    {
		Application->MessageBox(L"You should specify source table!",GLOBALMSGCAPTION,48);
        btnSourceTbs->SetFocus();
        return;
    }

	if (frmSelectFields->ShowModal() == mrOk)
    {
		if (frmSelectFields->lstvwFields->Items->Count > 0)
		{
			strAssociatedFieldName = frmSelectFields->lstvwFields->Selected->Caption;

			if (lstvwFields->Selected->SubItems->Count == 0)
			{
				lstvwFields->Selected->SubItems->Add(strAssociatedFieldName);
			}
			else
			{
				lstvwFields->Selected->SubItems->Strings[0] = strAssociatedFieldName;
			}

			if (! btnTransfer->Enabled)
				btnTransfer->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogs::MenuItem2Click(TObject *Sender)
{
	AnsiString strAssociatedFieldName;

	if (lstvwFields->Items->Count <= 0)
		return;

	if (! lstvwFields->Selected)
	{
		Application->MessageBox(L"You should specify a field first!",GLOBALMSGCAPTION,48);
		return;
	}

	lstvwFields->Selected->SubItems->Text = "";
	lstvwFields->Selected->SubItems->Clear();
}
//---------------------------------------------------------------------------
void TfrmLogs::UpdateStoryLibrary()
{
/*
	AnsiString strSQLClause,strSubSQLClause,strNodeID;

	if (m_strDatabaseName.IsEmpty())
	{
		Application->MessageBox("No database name in INI file!",GLOBALMSGCAPTION,48);
		return;
	}

	//ɾ��ԭ���Ĵ洢����
	strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id(\'"+
				   UPDATESTORYPROC+
				   (AnsiString)"\') and sysstat & 0xf = 4) drop procedure "+
				   UPDATESTORYPROC;

	try
	{
		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->ExecSQL();

		//�������е�Ҷ�ڵ�
		strSQLClause = (AnsiString)"select NodeID from "+
					   strSTORYLIBTREE+
					   " where NodeAttr = 1";

		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->Open();

		dmdLog->qeGlobal->First();
		while (! dmdLog->qeGlobal->Eof)
		{
			strNodeID = OrigStr(dmdLog->qeGlobal->FieldValues["NodeID"]);

			//ȷ��Ҷ�ڵ����������������ݱ��Ƿ����
			strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_old"+
						   strNodeID+
						   "')";

			qeInternal->SQL->Clear();
			qeInternal->SQL->Add(strSQLClause);
			qeInternal->Open();

			if (qeInternal->RecordCount <= 0)
			//������Old������֮
			{
				dmdLog->qeGlobal->Next();
				continue;
			}

			strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_new"+
						   strNodeID+
						   "')";

			qeInternal->SQL->Clear();
			qeInternal->SQL->Add(strSQLClause);
			qeInternal->Open();

			if (qeInternal->RecordCount <= 0)
			//������Old������֮
			{
				dmdLog->qeGlobal->Next();
				continue;
			}

			strSubSQLClause =  strSubSQLClause+
							   "BEGIN TRANSACTION "+
							   "INSERT dbo.lib_old"+
							   strNodeID+
							   " SELECT * FROM dbo.lib_new"+
							   strNodeID+
							   " HOLDLOCK "+
							   "DELETE dbo.lib_new"+
							   strNodeID+
							   " COMMIT TRANSACTION "+
							   " exec master..xpUpdateTextIndex '"+
							   m_strDatabaseName+
							   ".dbo.lib_old"+
							   strNodeID+
							   "' ";

			dmdLog->qeGlobal->Next();
		}

		if (! strSubSQLClause.IsEmpty())
		{
			strSQLClause = (AnsiString)"CREATE PROCEDURE "+
						   UPDATESTORYPROC+
						   " AS ";

			strSQLClause = strSQLClause + strSubSQLClause;

			dmdLog->qeGlobal->SQL->Clear();
			dmdLog->qeGlobal->SQL->Add(strSQLClause);
			dmdLog->qeGlobal->ExecSQL();
		}
	}
	catch(...)
	{
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		return;
	}
*/
	AnsiString strSQLClause,strSubSQLClause,strNodeID;
	int        i,nCurrentYear;
	AnsiString strFieldName;

	m_strDatabaseName = lblDBName->Caption;
	if (m_strDatabaseName.IsEmpty())
	{
		Application->MessageBox(L"Database Name Missing!",GLOBALMSGCAPTION,48);
		return;
	}

	try
	{
		//��ѯ����������ʾ�����
		strSQLClause = "select year = datepart(yy,getdate())";

		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->Open();

		dmdLog->qeGlobal->First();
		nCurrentYear = dmdLog->qeGlobal->FieldValues["year"];

		//ɾ��ԭ���Ĵ洢����
		strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id(\'"+
					   UPDATESTORYPROC+
					   (AnsiString)"\') and sysstat & 0xf = 4) drop procedure "+
					   UPDATESTORYPROC;

		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->ExecSQL();

		//�������е�Ҷ�ڵ�
		strSQLClause = (AnsiString)"select NodeID from "+
					   strSTORYLIBTREE+
					   " where NodeAttr = 1";

		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->Open();

		dmdLog->qeGlobal->First();
		while (! dmdLog->qeGlobal->Eof)
		{
			strNodeID = OrigStr(dmdLog->qeGlobal->FieldValues["NodeID"]);

			//ȷ��Ҷ�ڵ����������������ݱ��Ƿ����
			strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_old"+
						   strNodeID+
						   "')";

			qeInternal->SQL->Clear();
			qeInternal->SQL->Add(strSQLClause);
			qeInternal->Open();

			if (qeInternal->RecordCount <= 0)
			//������Old������֮
			{
				dmdLog->qeGlobal->Next();
				continue;
			}

			strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_new"+
						   strNodeID+
						   "')";

			qeInternal->SQL->Clear();
			qeInternal->SQL->Add(strSQLClause);
			qeInternal->Open();

			if (qeInternal->RecordCount <= 0)
			//������Old������֮
			{
				dmdLog->qeGlobal->Next();
				continue;
			}

			for (i=1990;i<=nCurrentYear+1;i++)
			{
				strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_"+
							   String(i)+
							   "_"+
							   strNodeID+
							   "')";

				qeInternal->SQL->Clear();
				qeInternal->SQL->Add(strSQLClause);
				qeInternal->Open();

				if (qeInternal->RecordCount <= 0)
				//��������ݱ�����֮
				{
					dmdLog->qeGlobal->Next();
					continue;
				}
			}
			strSQLClause =  (AnsiString)"(select FieldName from "+
							strLIBBASEFIELDS+
							") union (select FieldName from "
							strLIBEXTENDFIELDS+
							" where CategoryID = '"+
							SQLStr(strNodeID)+
							"')";
			dmdLog->qeSourceFields->SQL->Clear();
			dmdLog->qeSourceFields->SQL->Add(strSQLClause);
			dmdLog->qeSourceFields->Open();

			strFieldName = "";
			while (!dmdLog->qeSourceFields->Eof)
			{
				strFieldName += dmdLog->qeSourceFields->FieldByName("FieldName")->AsString + ",";
				dmdLog->qeSourceFields->Next();
			}
			strFieldName = strFieldName.SubString(1, strFieldName.Length() - 1);
			strSubSQLClause =  strSubSQLClause+
							   "BEGIN TRANSACTION\n"+
							   "INSERT dbo.lib_old"+
							   strNodeID+
							   " SELECT " +
							   strFieldName +
							   " FROM dbo.lib_new"+
							   strNodeID+
							   " where datepart(yy,Date) < 1990"+
							   "\n"+//HOLDLOCK"+
							   "DELETE dbo.lib_new"+
							   strNodeID+
							   " where datepart(yy,Date) < 1990"+
							   "\n"+
							   "COMMIT TRANSACTION\n"+
							   "exec master..xpUpdateTextIndex '"+
							   m_strDatabaseName+
							   ".dbo.lib_old"+
							   strNodeID+
							   "'\n";

			for (i=1990;i<=nCurrentYear+1;i++)
			{
			  dmdStory->daqCheckTable->Close();
			  dmdStory->daqCheckTable->SQL->Clear();
			  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
								   String(i)+ "_"+ strNodeID+ "')");
			  dmdStory->daqCheckTable->Open();
			  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

				strSubSQLClause = strSubSQLClause+
								 "BEGIN TRANSACTION\n"+
								 "INSERT dbo.lib_"+
								 String(i)+
								 "_"+
								 strNodeID+
								 " SELECT " +
								 strFieldName +
								 " FROM dbo.lib_new"+
								 strNodeID+
								 " where datepart(yy,Date) = "+
								 String(i)+
								 "\n"+// HOLDLOCK"+
								 "DELETE dbo.lib_new"+
								 strNodeID+
								 " where datepart(yy,Date) = "+
								 String(i)+
								 "\n"+
								 "COMMIT TRANSACTION\n"+
								 "exec master..xpUpdateTextIndex '"+
								 m_strDatabaseName+
								 ".dbo.lib_"+
								 String(i)+
								 "_"+
								 strNodeID+
								 "'\n";
			}

			strSubSQLClause = strSubSQLClause+
//                             "DELETE dbo.lib_new"+
//                             strNodeID+
							 "\n\n";

			dmdLog->qeGlobal->Next();
		}

		if (! strSubSQLClause.IsEmpty())
		{
			strSQLClause = (AnsiString)"CREATE PROCEDURE "+
						   UPDATESTORYPROC+
						   " AS\n";

			strSQLClause = strSQLClause + strSubSQLClause;

			dmdLog->qeGlobal->SQL->Clear();
			dmdLog->qeGlobal->SQL->Add(strSQLClause);
			dmdLog->qeGlobal->ExecSQL();
		}
	}
	catch(...)
	{
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		return;
	}
}
//---------------------------------------------------------------------------
void TfrmLogs::CreateTable(AnsiString strNodeID,AnsiString strTableType)
{
	m_strDatabaseName = lblDBName->Caption;

	AnsiString strSQLClause,strSubSQLClause,strFieldType;

	strSQLClause = (AnsiString)"(select FieldName,FieldType,FieldSize from "+
				   strLIBBASEFIELDS+
				   ") union (select FieldName,FieldType,FieldSize from "+
				   strLIBEXTENDFIELDS+
				   " where CategoryID = '"+
				   SQLStr(strNodeID)+
				   "'";

	dmdLog->qeGlobal->SQL->Clear();
	dmdLog->qeGlobal->SQL->Add(strSQLClause);
	dmdLog->qeGlobal->Open();

	strSQLClause = "";
	strSQLClause += "id int NOT NULL IDENTITY (1, 1),";
	dmdLog->qeGlobal->First();
	while (! dmdLog->qeGlobal->Eof)
	{
	  strSQLClause = strSQLClause+
					 dmdLog->qeGlobal->FieldValues["FieldName"]+
					 (AnsiString)" ";

	  strFieldType = dmdLog->qeGlobal->FieldValues["FieldType"];

	  if (strFieldType == (AnsiString)typeVARCHAR)
	  {
		  strSQLClause = strSQLClause +
						 strFieldType+
						 (AnsiString)" ("+
						 String((int)dmdLog->qeGlobal->FieldValues["FieldSize"])+
						 (AnsiString)") NULL,";
	  }
	  else
	  {
		  strSQLClause = strSQLClause +
						 strFieldType+
						 (AnsiString)" NULL,";
	  }

	  dmdLog->qeGlobal->Next();
	}

	if (! strSQLClause.IsEmpty())
	{
		//ָ������
		strSQLClause = strSQLClause+
					   "CONSTRAINT PK_lib_"+
					   strTableType+
					   strNodeID+
					   "_1__10 "+
					   "PRIMARY KEY CLUSTERED("+
					   FIELDSTORYID+
					   ")";

		strSQLClause = (AnsiString)"CREATE TABLE lib_"+
					   strTableType+
					   strNodeID+
					   "("+
					   strSQLClause+
					   ")";

		//ִ��SQL���
		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->ExecSQL();

		//����������
		strSQLClause = "CREATE INDEX NewsIndex1 ON dbo.lib_"+
					   strTableType+
					   strNodeID+
					   "("+
					   FIELDSTORYID+
					   ")";

		//ִ��SQL���
		dmdLog->qeGlobal->SQL->Clear();
		dmdLog->qeGlobal->SQL->Add(strSQLClause);
		dmdLog->qeGlobal->ExecSQL();

		//��������������й�����
		if (strTableType == "old")
		{
			if (! m_strDatabaseName.IsEmpty())
			{
				strSQLClause = strSubSQLClause;

				if (! strSQLClause.IsEmpty())
				{
					//ɾ��ĩβ��","
					strSQLClause.Delete(strSQLClause.Length(),1);

					strSQLClause = (AnsiString)"master..xpCreateTextIndex 'lib_old"+
								   strNodeID+
								   "','"+
								   m_strCodeType+
								   "','"+
								   m_strDatabaseName+
								   ".dbo.lib_old"+
								   strNodeID+
								   "',"+
								   strSQLClause;

					//ִ��SQL���
					dmdLog->qeGlobal->SQL->Clear();
					dmdLog->qeGlobal->SQL->Add(strSQLClause);
					dmdLog->qeGlobal->ExecSQL();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogs::chkOldClick(TObject *Sender)
{
	int i;

	edtSourceTb->Clear();
	btnTransfer->Enabled = false;

	for (i=0;i<lstvwFields->Items->Count;i++)
	{
		lstvwFields->Items->Item[i]->SubItems->Text = "";
		lstvwFields->Items->Item[i]->SubItems->Clear();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogs::btnPerformClick(TObject *Sender)
{
	AnsiString strSQLClause,strThisNodeID,strParentID,strCurrentNodeID;
	AnsiString strBaseFieldsSQLClause,strExtFieldsSQLClause;
	AnsiString strBaseFieldsIndexClause,strExtFieldsIndexClause;
	AnsiString strFieldName,strFieldType;
	AnsiString strPreSQLClause,strWarning,strErrMsg;
	AnsiString strSourceFieldName,strSourceFieldType;
	AnsiString strDestFieldName,strDestFieldType;
	TLocateOptions LocateOption;
	TStringList * slNodesList,* slNodesAccessedList,* slLeafNodesID;
	int nSourceFieldSize,nDestFieldSize;
	int i,nPos,nInitialRecNo,nNodeAttr,nAssociatingFields,nCurrentYear;
	bool bGenLibError,bUpdateStoryLibrary;

	LocateOption << loCaseInsensitive;

	strErrMsg = "Integrity Check Failed !";

	slNodesList         = NULL;
	slNodesAccessedList = NULL;
	slLeafNodesID       = NULL;
 	m_strDatabaseName = lblDBName->Caption;

	if ((!ckbClearEventLog->Checked)&&(!ckbCheckIntegrity->Checked))
	{
	   Application->MessageBox(L"You must specify the operation to perform.",GLOBALMSGCAPTION,48);
	   return;
	}
	try
	{
		if (Application->MessageBox(L"Perform the specified operation ?",GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION)
					== IDOK)
		{
			if (ckbClearEventLog->Checked)
			//���������־�Ĵ洢����
			{
				strSQLClause = "ClearLog";

				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);

				try
				{
					dmdLog->qeGlobal->ExecSQL();
				}
				catch(...)
				{
					strErrMsg = "Clear Event Log Failed !";
					throw;
				}
			}

			if (ckbCheckIntegrity->Checked)
			//�������Ҷ�ڵ��һ����
			{
				try
				{
					if (! dmdLog->tbCategory->Active)
						dmdLog->tbCategory->Open();
				}
				catch(...)
				{
					strErrMsg = "Failed to open story lib tree database !";
					throw;
				}

				//**************************
				//�ȼ��Ŀ¼�������һ����
				//�ж��Ƿ���ڹ����㡢����
				//**************************

				try
				{
					slNodesList         = new TStringList;
					slNodesAccessedList = new TStringList;

					dmdLog->tbCategory->First();

					//˳��������ݿ�����м�¼
					while (! dmdLog->tbCategory->Eof)
					{
					  strThisNodeID =
						  OrigStr(dmdLog->tbCategory->FieldValues["NodeID"]);

					  //��Ŀ¼�����ݱ��в����Ƿ����ͬ���Ľڵ�
					  strSQLClause = (AnsiString)"select NodeID,ParentID from "+
									 strSTORYLIBTREE+
									 " where NodeID = '"+
									 SQLStr(strThisNodeID)+
									 "'";

					  dmdLog->qeGlobal->SQL->Clear();
					  dmdLog->qeGlobal->SQL->Add(strSQLClause);
					  dmdLog->qeGlobal->Open();

					  if (dmdLog->qeGlobal->RecordCount > 1)
					  //������ͬ��ID����򵥵�ɾ����ǰ�ڵ�
					  {
						  dmdLog->tbCategory->Delete();

						  continue;
					  }

					  if (slNodesAccessedList->IndexOf(strThisNodeID) >= 0)
						  //����ýڵ��Ѿ��������У���ֱ�������ýڵ�
					  {
						  dmdLog->tbCategory->Next();
						  continue;
					  }

					  //dmdLog->qeGlobal�д�ŵ�ǰ�����ļ�¼
					  if (dmdLog->qeGlobal->FieldValues["ParentID"].IsNull())
					  //Ϊ���ڵ㣬���뵽�����ʽڵ�������
					  {
						  slNodesAccessedList->Add(strThisNodeID);
						  dmdLog->tbCategory->Next();
						  continue;
					  }

					  //�����ǰ����
					  slNodesList->Clear();

					  //�Ѹýڵ���뵱ǰ������������
					  slNodesList->Add(strThisNodeID);

					  while (1)
					  {
						  strParentID = OrigStr(dmdLog->qeGlobal->FieldValues["ParentID"]);

						  if (strParentID.IsEmpty())
						  //��ǰ�ڵ�Ϊ���ڵ㣬��
						  //�ѵ�ǰ�����е����нڵ���뵽�����ʽڵ�������
						  {
							  for (i=0;i<slNodesList->Count;i++)
								  slNodesAccessedList->Add(slNodesList->Strings[i]);

							  break;
						  }

						  //���ڵ��Ѿ������ʣ���
						  //�ѵ�ǰ�����е����нڵ���뵽�����ʽڵ�������
						  //ǰ�᣺һ���ڵ����ҽ���һ�����ڵ�
						  if (slNodesAccessedList->IndexOf(strParentID) >= 0)
						  {
							  for (i=0;i<slNodesList->Count;i++)
								  slNodesAccessedList->Add(slNodesList->Strings[i]);

							  break;
						  }

						  if (slNodesList->IndexOf(strParentID) >= 0)
						  {
							  strWarning = (AnsiString)L"Node "+
										   strParentID+
										   L" is a ring node,\n\n"+
										   L" Break the ring ?";

							  if (Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION)
										  == IDOK)
							  //���ڵ��Ѿ��ڵ�ǰ�ڵ���г��֣�
							  //������ǰ�����иýڵ�֮������нڵ��Ϊ���ڵ�
							  //������ԣ��ӵ�ǰ�ڵ�Ͽ��������ѵ�ǰ�ڵ��óɸ��ڵ�
							  {
								  //�������ݱ�ǰָ��
								  nInitialRecNo = dmdLog->tbCategory->RecNo;

								  //��ȡ��ǰ�ڵ��ʶ
								  strCurrentNodeID = slNodesList->Strings[slNodesList->Count-1];

								  //�ӵ�ǰ�ڵ����ɾ���ýڵ�
								  slNodesList->Delete(slNodesList->Count-1);

								  //�ѵ�ǰ�ڵ��óɸ��ڵ�
								  if (dmdLog->tbCategory->Locate("NodeID",SQLStr(strCurrentNodeID),LocateOption))
								  {
									  dmdLog->tbCategory->Edit();

									  dmdLog->tbCategory->FieldValues["ParentID"] = SQLStr("");
								  }

								  //�ָ����ݱ�ָ��
								  dmdLog->tbCategory->First();
								  dmdLog->tbCategory->MoveBy(nInitialRecNo-1);

								  //�ѵ�ǰ�ڵ���е����нڵ���뱻���ʽڵ��
								  for (i=0;i<slNodesList->Count;i++)
									  slNodesAccessedList->Add(slNodesList->Strings[i]);

								  break;
							  }
						  }

						  //��StoryLibTree���ݱ��в����Ƿ���ڸ��ڵ�
						  strSQLClause = (AnsiString)"select NodeID,ParentID from "+
										 strSTORYLIBTREE+
										 " where NodeID = '"+
										 SQLStr(strParentID)+
										 "'";

						  dmdLog->qeGlobal->SQL->Clear();
						  dmdLog->qeGlobal->SQL->Add(strSQLClause);
						  dmdLog->qeGlobal->Open();

						  if (dmdLog->qeGlobal->RecordCount == 0)
						  {
							  //��ȡ��ǰ�ڵ��ʶ
							  strCurrentNodeID = slNodesList->Strings[slNodesList->Count-1];

							  strWarning = (AnsiString)"Node "+
										   strCurrentNodeID+
										   " is an isolated node,\n\n"+
										   " Attach it to the root ?";

							  if (Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION)
										  == IDOK)
							  //��ǰ�����е����нڵ��Ϊ�����ڵ�
							  //������ԣ��ѵ�ǰ�ڵ��óɸ��ڵ�
							  {
								  //�������ݱ�ǰָ��
								  nInitialRecNo = dmdLog->tbCategory->RecNo;

								  //�ӵ�ǰ�ڵ����ɾ����ǰ�ڵ�
								  slNodesList->Delete(slNodesList->Count-1);

								  //�ѵ�ǰ�ڵ��óɸ��ڵ�
								  if (dmdLog->tbCategory->Locate("NodeID",SQLStr(strCurrentNodeID),LocateOption))
								  {
									  dmdLog->tbCategory->Edit();

									  dmdLog->tbCategory->FieldValues["ParentID"] = SQLStr("");
								  }

								  //�ָ����ݱ�ָ��
								  dmdLog->tbCategory->First();
								  dmdLog->tbCategory->MoveBy(nInitialRecNo-1);

								  //�ѵ�ǰ�ڵ���е����нڵ���뱻���ʽڵ��
								  for (i=0;i<slNodesList->Count;i++)
									  slNodesAccessedList->Add(slNodesList->Strings[i]);

								  break;
							  }
							  //������dmdLog->qeGlobal->RecordCount > 1 ������
							  //��һ���ڵ��ж�����ڵ�
						  }

						  //�Ѹýڵ���뵱ǰ����
						  slNodesList->Add(strParentID);

						  if (dmdLog->qeGlobal->FieldValues["ParentID"].IsNull())
						  //Ϊ���ڵ㣬���뵽�����ʽڵ�������
						  {
							  slNodesAccessedList->Add(strParentID);
							  break;
						  }
					  }

					  dmdLog->tbCategory->Next();
					}

					delete slNodesList;
					slNodesList = NULL;
					delete slNodesAccessedList;
					slNodesAccessedList = NULL;
				}
				catch(...)
				{
					strErrMsg = "Checking Story Lib Tree Failed !";
					throw;
				}

				//**********************************
				//���Ҷ�ڵ����ݱ��Ƿ��ж�Ӧ��Ҷ�ڵ�
				//�������ڣ���ɾ�������ݱ�
				//**********************************

				//��ѯ����������ʾ�����
				strSQLClause = "select year = datepart(yy,getdate())";

				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);
				dmdLog->qeGlobal->Open();

				dmdLog->qeGlobal->First();
				nCurrentYear = dmdLog->qeGlobal->FieldValues["year"];

				try
				{
					strSQLClause = "select name, id from sysobjects where type = 'U' and ((name like 'lib_old%') or (name like 'lib_new%'))";

					dmdLog->qeGlobal->SQL->Clear();
					dmdLog->qeGlobal->SQL->Add(strSQLClause);
					dmdLog->qeGlobal->Open();

					slLeafNodesID = new TStringList;

					dmdLog->qeGlobal->First();

					while (! dmdLog->qeGlobal->Eof)
					{
//						strThisNodeID = dmdLog->qeGlobal->FieldValues["name"];
						strThisNodeID = dmdLog->qeGlobal->Fields->Fields[0]->AsString;
						strThisNodeID.Delete(1,7);
						if (! strThisNodeID.IsEmpty())
						{
							if (slLeafNodesID->IndexOf(strThisNodeID) < 0)
							//�ڵ�ID��δ����
								slLeafNodesID->Add(strThisNodeID);
						}

						dmdLog->qeGlobal->Next();
					}

					strSQLClause = "select name, id from sysobjects where type = 'U' and (name like 'libS_%S_%' ESCAPE 'S')";

					dmdLog->qeGlobal->SQL->Clear();
					dmdLog->qeGlobal->SQL->Add(strSQLClause);
					dmdLog->qeGlobal->Open();

					dmdLog->qeGlobal->First();

					while (! dmdLog->qeGlobal->Eof)
					{
//						strThisNodeID = dmdLog->qeGlobal->FieldValues["name"];
						strThisNodeID = dmdLog->qeGlobal->Fields->Fields[0]->AsString;
						strThisNodeID.Delete(1,4);
						nPos = strThisNodeID.Pos("_");
						if (nPos > 0)
							strThisNodeID.Delete(1,nPos);
						if (! strThisNodeID.IsEmpty())
						{
							if (slLeafNodesID->IndexOf(strThisNodeID) < 0)
							//�ڵ�ID��δ����
								slLeafNodesID->Add(strThisNodeID);
						}

						dmdLog->qeGlobal->Next();
					}
				}
				catch(...)
				{
					strErrMsg = "Failed to search tables associating with leaf ndoes !";
					throw;
				}

				for (i=0;i<slLeafNodesID->Count;i++)
				{
					strThisNodeID = slLeafNodesID->Strings[i];

					strSQLClause = (AnsiString)"select NodeID from "+
								   strSTORYLIBTREE+
								   " where NodeID = '"+
								   SQLStr(strThisNodeID)+
								   "'";

					dmdLog->qeGlobal->SQL->Clear();
					dmdLog->qeGlobal->SQL->Add(strSQLClause);
					dmdLog->qeGlobal->Open();

					if (dmdLog->qeGlobal->RecordCount == 0)
					//�����ڶ�Ӧ��Ҷ�ڵ㣬��ɾ�������ݿ�
					{
						strWarning = (AnsiString)"The leaf node with its NodeID "+
									 strThisNodeID+
									 " does not exist,but its associating table exists,\n\n"+
									 "Remove such table ?";

						if (Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION)
									== IDOK)
						{
							//��ɾ����ص�������������
							try
							{
								strPreSQLClause = (AnsiString)"master..xpDropTextIndex '"+
												   m_strDatabaseName+
												   ".dbo.lib_old"+
												   strThisNodeID+
												   "'";

								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strPreSQLClause);
								dmdLog->qeGlobal->ExecSQL();
							}
							catch(...)
							{
								;
							}

/*
							try
							{
								strSQLClause = (AnsiString)"master..xpCreateTextIndex 'lib_old"+
											   strThisNodeID+
											   "','"+
											   m_strCodeType+
											   "','"+
											   m_strDatabaseName+
											   ".dbo.lib_old"+
											   strThisNodeID+
											   "',"+
											   strSQLClause;

								//ִ��SQL���
								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strSQLClause);
								dmdLog->qeGlobal->ExecSQL();
							}
							catch(...)
							{
								strErrMsg =  (AnsiString)"Creating Full Text Index on the node with its NodeID "+
											 strThisNodeID+
											 "failed !";

								throw;
							}
*/

							strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_old"+
										   strThisNodeID+
										   "') and sysstat & 0xf = 3) drop table lib_old"+
										   strThisNodeID;

							dmdLog->qeGlobal->SQL->Clear();
							dmdLog->qeGlobal->SQL->Add(strSQLClause);
							dmdLog->qeGlobal->ExecSQL();

							strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_new"+
										   strThisNodeID+
										   "') and sysstat & 0xf = 3) drop table lib_new"+
										   strThisNodeID;

							dmdLog->qeGlobal->SQL->Clear();
							dmdLog->qeGlobal->SQL->Add(strSQLClause);
							dmdLog->qeGlobal->ExecSQL();

							for (i=1990;i<=nCurrentYear+1;i++)
							{

							  dmdStory->daqCheckTable->Close();
							  dmdStory->daqCheckTable->SQL->Clear();
							  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
												   String(i)+ "_"+ strThisNodeID+ "')");
							  dmdStory->daqCheckTable->Open();
							  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

								//��ɾ����ص�������������
								try
								{
									strPreSQLClause = (AnsiString)"master..xpDropTextIndex '"+
													   m_strDatabaseName+
													   ".dbo.lib_"+
													   String(i)+
													   "_"+
													   strThisNodeID+
													   "'";

									dmdLog->qeGlobal->SQL->Clear();
									dmdLog->qeGlobal->SQL->Add(strPreSQLClause);
									dmdLog->qeGlobal->ExecSQL();
								}
								catch(...)
								{
									;
								}

								strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_"+
											   String(i)+
											   "_"+
											   strThisNodeID+
											   "') and sysstat & 0xf = 3) drop table lib_"+
											   String(i)+
											   "_"+
											   strThisNodeID;

								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strSQLClause);
								dmdLog->qeGlobal->ExecSQL();
							}
						}
					}
				}

				delete slLeafNodesID;
				slLeafNodesID = NULL;

				//****************************************
				//���Ŀ¼�����е�Ҷ�ڵ��Ƿ��ж�Ӧ�����ݱ�
				//�������ڣ��򴴽�֮
				//****************************************

				dmdLog->tbCategory->First();

				while (! dmdLog->tbCategory->Eof)
				{
					strThisNodeID = OrigStr(dmdLog->tbCategory->FieldValues["NodeID"]);

					nNodeAttr = dmdLog->tbCategory->FieldValues["NodeAttr"];

					bGenLibError        = false;
					bUpdateStoryLibrary = false;

					if (nNodeAttr == 1)
					//ΪҶ�ڵ�
					{
						strBaseFieldsSQLClause   = "";
						strBaseFieldsIndexClause = "";
						strExtFieldsSQLClause    = "";
						strExtFieldsIndexClause  = "";

						strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_old"+
									   strThisNodeID+
									   (AnsiString)"')";

						dmdLog->qeGlobal->SQL->Clear();
						dmdLog->qeGlobal->SQL->Add(strSQLClause);
						dmdLog->qeGlobal->Open();

						if (dmdLog->qeGlobal->RecordCount == 0)
						//�����ڶ�Ӧ��Old������֮
						{
							//��֯���ڽ���Ĳ�ѯ���
							OrganizeFieldsSQLClause(strThisNodeID,
													strBaseFieldsSQLClause,
													strExtFieldsSQLClause,
													strBaseFieldsIndexClause,
													strExtFieldsIndexClause);

							if ((! strBaseFieldsSQLClause.IsEmpty())||(! strExtFieldsSQLClause.IsEmpty()))
							{
								strSQLClause = (AnsiString)"CREATE TABLE lib_old"+
											   strThisNodeID+
											   "(";
								strSQLClause += "id int NOT NULL IDENTITY (1, 1),";
								if (! strBaseFieldsSQLClause.IsEmpty())
								  strSQLClause = strSQLClause + strBaseFieldsSQLClause;

								if (! strExtFieldsSQLClause.IsEmpty())
								  strSQLClause = strSQLClause + strExtFieldsSQLClause;

								//ָ������
								strSQLClause = strSQLClause +
											   "CONSTRAINT PK_lib_old"+
											   strThisNodeID+
											   "_1__10 "+
											   "PRIMARY KEY CLUSTERED("+
											   FIELDSTORYID+
											   ")";

								//����β��������
								strSQLClause = strSQLClause + ")";

								//����old��
								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strSQLClause);
								dmdLog->qeGlobal->ExecSQL();

								//�����������(WriteTime)
								strSQLClause = "CREATE INDEX NewsIndex1 ON dbo.lib_old"+
											   strThisNodeID+
											   "("+
											   FIELDWRITETIME+
											   ")";

								//ִ��SQL���
								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strSQLClause);
								dmdLog->qeGlobal->ExecSQL();

								//�����������(MaterialID)
								strSQLClause = "CREATE INDEX NewsIndex2 ON dbo.lib_old"+
											   strThisNodeID+
											   "("+
											   FIELDMATERIALID+
											   ")";

								//ִ��SQL���
								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strSQLClause);
								dmdLog->qeGlobal->ExecSQL();

								//�����������(Date)
								strSQLClause = "CREATE INDEX NewsIndex3 ON dbo.lib_old"+
											   strThisNodeID+
											   "("+
											   FIELDDateID+
											   ")";

								//ִ��SQL���
								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strSQLClause);
								dmdLog->qeGlobal->ExecSQL();

								//��������������й�����
								if (! m_strDatabaseName.IsEmpty())
								{
									strSQLClause = strBaseFieldsIndexClause+strExtFieldsIndexClause;

									if (! strSQLClause.IsEmpty())
									{
										//ɾ��ĩβ��","
										strSQLClause.Delete(strSQLClause.Length(),1);

										try
										{
											//�Ȳ�ѯ��ǰ���Ƿ����ȫ������������ɾ��֮
											strPreSQLClause = (AnsiString)"master..xpQueryTextIndex '"+
															  m_strDatabaseName+
															  ".dbo.lib_old"+
															  strThisNodeID+
															  "'";

											dmdLog->qeGlobal->SQL->Clear();
											dmdLog->qeGlobal->SQL->Add(strPreSQLClause);
											dmdLog->qeGlobal->Open();

											if (dmdLog->qeGlobal->RecordCount > 0)
											{
												strPreSQLClause = (AnsiString)"master..xpDropTextIndex '"+
																   m_strDatabaseName+
																   ".dbo.lib_old"+
																   strThisNodeID+
																   "'";

												dmdLog->qeGlobal->SQL->Clear();
												dmdLog->qeGlobal->SQL->Add(strPreSQLClause);
												dmdLog->qeGlobal->ExecSQL();
											}
										}
										catch(...)
										{
											;
										}

										try
										{
											strSQLClause = (AnsiString)"master..xpCreateTextIndex 'lib_old"+
														   strThisNodeID+
														   "','"+
														   m_strCodeType+
														   "','"+
														   m_strDatabaseName+
														   ".dbo.lib_old"+
														   strThisNodeID+
														   "',"+
														   strSQLClause;

											//ִ��SQL���
											dmdLog->qeGlobal->SQL->Clear();
											dmdLog->qeGlobal->SQL->Add(strSQLClause);
											dmdLog->qeGlobal->ExecSQL();
										}
										catch(...)
										{
											strErrMsg =  (AnsiString)"Creating Full Text Index on the node with its NodeID "+
														 strThisNodeID+
														 "failed !";

											throw;
										}
									}
								}

								bUpdateStoryLibrary = true;
							}
							else
							{
								strWarning = (AnsiString)"The leaf node with its NodeID "+
											 strThisNodeID+
											 " has no associating field,\n\n"+
											 " cannot create its associating table !";

								Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);

								bGenLibError = true;
							}
						}

						if (! bGenLibError)
						{
							try
							{
								strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_new"+
											   strThisNodeID+
											   (AnsiString)"')";

								dmdLog->qeGlobal->SQL->Clear();
								dmdLog->qeGlobal->SQL->Add(strSQLClause);
								dmdLog->qeGlobal->Open();

								if (dmdLog->qeGlobal->RecordCount == 0)
								//�����ڶ�Ӧ��New������֮
								{
									if (! bUpdateStoryLibrary)
									{
										//��֯���ڽ���Ĳ�ѯ���
										OrganizeFieldsSQLClause(strThisNodeID,
																strBaseFieldsSQLClause,
																strExtFieldsSQLClause,
																strBaseFieldsIndexClause,
																strExtFieldsIndexClause);
									}

									if ((! strBaseFieldsSQLClause.IsEmpty())||(! strExtFieldsSQLClause.IsEmpty()))
									{
										strSQLClause = (AnsiString)"CREATE TABLE lib_new"+
													   strThisNodeID+
													   "(";
										strSQLClause += "id int NOT NULL IDENTITY (1, 1),";
										if (! strBaseFieldsSQLClause.IsEmpty())
										  strSQLClause = strSQLClause + strBaseFieldsSQLClause;

										if (! strExtFieldsSQLClause.IsEmpty())
										  strSQLClause = strSQLClause + strExtFieldsSQLClause;

										//ָ������
										strSQLClause = strSQLClause +
													   "CONSTRAINT PK_lib_new"+
													   strThisNodeID+
													   "_1__10 "+
													   "PRIMARY KEY CLUSTERED("+
													   FIELDSTORYID+
													   ")";

										//����β��������
										strSQLClause = strSQLClause + ")";

										//ִ��SQL���
										dmdLog->qeGlobal->SQL->Clear();
										dmdLog->qeGlobal->SQL->Add(strSQLClause);
										dmdLog->qeGlobal->ExecSQL();

										//�����������(WriteTime)
										strSQLClause = "CREATE INDEX NewsIndex1 ON dbo.lib_new"+
													   strThisNodeID+
													   "("+
													   FIELDWRITETIME+
													   ")";

										//ִ��SQL���
										dmdLog->qeGlobal->SQL->Clear();
										dmdLog->qeGlobal->SQL->Add(strSQLClause);
										dmdLog->qeGlobal->ExecSQL();

										//�����������(MaterialID)
										strSQLClause = "CREATE INDEX NewsIndex2 ON dbo.lib_new"+
													   strThisNodeID+
													   "("+
													   FIELDMATERIALID+
													   ")";

										//ִ��SQL���
										dmdLog->qeGlobal->SQL->Clear();
										dmdLog->qeGlobal->SQL->Add(strSQLClause);
										dmdLog->qeGlobal->ExecSQL();

										//�����������(Date)
										strSQLClause = "CREATE INDEX NewsIndex3 ON dbo.lib_new"+
													   strThisNodeID+
													   "("+
													   FIELDDateID+
													   ")";

										//ִ��SQL���
										dmdLog->qeGlobal->SQL->Clear();
										dmdLog->qeGlobal->SQL->Add(strSQLClause);
										dmdLog->qeGlobal->ExecSQL();
									}
									else
									{
										strWarning = (AnsiString)"The leaf node with its NodeID "+
													 strThisNodeID+
													 " has no associating field,\n\n"+
													 " cannot create its associating table !";

										Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
									}
								}
							}
							catch(...)
							{
								strErrMsg = (AnsiString)"Creating lib_new"+
											 strThisNodeID+
											 " failed !";

								throw;
							}
						}

						for (i=1990;i<=nCurrentYear+1;i++)
						{
							strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_"+
										   String(i)+
										   "_"+
										   strThisNodeID+
										   (AnsiString)"')";

							dmdLog->qeGlobal->SQL->Clear();
							dmdLog->qeGlobal->SQL->Add(strSQLClause);
							dmdLog->qeGlobal->Open();

							if (dmdLog->qeGlobal->RecordCount == 0)
							//�������ض�����ݱ�����֮
							{
								//��֯���ڽ���Ĳ�ѯ���
								OrganizeFieldsSQLClause(strThisNodeID,
														strBaseFieldsSQLClause,
														strExtFieldsSQLClause,
														strBaseFieldsIndexClause,
														strExtFieldsIndexClause);

								if ((! strBaseFieldsSQLClause.IsEmpty())||(! strExtFieldsSQLClause.IsEmpty()))
								{
									strSQLClause = (AnsiString)"CREATE TABLE lib_"+
												   String(i)+
												   "_"+
												   strThisNodeID+
												   "(";
									strSQLClause += "id int NOT NULL IDENTITY (1, 1),";
									if (! strBaseFieldsSQLClause.IsEmpty())
									  strSQLClause = strSQLClause + strBaseFieldsSQLClause;

									if (! strExtFieldsSQLClause.IsEmpty())
									  strSQLClause = strSQLClause + strExtFieldsSQLClause;

									//ָ������
									strSQLClause = strSQLClause +
												   "CONSTRAINT INDEX_lib_"+
												   strThisNodeID+
												   " PRIMARY KEY CLUSTERED("+
												   FIELDSTORYID+
												   ")";

									//����β��������
									strSQLClause = strSQLClause + ")";

									//����old��
									dmdLog->qeGlobal->SQL->Clear();
									dmdLog->qeGlobal->SQL->Add(strSQLClause);
									dmdLog->qeGlobal->ExecSQL();

									//�����������(WriteTime)
									strSQLClause = "CREATE INDEX NewsIndex1 ON dbo.lib_"+
												   String(i)+
												   "_"+
												   strThisNodeID+
												   "("+
												   FIELDWRITETIME+
												   ")";

									//ִ��SQL���
									dmdLog->qeGlobal->SQL->Clear();
									dmdLog->qeGlobal->SQL->Add(strSQLClause);
									dmdLog->qeGlobal->ExecSQL();

									//�����������(MaterialID)
									strSQLClause = "CREATE INDEX NewsIndex2 ON dbo.lib_"+
												   String(i)+
												   "_"+
												   strThisNodeID+
												   "("+
												   FIELDMATERIALID+
												   ")";

									//ִ��SQL���
									dmdLog->qeGlobal->SQL->Clear();
									dmdLog->qeGlobal->SQL->Add(strSQLClause);
									dmdLog->qeGlobal->ExecSQL();

									//�����������(Date)
									strSQLClause = "CREATE INDEX NewsIndex3 ON dbo.lib_"+
												   String(i)+
												   "_"+
												   strThisNodeID+
												   "("+
												   FIELDDateID+
												   ")";

									//ִ��SQL���
									dmdLog->qeGlobal->SQL->Clear();
									dmdLog->qeGlobal->SQL->Add(strSQLClause);
									dmdLog->qeGlobal->ExecSQL();

									//��������������й�����
									if (! m_strDatabaseName.IsEmpty())
									{
										strSQLClause = strBaseFieldsIndexClause+strExtFieldsIndexClause;

										if (! strSQLClause.IsEmpty())
										{
											//ɾ��ĩβ��","
											strSQLClause.Delete(strSQLClause.Length(),1);

											try
											{
												//�Ȳ�ѯ��ǰ���Ƿ����ȫ������������ɾ��֮
												strPreSQLClause = (AnsiString)"master..xpQueryTextIndex '"+
																  m_strDatabaseName+
																  ".dbo.lib_"+
																  String(i)+
																  "_"+
																  strThisNodeID+
																  "'";

												dmdLog->qeGlobal->SQL->Clear();
												dmdLog->qeGlobal->SQL->Add(strPreSQLClause);
												dmdLog->qeGlobal->Open();

												if (dmdLog->qeGlobal->RecordCount > 0)
												{
													strPreSQLClause = (AnsiString)"master..xpDropTextIndex '"+
																	   m_strDatabaseName+
																	   ".dbo.lib_"+
																	   String(i)+
																	   "_"+
																	   strThisNodeID+
																	   "'";

													dmdLog->qeGlobal->SQL->Clear();
													dmdLog->qeGlobal->SQL->Add(strPreSQLClause);
													dmdLog->qeGlobal->ExecSQL();
												}
											}
											catch(...)
											{
												;
											}

											try
											{
												strSQLClause = (AnsiString)"master..xpCreateTextIndex 'lib_"+
															   String(i)+
															   "_"+
															   strThisNodeID+
															   "','"+
															   m_strCodeType+
															   "','"+
															   m_strDatabaseName+
															   ".dbo.lib_"+
															   String(i)+
															   "_"+
															   strThisNodeID+
															   "',"+
															   strSQLClause;

												//ִ��SQL���
												dmdLog->qeGlobal->SQL->Clear();
												dmdLog->qeGlobal->SQL->Add(strSQLClause);
												dmdLog->qeGlobal->ExecSQL();
											}
											catch(...)
											{
												strErrMsg =  (AnsiString)"Creating Full Text Index on the node with its NodeID "+
															 strThisNodeID+
															 "failed !";

												throw;
											}
										}
									}

									bUpdateStoryLibrary = true;
								}
								else
								{
									strWarning = (AnsiString)"The leaf node with its NodeID "+
												 strThisNodeID+
												 " has no associating field,\n\n"+
												 " cannot create its associating table !";

									Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);

									bGenLibError = true;
								}
							}
						}
					}

					dmdLog->tbCategory->Next();
				}

				//���´洢����
				UpdateStoryLibrary();

				//**********************************
				//���Ҷ�ڵ��Ľṹ��ģ����һ����
				//**********************************

				dmdLog->tbCategory->First();

				while (! dmdLog->tbCategory->Eof)
				{
					strThisNodeID = OrigStr(dmdLog->tbCategory->FieldValues["NodeID"]);

					nNodeAttr = dmdLog->tbCategory->FieldValues["NodeAttr"];

					if (nNodeAttr == 1)
					//ΪҶ�ڵ�
					{
						//��ȡ��Ҷ�ڵ��Ӧ�Ļ����ֶκ���չ�ֶ�
						strSQLClause =  (AnsiString)"(select FieldName,FieldType,FieldSize from "+
										strLIBBASEFIELDS+
										") union (select FieldName,FieldType,FieldSize from "
										strLIBEXTENDFIELDS+
										" where CategoryID = '"+
										SQLStr(strThisNodeID)+
										"')";

						dmdLog->qeSourceFields->SQL->Clear();
						dmdLog->qeSourceFields->SQL->Add(strSQLClause);
						dmdLog->qeSourceFields->Open();

						//��ȡOld��������ֶ�
						strSQLClause =  (AnsiString)"sp_columns lib_old"+
										strThisNodeID;

						dmdLog->qeDestFields->SQL->Clear();
						dmdLog->qeDestFields->SQL->Add(strSQLClause);
						dmdLog->qeDestFields->Open();

						//�ȼ�����л����ֶκ���չ�ֶ��Ƿ���Old���ֶ��г���

						nAssociatingFields = 0;

						while (! dmdLog->qeSourceFields->Eof)
						{
							strSourceFieldName = OrigStr(dmdLog->qeSourceFields->FieldValues["FieldName"]);
							strSourceFieldType = OrigStr(dmdLog->qeSourceFields->FieldValues["FieldType"]);
							nSourceFieldSize   = dmdLog->qeSourceFields->FieldValues["FieldSize"];

							strWarning = (AnsiString)"Template field "+
										 strSourceFieldName+
										 " does not match the table structure of lib_old"+
										 strThisNodeID;

							if (dmdLog->qeDestFields->Locate("COLUMN_NAME",strSourceFieldName,LocateOption))
							//���ڶ�Ӧ���ֶ�
							{
								strDestFieldType = dmdLog->qeDestFields->FieldValues["TYPE_NAME"];
								nDestFieldSize   = dmdLog->qeDestFields->FieldValues["LENGTH"];

								//��¼��Ӧ�ֶθ���
								nAssociatingFields++;

								if ((strDestFieldType != strSourceFieldType)||
								   ((strDestFieldType == typeVARCHAR)&&(nDestFieldSize != nSourceFieldSize)))
								//�ֶβ�ƥ��
								{
									Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
								}
							}
							else
							//�����ڶ�Ӧ�ֶ�
							{
								Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
							}

							dmdLog->qeSourceFields->Next();
						}

/*
						//���Date�ֶ�
						strWarning = (AnsiString)"Internal field Date"+
									 " does not match the table structure of lib_old"+
									 strThisNodeID;

						if (dmdLog->qeDestFields->Locate("COLUMN_NAME","Date",LocateOption))
						//���ڶ�Ӧ���ֶ�
						{
							strDestFieldType = dmdLog->qeDestFields->FieldValues["TYPE_NAME"];
							nDestFieldSize   = dmdLog->qeDestFields->FieldValues["LENGTH"];

							//��¼��Ӧ�ֶθ���
							nAssociatingFields++;

							if (strDestFieldType != "datetime")
							//�ֶβ�ƥ��
							{
								Application->MessageBox(strWarning.c_str(),GLOBALMSGCAPTION,48);
							}
						}
						else
						//�����ڶ�Ӧ�ֶ�
						{
							Application->MessageBox(strWarning.c_str(),GLOBALMSGCAPTION,48);
						}
*/

						if (dmdLog->qeDestFields->RecordCount > nAssociatingFields)
						//���Old���ֶ��������ڿɶ�Ӧ���ֶθ���,�������ֶνṹ��ģ���ֶβ�ƥ��
						{
							strWarning = "Field(s) ";

							dmdLog->qeDestFields->First();

							while (! dmdLog->qeDestFields->Eof)
							{
								strDestFieldName = dmdLog->qeDestFields->FieldValues["COLUMN_NAME"];

								if (! dmdLog->qeSourceFields->Locate("FieldName",SQLStr(strDestFieldName),LocateOption))
								{
									strWarning = strWarning+
												 strDestFieldName+
												 " ";
								}

								dmdLog->qeDestFields->Next();
							}

							strWarning = strWarning+
										 " of the table lib_old"+
										 strThisNodeID+
										 " do not match its Template !";

							Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
						}

						//��ȡNew��������ֶ�
						strSQLClause =  (AnsiString)"sp_columns lib_new"+
										strThisNodeID;

						dmdLog->qeDestFields->SQL->Clear();
						dmdLog->qeDestFields->SQL->Add(strSQLClause);
						dmdLog->qeDestFields->Open();

						//�ȼ�����л����ֶκ���չ�ֶ��Ƿ���New���ֶ��г���

						nAssociatingFields = 0;

						dmdLog->qeSourceFields->First();

						while (! dmdLog->qeSourceFields->Eof)
						{
							strSourceFieldName = OrigStr(dmdLog->qeSourceFields->FieldValues["FieldName"]);
							strSourceFieldType = OrigStr(dmdLog->qeSourceFields->FieldValues["FieldType"]);
							nSourceFieldSize   = dmdLog->qeSourceFields->FieldValues["FieldSize"];

							strWarning = (AnsiString)"Template field "+
										 strSourceFieldName+
										 " does not match the table structure of lib_new"+
										 strThisNodeID;

							if (dmdLog->qeDestFields->Locate("COLUMN_NAME",strSourceFieldName,LocateOption))
							//���ڶ�Ӧ���ֶ�
							{
								strDestFieldType = dmdLog->qeDestFields->FieldValues["TYPE_NAME"];
								nDestFieldSize   = dmdLog->qeDestFields->FieldValues["LENGTH"];

								//��¼��Ӧ�ֶεĸ���
								nAssociatingFields++;;

								if ((strDestFieldType != strSourceFieldType)||
								   ((strDestFieldType == typeVARCHAR)&&(nDestFieldSize != nSourceFieldSize)))
								//�ֶβ�ƥ��
								{
									Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
								}
							}
							else
							//�����ڶ�Ӧ�ֶ�
							{
								Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
							}

							dmdLog->qeSourceFields->Next();
						}

/*
						//���Date�ֶ�
						strWarning = (AnsiString)"Internal field Date"+
									 " does not match the table structure of lib_new"+
									 strThisNodeID;

						if (dmdLog->qeDestFields->Locate("COLUMN_NAME","Date",LocateOption))
						//���ڶ�Ӧ���ֶ�
						{
							strDestFieldType = dmdLog->qeDestFields->FieldValues["TYPE_NAME"];
							nDestFieldSize   = dmdLog->qeDestFields->FieldValues["LENGTH"];

							//��¼��Ӧ�ֶθ���
							nAssociatingFields++;

							if (strDestFieldType != "datetime")
							//�ֶβ�ƥ��
							{
								Application->MessageBox(strWarning.c_str(),GLOBALMSGCAPTION,48);
							}
						}
						else
						//�����ڶ�Ӧ�ֶ�
						{
							Application->MessageBox(strWarning.c_str(),GLOBALMSGCAPTION,48);
						}
*/

						if (dmdLog->qeDestFields->RecordCount > nAssociatingFields)
						//���New���ֶ��������ڿɶ�Ӧ���ֶθ���,�������ֶνṹ��ģ���ֶβ�ƥ��
						{
							strWarning = "Field(s) ";

							dmdLog->qeDestFields->First();

							while (! dmdLog->qeDestFields->Eof)
							{
								strDestFieldName = dmdLog->qeDestFields->FieldValues["COLUMN_NAME"];

								if (! dmdLog->qeSourceFields->Locate("FieldName",SQLStr(strDestFieldName),LocateOption))
								{
									strWarning = strWarning+
												 strDestFieldName+
												 " ";
								}

								dmdLog->qeDestFields->Next();
							}

							strWarning = strWarning+
										 " of the table lib_new"+
										 strThisNodeID+
										 " do not match its Template !";

							Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
						}

						//������ȡ��ݿ�������ֶ�

						for (i=1990;i<=nCurrentYear+1;i++)
						{

						  dmdStory->daqCheckTable->Close();
						  dmdStory->daqCheckTable->SQL->Clear();
						  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
																			   String(i)+ "_"+ strThisNodeID+ "')");
						  dmdStory->daqCheckTable->Open();
						  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

							strSQLClause =  (AnsiString)"sp_columns lib_"+
											String(i)+
											"_"+
											strThisNodeID;

							dmdLog->qeDestFields->SQL->Clear();
							dmdLog->qeDestFields->SQL->Add(strSQLClause);
							dmdLog->qeDestFields->Open();

							//�ȼ�����л����ֶκ���չ�ֶ��Ƿ���New���ֶ��г���

							nAssociatingFields = 0;

							dmdLog->qeSourceFields->First();

							while (! dmdLog->qeSourceFields->Eof)
							{
								strSourceFieldName = OrigStr(dmdLog->qeSourceFields->FieldValues["FieldName"]);
								strSourceFieldType = OrigStr(dmdLog->qeSourceFields->FieldValues["FieldType"]);
								nSourceFieldSize   = dmdLog->qeSourceFields->FieldValues["FieldSize"];

								strWarning = (AnsiString)"Template field "+
											 strSourceFieldName+
											 " does not match the table structure of lib_"+
											 String(i)+
											 "_"+
											 strThisNodeID;

								if (dmdLog->qeDestFields->Locate("COLUMN_NAME",strSourceFieldName,LocateOption))
								//���ڶ�Ӧ���ֶ�
								{
									strDestFieldType = dmdLog->qeDestFields->FieldValues["TYPE_NAME"];
									nDestFieldSize   = dmdLog->qeDestFields->FieldValues["LENGTH"];

									//��¼��Ӧ�ֶεĸ���
									nAssociatingFields++;;

									if ((strDestFieldType != strSourceFieldType)||
									   ((strDestFieldType == typeVARCHAR)&&(nDestFieldSize != nSourceFieldSize)))
									//�ֶβ�ƥ��
									{
										Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
									}
								}
								else
								//�����ڶ�Ӧ�ֶ�
								{
									Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
								}

								dmdLog->qeSourceFields->Next();
							}

/*
							//���Date�ֶ�
							strWarning = (AnsiString)"Internal field Date"+
										 " does not match the table structure of lib_"+
										 String(i)+
										 "_"+
										 strThisNodeID;

							if (dmdLog->qeDestFields->Locate("COLUMN_NAME","Date",LocateOption))
							//���ڶ�Ӧ���ֶ�
							{
								strDestFieldType = dmdLog->qeDestFields->FieldValues["TYPE_NAME"];
								nDestFieldSize   = dmdLog->qeDestFields->FieldValues["LENGTH"];

								//��¼��Ӧ�ֶθ���
								nAssociatingFields++;

								if (strDestFieldType != "datetime")
								//�ֶβ�ƥ��
								{
									Application->MessageBox(strWarning.c_str(),GLOBALMSGCAPTION,48);
								}
							}
							else
							//�����ڶ�Ӧ�ֶ�
							{
								Application->MessageBox(strWarning.c_str(),GLOBALMSGCAPTION,48);
							}
*/

							if (dmdLog->qeDestFields->RecordCount > nAssociatingFields)
							//�����ݱ��ֶ��������ڿɶ�Ӧ���ֶθ���,�������ֶνṹ��ģ���ֶβ�ƥ��
							{
								strWarning = "Field(s) ";

								dmdLog->qeDestFields->First();

								while (! dmdLog->qeDestFields->Eof)
								{
									strDestFieldName = dmdLog->qeDestFields->FieldValues["COLUMN_NAME"];

									if (! dmdLog->qeSourceFields->Locate("FieldName",SQLStr(strDestFieldName),LocateOption))
									{
										strWarning = strWarning+
													 strDestFieldName+
													 " ";
									}

									dmdLog->qeDestFields->Next();
								}

								strWarning = strWarning+
											 " of the table lib_"+
											 String(i)+
											 "_"+
											 strThisNodeID+
											 " do not match its Template !";

								Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
							}
						}
					}

					dmdLog->tbCategory->Next();
				}

				dmdLog->tbCategory->Close();

				//*************************
				//�û����û����һ���Լ��
				//*************************

				//���lib_GroupUser�е�GroupID�Ƿ������lib_Group����
				strSQLClause = (AnsiString)"select GroupID from lib_GroupUser where GroupID not in "+
							   "(select GroupID from lib_Group)";

				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);
				dmdLog->qeGlobal->Open();

				if (dmdLog->qeGlobal->RecordCount > 0)
				{
					strWarning = "Group(s) ";

					dmdLog->qeGlobal->First();

					while (! dmdLog->qeGlobal->Eof)
					{
						strWarning = strWarning+
									 dmdLog->qeGlobal->FieldValues["GroupID"]+
									 " ";

						dmdLog->qeGlobal->Next();
					}

					strWarning = strWarning+
								 " occur in lib_GroupUser do not exist in lib_Group !";

					Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
				}

				//���lib_GroupUser�е�UserID�Ƿ������lib_User����

				strSQLClause = (AnsiString)"select UserID from lib_GroupUser where UserID not in "+
							   "(select UserID from lib_User)";

				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);
				dmdLog->qeGlobal->Open();

				if (dmdLog->qeGlobal->RecordCount > 0)
				{
					strWarning = "User(s) ";

					dmdLog->qeGlobal->First();

					while (! dmdLog->qeGlobal->Eof)
					{
						strWarning = strWarning+
									 OrigStr(dmdLog->qeGlobal->FieldValues["UserID"])+
									 " ";

						dmdLog->qeGlobal->Next();
					}

					strWarning = strWarning+
								 " occur in lib_GroupUser do not exist in lib_User !";

					Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
				}

				//*************************
				//Ȩ�ޱ��һ���Լ��
				//*************************

				//���lib_Permission�е�GroupID�Ƿ������lib_Group����
				strSQLClause = (AnsiString)"select GroupID from lib_Permission where GroupID not in "+
							   "(select GroupID from lib_Group)";

				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);
				dmdLog->qeGlobal->Open();

				if (dmdLog->qeGlobal->RecordCount > 0)
				{
					strWarning = "Group(s) ";

					dmdLog->qeGlobal->First();

					while (! dmdLog->qeGlobal->Eof)
					{
						strWarning = strWarning+
									 dmdLog->qeGlobal->FieldValues["GroupID"]+
									 " ";

						dmdLog->qeGlobal->Next();
					}

					strWarning = strWarning+
								 " occur in "+
								 strPERMISSION+
								 " do not exist in lib_Group !";

					Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
				}

				//���lib_Permission�е�NodeID�Ƿ������lib_StoryLibTree����

				strSQLClause = (AnsiString)"select NodeID from "+
							   strPERMISSION+
							   " where NodeID not in "+
							   "(select NodeID from "+
							   strSTORYLIBTREE+
							   ")";

				dmdLog->qeGlobal->SQL->Clear();
				dmdLog->qeGlobal->SQL->Add(strSQLClause);
				dmdLog->qeGlobal->Open();

				if (dmdLog->qeGlobal->RecordCount > 0)
				{
					strWarning = "Story Tree node(s) ";

					dmdLog->qeGlobal->First();

					while (! dmdLog->qeGlobal->Eof)
					{
						strWarning = strWarning+
									 dmdLog->qeGlobal->FieldValues["NodeID"]+
									 " ";

						dmdLog->qeGlobal->Next();
					}

					strWarning = strWarning+
								 " occur in "+
								 strPERMISSION+
								 " do not exist in "+
								 strSTORYLIBTREE+
								 " !";

					Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
				}
			}

			Application->MessageBox(L"Checking Process Successfully Completed!",GLOBALMSGCAPTION,48);
		}
	}
	catch(...)
	{
		Application->MessageBox(UnicodeString(strErrMsg).c_str(),GLOBALMSGCAPTION,48);

		if (dmdLog->tbCategory->Active)
			dmdLog->tbCategory->Close();

		if (slNodesList)
			delete slNodesList;

		if (slNodesAccessedList)
			delete slNodesAccessedList;

		if (slLeafNodesID)
			delete slLeafNodesID;

		return;
	}
}
//---------------------------------------------------------------------------
void TfrmLogs::OrganizeFieldsSQLClause(AnsiString  strNodeID,
									   AnsiString& strBaseFieldsSQLClause,
									   AnsiString& strExtFieldsSQLClause,
									   AnsiString& strBaseFieldsIndexClause,
									   AnsiString& strExtFieldsIndexClause)
{
    AnsiString strFieldName,strFieldType;
    AnsiString strSQLClause;

    strBaseFieldsSQLClause   = "";
    strExtFieldsSQLClause    = "";
    strBaseFieldsIndexClause = "";
    strExtFieldsIndexClause  = "";

    //��֯�����ֶβ��ֵ��Ӳ�ѯ
    strSQLClause = (AnsiString)"select * from "+
                   strLIBBASEFIELDS;

    dmdLog->qeBaseFields->SQL->Clear();
    dmdLog->qeBaseFields->SQL->Add(strSQLClause);
    dmdLog->qeBaseFields->Open();

    dmdLog->qeBaseFields->First();
    while (! dmdLog->qeBaseFields->Eof)
    {
        strFieldName = OrigStr(dmdLog->qeBaseFields->FieldValues["FieldName"]);

        strBaseFieldsSQLClause = strBaseFieldsSQLClause+
                                 strFieldName+
                                 (AnsiString)" ";

        strFieldType = OrigStr(dmdLog->qeBaseFields->FieldValues["FieldType"]);

        if (strFieldType == (AnsiString)typeVARCHAR)
        {
            if (strFieldName == FIELDSTORYID)
            {
                strBaseFieldsSQLClause = strBaseFieldsSQLClause+
                                         strFieldType+
                                         (AnsiString)" ("+
                                         String((int)dmdLog->qeBaseFields->FieldValues["FieldSize"])+
                                         (AnsiString)") NOT NULL,";
            }
            else
            {
               strBaseFieldsSQLClause = strBaseFieldsSQLClause+
                                        strFieldType+
                                        (AnsiString)" ("+
                                        String((int)dmdLog->qeBaseFields->FieldValues["FieldSize"])+
                                        (AnsiString)") NULL,";
            }
        }
        else
        {
            strBaseFieldsSQLClause = strBaseFieldsSQLClause+
                                     strFieldType+
                                     (AnsiString)" NULL,";
        }

        if ((strFieldType == typeVARCHAR)||(strFieldType == typeTEXT))
        {
             strBaseFieldsIndexClause = strBaseFieldsIndexClause+
                                        "'"+
                                        strFieldName+
                                        "',";
             if (strFieldName == FIELDSTORYID)
                  strBaseFieldsIndexClause = strBaseFieldsIndexClause + "'Date',";
        }

        dmdLog->qeBaseFields->Next();
    }

/*
    //����Date�ֶΣ����ֶβ������ڻ����ֶα��У�
    strBaseFieldsSQLClause = strBaseFieldsSQLClause+
                             "Date datetime NOT NULL,";
*/

    //��֯��չ�ֶβ��ֵ��Ӳ�ѯ
    strSQLClause = (AnsiString)"select * from "+
                   strLIBEXTENDFIELDS+
                   " where CategoryID = '"+
                   SQLStr(strNodeID)+
                   "'";

    dmdLog->qeExtFields->SQL->Clear();
    dmdLog->qeExtFields->SQL->Add(strSQLClause);
    dmdLog->qeExtFields->Open();

    dmdLog->qeExtFields->First();
    while (! dmdLog->qeExtFields->Eof)
    {
        strFieldName = OrigStr(dmdLog->qeExtFields->FieldValues["FieldName"]);

        strExtFieldsSQLClause = strExtFieldsSQLClause+
                                strFieldName+
                                (AnsiString)" ";

        strFieldType = OrigStr(dmdLog->qeExtFields->FieldValues["FieldType"]);

        if (strFieldType == (AnsiString)typeVARCHAR)
        {
            if (strFieldName == FIELDSTORYID)
            {
                strExtFieldsSQLClause = strExtFieldsSQLClause+
                                        strFieldType+
                                        (AnsiString)" ("+
                                        String((int)dmdLog->qeExtFields->FieldValues["FieldSize"])+
                                        (AnsiString)") NOT NULL,";
            }
            else
            {
               strExtFieldsSQLClause = strExtFieldsSQLClause+
                                       strFieldType+
                                       (AnsiString)" ("+
                                       String((int)dmdLog->qeExtFields->FieldValues["FieldSize"])+
                                       (AnsiString)") NULL,";
			}
		}
		else
		{
			strExtFieldsSQLClause = strExtFieldsSQLClause+
									strFieldType+
									(AnsiString)" NULL,";
		}

		if ((strFieldType == typeVARCHAR)||(strFieldType == typeTEXT))
		{
			 strExtFieldsIndexClause = strExtFieldsIndexClause+
									   "'"+
									   strFieldName+
									   "',";
		}

		dmdLog->qeExtFields->Next();
	}
}

void __fastcall TfrmLogs::tbbtnRefreshClick(TObject *Sender)
{
 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
 strOpID = lblOpID->Caption;
  LogBegin = StrToDateTime(lblLogBegin->Caption, FS );
  LogEnd = StrToDateTime(lblLogEnd->Caption, FS );

   RefreshLogs(LogBegin,LogEnd,strOpID);
}
//---------------------------------------------------------------------------







