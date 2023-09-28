//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "utGlobDm.h"
#include "TemplateForm.h"
#include "WizardFrm.h"
#include "ImportFrm.h"
#include "CommFunc.h"
#include "utNewExt.h"
#include "utAuth.h"
#include "StoryData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define COMMONUSERGROUP -1

int g_naFieldTypeSize[MAXFIELDTYPES] = {80,4,16,8,16};

TfrmTemplate *frmTemplate;
//---------------------------------------------------------------------------
__fastcall TfrmTemplate::TfrmTemplate(TComponent* Owner)
    : TForm(Owner)
{
    m_strDatabaseName = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmTemplate::tbbtnAddClick(TObject *Sender)
{
/*
  AnsiString strDirectory;
  strDirectory=GetDirectory(trvwTemplate);
  frmWizard->cboDirectory->Text=strDirectory;
  if (CheckStrings(frmWizard->cboDirectory,strDirectory))
    frmWizard->cboDirectory->Items->Add(strDirectory);
*/
    if (trvwTemplate->Items->Count>0)
    {
      if ((GetFocus()!=trvwTemplate->Handle)||(! trvwTemplate->Selected))
      {
		  Application->MessageBox(L"You should select a node first !",GLOBALMSGCAPTION,48);
          return;
	  }
    }
    frmWizard->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TfrmTemplate::muiPopImportClick(TObject *Sender)
{
  frmInout->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::muiPopRenameClick(TObject *Sender)
{
	if ((GetFocus()!=trvwTemplate->Handle)||(! trvwTemplate->Selected))
	{
		Application->MessageBox(L"You should select a node first !",GLOBALMSGCAPTION,48);
        return;
	}
    trvwTemplate->ReadOnly = false;
	trvwTemplate->Selected->EditText();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::tbbtnBrowseClick(TObject *Sender)
{
    m_nTemplateType = BROWSETEMPLATE;

    ModifyTemplate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::tbbtnSearchClick(TObject *Sender)
{
    m_nTemplateType = SEARCHTEMPLATE;

    ModifyTemplate();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTemplate::tbbtnAdvanceClick(TObject *Sender)
{
    m_nTemplateType = ADVANCEDTEMPLATE;

	ModifyTemplate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::trvwTemplateChange(TObject *Sender,
	  TTreeNode *Node)
{
	AnsiString strSQLClause,strNodeID,strDir;
	TTreeNode * tnParentNode;
	TListItem* pListItem;
	int nRetVal;

	strDir = (AnsiString)"";
	tnParentNode = Node->Parent;
	while (tnParentNode)
	{
        strDir.Insert(tnParentNode->Text+(AnsiString)"\\",0);
        tnParentNode = tnParentNode->Parent;
    }
    strDir = strDir+Node->Text;

    strNodeID = (AnsiString)((char *)Node->Data);

	if (frmWizard->cboDirectory->Items->Count > 1)
        frmWizard->cboDirectory->Items->Delete(1);

    strSQLClause = (AnsiString)"select * from "+
    			   (AnsiString)strLIBEXTENDFIELDS+
				   (AnsiString)" where CategoryID = '"+
                   SQLStr(strNodeID)+
                   (AnsiString)"'";

    try
    {
	    qeInternal->SQL->Clear();
		qeInternal->SQL->Add(strSQLClause);
	    qeInternal->Open();

        if (qeInternal->RecordCount == 0)
        {
            mniRemoveField->Enabled = false;
            mniModifyField->Enabled = false;
        }
        else
        {
            mniRemoveField->Enabled = true;
            mniModifyField->Enabled = true;
        }

		lstvwFields->Items->Clear();
        qeInternal->First();
		while (! qeInternal->Eof)
        {
            pListItem = lstvwFields->Items->Add();
			pListItem->Caption = OrigStr(qeInternal->FieldValues["FieldName"]);

			pListItem->SubItems->Add(OrigStr(qeInternal->FieldValues["FieldType"]));
			pListItem->SubItems->Add(qeInternal->FieldValues["FieldSize"]);

			qeInternal->Next();
		}

		//检查是否为叶节点
		nRetVal = IsLeafNode(strNodeID);

		if (nRetVal == DATABASEERROR)
		//数据库错误
		{
		  Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		  return;
		}

		if (nRetVal == LEAFNODE)
		//叶节点
		{
			//叶节点不能创建子节点
			frmWizard->cboDirectory->ItemIndex = 0;

			//叶节点允许修改界面模板
			tbbtnInput->Enabled    = true;
			tbbtnBrowse->Enabled   = true;
			tbbtnSearch->Enabled   = true;
			tbbtnAdvance->Enabled  = true;
			tbbtnList->Enabled     = true;
            muiPopInput->Enabled   = true;
            muiPopBrowse->Enabled  = true;
            muiPopSearch->Enabled  = true;
            muiPopAdvance->Enabled = true;
            muiPopList->Enabled    = true;
            muiInput->Enabled   = true;
            muiBrowse->Enabled  = true;
            muiSearch->Enabled  = true;
            muiAdvance->Enabled = true;
            muiList->Enabled    = true;


            //叶节点允许创建相关表
//            muiCreateTable->Enabled = true;
        }
        else
        {
            //设置当前节点
            frmWizard->cboDirectory->Items->Add(strDir);
            frmWizard->cboDirectory->ItemIndex = 1;

            tbbtnInput->Enabled    = false;
            tbbtnBrowse->Enabled   = false;
            tbbtnSearch->Enabled   = false;
            tbbtnAdvance->Enabled  = false;
            tbbtnList->Enabled     = false;            
            muiPopInput->Enabled   = false;
            muiPopBrowse->Enabled  = false;
            muiPopSearch->Enabled  = false;
            muiPopAdvance->Enabled = false;
            muiPopList->Enabled    = false;
            muiInput->Enabled   = false;
            muiBrowse->Enabled  = false;
            muiSearch->Enabled  = false;
            muiAdvance->Enabled = false;
            muiList->Enabled    = false;


            tbbtnAdd->Enabled       = true;
            muiPopTemplate->Enabled = true;

//            muiCreateTable->Enabled = false;
        }
    }
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------
AnsiString TfrmTemplate::GetDirectory(TTreeView *pTreeView)
{
  AnsiString strDirectory;
  TTreeNode *pTreeNode;
  pTreeNode=pTreeView->Selected;
  strDirectory=pTreeNode->Text;
  for (int i=0;i<pTreeView->Selected->Level;i++)
  {
    strDirectory.Insert(pTreeNode->Parent->Text+(AnsiString)"\\",0);
    pTreeNode=pTreeNode->Parent;
  }
  return strDirectory;
}
bool TfrmTemplate::SetDirectory(TTreeView *pTreeview,TTreeNode *pTreeNode,AnsiString strDirectory)
{
  return true;
}
bool TfrmTemplate::CheckStrings(TComboBox *pCombox,AnsiString str)
{
  for (int i=0;i<pCombox->Items->Count;i++)
  {
    if (str==pCombox->Items->Strings[i])
      return false;
  }
  return true; 
}


void __fastcall TfrmTemplate::FormCreate(TObject *Sender)
{
    dmTemplate->tbCategory->TableName   = "lib_StoryLibTree";
    dmTemplate->tbPermission->TableName = "lib_Permission";
    dmTemplate->tbUser->TableName       = "lib_User";
    dmTemplate->tbGroup->TableName      = "lib_Group";
    dmTemplate->tbGroupUser->TableName  = "lib_GroupUser";
    dmTemplate->tbExtFields->TableName  = "lib_ExtendFields";
    dmTemplate->tbBaseFields->TableName = "lib_BaseFields";
//    dmTemplate->tbTemp->TableName       = "lib_GroupUser";
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::mniRemoveFieldClick(TObject *Sender)
{
    AnsiString strNodeID,strSQLClause;

    if (! trvwTemplate->Selected)
    {
    	Application->MessageBox(L"You should specify a node first!",GLOBALMSGCAPTION,48);
        return;
    }

    if (lstvwFields->Items->Count <= 0)
        return;

    if (! lstvwFields->Selected)
        return;

    strNodeID = (AnsiString)((char *)trvwTemplate->Selected->Data);

    strSQLClause = "delete from lib_ExtendFields where CategoryID = '"+
                   SQLStr(strNodeID)+
                   "' and FieldName = '"+
                   SQLStr(lstvwFields->Selected->Caption)+
                   "'";

    try
    {
	    dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
	    dmTemplate->qeGlobal->ExecSQL();
    }
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }

    lstvwFields->Selected->Delete();

    if (lstvwFields->Items->Count <= 0)
    {
        mniRemoveField->Enabled = false;

        mniModifyField->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTemplate::mniNewFieldClick(TObject *Sender)
{
    if (! trvwTemplate->Selected)
    {
    	Application->MessageBox(L"You should specify a node first!",GLOBALMSGCAPTION,48);
        return;
    }

    fmEditExtFields->m_nEditState = 1;

    fmEditExtFields->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTemplate::mniModifyFieldClick(TObject *Sender)
{
    if (! trvwTemplate->Selected)
    {
    	Application->MessageBox(L"You should specify a node first!",GLOBALMSGCAPTION,48);
        return;
    }

    if (lstvwFields->Items->Count <= 0)
        return;

    if (! lstvwFields->Selected)
        return;

    fmEditExtFields->m_nEditState = 2;

    fmEditExtFields->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTemplate::tbbtnDeleteClick(TObject *Sender)
{
	AnsiString strSQLClause,strNodeID,strWarning;
	TTreeNode * tnRootNode,* tnCurrentNode,* tnParentNode;
	TStringList * slStack;
	int nNodeAttr,nIndexInTreeView;

	if ((GetFocus()!=trvwTemplate->Handle)||(! trvwTemplate->Selected))
	{
		Application->MessageBox(L"You should select a node first !",GLOBALMSGCAPTION,48);
        return;
	}

	tnRootNode = trvwTemplate->Selected;
	strNodeID = (AnsiString)((char *)tnRootNode->Data);

    strSQLClause = (AnsiString)"select NodeAttr from "+
	               strSTORYLIBTREE+
		           " where NodeID = '"+
                   SQLStr(strNodeID)+
                   (AnsiString)"'";

	slStack = NULL;

	try
   {
	   dmTemplate->qeGlobal->SQL->Clear();
	   dmTemplate->qeGlobal->SQL->Add(strSQLClause);
	   dmTemplate->qeGlobal->Open();

	   if (dmTemplate->qeGlobal->RecordCount <= 0)
	   //没有找到该节点，数据库不一致
		   return;

		nNodeAttr = (int)dmTemplate->qeGlobal->FieldValues["NodeAttr"];
		if (nNodeAttr == 1)
		//叶节点
			strWarning = (AnsiString)"Are you sure to remove this leaf node and its assocaited table?";
		else
			strWarning = (AnsiString)"Are you sure to remove this node and all its child nodes?";

		if (Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION)
        			== ID_OK)
		{
			slStack = new TStringList;

			//后序周游以tnRootNode为根的子树，并释放各节点的Data空间
			if (tnRootNode->Count == 0)	//无子女
			{
				if (nNodeAttr == 1)
				//删除叶节点所关联的数据表
				{
					RemoveAssociatedTable(strNodeID);
				}

				RemoveNode(strNodeID);

				//释放节点额外数据空间
				delete tnRootNode->Data;
			}
			else
    		{
        		//父节点入栈
				tnParentNode = tnRootNode;
				slStack->Add(String((int)tnParentNode->ItemId));

				//把父节点的第一个子节点作为当前节点
				tnCurrentNode = tnParentNode->getFirstChild();

				while (slStack->Count > 0)	//栈不空
				{
					if (! tnCurrentNode)	//没有兄弟节点
					{
						//父节点出栈
						nIndexInTreeView =
							atoi(AnsiString(slStack->Strings[slStack->Count-1]).c_str());
						tnCurrentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);
						slStack->Delete(slStack->Count-1);

						//从资料树节点表中删除当前节点
						strNodeID = (AnsiString)((char *)tnCurrentNode->Data);

						RemoveNode(strNodeID);

						delete tnCurrentNode->Data;

						if (slStack->Count > 0)
    					{
							//栈顶为当前节点的父节点
							nIndexInTreeView =
								atoi(AnsiString(slStack->Strings[slStack->Count-1]).c_str());
							tnParentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);

							tnCurrentNode = tnParentNode->GetNextChild(tnCurrentNode);
						 }

						continue;
					}

					if (tnCurrentNode->Count == 0)	//当前节点没有子女
    				{
						strNodeID = (AnsiString)((char *)tnCurrentNode->Data);

						//确定当前节点是不是叶节点
						strSQLClause = (AnsiString)"select NodeAttr from "+
									   strSTORYLIBTREE+
									   " where NodeID = '"+
									   SQLStr(strNodeID)+
									   (AnsiString)"'";

					   dmTemplate->qeGlobal->SQL->Clear();
					   dmTemplate->qeGlobal->SQL->Add(strSQLClause);
					   dmTemplate->qeGlobal->Open();

					   if (dmTemplate->qeGlobal->RecordCount <= 0)
					   //没有找到该节点，数据库不一致
						   return;

						nNodeAttr = (int)dmTemplate->qeGlobal->FieldValues["NodeAttr"];

						if (nNodeAttr == 1)
						{
							RemoveAssociatedTable(strNodeID);
						}

						RemoveNode(strNodeID);

						delete tnCurrentNode->Data;

						//前进至下一个兄弟节点
    					tnCurrentNode = tnParentNode->GetNextChild(tnCurrentNode);
        			}
            		else	//有子女
					{
						//当前节点入栈
						tnParentNode = tnCurrentNode;
						slStack->Add(String((int)tnParentNode->ItemId));

						//前进至当前节点的第一个子节点
						tnCurrentNode = tnParentNode->getFirstChild();
        			}
				}
			}

			delete slStack;

            //更新存储过程
            UpdateStoryLibrary();

            tnRootNode->Delete();

            lstvwFields->Items->Clear();
		}
	}
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);

		if (slStack != NULL)
			delete slStack;

        return;
    }
}
//---------------------------------------------------------------------------
void TfrmTemplate::RemoveNode(AnsiString strNodeID)
{
	AnsiString strSQLClause;

	//从资料树表中删除该节点
	strSQLClause = (AnsiString)"delete from "+
				   strSTORYLIBTREE+
				   " where NodeID = '"+
				   SQLStr(strNodeID)+
				   "'";

	try
	{
		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();

		//从扩展字段表中删除该节点所关联的所有字段
		strSQLClause = (AnsiString)"delete from "+
					   strLIBEXTENDFIELDS+
					   " where CategoryID = '"+
					   SQLStr(strNodeID)+
					   "'";

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();
	}
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);

        return;
    }
}
//---------------------------------------------------------------------------
void TfrmTemplate::RemoveAssociatedTable(AnsiString strNodeID)
{
	AnsiString strSQLClause;
    int        i,nCurrentYear;

    try
    {
		m_strDatabaseName = lblDBName->Caption;

		if (! m_strDatabaseName.IsEmpty())
		//删除相关索引
		{
			strSQLClause = (AnsiString)"master..xpDropTextIndex '"+
						   m_strDatabaseName+
						   ".dbo.lib_old"+
						   strNodeID+
						   "'";

			dmTemplate->qeGlobal->SQL->Clear();
			dmTemplate->qeGlobal->SQL->Add(strSQLClause);
			dmTemplate->qeGlobal->ExecSQL();
		}
	}
	catch(...)
	{
		Application->MessageBox(L"Failed to remove text index !",GLOBALMSGCAPTION,48);
	}

	try
	{
	  //查询服务器上显示的年份
	  strSQLClause = "select year = datepart(yy,getdate())";

	  dmTemplate->qeGlobal->SQL->Clear();
	  dmTemplate->qeGlobal->SQL->Add(strSQLClause);
	  dmTemplate->qeGlobal->Open();

	  dmTemplate->qeGlobal->First();
	  nCurrentYear = dmTemplate->qeGlobal->FieldValues["year"];
	}
	catch(...)
	{
		Application->MessageBox(L"Failed to get system date !",GLOBALMSGCAPTION,48);
		return;
	}

	for (i=1990;i<=nCurrentYear+1;i++)
	{

		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
							   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;


		strSQLClause = (AnsiString)"master..xpDropTextIndex '"+
					   m_strDatabaseName+
					   ".dbo.lib_"+
					   String(i)+
					   "_"+
					   strNodeID+
					   "'";

		try
		{
			dmTemplate->qeGlobal->SQL->Clear();
			dmTemplate->qeGlobal->SQL->Add(strSQLClause);
			dmTemplate->qeGlobal->ExecSQL();
		}
		catch(...)
		{
			continue;
		}
	}

	//删除叶节点所关联的所有数据表

	try
	{
		//删除Old表
		strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_old"+
					   strNodeID+
					   "') and sysstat & 0xf = 3) drop table lib_old"+
					   strNodeID;

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();

		//删除New表
		strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_new"+
					   strNodeID+
					   "') and sysstat & 0xf = 3) drop table lib_new"+
					   strNodeID;

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();

		//删除年份表
		for (i=1990;i<=nCurrentYear+1;i++)
		{
			strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_"+
						   String(i)+
						   "_"+
						   strNodeID+
						   "') and sysstat & 0xf = 3) drop table lib_"+
						   String(i)+
						   "_"+
						   strNodeID;

			dmTemplate->qeGlobal->SQL->Clear();
			dmTemplate->qeGlobal->SQL->Add(strSQLClause);
			dmTemplate->qeGlobal->ExecSQL();
		}
	}
	catch(...)
	{
		Application->MessageBox(L"Failed to remove associated table !",GLOBALMSGCAPTION,48);

		return;
	}
}
//---------------------------------------------------------------------------
/*
void __fastcall TfrmTemplate::muiCreateTableClick(TObject *Sender)
{
	AnsiString strNodeID;

	if (! trvwTemplate->Selected)
	{
		Application->MessageBox(L"You should specify a node first!",GLOBALMSGCAPTION,48);
		return;
	}

	strNodeID = (AnsiString)((char *)trvwTemplate->Selected->Data);

	CreateTable(strNodeID,true);
}
*/
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*
void __fastcall TfrmTemplate::tbbCopyClick(TObject *Sender)
{
	AnsiString strSQLClause,strNodeID;

	if (! trvwTemplate->Selected)
    {
    	Application->MessageBox(L"You should specify a node first!",GLOBALMSGCAPTION,48);
        return;
    }

    m_nNodeID = (int)(trvwTemplate->Selected->ItemId);

    if (! tbbPaste->Enabled)
        tbbPaste->Enabled = true;
}
*/
//---------------------------------------------------------------------------
/*
void __fastcall TfrmTemplate::tbbPasteClick(TObject *Sender)
{
    TTreeNode * tnExportedNode;
    AnsiString strSourceNodeID,strDestNodeID,strSQLClause;
    AnsiString strFieldName,strFieldType,strFieldSize;
	TLocateOptions LocateOption;
    int nCount = 0;    

    if (! trvwTemplate->Selected)
    {
    	Application->MessageBox(L"You should specify a node first!",GLOBALMSGCAPTION,48);
        return;
    }

    if (m_nNodeID < 0)
    {
    	Application->MessageBox(L"Source Node Not Exist!",GLOBALMSGCAPTION,48);
        return;
    }

    tnExportedNode = trvwTemplate->Items->GetNode((HTREEITEM)m_nNodeID);
    if (! tnExportedNode)
    {
    	Application->MessageBox(L"Source Node Not Exist!",GLOBALMSGCAPTION,48);
        return;
    }

    strSourceNodeID = (AnsiString)((char *)tnExportedNode->Data);
    strDestNodeID   = (AnsiString)((char *)trvwTemplate->Selected->Data);

    if (strSourceNodeID == strDestNodeID)
    //源节点与目标节点相同
        return;

    //检索源节点的扩展字段
    strSQLClause = (AnsiString)"select * from "+
                   strLIBEXTENDFIELDS+
                   " where CategoryID = '"+
                   SQLStr(strSourceNodeID)+
                   "'";

    try
    {
        qeInternal2->SQL->Clear();
        qeInternal2->SQL->Add(strSQLClause);
        qeInternal2->Open();

        qeInternal2->First();

        if (qeInternal2->RecordCount <= 0)
        {
        	Application->MessageBox(L"No Fields for paste!",GLOBALMSGCAPTION,48);
            return;
        }

        //检索当前节点的扩展字段
        strSQLClause = "select FieldName from "+
                       (AnsiString)strLIBEXTENDFIELDS+
                       " where CategoryID = '"+
                       SQLStr(strDestNodeID)+
                       "'";

        qeInternal->SQL->Clear();
        qeInternal->SQL->Add(strSQLClause);
        qeInternal->Open();

		//确定Locate操作的参数
		LocateOption << loCaseInsensitive;

        while (! qeInternal2->Eof)
        {
            //读出源节点的扩展字段
            strFieldName = OrigStr(qeInternal2->FieldValues["FieldName"]);
            strFieldType = OrigStr(qeInternal2->FieldValues["FieldType"]);
            strFieldSize = qeInternal2->FieldValues["FieldSize"];

            //确定源节点的扩展字段名是否与目标节点的扩展字段重名
            if (qeInternal->Locate("FieldName",SQLStr(strFieldName),LocateOption))
            {
                qeInternal2->Next();
                continue;
            }

            //增加扩展字段
            strSQLClause = "insert into "+
                           (AnsiString)strLIBEXTENDFIELDS+
                           " values('"+
                           SQLStr(strDestNodeID)+
                           "','"+
                           SQLStr(strFieldName)+
                           "','"+
                           SQLStr(strFieldType)+
                           "',"+
                           strFieldSize+
                           ")";

           dmTemplate->qeGlobal->SQL->Clear();
           dmTemplate->qeGlobal->SQL->Add(strSQLClause);
           dmTemplate->qeGlobal->ExecSQL();

            lstvwFields->Items->Add();
            lstvwFields->Items->Item[lstvwFields->Items->Count-1]->Caption = strFieldName;

            lstvwFields->Items->Item[lstvwFields->Items->Count-1]->SubItems->Add(strFieldType);
            lstvwFields->Items->Item[lstvwFields->Items->Count-1]->SubItems->Add(strFieldSize);

            qeInternal2->Next();

            nCount++;
        }

        if (nCount > 0)
            CreateTable(strDestNodeID,false);
    }
    catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
*/
//---------------------------------------------------------------------------
void TfrmTemplate::CreateTable(AnsiString strNodeID,bool bToCreate)
{
    bool bIsExist;
    AnsiString strSQLClause,strBaseFieldsSQLClause,strExtFieldsSQLClause,strWarning;
    AnsiString strFieldName,strFieldType,strFieldSize;
    int i;

    try
    {
        bIsExist = false;

        strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_old"+
                       strNodeID+
                       (AnsiString)"')";

        dmTemplate->qeGlobal->SQL->Clear();
        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
        dmTemplate->qeGlobal->Open();

        if (dmTemplate->qeGlobal->RecordCount > 0)
            bIsExist = true;
/*
        else
        {
            strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_new"+
                           strNodeID+
                           (AnsiString)"')";

            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->Open();

            if (dmTemplate->qeGlobal->RecordCount > 0)
                bIsExist = true;
        }
*/
        if (! bIsExist)
        //关联库不存在
        {
            if (! bToCreate)
                return;

		    if (Application->MessageBox(L"Create a new table with current fields?",
            	GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION) == IDOK)
            {
				return;
		    }

            //删除New库（若存在）
            strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_new"+
                           strNodeID+
                           (AnsiString)"') and sysstat & 0xf = 3) drop table "+
                           (AnsiString)"lib_new"+
                           strNodeID;

            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->ExecSQL();
        }
        else
        {
            strSQLClause = (AnsiString)"select * from lib_old"+
                           strNodeID;

            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->Open();

            if (dmTemplate->qeGlobal->RecordCount > 0)
            {
                Application->MessageBox(L"There's data in current table,create failed!",
                    GLOBALMSGCAPTION,48);

                return;
            }
            else
            {
            	strWarning = (AnsiString)"A table associated with current category already exists,\n\n"+
                             (AnsiString)"Overwrite it?";

				if (Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION) == IDOK)
                {
                    strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_old"+
                    			   strNodeID+
                                   (AnsiString)"') and sysstat & 0xf = 3) drop table "+
                                   (AnsiString)"lib_old"+
								   strNodeID;

                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_new"+
                    			   strNodeID+
                                   (AnsiString)"') and sysstat & 0xf = 3) drop table "+
                                   (AnsiString)"lib_new"+
                                   strNodeID;

                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();
                }
                else
                    return;
            }
        }

        //组织扩展字段部分的子查询
        for (i=0;i<lstvwFields->Items->Count;i++)
        {
             strFieldName = lstvwFields->Items->Item[i]->Caption;
             strFieldType = lstvwFields->Items->Item[i]->SubItems->Strings[0];
             strFieldSize = lstvwFields->Items->Item[i]->SubItems->Strings[1];

             strExtFieldsSQLClause = strExtFieldsSQLClause+
                                     strFieldName+
                                     (AnsiString)" ";

             if (strFieldType == (AnsiString)typeVARCHAR)
             {
                  strExtFieldsSQLClause = strExtFieldsSQLClause +
                                          strFieldType+
                                          (AnsiString)" ("+
                                          strFieldSize+
                                          (AnsiString)") NULL,";
             }
             else
             {
                  strExtFieldsSQLClause = strExtFieldsSQLClause +
                                          strFieldType+
                                          (AnsiString)" NULL,";
             }
        }

        //组织基本字段部分的子查询
        strSQLClause = (AnsiString)"select * from "+
                       strLIBBASEFIELDS;

        dmTemplate->qeGlobal->SQL->Clear();
        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
        dmTemplate->qeGlobal->Open();

        strBaseFieldsSQLClause = "";
        dmTemplate->qeGlobal->First();
        while (! dmTemplate->qeGlobal->Eof)
        {
          strBaseFieldsSQLClause = strBaseFieldsSQLClause+
								   dmTemplate->qeGlobal->FieldValues["FieldName"]+
                                   (AnsiString)" ";

		  strFieldType = dmTemplate->qeGlobal->FieldValues["FieldType"];

		  if (strFieldType == (AnsiString)typeVARCHAR)
		  {
              strBaseFieldsSQLClause = strBaseFieldsSQLClause +
                                       strFieldType+
                                       (AnsiString)" ("+
                                       String((int)dmTemplate->qeGlobal->FieldValues["FieldSize"])+
                                       (AnsiString)") NULL,";
          }
          else
          {
              strBaseFieldsSQLClause = strBaseFieldsSQLClause +
                                       strFieldType+
                                       (AnsiString)" NULL,";
          }

          dmTemplate->qeGlobal->Next();
        }

        if ((! strBaseFieldsSQLClause.IsEmpty())||(! strExtFieldsSQLClause.IsEmpty()))
        {
            strSQLClause = (AnsiString)"CREATE TABLE lib_old"+
                           strNodeID+
                           "(";

            if (! strBaseFieldsSQLClause.IsEmpty())
              strSQLClause = strSQLClause + strBaseFieldsSQLClause;

            if (! strExtFieldsSQLClause.IsEmpty())
              strSQLClause = strSQLClause + strExtFieldsSQLClause;

            //删除结尾的","
            strSQLClause.Delete(strSQLClause.Length(),1);

            //给结尾加上括号
            strSQLClause = strSQLClause + ")";

            //执行SQL语句
            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->ExecSQL();

            strSQLClause = (AnsiString)"CREATE TABLE lib_new"+
                           strNodeID+
                           "(";

            if (! strBaseFieldsSQLClause.IsEmpty())
              strSQLClause = strSQLClause + strBaseFieldsSQLClause;

            if (! strExtFieldsSQLClause.IsEmpty())
              strSQLClause = strSQLClause + strExtFieldsSQLClause;

            //删除结尾的","
            strSQLClause.Delete(strSQLClause.Length(),1);

            //给结尾加上括号
            strSQLClause = strSQLClause + ")";

            //执行SQL语句
            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->ExecSQL();
        }
    }
    catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------
void TfrmTemplate::ModifyTemplate()
{
    AnsiString strSQLClause,strNodeID;
    int nRetVal;

    if (! trvwTemplate->Selected)
    {
    	Application->MessageBox(L"You should specify a leaf node first!",GLOBALMSGCAPTION,48);
        return;
    }

    try
    {
        strNodeID = (AnsiString)((char *)trvwTemplate->Selected->Data);

        nRetVal = IsLeafNode(strNodeID);

        if (nRetVal == DATABASEERROR)
        //不存在指定节点，数据库不一致
            return;

        if (nRetVal != LEAFNODE)
        //当前节点不是叶节点，不能修改界面模板
        {
            Application->MessageBox(L"You should specify a leaf node!",GLOBALMSGCAPTION,48);
            return;
        }
    }
    catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }

    frmInout->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::tbbtnInputClick(TObject *Sender)
{
    m_nTemplateType = INPUTTEMPLATE;

    ModifyTemplate();
}
//---------------------------------------------------------------------------
int TfrmTemplate::IsLeafNode(AnsiString strNodeID)
{
    AnsiString strSQLClause;
    int nNodeAttr;
    
    try
    {
        strSQLClause = (AnsiString)"select NodeAttr from "+
                       strSTORYLIBTREE+
                       " where NodeID = '"+
                       SQLStr(strNodeID)+
                       "'";

        dmTemplate->qeGlobal->SQL->Clear();
        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
        dmTemplate->qeGlobal->Open();

        if (dmTemplate->qeGlobal->RecordCount <= 0)
        //不存在指定节点，数据库不一致
            return DATABASEERROR;

        dmTemplate->qeGlobal->First();
        nNodeAttr = (int)dmTemplate->qeGlobal->FieldValues["NodeAttr"];

        if (nNodeAttr == 1)
        //当前节点为叶节点
            return LEAFNODE;
        else
        //中间节点
            return MIDNODE;
    }
    catch(...)
    {
        return DATABASEERROR;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::tbbtnListClick(TObject *Sender)
{
    m_nTemplateType = LISTTEMPLATE;

    ModifyTemplate();
}
//---------------------------------------------------------------------------
void TfrmTemplate::UpdateStoryLibrary()
{
    AnsiString strSQLClause,strSubSQLClause,strNodeID;
    int        i,nCurrentYear;

	m_strDatabaseName = lblDBName->Caption;

	if (m_strDatabaseName.IsEmpty())
	{
		Application->MessageBox(L"Database Name Missing!",GLOBALMSGCAPTION,48);
		return;
	}

	try
	{
		//查询服务器上显示的年份
		strSQLClause = "select year = datepart(yy,getdate())";

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->Open();

		dmTemplate->qeGlobal->First();
		nCurrentYear = dmTemplate->qeGlobal->FieldValues["year"];

		//删除原来的存储过程
		strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id(\'"+
					   UPDATESTORYPROC+
					   (AnsiString)"\') and sysstat & 0xf = 4) drop procedure "+
					   UPDATESTORYPROC;

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();

		//检索所有的叶节点
		strSQLClause = (AnsiString)"select NodeID from "+
					   strSTORYLIBTREE+
					   " where NodeAttr = 1";

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->Open();

		dmTemplate->qeGlobal->First();
		while (! dmTemplate->qeGlobal->Eof)
		{
			strNodeID = OrigStr(dmTemplate->qeGlobal->FieldValues["NodeID"]);

			//确定叶节点所关联的两个数据表是否存在
			strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_old"+
						   strNodeID+
						   "')";

			qeInternal->SQL->Clear();
			qeInternal->SQL->Add(strSQLClause);
			qeInternal->Open();

			if (qeInternal->RecordCount <= 0)
			//不存在Old表，忽略之
			{
				dmTemplate->qeGlobal->Next();
				continue;
			}

			strSQLClause = (AnsiString)"select * from sysobjects where id = object_id('lib_new"+
						   strNodeID+
						   "')";

			qeInternal->SQL->Clear();
			qeInternal->SQL->Add(strSQLClause);
			qeInternal->Open();

			if (qeInternal->RecordCount <= 0)
			//不存在Old表，忽略之
			{
				dmTemplate->qeGlobal->Next();
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
				//不存在年份表，忽略之
				{
					dmTemplate->qeGlobal->Next();
					continue;
				}
			}

			strSubSQLClause =  strSubSQLClause+
							   "BEGIN TRANSACTION\n"+
							   "INSERT dbo.lib_old"+
							   strNodeID+
							   " SELECT * FROM dbo.lib_new"+
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
								 " SELECT * FROM dbo.lib_new"+
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
							 "\n\n";

			dmTemplate->qeGlobal->Next();
		}

		if (! strSubSQLClause.IsEmpty())
		{
			strSQLClause = (AnsiString)"CREATE PROCEDURE "+
						   UPDATESTORYPROC+
						   " AS\n";

			strSQLClause = strSQLClause + strSubSQLClause;

			dmTemplate->qeGlobal->SQL->Clear();
			dmTemplate->qeGlobal->SQL->Add(strSQLClause);
			dmTemplate->qeGlobal->ExecSQL();
		}
	}
	catch(...)
	{
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::tbbtnAuthorizeClick(TObject *Sender)
{
	fmAuthority->ShowModal();
}
//---------------------------------------------------------------------------






void __fastcall TfrmTemplate::trvwTemplateMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
	{
		TTreeNode * tnSelectedNode = trvwTemplate->GetNodeAt(X,Y);
		if (tnSelectedNode != NULL)
			trvwTemplate->Selected = tnSelectedNode;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTemplate::RefreshTree()
{
	TTreeNode * tnParentNode,* tnNodeJustAdded,* tnCurrentNode;
    TTreeNode * tnTempNode;
    TTreeNode * tnSelected;
    AnsiString strCaption,strNodeID,strParentID,strMidID,strLeafID;
    AnsiString strSQLClause,strUserAccount;
    AnsiString strSelected;
    TStringList * slNodeAddedMid,       //存在节点（非叶节点）队列
    			* slNodeUnAddedLeaf,	//叶节点等待队列
                * slNodeUnAddedMid,		//非叶节点等待队列
                * slNodeWaken;			//被“唤醒”节点的“栈”
	int nIndexInStringList,nIndexInStringListLeaf,nIndexInStringListMid,nIndexInTreeView;
    bool bIsAdded,bUnAuthorized;
    int nNodeAttr;
	TLocateOptions LocateOption;
    AnsiString strINIFileName;
    char strReturned[256];

    try
    {
        tnSelected = NULL;
        if (trvwTemplate->Selected!=NULL)
           strSelected=(AnsiString)((char*)trvwTemplate->Selected->Data);
        trvwTemplate->Items->Clear();
        if (dmTemplate->tbCategory->Active)
        	dmTemplate->tbCategory->Close();

        if (! dmTemplate->tbCategory->Active)
        	dmTemplate->tbCategory->Open();

        if (! dmTemplate->tbPermission->Active)
        	dmTemplate->tbPermission->Open();

        //记录用户帐号
//		m_nAuthority = dfrm->nGroupType;
		m_nAuthority = dmdStory->nGroupType;

		if (m_nAuthority == COMMONUSERGROUP)
		{
			//创建查询子句—确定该用户所在的组
			strSQLClause = (AnsiString)"select GroupID from lib_GroupUser where UserID = '"+
						   dmdStory->strCurrentUserID+
                           (AnsiString)"'";

			//访问Permission表，读出该用户组所能访问的节点号
            strSQLClause = (AnsiString)"select DISTINCT NodeID from lib_Permission where GroupID in ("+
                           strSQLClause+
                           (AnsiString)")";

            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->Open();

            if (dmTemplate->qeGlobal->RecordCount == 0)
            //不能访问任何节点
            {
                Application->MessageBox(L"You are unauthorized,forbidden to access the category lib!",GLOBALMSGCAPTION,48);
                return;
            }
        }

		//创建链表
		slNodeAddedMid		= new TStringList;
		slNodeUnAddedLeaf	= new TStringList;
		slNodeUnAddedMid	= new TStringList;
		slNodeWaken			= new TStringList;

		//确定Locate操作的参数
		LocateOption << loCaseInsensitive;

		//访问tbCategory一遍，生成相应的树结构
		dmTemplate->tbCategory->First();
		while (! dmTemplate->tbCategory->Eof)
		{
    		bIsAdded = true;
			bUnAuthorized = false;

			strNodeID    = OrigStr(dmTemplate->tbCategory->FieldValues["NodeID"]);
            if (dmTemplate->tbCategory->FieldValues["remark"].IsNull())
                strCaption = "";
            else
				strCaption   = OrigStr(dmTemplate->tbCategory->FieldValues["remark"]);
			nNodeAttr    = (int)dmTemplate->tbCategory->FieldValues["NodeAttr"];

    		if (dmTemplate->tbCategory->FieldValues["ParentID"].IsNull())
			//表明它为根节点
			{
        		if (m_nAuthority == COMMONUSERGROUP)
				//普通用户
				{
            		if (! dmTemplate->qeGlobal->Locate("NodeID",strNodeID,LocateOption))
					//不允许访问该节点，则不予显示
						bUnAuthorized = true;
				}

				if (! bUnAuthorized)
				//允许访问该节点
				{
            		//创建子节点
            		tnNodeJustAdded =
                		trvwTemplate->Items->AddChild(NULL,strCaption);
					//开辟节点的额外数据空间，用以存放节点号
					tnNodeJustAdded->Data = new char[NODEIDSIZE];
					lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
                    if (strNodeID==strSelected)
                        tnSelected=tnNodeJustAdded;

                    if (nNodeAttr == 1)
                    {
                        tnNodeJustAdded->ImageIndex    = 1;
                        tnNodeJustAdded->SelectedIndex = 1;
                    }
				}
			}
			else
			{
				strParentID = OrigStr(dmTemplate->tbCategory->FieldValues["ParentID"]);

				if (strParentID.IsEmpty())
				//表明它为根节点
				{
					if (m_nAuthority == COMMONUSERGROUP)
					//普通用户
					{
                		if (! dmTemplate->qeGlobal->Locate("NodeID",strNodeID,LocateOption))
						//不允许访问该节点，则不予显示
						bUnAuthorized = true;
					}

					if (! bUnAuthorized)
					{
                		tnNodeJustAdded =
                    		trvwTemplate->Items->AddChild(NULL,strCaption);
						tnNodeJustAdded->Data = new char[NODEIDSIZE];
						lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
                        if (strNodeID==strSelected)
                           tnSelected=tnNodeJustAdded;
                        
                        if (nNodeAttr == 1)
                        {
                            tnNodeJustAdded->ImageIndex    = 1;
                            tnNodeJustAdded->SelectedIndex = 1;
                        }
					}
				}
				else
				{
					nIndexInStringList = slNodeAddedMid->IndexOf(strParentID);
					if (nIndexInStringList >= 0)
    				//其父节点已经存在
        			{
	            		if (m_nAuthority == COMMONUSERGROUP)
    					//普通用户
        				{
    						if (! dmTemplate->qeGlobal->Locate("NodeID",strNodeID,LocateOption))
                			//不允许访问该节点，则不予显示
                        		bUnAuthorized = true;
						}

						if (! bUnAuthorized)
						{
                    		//找到父节点标识在链表中的位置
                    		nIndexInTreeView =
								atoi(AnsiString(slNodeAddedMid->Strings[nIndexInStringList+1]).c_str());
							//由父节点标识生成父节点指针
							tnParentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);
							//加入子节点
							tnNodeJustAdded =
								trvwTemplate->Items->AddChild(tnParentNode,strCaption);
							tnNodeJustAdded->Data = new char[NODEIDSIZE];
							lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
                            if (strNodeID==strSelected)
                               tnSelected=tnNodeJustAdded;

                            if (nNodeAttr == 1)
                            {
                                tnNodeJustAdded->ImageIndex    = 1;
                                tnNodeJustAdded->SelectedIndex = 1;
                            }
						}
					}
					else
    				//父节点不存在，不能加入
        	    		bIsAdded = false;
				}
			}

			if (! bUnAuthorized)
			//忽略未经授权的节点
			{
				if (bIsAdded)
				//已被成功加入
				{
        			if (nNodeAttr == 0)
					//若为叶节点则忽略
					{
						//启动“唤醒”过程
   						//这实际上包含了一个递归过程，
       					//现引入类似栈的唤醒队列slNodeWaken来实现
						tnCurrentNode = tnNodeJustAdded;
               			//清栈
               			slNodeWaken->Clear();

						while (1)
						{
	            			//把当前非叶节点加入存在节点（非叶节点）队列
							slNodeAddedMid->Add(strNodeID);
							slNodeAddedMid->Add(String((int)tnCurrentNode->ItemId));

							//“唤醒”所有叶节点
							nIndexInStringListLeaf =
                    				slNodeUnAddedLeaf->IndexOf(strNodeID);

							while ( nIndexInStringListLeaf >= 0)
   	    					//一次性“唤醒”当前节点在等待队列中的所有子节点
       	    				{
								bUnAuthorized = false;
                                            
	           					//“唤醒”
    	        				strCaption = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-1];
    	        				strLeafID  = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-2];

								//检查授权机制
		            			if (m_nAuthority == COMMONUSERGROUP)
        						//普通用户
                				{
		                			if (! dmTemplate->qeGlobal->Locate("NodeID",strLeafID,LocateOption))
        							//不允许访问该节点，则不予显示
                					bUnAuthorized = true;
								}

								if (! bUnAuthorized)
        						{
				        			tnTempNode = trvwTemplate->Items->AddChild(tnCurrentNode,strCaption);
    		        				tnTempNode->Data = new char[NODEIDSIZE];
				   					lstrcpy((char *)tnTempNode->Data,strLeafID.c_str());
                                    if (nNodeAttr == 1)
                                    {
                                        tnTempNode->ImageIndex    = 1;
                                        tnTempNode->SelectedIndex = 1;
                                    }
								}

        						//把被唤醒节点从等待队列中删除
   	        					slNodeUnAddedLeaf->Delete(nIndexInStringListLeaf);
       	        				slNodeUnAddedLeaf->Delete(nIndexInStringListLeaf-1);
       	        				slNodeUnAddedLeaf->Delete(nIndexInStringListLeaf-2);

								//继续“唤醒”下一个子节点
								nIndexInStringListLeaf =
                        				slNodeUnAddedLeaf->IndexOf(strNodeID);
       						}

							//“唤醒”所有非叶节点
							nIndexInStringListMid =
                					slNodeUnAddedMid->IndexOf(strNodeID);

							while ( nIndexInStringListMid >= 0)
    	    				//一次性“唤醒”等待队列中的所有子节点
        	    			{
								bUnAuthorized = false;
                                            
            					//“唤醒”
	            				strMidID =
                        			slNodeUnAddedMid->Strings[nIndexInStringListMid-2];
	            				strCaption =
                        			slNodeUnAddedMid->Strings[nIndexInStringListMid-1];

								//检查授权机制
		            			if (m_nAuthority == COMMONUSERGROUP)
        						//普通用户
                				{
		                			if (! dmTemplate->qeGlobal->Locate("NodeID",strLeafID,LocateOption))
        							//不允许访问该节点，则不予显示
                					bUnAuthorized = true;
								}

								if (! bUnAuthorized)
        						{
				        			tnNodeJustAdded =
    	                    			trvwTemplate->Items->AddChild(tnCurrentNode,strCaption);
    		        				tnNodeJustAdded->Data = new char[NODEIDSIZE];
				   					lstrcpy((char *)tnNodeJustAdded->Data,strMidID.c_str());
                                    if (strNodeID==strSelected)
                                       tnSelected=tnNodeJustAdded;

                            
	            					//把被“唤醒”的非叶节点入栈
    	            				slNodeWaken->Add(strMidID);
									slNodeWaken->Add(String((int)tnNodeJustAdded->ItemId));
								}

	        					//把被唤醒节点从等待队列中删除
    	        				slNodeUnAddedMid->Delete(nIndexInStringListMid);
        	        			slNodeUnAddedMid->Delete(nIndexInStringListMid-1);
        	        			slNodeUnAddedMid->Delete(nIndexInStringListMid-2);

								//“唤醒”下一个子节点
								nIndexInStringListMid =
                        			slNodeUnAddedMid->IndexOf(strNodeID);
        					}

                			if (slNodeWaken->Count > 0)
							//栈不空则弹出栈顶元素
							{
            	    			strNodeID = slNodeWaken->Strings[0];
								nIndexInTreeView =
									atoi(AnsiString(slNodeWaken->Strings[1]).c_str());
								tnCurrentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);

    	        				slNodeWaken->Delete(1);
        	        			slNodeWaken->Delete(0);
							}
							else
    							//栈为空则退出无限循环
								break;
						}
					}
				}
				else
				{
        			//进入等待队列
					if (nNodeAttr == 0)
					//非叶节点等待队列
					{
						slNodeUnAddedMid->Add(strNodeID);
						slNodeUnAddedMid->Add(strCaption);
						slNodeUnAddedMid->Add(strParentID);
					}
					else
					//叶节点等待队列
					{
						slNodeUnAddedLeaf->Add(strNodeID);
						slNodeUnAddedLeaf->Add(strCaption);
						slNodeUnAddedLeaf->Add(strParentID);                
					}
				}
			}

    		dmTemplate->tbCategory->Next();
		}

		//删除链表
		delete slNodeAddedMid;
		delete slNodeUnAddedLeaf;
		delete slNodeUnAddedMid;
		delete slNodeWaken;

      	if (m_nAuthority == COMMONUSERGROUP)
        //普通用户
        {
			tbbtnAdd->Enabled    = false;
			tbbtnDelete->Enabled = false;
            tbbtnInput->Enabled  = false;
        }
        else
        {
			tbbtnAdd->Enabled    = true;
			tbbtnDelete->Enabled = true;
            tbbtnInput->Enabled  = true;
        }

		m_nNodeID = -1;
//        tbbPaste->Enabled = false;

		lstvwFields->Columns->Items[0]->Caption = (AnsiString)strFIELDNAME;
		lstvwFields->Columns->Items[1]->Caption = (AnsiString)strFIELDTYPE;
		lstvwFields->Columns->Items[2]->Caption = (AnsiString)strFIELDSIZE;

        //从初始化文件中读取当前数据库名
        strINIFileName = IniName();

        GetPrivateProfileString("Database Connection","DatabaseName","ERROR",
                                strReturned,256,strINIFileName.c_str());

        if (strcmp(strReturned,"ERROR") == 0)
            m_strDatabaseName = "";
        else
			m_strDatabaseName = (AnsiString)strReturned;

		lblDBName->Caption = m_strDatabaseName;

        //读取语言码
        strSQLClause = "select CodeType from lib_SysConfig";

        //执行SQL语句
        dmTemplate->qeGlobal->SQL->Clear();
        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
        dmTemplate->qeGlobal->Open();

        if (dmTemplate->qeGlobal->RecordCount <= 0)
            m_strCodeType = "";
        else
        {
            if (dmTemplate->qeGlobal->FieldValues["CodeType"].IsNull())
                m_strCodeType = "";
            else
                m_strCodeType = OrigStr(dmTemplate->qeGlobal->FieldValues["CodeType"]);
        }
        if (tnSelected!=NULL)
           tnSelected->Selected=true;

	}
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
void __fastcall TfrmTemplate::tbbtnRefreshClick(TObject *Sender)
{
    RefreshTree();
}
//---------------------------------------------------------------------------



void __fastcall TfrmTemplate::trvwTemplateKeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
	if (Key==VK_DELETE)
		tbbtnDeleteClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTemplate::FormShow(TObject *Sender)
{
RefreshTree();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTemplate::trvwTemplateEdited(TObject *Sender, TTreeNode *Node,
          UnicodeString &S)
{
    AnsiString strSQLClause,strNodeID;

    strNodeID = (AnsiString)((char *)Node->Data);

    strSQLClause = (AnsiString)"Update "+
                   strSTORYLIBTREE+
                   " Set remark = '"+
                   SQLStr(S)+
                   "' where NodeID = '"+
                   SQLStr(strNodeID)+
                   "'";

    trvwTemplate->ReadOnly = true;

    try
	{
        dmTemplate->qeGlobal->SQL->Clear();
        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
        dmTemplate->qeGlobal->ExecSQL();
    }
    catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
	}
}
//---------------------------------------------------------------------------

