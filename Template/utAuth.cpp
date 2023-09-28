//---------------------------------------------------------------------------
#include <stdlib.h>
#include <vcl.h>
#pragma hdrstop


#include "utAuth.h"
#include "CommFunc.h"
#include "utGlobDm.h"
#include "TemplateForm.h"

#define COMMONUSER 3
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmAuthority *fmAuthority;
//---------------------------------------------------------------------------
__fastcall TfmAuthority::TfmAuthority(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmAuthority::trvwTemplateChange(TObject *Sender,
      TTreeNode *Node)
{
	AnsiString strSQLClause,strNodeID;

    strNodeID = (AnsiString)((char *)Node->Data);

    try
    {
        strSQLClause = (AnsiString)"select GroupID from "+
                       (AnsiString)strPERMISSION+
                       (AnsiString)" where NodeID = '"+
                       SQLStr(strNodeID)+
                       (AnsiString)"'";

	    dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
	    dmTemplate->qeGlobal->Open();

        //显示能访问当前节点的所有用户组
        lbAuthority->Items->Clear();
        dmTemplate->qeGlobal->First();
        while (! dmTemplate->qeGlobal->Eof)
        {
            lbAuthority->Items->Add(OrigStr(dmTemplate->qeGlobal->FieldValues["GroupID"]));

            dmTemplate->qeGlobal->Next();
        }

        if (lbAuthority->Items->Count <= 0)
            btnRevoke->Enabled = false;
        else
            btnRevoke->Enabled = true;
    }
    catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmAuthority::FormShow(TObject *Sender)
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

    trvwTemplate->Items->Clear();
    
    try
    {
        if (dmTemplate->tbCategory->Active)
        	dmTemplate->tbCategory->Close();
        if (! dmTemplate->tbCategory->Active)
        	dmTemplate->tbCategory->Open();

		//创建链表
		slNodeAddedMid		= new TStringList;
		slNodeUnAddedLeaf	= new TStringList;
		slNodeUnAddedMid	= new TStringList;
		slNodeWaken			= new TStringList;

		//访问tbCategory一遍，生成相应的树结构
		dmTemplate->tbCategory->First();
		while (! dmTemplate->tbCategory->Eof)
		{
    		bIsAdded = true;

			strNodeID    = OrigStr(dmTemplate->tbCategory->FieldValues["NodeID"]);
            if (dmTemplate->tbCategory->FieldValues["remark"].IsNull())
                strCaption = "";
            else
				strCaption   = OrigStr(dmTemplate->tbCategory->FieldValues["remark"]);
			nNodeAttr    = (int)dmTemplate->tbCategory->FieldValues["NodeAttr"];

    		if (dmTemplate->tbCategory->FieldValues["ParentID"].IsNull())
			//表明它为根节点
			{
                //创建子节点
                tnNodeJustAdded =
                    trvwTemplate->Items->AddChild(NULL,strCaption);
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
				strParentID = OrigStr(dmTemplate->tbCategory->FieldValues["ParentID"]);

				if (strParentID.IsEmpty())
				//表明它为根节点
				{
                    tnNodeJustAdded =
                        trvwTemplate->Items->AddChild(NULL,strCaption);
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
                            StrToInt(slNodeAddedMid->Strings[nIndexInStringList+1]);
                        //由父节点标识生成父节点指针
                        tnParentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);
                        //加入子节点
                        tnNodeJustAdded =
                            trvwTemplate->Items->AddChild(tnParentNode,strCaption);
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

                            tnTempNode = trvwTemplate->Items->AddChild(tnCurrentNode,strCaption);
                            tnTempNode->Data = new char[NODEIDSIZE];
                            lstrcpy((char *)tnTempNode->Data,strLeafID.c_str());
                            if (nNodeAttr == 1)
                            {
                                tnTempNode->ImageIndex    = 1;
                                tnTempNode->SelectedIndex = 1;
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
                            //“唤醒”
                            strMidID =
                                slNodeUnAddedMid->Strings[nIndexInStringListMid-2];
                            strCaption =
                                slNodeUnAddedMid->Strings[nIndexInStringListMid-1];

                            tnNodeJustAdded =
                                trvwTemplate->Items->AddChild(tnCurrentNode,strCaption);
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

    		dmTemplate->tbCategory->Next();
		}

		//删除链表
		delete slNodeAddedMid;
		delete slNodeUnAddedLeaf;
		delete slNodeUnAddedMid;
		delete slNodeWaken;

        if (trvwTemplate->Items->Count > 0)
            trvwTemplate->Selected = trvwTemplate->Items->Item[0];

        //显示所有用户组        
        strSQLClause = (AnsiString)"select GroupID from "+
                       (AnsiString)strGROUP+
                       " where GroupType = "+
                       String(COMMONUSER);

	    dmTemplate->qeGlobal->SQL->Clear();
		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
	    dmTemplate->qeGlobal->Open();

        lbGroup->Items->Clear();
        dmTemplate->qeGlobal->First();
        while (! dmTemplate->qeGlobal->Eof)
        {
            lbGroup->Items->Add(OrigStr(dmTemplate->qeGlobal->FieldValues["GroupID"]));

            dmTemplate->qeGlobal->Next();
        }

        if (lbGroup->Items->Count <= 0)
            btnGrant->Enabled = false;    
	}
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmAuthority::btnGrantClick(TObject *Sender)
{
    int i;
    TTreeNode * tnParentNode;
    AnsiString strSQLClause,strNodeID,strNodeParent,strGroupID;

    if (! trvwTemplate->Selected)
        return;

    strNodeID = (AnsiString)((char *)trvwTemplate->Selected->Data);

    tnParentNode =  trvwTemplate->Selected->Parent;
    if (tnParentNode == NULL)
        strNodeParent = "";
    else
        strNodeParent = (AnsiString)((char *)tnParentNode->Data);

    try
    {
        for (i=0;i<lbGroup->Items->Count;i++)
        {
            if (lbGroup->Selected[i])
            //被选中项
            {
                strGroupID = lbGroup->Items->Strings[i];
                //检查其父节点是否已经授权
                if (! strNodeParent.IsEmpty())
                //有父节点
                {
                    strSQLClause = (AnsiString)"select GroupID,NodeID from "+
                                   (AnsiString)strPERMISSION+
                                   (AnsiString)" where GroupID = '"+
                                   SQLStr(strGroupID)+
                                   "' and NodeID = '"+
                                   SQLStr(strNodeParent)+
                                   (AnsiString)"'";

                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->Open();

                    if (dmTemplate->qeGlobal->RecordCount <= 0)
                    //父节点还未授权
                        continue;
                }

                if (lbAuthority->Items->IndexOf(lbGroup->Items->Strings[i]) == -1)
                {
                    strSQLClause = (AnsiString)"insert into "+
                                   strPERMISSION+
                                   " Values('"+
                                   SQLStr(lbGroup->Items->Strings[i])+
                                   "','"+
                                   SQLStr(strNodeID)+
                                   "',1)";

                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    lbAuthority->Items->Add(lbGroup->Items->Strings[i]);

                    lbGroup->Selected[i] = false;

                    if (! btnRevoke->Enabled)
                        btnRevoke->Enabled = true;
                }
            }
        }
    }
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmAuthority::btnRevokeClick(TObject *Sender)
{
    int i;
    AnsiString strSQLClause,strNodeID;

    if (! trvwTemplate->Selected)
        return;

    strNodeID = (AnsiString)((char *)trvwTemplate->Selected->Data);

    try
    {
        for (i=lbAuthority->Items->Count-1;i>=0;i--)
        {
            if (lbAuthority->Selected[i])
            //被选中项
            {
                strSQLClause = (AnsiString)"delete from "+
                               strPERMISSION+
                               " where NodeID = '"+
                               SQLStr(strNodeID)+
                               "' and GroupID = '"+
                               SQLStr(lbAuthority->Items->Strings[i])+
                               "'";

                dmTemplate->qeGlobal->SQL->Clear();
                dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                dmTemplate->qeGlobal->ExecSQL();

                lbAuthority->Items->Delete(i);
            }
        }
    }
    catch(...)
    {
		Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }

	if (lbAuthority->Items->Count <= 0)
    	btnRevoke->Enabled = false;
}
//---------------------------------------------------------------------------

