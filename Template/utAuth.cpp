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

        //��ʾ�ܷ��ʵ�ǰ�ڵ�������û���
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
    TStringList * slNodeAddedMid,       //���ڽڵ㣨��Ҷ�ڵ㣩����
    			* slNodeUnAddedLeaf,	//Ҷ�ڵ�ȴ�����
                * slNodeUnAddedMid,		//��Ҷ�ڵ�ȴ�����
                * slNodeWaken;			//�������ѡ��ڵ�ġ�ջ��
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

		//��������
		slNodeAddedMid		= new TStringList;
		slNodeUnAddedLeaf	= new TStringList;
		slNodeUnAddedMid	= new TStringList;
		slNodeWaken			= new TStringList;

		//����tbCategoryһ�飬������Ӧ�����ṹ
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
			//������Ϊ���ڵ�
			{
                //�����ӽڵ�
                tnNodeJustAdded =
                    trvwTemplate->Items->AddChild(NULL,strCaption);
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
				strParentID = OrigStr(dmTemplate->tbCategory->FieldValues["ParentID"]);

				if (strParentID.IsEmpty())
				//������Ϊ���ڵ�
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
    				//�丸�ڵ��Ѿ�����
        			{
                        //�ҵ����ڵ��ʶ�������е�λ��
                        nIndexInTreeView =
                            StrToInt(slNodeAddedMid->Strings[nIndexInStringList+1]);
                        //�ɸ��ڵ��ʶ���ɸ��ڵ�ָ��
                        tnParentNode = trvwTemplate->Items->GetNode((HTREEITEM)nIndexInTreeView);
                        //�����ӽڵ�
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

                            tnTempNode = trvwTemplate->Items->AddChild(tnCurrentNode,strCaption);
                            tnTempNode->Data = new char[NODEIDSIZE];
                            lstrcpy((char *)tnTempNode->Data,strLeafID.c_str());
                            if (nNodeAttr == 1)
                            {
                                tnTempNode->ImageIndex    = 1;
                                tnTempNode->SelectedIndex = 1;
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
                            //�����ѡ�
                            strMidID =
                                slNodeUnAddedMid->Strings[nIndexInStringListMid-2];
                            strCaption =
                                slNodeUnAddedMid->Strings[nIndexInStringListMid-1];

                            tnNodeJustAdded =
                                trvwTemplate->Items->AddChild(tnCurrentNode,strCaption);
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

    		dmTemplate->tbCategory->Next();
		}

		//ɾ������
		delete slNodeAddedMid;
		delete slNodeUnAddedLeaf;
		delete slNodeUnAddedMid;
		delete slNodeWaken;

        if (trvwTemplate->Items->Count > 0)
            trvwTemplate->Selected = trvwTemplate->Items->Item[0];

        //��ʾ�����û���        
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
            //��ѡ����
            {
                strGroupID = lbGroup->Items->Strings[i];
                //����丸�ڵ��Ƿ��Ѿ���Ȩ
                if (! strNodeParent.IsEmpty())
                //�и��ڵ�
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
                    //���ڵ㻹δ��Ȩ
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
            //��ѡ����
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

