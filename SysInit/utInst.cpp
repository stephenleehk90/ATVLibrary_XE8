//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "shlobj.h"
#include "utInst.h"
#include "utdmglob.h"
#include "CommFunc.h"
#include "utConfm.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

#define STOREDPROCCOUNT 2
#define SEARCHENGINEPROCCOUNT 13
#define TABLECOUNT 14
#define MAXRESTYPES 4
#define MAXBASEFIELDS 3
#define USERNAME "U_TDvn[l|cx"
#define PASSWORD "dqymbibaeuo{}d"

#define LANGUAGE_BIG5 "B5"
#define LANGUAGE_GB   "GB"

#define MINCACHESIZE     0
#define MAXCACHESIZE     100
#define DEFAULTCACHESIZE 50
AnsiString l_strAdmin("admin");
AnsiString l_strAdminGroup("admins");
AnsiString l_strLibraryINIFile("Library.ini");
AnsiString l_strLoginName("ATVLibLogin");
AnsiString l_strSear("ATVLibLogin");

AnsiString saResTypes[MAXRESTYPES][3] =
           {
           		{ "1","Tapes","1"},
                { "2","Books","7"},
                { "3","Magazines","3"},
                { "4","Files","3" }
           };

TfmInit *fmInit;

AnsiString saStoredProcName[STOREDPROCCOUNT] =
		   {
           		"dbo.ClearLog",
                "dbo.GetNewFileID",
           };

AnsiString saTableName[TABLECOUNT] =
		   {
                "dbo.lib_BaseFields",
                "dbo.lib_ExtendFields",
                "dbo.lib_Group",
                "dbo.lib_GroupUser",
				"dbo.lib_WordBank",                
                "dbo.lib_LendInfo",
                "dbo.lib_Log",
                "dbo.lib_Res",
                "dbo.lib_StoryLibTree",
                "dbo.lib_SysConfig",
                "dbo.lib_SystemFileID",
                "dbo.lib_User",
                "dbo.lib_Permission",
                "dbo.lib_ResTypes"
           };

AnsiString saSearchEngineProc[SEARCHENGINEPROCCOUNT] =
		   {
           		"xpAddWord",
                "xpBuildWordIndexFile",
                "xpCompress",
                "xpConfig",
                "xpCreateTextIndex",
                "xpDeleteWord",
                "xpDropTextIndex",
                "xpGetTextPos",
                "xpQueryTextIndex",
                "xpSelect",
                "xpSetup",
                "xpSimpleSelect",
                "xpUpdateTextIndex"
           };

AnsiString saBaseFields[MAXBASEFIELDS][3] =
		   {
           		{ "StoryID","varchar","32" },
                { "WriteTime","datetime","8" },
                { "MaterialID","varchar","32" }
           };
