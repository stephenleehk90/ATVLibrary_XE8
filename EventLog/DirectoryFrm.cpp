//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CommFunc.h"
#include "LogFrm.h"
#include "DirectoryFrm.h"
#include "LogData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDirectory *frmDirectory;
//---------------------------------------------------------------------------
__fastcall TfrmDirectory::TfrmDirectory(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectory::FormDeactivate(TObject *Sender)
{
  Close();
}

void TfrmDirectory::InitForm()
{
	TTreeNode * tnParentNode,* tnNodeJustAdded,* tnCurrentNode;
	TTreeNode * tnTempNode;
	AnsiString strCaption,strNodeID,strParentID,strMidID,strLeafID;
	AnsiString strSQLClause,strUserAccount;
	TStringList * slNodeAddedMid,       //���ڽڵ㣨��Ҷ�ڵ㣩����
				* slNodeUnAddedLeaf,	//Ҷ�ڵ�ȴ�����
				* slNodeUnAddedMid,		//��Ҷ�ڵ�ȴ�����
				* slNodeWaken;			//�������ѡ��ڵ�ġ�ջ��
	int nIndexInStringList,nIndexInStringListLeaf,nIndexInStringListMid,nIndexInTreeView;
	bool bIsAdded;
	int nNodeAttr;

	trvwStory->Items->Clear();

	try
	{
		if (dmdLog->tbCategory->Active)
			dmdLog->tbCategory->Close();
		if (! dmdLog->tbCategory->Active)
			dmdLog->tbCategory->Open();

		//��������
		slNodeAddedMid		= new TStringList;
		slNodeUnAddedLeaf	= new TStringList;
		slNodeUnAddedMid	= new TStringList;
		slNodeWaken			= new TStringList;

		//����tbCategoryһ�飬������Ӧ�����ṹ
		dmdLog->tbCategory->First();
		while (! dmdLog->tbCategory->Eof)
		{
			bIsAdded = true;

			strNodeID    = OrigStr(dmdLog->tbCategory->FieldValues["NodeID"]);
			if (dmdLog->tbCategory->FieldValues["remark"].IsNull())
				strCaption = "";
			else
				strCaption   = OrigStr(dmdLog->tbCategory->FieldValues["remark"]);
			nNodeAttr    = (int)dmdLog->tbCategory->FieldValues["NodeAttr"];

			if (dmdLog->tbCategory->FieldValues["ParentID"].IsNull())
			//������Ϊ���ڵ�
			{
				//�����ӽڵ�
				tnNodeJustAdded =
					trvwStory->Items->AddChild(NULL,strCaption);
				//���ٽڵ�Ķ������ݿռ䣬���Դ�Žڵ��
				tnNodeJustAdded->Data = new char[NODEIDSIZE];
				lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
				if (nNodeAttr == 1)
				{
					tnNodeJustAdded->ImageIndex    = 1;
					tnNodeJustAdded->SelectedIndex = 1;
				}
			}
			else
			{
				strParentID = OrigStr(dmdLog->tbCategory->FieldValues["ParentID"]);

				if (strParentID.IsEmpty())
				//������Ϊ���ڵ�
				{
					tnNodeJustAdded =
						trvwStory->Items->AddChild(NULL,strCaption);
					tnNodeJustAdded->Data = new char[NODEIDSIZE];
					lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
					if (nNodeAttr == 1)
					{
						tnNodeJustAdded->ImageIndex    = 1;
						tnNodeJustAdded->SelectedIndex = 1;
					}
				}
				else
				{
					nIndexInStringList = slNodeAddedMid->IndexOf(strParentID);
					if (nIndexInStringList >= 0)
					//�丸�ڵ��Ѿ�����
					{
						//�ҵ����ڵ��ʶ�������е�λ��
						nIndexInTreeView =
							atoi(AnsiString(slNodeAddedMid->Strings[nIndexInStringList+1]).c_str());
						//�ɸ��ڵ��ʶ���ɸ��ڵ�ָ��
						tnParentNode = trvwStory->Items->GetNode((HTREEITEM)nIndexInTreeView);
						//�����ӽڵ�
						tnNodeJustAdded =
							trvwStory->Items->AddChild(tnParentNode,strCaption);
						tnNodeJustAdded->Data = new char[NODEIDSIZE];
						lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
						if (nNodeAttr == 1)
						{
						  tnNodeJustAdded->ImageIndex    = 1;
						  tnNodeJustAdded->SelectedIndex = 1;
						}
					}
					else
					//���ڵ㲻���ڣ����ܼ���
						bIsAdded = false;
				}
			}

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
							//�����ѡ�
							strCaption = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-1];
							strLeafID  = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-2];

							tnTempNode = trvwStory->Items->AddChild(tnCurrentNode,strCaption);
							tnTempNode->Data = new char[NODEIDSIZE];
							lstrcpy((char *)tnTempNode->Data,strLeafID.c_str());
							tnTempNode->ImageIndex    = 1;
							tnTempNode->SelectedIndex = 1;

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
							//�����ѡ�
							strMidID =
								slNodeUnAddedMid->Strings[nIndexInStringListMid-2];
							strCaption =
								slNodeUnAddedMid->Strings[nIndexInStringListMid-1];

							tnNodeJustAdded =
								trvwStory->Items->AddChild(tnCurrentNode,strCaption);
							tnNodeJustAdded->Data = new char[NODEIDSIZE];
							lstrcpy((char *)tnNodeJustAdded->Data,strMidID.c_str());

							//�ѱ������ѡ��ķ�Ҷ�ڵ���ջ
							slNodeWaken->Add(strMidID);
							slNodeWaken->Add(String((int)tnNodeJustAdded->ItemId));

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
							tnCurrentNode = trvwStory->Items->GetNode((HTREEITEM)nIndexInTreeView);

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

			dmdLog->tbCategory->Next();
		}

		//ɾ������
		delete slNodeAddedMid;
		delete slNodeUnAddedLeaf;
		delete slNodeUnAddedMid;
		delete slNodeWaken;
	}
	catch(...)
	{
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDirectory::trvwStoryChange(TObject *Sender,
	  TTreeNode *Node)
{
	AnsiString strNodeID,strSQLClause,strFieldName;
	int i;

	strNodeID = (AnsiString)((char *)Node->Data);

	if (IsLeafNode(strNodeID) == LEAFNODE)
	{
		if (frmLogs->m_nDataType == 1)
			frmLogs->edtDestTb->Text   = strNodeID;
		else
		{
            frmLogs->edtSourceTb->Text = strNodeID;

            for (i=0;i<frmLogs->lstvwFields->Items->Count;i++)
            {
                frmLogs->lstvwFields->Items->Item[i]->SubItems->Text = "";            
                frmLogs->lstvwFields->Items->Item[i]->SubItems->Clear();
            }

            return;
        }

        frmLogs->lstvwFields->Items->Clear();

        try
        {
            //���������ֶ�
            strSQLClause = (AnsiString)"select * from "+
                           strLIBBASEFIELDS;

            dmdLog->qeBaseFields->SQL->Clear();
            dmdLog->qeBaseFields->SQL->Add(strSQLClause);
            dmdLog->qeBaseFields->Open();

            while (! dmdLog->qeBaseFields->Eof)
            {
				strFieldName = OrigStr(dmdLog->qeBaseFields->FieldValues["FieldName"]);

                if (strFieldName != FIELDSTORYID)
                {
                    frmLogs->lstvwFields->Items->Add();
                    frmLogs->lstvwFields->Items->Item[frmLogs->lstvwFields->Items->Count-1]->Caption =
                        strFieldName;
                }

                dmdLog->qeBaseFields->Next();
            }

            //������չ�ֶ�
            strSQLClause = (AnsiString)"select * from "+
                           (AnsiString)strLIBEXTENDFIELDS+
                           (AnsiString)" where CategoryID = '"+
                           SQLStr(strNodeID)+
                           (AnsiString)"'";

            dmdLog->qeExtFields->SQL->Clear();
            dmdLog->qeExtFields->SQL->Add(strSQLClause);
            dmdLog->qeExtFields->Open();

            while (! dmdLog->qeExtFields->Eof)
            {
                frmLogs->lstvwFields->Items->Add();
                frmLogs->lstvwFields->Items->Item[frmLogs->lstvwFields->Items->Count-1]->Caption =
                    OrigStr(dmdLog->qeExtFields->FieldValues["FieldName"]);

                dmdLog->qeExtFields->Next();
            }
        }
        catch(...)
        {
            Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
            return;
        }
    }
}
//---------------------------------------------------------------------------
int TfrmDirectory::IsLeafNode(AnsiString strNodeID)
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

        dmdLog->qeGlobal->SQL->Clear();
        dmdLog->qeGlobal->SQL->Add(strSQLClause);
        dmdLog->qeGlobal->Open();

        if (dmdLog->qeGlobal->RecordCount <= 0)
        //������ָ���ڵ㣬���ݿⲻһ��
            return DATABASEERROR;

        dmdLog->qeGlobal->First();
        nNodeAttr = (int)dmdLog->qeGlobal->FieldValues["NodeAttr"];

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
