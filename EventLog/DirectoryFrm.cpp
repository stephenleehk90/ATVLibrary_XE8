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
	TStringList * slNodeAddedMid,       //存在节点（非叶节点）队列
				* slNodeUnAddedLeaf,	//叶节点等待队列
				* slNodeUnAddedMid,		//非叶节点等待队列
				* slNodeWaken;			//被“唤醒”节点的“栈”
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

		//创建链表
		slNodeAddedMid		= new TStringList;
		slNodeUnAddedLeaf	= new TStringList;
		slNodeUnAddedMid	= new TStringList;
		slNodeWaken			= new TStringList;

		//访问tbCategory一遍，生成相应的树结构
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
			//表明它为根节点
			{
				//创建子节点
				tnNodeJustAdded =
					trvwStory->Items->AddChild(NULL,strCaption);
				//开辟节点的额外数据空间，用以存放节点号
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
				//表明它为根节点
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
					//其父节点已经存在
					{
						//找到父节点标识在链表中的位置
						nIndexInTreeView =
							atoi(AnsiString(slNodeAddedMid->Strings[nIndexInStringList+1]).c_str());
						//由父节点标识生成父节点指针
						tnParentNode = trvwStory->Items->GetNode((HTREEITEM)nIndexInTreeView);
						//加入子节点
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
					//父节点不存在，不能加入
						bIsAdded = false;
				}
			}

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
							//“唤醒”
							strCaption = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-1];
							strLeafID  = slNodeUnAddedLeaf->Strings[nIndexInStringListLeaf-2];

							tnTempNode = trvwStory->Items->AddChild(tnCurrentNode,strCaption);
							tnTempNode->Data = new char[NODEIDSIZE];
							lstrcpy((char *)tnTempNode->Data,strLeafID.c_str());
							tnTempNode->ImageIndex    = 1;
							tnTempNode->SelectedIndex = 1;

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
							//“唤醒”
							strMidID =
								slNodeUnAddedMid->Strings[nIndexInStringListMid-2];
							strCaption =
								slNodeUnAddedMid->Strings[nIndexInStringListMid-1];

							tnNodeJustAdded =
								trvwStory->Items->AddChild(tnCurrentNode,strCaption);
							tnNodeJustAdded->Data = new char[NODEIDSIZE];
							lstrcpy((char *)tnNodeJustAdded->Data,strMidID.c_str());

							//把被“唤醒”的非叶节点入栈
							slNodeWaken->Add(strMidID);
							slNodeWaken->Add(String((int)tnNodeJustAdded->ItemId));

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
							tnCurrentNode = trvwStory->Items->GetNode((HTREEITEM)nIndexInTreeView);

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

			dmdLog->tbCategory->Next();
		}

		//删除链表
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
            //检索基本字段
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

            //检索扩展字段
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
        //不存在指定节点，数据库不一致
            return DATABASEERROR;

        dmdLog->qeGlobal->First();
        nNodeAttr = (int)dmdLog->qeGlobal->FieldValues["NodeAttr"];

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