//---------------------------------------------------------------------------
__fastcall TfmInit::TfmInit(TComponent* Owner)
	: TForm(Owner)
{
	m_nState = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfmInit::FormClose(TObject *Sender, TCloseAction &Action)
{
	try
    {
    	if (dmGlobal->dbGlobal->Connected)
	        dmGlobal->dbGlobal->Close();
    }
    catch(...)
    {
    	Application->MessageBox("Dababase Access Failed!",MSGBOXCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmInit::btnConnectClick(TObject *Sender)
{
	AnsiString strPassword,strINIPath,strCacheSize;
    AnsiString strSQLClause,strINIFileName,strSearchDbName;
	TCursor crOldCursor;
    int i,nSUID,nIndex;

	crOldCursor = Screen->Cursor;

	try
    {
	    if (m_nState == 1)
		//还未建立连接
    	{
			m_strServerName = edServerName->Text.Trim();
    		m_strDbName     = edDbName->Text.Trim();
			strPassword     = edPassword->Text.Trim();
            strSearchDbName = edSearchDb->Text.Trim();

    		if (m_strServerName.IsEmpty())
			{
    			Application->MessageBox("Please specify the Server Name!",MSGBOXCAPTION,48);
        		edServerName->SetFocus();
				return;
    		}

			if (m_strDbName.IsEmpty())
    		{
    			Application->MessageBox("Please specify the Database Name!",MSGBOXCAPTION,48);
				edDbName->SetFocus();
    			return;
			}

    		if (strSearchDbName.IsEmpty())
			{
    			Application->MessageBox("Please specify the database of retrieve engine!",MSGBOXCAPTION,48);
        		edSearchDb->SetFocus();
				return;
    		}

            if (dmGlobal->dbGlobal->Connected)
                dmGlobal->dbGlobal->Close();

            if (dmGlobal->dbMaster->Connected)
                dmGlobal->dbMaster->Close();

            //与指定数据库建立连接
            nIndex = dmGlobal->dbGlobal->Params->IndexOfName("DATABASE NAME");
            if (nIndex >= 0)
                dmGlobal->dbGlobal->Params->Delete(nIndex);

            nIndex = dmGlobal->dbGlobal->Params->IndexOfName("SERVER NAME");
            if (nIndex >= 0)
                dmGlobal->dbGlobal->Params->Delete(nIndex);

            nIndex = dmGlobal->dbGlobal->Params->IndexOfName("USER NAME");
            if (nIndex >= 0)
                dmGlobal->dbGlobal->Params->Delete(nIndex);

            nIndex = dmGlobal->dbGlobal->Params->IndexOfName("PASSWORD");
            if (nIndex >= 0)
                dmGlobal->dbGlobal->Params->Delete(nIndex);

  			dmGlobal->dbGlobal->Params->Add((AnsiString)"DATABASE NAME="+m_strDbName);
	  		dmGlobal->dbGlobal->Params->Add((AnsiString)"SERVER NAME="+m_strServerName);
		  	dmGlobal->dbGlobal->Params->Add((AnsiString)"USER NAME=sa");
  			dmGlobal->dbGlobal->Params->Add((AnsiString)"PASSWORD="+strPassword);

            //与master主库建立连接
            nIndex = dmGlobal->dbMaster->Params->IndexOfName("DATABASE NAME");
            if (nIndex >= 0)
                dmGlobal->dbMaster->Params->Delete(nIndex);

            nIndex = dmGlobal->dbMaster->Params->IndexOfName("SERVER NAME");
            if (nIndex >= 0)
                dmGlobal->dbMaster->Params->Delete(nIndex);

            nIndex = dmGlobal->dbMaster->Params->IndexOfName("USER NAME");
            if (nIndex >= 0)
                dmGlobal->dbMaster->Params->Delete(nIndex);

            nIndex = dmGlobal->dbMaster->Params->IndexOfName("PASSWORD");
            if (nIndex >= 0)
                dmGlobal->dbMaster->Params->Delete(nIndex);

  			dmGlobal->dbMaster->Params->Add((AnsiString)"DATABASE NAME=master");
	  		dmGlobal->dbMaster->Params->Add((AnsiString)"SERVER NAME="+m_strServerName);
		  	dmGlobal->dbMaster->Params->Add((AnsiString)"USER NAME=sa");
  			dmGlobal->dbMaster->Params->Add((AnsiString)"PASSWORD="+strPassword);

            //与搜索引擎数据库建立连接
            nIndex = dmGlobal->dbSearch->Params->IndexOfName("DATABASE NAME");
            if (nIndex >= 0)
                dmGlobal->dbSearch->Params->Delete(nIndex);

            nIndex = dmGlobal->dbSearch->Params->IndexOfName("SERVER NAME");
            if (nIndex >= 0)
                dmGlobal->dbSearch->Params->Delete(nIndex);

            nIndex = dmGlobal->dbSearch->Params->IndexOfName("USER NAME");
            if (nIndex >= 0)
                dmGlobal->dbSearch->Params->Delete(nIndex);

            nIndex = dmGlobal->dbSearch->Params->IndexOfName("PASSWORD");
            if (nIndex >= 0)
                dmGlobal->dbSearch->Params->Delete(nIndex);

  			dmGlobal->dbSearch->Params->Add((AnsiString)"DATABASE NAME="+strSearchDbName);
	  		dmGlobal->dbSearch->Params->Add((AnsiString)"SERVER NAME="+m_strServerName);
		  	dmGlobal->dbSearch->Params->Add((AnsiString)"USER NAME=sa");
  			dmGlobal->dbSearch->Params->Add((AnsiString)"PASSWORD="+strPassword);

			Screen->Cursor = crHourGlass;

        	dmGlobal->dbGlobal->Open();

        	dmGlobal->dbMaster->Open();

        	dmGlobal->dbSearch->Open();

    	    btnConnect->Caption = "&Initialize";

        	m_nState = 2;

	    	Application->MessageBox("Connection Succeeded!\n\nClick <Initialize> to start initialization!",MSGBOXCAPTION,48);

			gbFunc1->Enabled = false;

			Screen->Cursor = crOldCursor;
    	}
		else
        //已经建立连接
		{
        	if (rbTarget2->Checked)
            {
            	if (fmConfirm->ShowModal() != mrOk)
                {
					Screen->Cursor = crOldCursor;
                	return;
                }

				if (! dmGlobal->dbGlobal->Connected)
				{
	    			Application->MessageBox("Dababase Not Opened!",MSGBOXCAPTION,48);
    				return;
				}

				//清除数据库
    			if (Application->MessageBox("All data in this database will be erased!",
        									"Warning",MB_OKCANCEL+MB_ICONQUESTION) == IDCANCEL)
        			return;

				crOldCursor = Screen->Cursor;
				Screen->Cursor = crHourGlass;

		//        fmMsg->Show();
		//        fmMsg->rxlMsg->Caption = "正在清理数据库 ......";
		//        UpdateWindow(fmMsg->Handle);

				//Drop Stored Procedure
				for (i=0;i<STOREDPROCCOUNT;i++)
				{
        			strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id(\'"+
    	    					   saStoredProcName[i]+
        						   (AnsiString)"\') and sysstat & 0xf = 4) drop procedure "+
            					   saStoredProcName[i];

					dmGlobal->qeGlobal->SQL->Clear();
    				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
        			dmGlobal->qeGlobal->ExecSQL();
				}

				//Drop Table
				for (i=0;i<TABLECOUNT;i++)
				{
        			strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id(\'"+
    	    					   saTableName[i]+
        						   (AnsiString)"\') and sysstat & 0xf = 3) drop table "+
            					   saTableName[i];

					dmGlobal->qeGlobal->SQL->Clear();
    				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
        			dmGlobal->qeGlobal->ExecSQL();
				}

		//        fmMsg->rxlMsg->Caption = "创建数据表 ......";
		//        UpdateWindow(fmMsg->Handle);

				//Create Tables and Stored Procedure

				//Object:  Table dbo.lib_BaseFields
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_BaseFields("+
							   (AnsiString)"FieldID int NOT NULL,"+
							   (AnsiString)"FieldName varchar (32) NOT NULL,"+
							   (AnsiString)"FieldType varchar (12) NOT NULL,"+
							   (AnsiString)"FieldSize int NOT NULL,"+
							   (AnsiString)"CONSTRAINT PK_lib_BaseFields_1__13 "+
							   (AnsiString)"PRIMARY KEY  CLUSTERED(FieldID) "+
							   (AnsiString)"WITH  FILLFACTOR = 90)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_ExtendFields
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_ExtendFields("+
							   (AnsiString)"CategoryID varchar (32) NOT NULL,"+
							   (AnsiString)"FieldName varchar (32) NOT NULL,"+
							   (AnsiString)"FieldType varchar (12) NOT NULL,"+
							   (AnsiString)"FieldSize int NOT NULL)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_Group
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_Group("+
							   (AnsiString)"GroupID varchar (32) NOT NULL,"+
							   (AnsiString)"GroupName varchar (32) NULL,"+
							   (AnsiString)"remark varchar (62) NULL,"+
							   (AnsiString)"GroupType int NOT NULL,"+
							   (AnsiString)"CONSTRAINT PK_lib_Group_1__13 "+
							   (AnsiString)"PRIMARY KEY  CLUSTERED(GroupID) "+
							   (AnsiString)"WITH  FILLFACTOR = 90)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_Group
				strSQLClause = (AnsiString)"CREATE UNIQUE INDEX lib_GroupIndex1 ON "+
							   (AnsiString)"dbo.lib_Group(GroupID) WITH  FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_GroupUser
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_GroupUser("+
							   (AnsiString)"GroupID varchar (32) NOT NULL,"+
							   (AnsiString)"UserID varchar (32) NOT NULL,"+
                               (AnsiString)"CONSTRAINT PK_lib_GroupUser_1__13 "+
                               (AnsiString)"PRIMARY KEY  CLUSTERED(GroupID,UserID))";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_GroupUser
				strSQLClause = (AnsiString)"CREATE INDEX lib_GroupUserIndex1 ON "+
							   (AnsiString)"dbo.lib_GroupUser(GroupID,UserID) WITH  FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();


/*
				//Object:  Table dbo.lib_Keyword
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_Keyword("+
							   (AnsiString)"Keyname varchar (32) NOT NULL,"+
							   (AnsiString)"Value varchar (32) NOT NULL)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();
*/
				//Object:  Table dbo.lib_WordBank
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_WordBank("+
                        "English varchar (62) NOT NULL ,"+
	                    "Chinese varchar (32) NULL ,"+
	                    "CONSTRAINT PK_lib_WordBank_1__10 PRIMARY KEY  NONCLUSTERED"+
	                        "( English )"+
                        ")";
				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

                //Object: Index of dbo.lib_WordBank
                strSQLClause= "CREATE  INDEX idx_lib_WordBank_Chinese ON dbo.lib_WordBank(Chinese)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_LendInfo
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_LendInfo("+
							   (AnsiString)"UserID varchar (32) NOT NULL,"+
							   (AnsiString)"ResID varchar (32) NOT NULL,"+
							   (AnsiString)"LendTime datetime NOT NULL,"+
                               (AnsiString)"DueTime datetime NOT NULL,"+
                               (AnsiString)"LendOpID varchar (32) NOT NULL,"+
							   (AnsiString)"ReturnTime datetime NULL,"+
							   (AnsiString)"ReturnOpID varchar (32) NULL,"+
							   (AnsiString)"remark varchar (62) NULL,"+
                               (AnsiString)"IsLost int NULL)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_LendInfo
				strSQLClause = (AnsiString)"CREATE INDEX lib_LendInfoIndex1 ON "+
							   (AnsiString)"dbo.lib_LendInfo(UserID) WITH  FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_LendInfo
				strSQLClause = (AnsiString)"CREATE  INDEX lib_LendInfoIndex2 ON "+
							   (AnsiString)"dbo.lib_LendInfo(ResID) WITH FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_Log
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_Log("+
							   (AnsiString)"LogID varchar (32) NOT NULL,"+
							   (AnsiString)"OpID varchar (32) NOT NULL,"+
							   (AnsiString)"OpTime datetime NOT NULL,"+
							   (AnsiString)"Operation varchar (32) NOT NULL,"+
							   (AnsiString)"ObjID varchar (32) NOT NULL,"+
							   (AnsiString)"ObjStr varchar (62) NULL,"+
							   (AnsiString)"Module varchar (32) NOT NULL,"+
							   (AnsiString)"remark varchar (62) NULL,"+
							   (AnsiString)"CONSTRAINT PK_lib_Log_1__13 "+
							   (AnsiString)"PRIMARY KEY CLUSTERED(LogID) WITH  FILLFACTOR = 60)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_Log
				strSQLClause = (AnsiString)"CREATE UNIQUE INDEX lib_LogIndex1 ON "+
							   (AnsiString)"dbo.lib_Log(LogID) WITH  FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_Log
				strSQLClause = (AnsiString)"CREATE INDEX lib_LogIndex2 ON "+
							   (AnsiString)"dbo.lib_Log(OpTime) WITH  FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_Res
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_Res("+
							   (AnsiString)"ResID varchar (32) NOT NULL,"+
							   (AnsiString)"ResType int NOT NULL,"+
							   (AnsiString)"remark varchar (62) NULL,"+
							   (AnsiString)"RegTime datetime NOT NULL,"+
							   (AnsiString)"Position varchar (34) NULL,"+
							   (AnsiString)"state int NOT NULL ,"+
							   (AnsiString)"BorrowerID varchar (32) NULL,"+
							   (AnsiString)"AstLibs varchar (102) NULL,"+
							   (AnsiString)"CONSTRAINT PK_lib_Res_1__13 "+
							   (AnsiString)"PRIMARY KEY CLUSTERED(ResID) WITH FILLFACTOR = 90)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_Res
				strSQLClause = (AnsiString)"CREATE UNIQUE INDEX lib_ResIndex1 ON "+
							   (AnsiString)"dbo.lib_Res(ResID) WITH FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_Res
				strSQLClause = (AnsiString)"CREATE INDEX lib_ResIndex2 ON "+
							   (AnsiString)"dbo.lib_Res(RegTime)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_StoryLibTree
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_StoryLibTree("+
							   (AnsiString)"NodeID varchar (32) NOT NULL,"+
							   (AnsiString)"ParentID varchar (32) NULL,"+
							   (AnsiString)"remark varchar (62) NULL,"+
							   (AnsiString)"NodeAttr int NOT NULL,"+
							   (AnsiString)"ListOrder int NOT NULL,"+
                               (AnsiString)"InputTem text NULL,"+
                               (AnsiString)"ViewTem text NULL,"+
                               (AnsiString)"SearchTem text NULL,"+
                               (AnsiString)"AdvanceTem text NULL,"+
                               (AnsiString)"ListTem text NULL,"+
							   (AnsiString)"CONSTRAINT PK_lib_StoryLibTree_1__13 "+
							   (AnsiString)"PRIMARY KEY CLUSTERED(NodeID) WITH FILLFACTOR = 90)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_SysConfig
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_SysConfig("+
							   (AnsiString)"LogKeepDays int NULL,"+
							   (AnsiString)"LendDuration int NULL,"+
                               "WWWServerIP varchar(100),"+
                               "CodeType varchar(5) NULL)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_SystemFileID
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_SystemFileID("+
							   (AnsiString)"Date datetime NOT NULL,"+
							   (AnsiString)"SerialNo int NOT NULL)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_User
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_User("+
							   (AnsiString)"UserID varchar (32) NOT NULL,"+
							   (AnsiString)"name varchar (32) NULL,"+
							   (AnsiString)"code varchar (32) NULL,"+
							   (AnsiString)"department varchar (32) NULL,"+
							   (AnsiString)"phone varchar (32) NULL,"+
							   (AnsiString)"remark varchar (62) NULL,"+
							   (AnsiString)"passwd varchar (32) NOT NULL,"+
							   (AnsiString)"config image NULL,"+
							   (AnsiString)"CONSTRAINT PK_lib_User_1__13 "+
							   (AnsiString)"PRIMARY KEY CLUSTERED(UserID) WITH FILLFACTOR = 90)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Index of Table dbo.lib_User
				strSQLClause = (AnsiString)"CREATE INDEX lib_UserIndex1 ON "+
							   (AnsiString)"dbo.lib_User(UserID) WITH FILLFACTOR = 60";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_Permission
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_Permission("+
							   (AnsiString)"GroupID varchar (255) NULL,"+
							   (AnsiString)"NodeID varchar (255) NULL,"+
							   (AnsiString)"Permission int NULL)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Table dbo.lib_ResTypes
				strSQLClause = (AnsiString)"CREATE TABLE dbo.lib_ResTypes("+
							   (AnsiString)"TypeID int NOT NULL,"+
							   (AnsiString)"TypeName varchar (32) NOT NULL,"+
							   (AnsiString)"BorrowDays int NOT NULL,"+
							   (AnsiString)"CONSTRAINT PK_lib_ResTypes_1__13 "+
							   (AnsiString)"PRIMARY KEY CLUSTERED(TypeID) WITH FILLFACTOR = 90)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

		//        fmMsg->rxlMsg->Caption = "创建存储过程 ......";
		//        UpdateWindow(fmMsg->Handle);

				//Object:  Stored Procedure dbo.ClearLog
				strSQLClause = (AnsiString)"CREATE PROCEDURE ClearLog "+
        					   (AnsiString)"AS "+
							   (AnsiString)"DECLARE	 @Duration INT,@Date DATETIME "+
							   (AnsiString)"SELECT @Date = GETDATE () "+
							   (AnsiString)"SELECT @Duration = LogKeepDays from lib_SysConfig "+
							   (AnsiString)"DELETE lib_Log where OpTime < dateadd(day,0 - @Duration,@Date)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				//Object:  Stored Procedure dbo.GetNewFileID
				strSQLClause = (AnsiString)"CREATE PROCEDURE GetNewFileID "+
        					   (AnsiString)"AS "+
							   (AnsiString)"DECLARE @FileID VARCHAR(30),@Date DATETIME,@SerialNo INT "+
							   (AnsiString)"SET NOCOUNT ON "+
							   (AnsiString)"SELECT @Date = GETDATE() "+
							   (AnsiString)"SELECT @SerialNo = 0 "+
							   (AnsiString)"UPDATE lib_SystemFileID SET "+
							   (AnsiString)"@SerialNo = SerialNo, SerialNo = SerialNo + 1 "+
							   (AnsiString)"WHERE DATEDIFF(dy,Date,@Date)=0 "+
							   (AnsiString)"IF @SerialNo = 0 "+
							   (AnsiString)"BEGIN "+
							   (AnsiString)"INSERT INTO lib_SystemFileID VALUES(@Date, 2) "+
							   (AnsiString)"SELECT @SerialNo = 1 "+
							   (AnsiString)"DELETE FROM lib_SystemFileID WHERE Date < @Date "+
							   (AnsiString)"END "+
							   (AnsiString)"SELECT @FileID = CONVERT(VARCHAR(4), DATEPART(year, @Date)) "+
							   (AnsiString)"IF DATEPART(month, @Date) < 10 "+
							   (AnsiString)"SELECT @FileID = @FileID + '0' "+
							   (AnsiString)"SELECT @FileID = @FileID + CONVERT(VARCHAR(2), DATEPART(month, @Date)) "+
							   (AnsiString)"IF DATEPART(day, @Date) < 10 "+
							   (AnsiString)"SELECT @FileID = @FileID + '0' "+
							   (AnsiString)"SELECT @FileID = @FileID + CONVERT(VARCHAR(2), DATEPART(day, @Date)) "+
							   (AnsiString)"SELECT @FileID = "+
							   (AnsiString)"CASE "+
							   (AnsiString)"WHEN @SerialNo < 10 THEN @FileID + '0000' + CONVERT(VARCHAR(5), @SerialNo) "+
							   (AnsiString)"WHEN @SerialNo < 100 THEN @FileID + '000' + CONVERT(VARCHAR(5), @SerialNo) "+
							   (AnsiString)"WHEN @SerialNo < 1000 THEN @FileID + '00' + CONVERT(VARCHAR(5), @SerialNo) "+
							   (AnsiString)"WHEN @SerialNo < 10000 THEN @FileID + '0' + CONVERT(VARCHAR(5), @SerialNo) "+
							   (AnsiString)"ELSE @FileID + CONVERT(VARCHAR(5), @SerialNo) "+
							   (AnsiString)"END "+
							   (AnsiString)"SELECT 'FileID' = @FileID "+
							   (AnsiString)"SET NOCOUNT OFF";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

		//        fmMsg->rxlMsg->Caption = "创建内置帐号 ......";
		//        UpdateWindow(fmMsg->Handle);

				//创建内置帐号
                //检查是否有这个login
                strSQLClause = (AnsiString)"select suid from master..syslogins where name = '"+
                			   l_strLoginName+
                               "'";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->Open();

                if (dmGlobal->qeGlobal->RecordCount == 0)
                //建立这个Login ,sp_addlogin不能在事务中执行
                {
					strSQLClause = (AnsiString)"sp_addlogin '"+
                    			   l_strLoginName+
                                   "','testourstrength'";

                    dmGlobal->qeGlobal->SQL->Clear();
                    dmGlobal->qeGlobal->SQL->Add(strSQLClause);
                    dmGlobal->qeGlobal->ExecSQL();
                }
                else
                {
                	//该Login已建立,得到他的SUID.
                    nSUID = dmGlobal->qeGlobal->FieldValues["suid"];

	                //检查在该数据库中是否有他的别名,如果有的话删除他的别名,
                    //每个Login在一个数据库中只能有一个别名.
                    strSQLClause = (AnsiString)"select suid from sysalternates where suid = "+
                    			   IntToStr(nSUID);

                    dmGlobal->qeGlobal->SQL->Clear();
                    dmGlobal->qeGlobal->SQL->Add(strSQLClause);
                    dmGlobal->qeGlobal->Open();

	                if (dmGlobal->qeGlobal->RecordCount>0) //有别名,删除
                    {
                    	strSQLClause = (AnsiString)"sp_dropalias "+
                        			   l_strLoginName;

                        dmGlobal->qeGlobal->SQL->Clear();
                        dmGlobal->qeGlobal->SQL->Add(strSQLClause);
                        dmGlobal->qeGlobal->ExecSQL();
                    }

                    //在主库(master)里执行上述同样的操作
                    strSQLClause = (AnsiString)"select suid from sysalternates where suid = "+
                    			   IntToStr(nSUID);

                    dmGlobal->qeMaster->SQL->Clear();
                    dmGlobal->qeMaster->SQL->Add(strSQLClause);
                    dmGlobal->qeMaster->Open();

	                if (dmGlobal->qeMaster->RecordCount>0) //有别名,删除
                    {
                    	strSQLClause = (AnsiString)"sp_dropalias "+
                        			   l_strLoginName;

                        dmGlobal->qeMaster->SQL->Clear();
                        dmGlobal->qeMaster->SQL->Add(strSQLClause);
                        dmGlobal->qeMaster->ExecSQL();
                    }

                    //在搜索引擎数据库里执行上述同样的操作
                    strSQLClause = (AnsiString)"select suid from sysalternates where suid = "+
                    			   IntToStr(nSUID);

                    dmGlobal->qeSearch->SQL->Clear();
                    dmGlobal->qeSearch->SQL->Add(strSQLClause);
                    dmGlobal->qeSearch->Open();

	                if (dmGlobal->qeSearch->RecordCount>0) //有别名,删除
                    {
                    	strSQLClause = (AnsiString)"sp_dropalias "+
                        			   l_strLoginName;

                        dmGlobal->qeSearch->SQL->Clear();
                        dmGlobal->qeSearch->SQL->Add(strSQLClause);
                        dmGlobal->qeSearch->ExecSQL();
                    }
                }

                //将新建立的这个Login在工作库内别名为dbo.
                strSQLClause = (AnsiString)"sp_addalias "+
                               l_strLoginName+
                               ",dbo";

                dmGlobal->qeGlobal->SQL->Clear();
                dmGlobal->qeGlobal->SQL->Add(strSQLClause);
                dmGlobal->qeGlobal->ExecSQL();

                //将新建立的这个Login在master库内别名为dbo.
                dmGlobal->qeMaster->SQL->Clear();
                dmGlobal->qeMaster->SQL->Add(strSQLClause);
                dmGlobal->qeMaster->ExecSQL();

                //将新建立的这个Login在搜索引擎库内别名为dbo.
                dmGlobal->qeSearch->SQL->Clear();
                dmGlobal->qeSearch->SQL->Add(strSQLClause);
                dmGlobal->qeSearch->ExecSQL();

                //将新建立的这个Login的缺省数据库指向工作库
                strSQLClause = (AnsiString)"sp_defaultdb "+
                               l_strLoginName+
                               ","+
                               m_strDbName;

               dmGlobal->qeGlobal->SQL->Clear();
               dmGlobal->qeGlobal->SQL->Add(strSQLClause);
               dmGlobal->qeGlobal->ExecSQL();

		//        fmMsg->rxlMsg->Caption = "设置相关权限 ......";
		//        UpdateWindow(fmMsg->Handle);

				//创建缺省的Admin用户
				dmGlobal->tbUser->TableName = "lib_User";
				if (! dmGlobal->tbUser->Active)
        			dmGlobal->tbUser->Open();

				strSQLClause = (AnsiString)"Insert into lib_User values('"+
            				   SQLStr(l_strAdmin)+
							   (AnsiString)"',NULL,NULL,NULL,NULL,NULL,'.',NULL)";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				dmGlobal->tbUser->Close();

				//创建缺省的Admin用户组
				dmGlobal->tbGroup->TableName = "lib_Group";
				if (! dmGlobal->tbGroup->Active)
        			dmGlobal->tbGroup->Open();

				strSQLClause = (AnsiString)"Insert into lib_Group values('"+
            				   SQLStr(l_strAdminGroup)+
							   (AnsiString)"',NULL,NULL,"+
							   String(0)+
							   (AnsiString)")";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				dmGlobal->tbGroup->Close();

				//创建缺省的Admins/Admin组用户
				dmGlobal->tbGroupUser->TableName = "lib_GroupUser";
				if (! dmGlobal->tbGroupUser->Active)
        			dmGlobal->tbGroupUser->Open();

				strSQLClause = (AnsiString)"Insert into lib_GroupUser values('"+
            				   SQLStr(l_strAdminGroup)+
							   (AnsiString)"','"+
							   SQLStr(l_strAdmin)+
							   (AnsiString)"')";

				dmGlobal->qeGlobal->SQL->Clear();
				dmGlobal->qeGlobal->SQL->Add(strSQLClause);
				dmGlobal->qeGlobal->ExecSQL();

				dmGlobal->tbGroupUser->Close();

				//初始化系统设置表
				dmGlobal->tbSysConfig->TableName = "lib_SysConfig";
				if (! dmGlobal->tbSysConfig->Active)
        			dmGlobal->tbSysConfig->Open();

				dmGlobal->tbSysConfig->Append();
				dmGlobal->tbSysConfig->FieldValues["LogKeepDays"]  = 7;
				dmGlobal->tbSysConfig->FieldValues["LendDuration"] = 1;
				dmGlobal->tbSysConfig->FieldValues["WWWServerIP"]  =  SQLStr("127.0.0.1");
				if (rbBIG5->Checked)
					dmGlobal->tbSysConfig->FieldValues["CodeType"] =  SQLStr(LANGUAGE_BIG5);
                else
					dmGlobal->tbSysConfig->FieldValues["CodeType"] =  SQLStr(LANGUAGE_GB);
				dmGlobal->tbSysConfig->Post();

				dmGlobal->tbSysConfig->Close();

                //初始化资料类型表
                for (i=0;i<MAXRESTYPES;i++)
                {
					strSQLClause = (AnsiString)"Insert into lib_ResTypes values("+
    	        				   SQLStr(saResTypes[i][0])+
								   (AnsiString)",'"+
								   SQLStr(saResTypes[i][1])+
								   (AnsiString)"',"+
                                   SQLStr(saResTypes[i][2])+
                                   (AnsiString)")";

					dmGlobal->qeGlobal->SQL->Clear();
					dmGlobal->qeGlobal->SQL->Add(strSQLClause);
					dmGlobal->qeGlobal->ExecSQL();
                }

                //初始化基本字段表
                for (i=0;i<MAXBASEFIELDS;i++)
                {
                  strSQLClause = (AnsiString)"Insert into lib_BaseFields values("+
								 String(i+1)+
                                 ",'"+
                                 SQLStr(saBaseFields[i][0])+
                                 (AnsiString)"','"+
                                 SQLStr(saBaseFields[i][1])+
                                 (AnsiString)"',"+
                                 saBaseFields[i][2]+
                                 ")";

                  dmGlobal->qeGlobal->SQL->Clear();
                  dmGlobal->qeGlobal->SQL->Add(strSQLClause);
                  dmGlobal->qeGlobal->ExecSQL();
        		}
			}
			else
			{
				crOldCursor = Screen->Cursor;
				Screen->Cursor = crHourGlass;
			}
			strINIFileName = IniName();

            //创建Library系统的INI文件
		    if (! WritePrivateProfileString("DATABASE CONNECTION","DATABASENAME",
    							            m_strDbName.c_str(),strINIFileName.c_str()))
		    {
			   	Application->MessageBox("Write System INI File Failed!",MSGBOXCAPTION,48);
				Screen->Cursor = crOldCursor;
				return;
		    }

		    if (! WritePrivateProfileString("DATABASE CONNECTION","SERVERNAME",
    							            m_strServerName.c_str(),strINIFileName.c_str()))
		    {
			   	Application->MessageBox("Write System INI File Failed!",MSGBOXCAPTION,48);
				Screen->Cursor = crOldCursor;
				return;
		    }

		    if (! WritePrivateProfileString("DATABASE CONNECTION","USERNAME",
    							            USERNAME,strINIFileName.c_str()))
		    {
			   	Application->MessageBox("Write System INI File Failed!",MSGBOXCAPTION,48);
				Screen->Cursor = crOldCursor;
				return;
		    }

		    if (! WritePrivateProfileString("DATABASE CONNECTION","PASSWORD",
    							            PASSWORD,strINIFileName.c_str()))
		    {
			   	Application->MessageBox("Write System INI File Failed!",MSGBOXCAPTION,48);
				Screen->Cursor = crOldCursor;
				return;
		    }

            if (rbRetrieve->Checked)
            {
               strCacheSize = meCacheSize->Text.Trim();

            if (! WritePrivateProfileString("DATABASE CONNECTION","CacheNumber",
                                               strCacheSize.c_str(),strINIFileName.c_str()))
               {
                   Application->MessageBox("Write System INI File Failed!",MSGBOXCAPTION,48);
                   Screen->Cursor = crOldCursor;
                   return;
               }
            }

            m_nState = 3;
		}
	}
    catch(...)
    {
    	if (m_nState == 1)
	    	Application->MessageBox("Connection Failed!",MSGBOXCAPTION,48);
        else
			Application->MessageBox("Database Access Failed!",MSGBOXCAPTION,48);

		Screen->Cursor = crOldCursor;

        return;
    }

    if (m_nState == 3)
    {
       //检测搜索引擎
       try
       {
          strSQLClause = (AnsiString)"xpBuildWordIndexFile '"+
                         LANGUAGE_B5+
                         "'";

          dmGlobal->qeMaster->SQL->Clear();
          dmGlobal->qeMaster->SQL->Add(strSQLClause);
          dmGlobal->qeMaster->ExecSQL();

          strSQLClause = (AnsiString)"xpConfig 90000000,90000";

          dmGlobal->qeMaster->SQL->Clear();
          dmGlobal->qeMaster->SQL->Add(strSQLClause);
          dmGlobal->qeMaster->ExecSQL();
       }
       catch(...)
       {
           if (m_nState == 1)
               Application->MessageBox("Connection Failed!",MSGBOXCAPTION,48);
           else
               Application->MessageBox("Please install Search Engine before running!",MSGBOXCAPTION,48);
       }

       Application->MessageBox("Initialization Compeleted!",MSGBOXCAPTION,48);
    }

    Screen->Cursor = crOldCursor;
}
//---------------------------------------------------------------------------
void __fastcall TfmInit::FormKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	if (Key == VK_RETURN)
		FindNextControl(ActiveControl,true,true,true)->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfmInit::rbTarget2Click(TObject *Sender)
{
	Application->MessageBox("All Data in the specified database will be erased!","Warning",48);
}
//---------------------------------------------------------------------------
void __fastcall TfmInit::FormShow(TObject *Sender)
{
	edServerName->SetFocus();
}
//---------------------------------------------------------------------------
AnsiString TfmInit::IniName(void)
{
     AnsiString strINIFileName;
     bool bExisted;
     int i,nLength;
     char * pName;
     char pBuffer[MAX_PATH];

     if (rbRetrieve->Checked)
     {
		if (GetSystemDirectory(pBuffer,MAX_PATH) != 0)
        {
            strINIFileName = pBuffer;

            if (pBuffer[strINIFileName.Length()-1] == '\\')
                strINIFileName = strINIFileName + l_strLibraryINIFile;
            else
                strINIFileName = strINIFileName + "\\" + l_strLibraryINIFile;
        }
        else
        {
			Application->MessageBox("Get system directory failed,\n\nUse the current directory instead!","Warning",48);
			strINIFileName = l_strLibraryINIFile;
        }
     }
     else
     {
        strINIFileName = Application->ExeName;
        pName          = strINIFileName.c_str();

        bExisted = false;
        nLength  = strINIFileName.Length();
        for (i=nLength-1;i--;i>=0)
        {
           if (pName[i] == '\\')
           {
               bExisted = true;
               break;
           }
        }

        if (bExisted)
        {
           strINIFileName = strINIFileName.SubString(1,i+1)+
                            l_strLibraryINIFile;
        }
        else
           strINIFileName = l_strLibraryINIFile;
     }

     return strINIFileName;
}
//---------------------------------------------------------------------------
/*
void __fastcall TfmInit::btnINIPathClick(TObject *Sender)
{
	AnsiString strPath;

	strPath = browseForFolder(this->Handle);

    if (! strPath.IsEmpty())
    	edINIPath->Text = strPath;
}
*/
//---------------------------------------------------------------------------
AnsiString TfmInit::browseForFolder(HWND hParentWnd)
{
	BROWSEINFO BrowseInfo;
	TCHAR szFile[MAX_PATH];       //foldername缓冲区
	TCHAR szPath[MAX_PATH];       //filepath缓冲区
	LPMALLOC g_pMalloc;
	LPITEMIDLIST pidlBrowse;
    AnsiString strFolder;

    //清空当前路径
    strFolder = "";

    //获取IMALLOC指针
	SHGetMalloc(&g_pMalloc);

	LPITEMIDLIST  pidl;
	LPSHELLFOLDER pDesktopFolder;
	char          szRootPath[MAX_PATH];
	OLECHAR       olePath[MAX_PATH];
	ULONG         chEaten;
	ULONG         dwAttributes;
	HRESULT       hr;

    //初始化根目录
	BrowseInfo.pidlRoot = NULL;

	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		//IShellFolder::ParseDisplayName需要Unicode类型的文件名
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,szRootPath,-1,olePath,MAX_PATH);

		//把路径转换成ITEMIDLIST
		hr = pDesktopFolder->ParseDisplayName(NULL,NULL,olePath,
											  &chEaten,&pidl,&dwAttributes);
/*
		if (! FAILED(hr))
		{
			BrowseInfo.pidlRoot = pidl;
		}
*/        
	}
    else
        return strFolder;

	//初始化BROWSEINFO
	BrowseInfo.hwndOwner	  = hParentWnd;
	BrowseInfo.pszDisplayName = szFile;
	BrowseInfo.lpszTitle	  = "Browse for folder";
	BrowseInfo.ulFlags		  = BIF_RETURNONLYFSDIRS;
	BrowseInfo.lpfn			  = NULL;
	BrowseInfo.lParam		  = 0;
	BrowseInfo.iImage		  = 1;

    //打开目录对话框
	pidlBrowse = SHBrowseForFolder(&BrowseInfo);

    //返回值为NULL，表示没有选定目录
    if (pidlBrowse != NULL)
	{
        //把当前ITEM转换成系统路径
        if (SHGetPathFromIDList(pidlBrowse, szPath))
		{
			strFolder = szPath;
		}

        //释放SHBrowseForFolder.返回的item identifier指针
        g_pMalloc->Free(pidlBrowse);
	}

    return strFolder;
}
//---------------------------------------------------------------------------
/*
void __fastcall TfmInit::cbINIPathClick(TObject *Sender)
{
	if (cbINIPath->Checked)
    {
    	edINIPath->Enabled  = true;
        edINIPath->Clear();
        btnINIPath->Enabled = true;
    }
    else
    {
    	edINIPath->Enabled  = false;
        edINIPath->Clear();
        btnINIPath->Enabled = false;
    }
}
*/
//---------------------------------------------------------------------------
void __fastcall TfmInit::cbInitialFullTextClick(TObject *Sender)
{
	if (rbRetrieve->Checked)
    {
        meCacheSize->Enabled = true;
        meCacheSize->Text    = DEFAULTCACHESIZE;
    }
    else
    {
        meCacheSize->Clear();
        meCacheSize->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmInit::meCacheSizeExit(TObject *Sender)
{
	int nCacheSize;
    AnsiString strWarning,strCacheSize;

    strWarning = (AnsiString)"Cache Size must between "+
                 String(MINCACHESIZE)+
                 " and "+
                 String(MAXCACHESIZE);

	strCacheSize = meCacheSize->Text.Trim();

    if (strCacheSize.IsEmpty())
    {
    	Application->MessageBox(strWarning.c_str(),MSGBOXCAPTION,48);
        meCacheSize->SetFocus();
        return;
    }

	nCacheSize = StrToInt(strCacheSize);

    if ((nCacheSize < MINCACHESIZE)||(nCacheSize > MAXCACHESIZE))
    {
    	Application->MessageBox(strWarning.c_str(),MSGBOXCAPTION,48);
        meCacheSize->SetFocus();
        return;
    }
}
//---------------------------------------------------------------------------
