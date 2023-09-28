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

		//����Ƿ�ΪҶ�ڵ�
		nRetVal = IsLeafNode(strNodeID);

		if (nRetVal == DATABASEERROR)
		//���ݿ����
		{
		  Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		  return;
		}

		if (nRetVal == LEAFNODE)
		//Ҷ�ڵ�
		{
			//Ҷ�ڵ㲻�ܴ����ӽڵ�
			frmWizard->cboDirectory->ItemIndex = 0;

			//Ҷ�ڵ������޸Ľ���ģ��
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


            //Ҷ�ڵ���������ر�
//            muiCreateTable->Enabled = true;
        }
        else
        {
            //���õ�ǰ�ڵ�
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
	   //û���ҵ��ýڵ㣬���ݿⲻһ��
		   return;

		nNodeAttr = (int)dmTemplate->qeGlobal->FieldValues["NodeAttr"];
		if (nNodeAttr == 1)
		//Ҷ�ڵ�
			strWarning = (AnsiString)"Are you sure to remove this leaf node and its assocaited table?";
		else
			strWarning = (AnsiString)"Are you sure to remove this node and all its child nodes?";

		if (Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION)
        			== ID_OK)
		{
			slStack = new TStringList;

			//����������tnRootNodeΪ�������������ͷŸ��ڵ��Data�ռ�
			if (tnRootNode->Count == 0)	//����Ů
			{
				if (nNodeAttr == 1)
				//ɾ��Ҷ�ڵ������������ݱ�
				{
					RemoveAssociatedTable(strNodeID);
				}

				RemoveNode(strNodeID);

				//�ͷŽڵ�������ݿռ�
				delete tnRootNode->Data;
			}
			else
    		{
        		//���ڵ���ջ
				tnParentNode = tnRootNode;
				slStack->Add(String((int)tnParentNode->ItemId));

				//�Ѹ��ڵ�ĵ�һ���ӽڵ���Ϊ��ǰ�ڵ�
				tnCurrentNode = tnParentNode->getFirstChild();

				while (slStack->Count > 0)	//ջ����
				{
					if (! tnCurrentNode)	//û���ֵܽڵ�
					{
						//���ڵ��ջ
						nIndexInTreeView =
							atoi(AnsiString(slStack->Strings[slStack->Count-1]).c_str());
						tnCurrentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);
						slStack->Delete(slStack->Count-1);

						//���������ڵ����ɾ����ǰ�ڵ�
						strNodeID = (AnsiString)((char *)tnCurrentNode->Data);

						RemoveNode(strNodeID);

						delete tnCurrentNode->Data;

						if (slStack->Count > 0)
    					{
							//ջ��Ϊ��ǰ�ڵ�ĸ��ڵ�
							nIndexInTreeView =
								atoi(AnsiString(slStack->Strings[slStack->Count-1]).c_str());
							tnParentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);

							tnCurrentNode = tnParentNode->GetNextChild(tnCurrentNode);
						 }

						continue;
					}

					if (tnCurrentNode->Count == 0)	//��ǰ�ڵ�û����Ů
    				{
						strNodeID = (AnsiString)((char *)tnCurrentNode->Data);

						//ȷ����ǰ�ڵ��ǲ���Ҷ�ڵ�
						strSQLClause = (AnsiString)"select NodeAttr from "+
									   strSTORYLIBTREE+
									   " where NodeID = '"+
									   SQLStr(strNodeID)+
									   (AnsiString)"'";

					   dmTemplate->qeGlobal->SQL->Clear();
					   dmTemplate->qeGlobal->SQL->Add(strSQLClause);
					   dmTemplate->qeGlobal->Open();

					   if (dmTemplate->qeGlobal->RecordCount <= 0)
					   //û���ҵ��ýڵ㣬���ݿⲻһ��
						   return;

						nNodeAttr = (int)dmTemplate->qeGlobal->FieldValues["NodeAttr"];

						if (nNodeAttr == 1)
						{
							RemoveAssociatedTable(strNodeID);
						}

						RemoveNode(strNodeID);

						delete tnCurrentNode->Data;

						//ǰ������һ���ֵܽڵ�
    					tnCurrentNode = tnParentNode->GetNextChild(tnCurrentNode);
        			}
            		else	//����Ů
					{
						//��ǰ�ڵ���ջ
						tnParentNode = tnCurrentNode;
						slStack->Add(String((int)tnParentNode->ItemId));

						//ǰ������ǰ�ڵ�ĵ�һ���ӽڵ�
						tnCurrentNode = tnParentNode->getFirstChild();
        			}
				}
			}

			delete slStack;

            //���´洢����
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

	//������������ɾ���ýڵ�
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

		//����չ�ֶα���ɾ���ýڵ��������������ֶ�
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
		//ɾ���������
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
	  //��ѯ����������ʾ�����
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

	//ɾ��Ҷ�ڵ����������������ݱ�

	try
	{
		//ɾ��Old��
		strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_old"+
					   strNodeID+
					   "') and sysstat & 0xf = 3) drop table lib_old"+
					   strNodeID;

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();

		//ɾ��New��
		strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id('lib_new"+
					   strNodeID+
					   "') and sysstat & 0xf = 3) drop table lib_new"+
					   strNodeID;

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();

		//ɾ����ݱ�
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
    //Դ�ڵ���Ŀ��ڵ���ͬ
        return;

    //����Դ�ڵ����չ�ֶ�
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

        //������ǰ�ڵ����չ�ֶ�
        strSQLClause = "select FieldName from "+
                       (AnsiString)strLIBEXTENDFIELDS+
                       " where CategoryID = '"+
                       SQLStr(strDestNodeID)+
                       "'";

        qeInternal->SQL->Clear();
        qeInternal->SQL->Add(strSQLClause);
        qeInternal->Open();

		//ȷ��Locate�����Ĳ���
		LocateOption << loCaseInsensitive;

        while (! qeInternal2->Eof)
        {
            //����Դ�ڵ����չ�ֶ�
            strFieldName = OrigStr(qeInternal2->FieldValues["FieldName"]);
            strFieldType = OrigStr(qeInternal2->FieldValues["FieldType"]);
            strFieldSize = qeInternal2->FieldValues["FieldSize"];

            //ȷ��Դ�ڵ����չ�ֶ����Ƿ���Ŀ��ڵ����չ�ֶ�����
            if (qeInternal->Locate("FieldName",SQLStr(strFieldName),LocateOption))
            {
                qeInternal2->Next();
                continue;
            }

            //������չ�ֶ�
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
        //�����ⲻ����
        {
            if (! bToCreate)
                return;

		    if (Application->MessageBox(L"Create a new table with current fields?",
            	GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION) == IDOK)
            {
				return;
		    }

            //ɾ��New�⣨�����ڣ�
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

        //��֯��չ�ֶβ��ֵ��Ӳ�ѯ
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

        //��֯�����ֶβ��ֵ��Ӳ�ѯ
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

            //ɾ����β��","
            strSQLClause.Delete(strSQLClause.Length(),1);

            //����β��������
            strSQLClause = strSQLClause + ")";

            //ִ��SQL���
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

            //ɾ����β��","
            strSQLClause.Delete(strSQLClause.Length(),1);

            //����β��������
            strSQLClause = strSQLClause + ")";

            //ִ��SQL���
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
        //������ָ���ڵ㣬���ݿⲻһ��
            return;

        if (nRetVal != LEAFNODE)
        //��ǰ�ڵ㲻��Ҷ�ڵ㣬�����޸Ľ���ģ��
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
        //������ָ���ڵ㣬���ݿⲻһ��
            return DATABASEERROR;

        dmTemplate->qeGlobal->First();
        nNodeAttr = (int)dmTemplate->qeGlobal->FieldValues["NodeAttr"];

        if (nNodeAttr == 1)
        //��ǰ�ڵ�ΪҶ�ڵ�
            return LEAFNODE;
        else
        //�м�ڵ�
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
		//��ѯ����������ʾ�����
		strSQLClause = "select year = datepart(yy,getdate())";

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->Open();

		dmTemplate->qeGlobal->First();
		nCurrentYear = dmTemplate->qeGlobal->FieldValues["year"];

		//ɾ��ԭ���Ĵ洢����
		strSQLClause = (AnsiString)"if exists (select * from sysobjects where id = object_id(\'"+
					   UPDATESTORYPROC+
					   (AnsiString)"\') and sysstat & 0xf = 4) drop procedure "+
					   UPDATESTORYPROC;

		dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
		dmTemplate->qeGlobal->ExecSQL();

		//�������е�Ҷ�ڵ�
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
			//������Old������֮
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
				//��������ݱ�����֮
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
    TStringList * slNodeAddedMid,       //���ڽڵ㣨��Ҷ�ڵ㣩����
    			* slNodeUnAddedLeaf,	//Ҷ�ڵ�ȴ�����
                * slNodeUnAddedMid,		//��Ҷ�ڵ�ȴ�����
                * slNodeWaken;			//�������ѡ��ڵ�ġ�ջ��
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

        //��¼�û��ʺ�
//		m_nAuthority = dfrm->nGroupType;
		m_nAuthority = dmdStory->nGroupType;

		if (m_nAuthority == COMMONUSERGROUP)
		{
			//������ѯ�Ӿ䡪ȷ�����û����ڵ���
			strSQLClause = (AnsiString)"select GroupID from lib_GroupUser where UserID = '"+
						   dmdStory->strCurrentUserID+
                           (AnsiString)"'";

			//����Permission���������û������ܷ��ʵĽڵ��
            strSQLClause = (AnsiString)"select DISTINCT NodeID from lib_Permission where GroupID in ("+
                           strSQLClause+
                           (AnsiString)")";

            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->Open();

            if (dmTemplate->qeGlobal->RecordCount == 0)
            //���ܷ����κνڵ�
            {
                Application->MessageBox(L"You are unauthorized,forbidden to access the category lib!",GLOBALMSGCAPTION,48);
                return;
            }
        }

		//��������
		slNodeAddedMid		= new TStringList;
		slNodeUnAddedLeaf	= new TStringList;
		slNodeUnAddedMid	= new TStringList;
		slNodeWaken			= new TStringList;

		//ȷ��Locate�����Ĳ���
		LocateOption << loCaseInsensitive;

		//����tbCategoryһ�飬������Ӧ�����ṹ
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
			//������Ϊ���ڵ�
			{
        		if (m_nAuthority == COMMONUSERGROUP)
				//��ͨ�û�
				{
            		if (! dmTemplate->qeGlobal->Locate("NodeID",strNodeID,LocateOption))
					//��������ʸýڵ㣬������ʾ
						bUnAuthorized = true;
				}

				if (! bUnAuthorized)
				//������ʸýڵ�
				{
            		//�����ӽڵ�
            		tnNodeJustAdded =
                		trvwTemplate->Items->AddChild(NULL,strCaption);
					//���ٽڵ�Ķ������ݿռ䣬���Դ�Žڵ��
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
				//������Ϊ���ڵ�
				{
					if (m_nAuthority == COMMONUSERGROUP)
					//��ͨ�û�
					{
                		if (! dmTemplate->qeGlobal->Locate("NodeID",strNodeID,LocateOption))
						//��������ʸýڵ㣬������ʾ
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
    				//�丸�ڵ��Ѿ�����
        			{
	            		if (m_nAuthority == COMMONUSERGROUP)
    					//��ͨ�û�
        				{
    						if (! dmTemplate->qeGlobal->Locate("NodeID",strNodeID,LocateOption))
                			//��������ʸýڵ㣬������ʾ
                        		bUnAuthorized = true;
						}

						if (! bUnAuthorized)
						{
                    		//�ҵ����ڵ��ʶ�������е�λ��
                    		nIndexInTreeView =
								atoi(AnsiString(slNodeAddedMid->Strings[nIndexInStringList+1]).c_str());
							//�ɸ��ڵ��ʶ���ɸ��ڵ�ָ��
							tnParentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);
							//�����ӽڵ�
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
    				//���ڵ㲻���ڣ����ܼ���
        	    		bIsAdded = false;
				}
			}

			if (! bUnAuthorized)
			//����δ����Ȩ�Ľڵ�
			{
				if (bIsAdded)
				//�ѱ��ɹ�����
				{
        			if (nNodeAttr == 0)
					//��ΪҶ�ڵ������
					{
						//���������ѡ�����
   						//��ʵ���ϰ�����һ���ݹ���̣�
       					//����������ջ�Ļ��Ѷ���slNodeWaken��ʵ��
						tnCurrentNode = tnNodeJustAdded;
               			//��ջ
               			slNodeWaken->Clear();

						while (1)
						{
	            			//�ѵ�ǰ��Ҷ�ڵ������ڽڵ㣨��Ҷ�ڵ㣩����
							slNodeAddedMid->Add(strNodeID);
							slNodeAddedMid->Add(String((int)tnCurrentNode->ItemId));

							//�����ѡ�����Ҷ�ڵ�
							nIndexInStringListLeaf =
                    				slNodeUnAddedLeaf->IndexOf(strNodeID);

							while ( nIndexInStringListLeaf >= 0)
   	    					//һ���ԡ����ѡ���ǰ�ڵ��ڵȴ������е������ӽڵ�
       	    				{
								bUnAuthorized = false;
                                            
	           					//�����ѡ�
    	        				strCaption = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-1];
    	        				strLeafID  = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-2];

								//�����Ȩ����
		            			if (m_nAuthority == COMMONUSERGROUP)
        						//��ͨ�û�
                				{
		                			if (! dmTemplate->qeGlobal->Locate("NodeID",strLeafID,LocateOption))
        							//��������ʸýڵ㣬������ʾ
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

        						//�ѱ����ѽڵ�ӵȴ�������ɾ��
   	        					slNodeUnAddedLeaf->Delete(nIndexInStringListLeaf);
       	        				slNodeUnAddedLeaf->Delete(nIndexInStringListLeaf-1);
       	        				slNodeUnAddedLeaf->Delete(nIndexInStringListLeaf-2);

								//���������ѡ���һ���ӽڵ�
								nIndexInStringListLeaf =
                        				slNodeUnAddedLeaf->IndexOf(strNodeID);
       						}

							//�����ѡ����з�Ҷ�ڵ�
							nIndexInStringListMid =
                					slNodeUnAddedMid->IndexOf(strNodeID);

							while ( nIndexInStringListMid >= 0)
    	    				//һ���ԡ����ѡ��ȴ������е������ӽڵ�
        	    			{
								bUnAuthorized = false;
                                            
            					//�����ѡ�
	            				strMidID =
                        			slNodeUnAddedMid->Strings[nIndexInStringListMid-2];
	            				strCaption =
                        			slNodeUnAddedMid->Strings[nIndexInStringListMid-1];

								//�����Ȩ����
		            			if (m_nAuthority == COMMONUSERGROUP)
        						//��ͨ�û�
                				{
		                			if (! dmTemplate->qeGlobal->Locate("NodeID",strLeafID,LocateOption))
        							//��������ʸýڵ㣬������ʾ
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

                            
	            					//�ѱ������ѡ��ķ�Ҷ�ڵ���ջ
    	            				slNodeWaken->Add(strMidID);
									slNodeWaken->Add(String((int)tnNodeJustAdded->ItemId));
								}

	        					//�ѱ����ѽڵ�ӵȴ�������ɾ��
    	        				slNodeUnAddedMid->Delete(nIndexInStringListMid);
        	        			slNodeUnAddedMid->Delete(nIndexInStringListMid-1);
        	        			slNodeUnAddedMid->Delete(nIndexInStringListMid-2);

								//�����ѡ���һ���ӽڵ�
								nIndexInStringListMid =
                        			slNodeUnAddedMid->IndexOf(strNodeID);
        					}

                			if (slNodeWaken->Count > 0)
							//ջ�����򵯳�ջ��Ԫ��
							{
            	    			strNodeID = slNodeWaken->Strings[0];
								nIndexInTreeView =
									atoi(AnsiString(slNodeWaken->Strings[1]).c_str());
								tnCurrentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);

    	        				slNodeWaken->Delete(1);
        	        			slNodeWaken->Delete(0);
							}
							else
    							//ջΪ�����˳�����ѭ��
								break;
						}
					}
				}
				else
				{
        			//����ȴ�����
					if (nNodeAttr == 0)
					//��Ҷ�ڵ�ȴ�����
					{
						slNodeUnAddedMid->Add(strNodeID);
						slNodeUnAddedMid->Add(strCaption);
						slNodeUnAddedMid->Add(strParentID);
					}
					else
					//Ҷ�ڵ�ȴ�����
					{
						slNodeUnAddedLeaf->Add(strNodeID);
						slNodeUnAddedLeaf->Add(strCaption);
						slNodeUnAddedLeaf->Add(strParentID);                
					}
				}
			}

    		dmTemplate->tbCategory->Next();
		}

		//ɾ������
		delete slNodeAddedMid;
		delete slNodeUnAddedLeaf;
		delete slNodeUnAddedMid;
		delete slNodeWaken;

      	if (m_nAuthority == COMMONUSERGROUP)
        //��ͨ�û�
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

        //�ӳ�ʼ���ļ��ж�ȡ��ǰ���ݿ���
        strINIFileName = IniName();

        GetPrivateProfileString("Database Connection","DatabaseName","ERROR",
                                strReturned,256,strINIFileName.c_str());

        if (strcmp(strReturned,"ERROR") == 0)
            m_strDatabaseName = "";
        else
			m_strDatabaseName = (AnsiString)strReturned;

		lblDBName->Caption = m_strDatabaseName;

        //��ȡ������
        strSQLClause = "select CodeType from lib_SysConfig";

        //ִ��SQL���
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

