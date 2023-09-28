//---------------------------------------------------------------------------
#pragma once
#include <vcl.h>
#pragma hdrstop

#include "StoryData.h"
#include "CommFunc.h"
#include "frm_Main.h"
#include "frm_StoryInput.h"
#include "DataProcess.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
int m_nNodeID;
TTreeNode* tnSelected;
//extern AnsiString strCurrentUserID;
extern int nGroupType;
bool RefreshTree()
{
	TTreeNode * tnParentNode,* tnNodeJustAdded,* tnCurrentNode;
	TTreeNode * tnTempNode;
	AnsiString strCaption,strNodeID,strParentID,strMidID,strLeafID,strSelected;
	AnsiString strSQLClause,strUserAccount;

	TStringList * slNodeAddedMid,       //存在节点（非叶节点）队列
				* slNodeUnAddedLeaf,	//叶节点等待队列
				* slNodeUnAddedMid,		//非叶节点等待队列
				* slNodeWaken;			//被“唤醒”节点的“栈”
	int nIndexInStringList,nIndexInStringListLeaf,nIndexInStringListMid,nIndexInTreeView;
	bool bIsAdded;
	int nNodeAttr;

	if (frmStoryInput->trvwStory->Selected!=NULL)
	  strSelected=(AnsiString)((char*)frmStoryInput->trvwStory->Selected->Data);
	frmStoryInput->trvwStory->Items->Clear();
	tnSelected=NULL;

	try
	{
		if (dmdStory->tbCategory->Active)
			dmdStory->tbCategory->Close();
		if (! dmdStory->tbCategory->Active)
			dmdStory->tbCategory->Open();

		//创建链表
		slNodeAddedMid		= new TStringList;
		slNodeUnAddedLeaf	= new TStringList;
		slNodeUnAddedMid	= new TStringList;
		slNodeWaken			= new TStringList;

		//访问tbCategory一遍，生成相应的树结构
		dmdStory->tbCategory->First();
		while (! dmdStory->tbCategory->Eof)
		{
			bIsAdded = true;

//			strNodeID    = OrigStr((AnsiString)dmdStory->tbCategory->FieldValues["NodeID"]);
			strNodeID    = OrigStr(dmdStory->tbCategory->FieldValues["NodeID"]);
			if (dmdStory->tbCategory->FieldValues["remark"].IsNull())
				strCaption = "";
			else
//				strCaption   = OrigStr((AnsiString)dmdStory->tbCategory->FieldValues["remark"]);
				strCaption   = OrigStr(dmdStory->tbCategory->FieldValues["remark"]);
			nNodeAttr    = (int)dmdStory->tbCategory->FieldValues["NodeAttr"];

			if (dmdStory->tbCategory->FieldValues["ParentID"].IsNull())
			//表明它为根节点
			{
				//创建子节点
				tnNodeJustAdded =
					frmStoryInput->trvwStory->Items->AddChild(NULL,strCaption);
				//开辟节点的额外数据空间，用以存放节点号
				tnNodeJustAdded->Data = new char[NODEIDSIZE];
				lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
				if (strNodeID==strSelected || tnSelected==NULL)
				   tnSelected=tnNodeJustAdded;
				if (nNodeAttr == 1)
				{
					tnNodeJustAdded->ImageIndex    = 1;
					tnNodeJustAdded->SelectedIndex = 1;
				}
			}
			else
			{
				strParentID = OrigStr(dmdStory->tbCategory->FieldValues["ParentID"]);

				if (strParentID.IsEmpty())
				//表明它为根节点
				{
					tnNodeJustAdded =
						frmStoryInput->trvwStory->Items->AddChild(NULL,strCaption);
					tnNodeJustAdded->Data = new char[NODEIDSIZE];
					lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
					if (strNodeID==strSelected || tnSelected==NULL)
					   tnSelected=tnNodeJustAdded;

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
						tnParentNode = frmStoryInput->trvwStory->Items->GetNode((HTREEITEM)nIndexInTreeView);
						//加入子节点
						tnNodeJustAdded =
							frmStoryInput->trvwStory->Items->AddChild(tnParentNode,strCaption);
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

							tnTempNode = frmStoryInput->trvwStory->Items->AddChild(tnCurrentNode,strCaption);
							tnTempNode->Data = new char[NODEIDSIZE];
							lstrcpy((char *)tnTempNode->Data,strLeafID.c_str());
							if (strLeafID==strSelected)
								tnSelected=tnNodeJustAdded;

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
								frmStoryInput->trvwStory->Items->AddChild(tnCurrentNode,strCaption);
							tnNodeJustAdded->Data = new char[NODEIDSIZE];
							lstrcpy((char *)tnNodeJustAdded->Data,strMidID.c_str());
							if (strMidID==strSelected)
								tnSelected=tnNodeJustAdded;


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
							tnCurrentNode = frmStoryInput->trvwStory->Items->GetNode((HTREEITEM)nIndexInTreeView);

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

			dmdStory->tbCategory->Next();
		}

		//删除链表
		delete slNodeAddedMid;
		delete slNodeUnAddedLeaf;
		delete slNodeUnAddedMid;
		delete slNodeWaken;
	}
	catch(...)
	{
		Application->MessageBox(L"Database error,the story tree can not be correctly showed.",L"TBS Library",48);
		return false;
	}
	if (tnSelected!=NULL)
	{
	  tnSelected->Selected=true;
	}
	return true;
}
bool BrowseScript(AnsiString strNodeID)
{
  TListColumn* pColumn;
  TDateTime RegTime;
  AnsiString strSQLClause,strFieldName,strFieldType;
  AnsiString strBeginDate,strEndDate;
  int nCurrentYear;
  int i,nRecords=0;
  TStringList* strlstConfig;

  strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  dmdStory->daqCheck->Open();
  nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

  strBeginDate=frmStoryInput->BeginDate.FormatString("yyyy-mm-dd");
  strEndDate=(frmStoryInput->EndDate).FormatString("yyyy-mm-dd");

  strlstConfig=new TStringList;
  strSQLClause=(AnsiString)"select FieldName,FieldType from lib_BaseFields";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
    dmdStory->daqCheck->Open();
  }
  catch(...)
  {
	frmStoryInput->stbStory->Panels->Items[1]->Text="";
	frmStoryInput->stbStory->Panels->Items[2]->Text="";
	delete strlstConfig;
    return false;
  }

//  frmStoryInput->lstvwStory->BeginUpdate();
  frmStoryInput->lstvwStory->Items->Clear();
  if (!frmStoryInput->m_bRefreshStory)
  {
    frmStoryInput->lstvwStory->Columns->Clear();
  }
  for (i=0;i<dmdStory->daqCheck->RecordCount;i++)
  {
    strFieldType=OrigStr(dmdStory->daqCheck->FieldValues["FieldType"]);
    if ((strFieldType!="image")&&(strFieldType!="text"))
    {
      if (!frmStoryInput->m_bRefreshStory)
      {
        pColumn=frmStoryInput->lstvwStory->Columns->Add();
        pColumn->Caption=OrigStr(dmdStory->daqCheck->FieldValues["FieldName"]);
      }
      strlstConfig->Add(strFieldType);
    }
    dmdStory->daqCheck->Next();
  }
  strSQLClause=(AnsiString)"select FieldName,FieldType from lib_ExtendFields where CategoryID='"
               +SQLStr(strNodeID)+(AnsiString)"'";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
    dmdStory->daqCheck->Open();
  }
  catch(...)
  {
//	frmStoryInput->lstvwStory->EndUpdate();
	frmStoryInput->stbStory->Panels->Items[1]->Text="";
    frmStoryInput->stbStory->Panels->Items[2]->Text="";
    delete strlstConfig;
    return false;
  }

  for (i=0;i<dmdStory->daqCheck->RecordCount;i++)
  {
    strFieldType=OrigStr(dmdStory->daqCheck->FieldValues["FieldType"]);
    if ((strFieldType!="image")&&(strFieldType!="text"))
    {
	  if (!frmStoryInput->m_bRefreshStory)
      {
        pColumn=frmStoryInput->lstvwStory->Columns->Add();
        pColumn->Caption=OrigStr(dmdStory->daqCheck->FieldValues["FieldName"]);
      }
      strlstConfig->Add(strFieldType);
    }
    dmdStory->daqCheck->Next();
  }
  if (!frmStoryInput->m_bRefreshStory)
  {
	for (i=0;i<frmStoryInput->lstvwStory->Columns->Count;i++)
	   frmStoryInput->lstvwStory->Columns->Items[i]->Width = 100;

//    frmStoryInput->lstvwStory->SaveSettings->RegistryKey = (AnsiString)"TBSLibrary\\StoryInput\\"+strNodeID;
//    frmStoryInput->lstvwStory->LoadSettings();
  }
	   frmStoryInput->lstvwStory->Columns->Items[0]->Width = 120;
	   frmStoryInput->lstvwStory->Columns->Items[1]->Width = 120;
  //开始计算记录条数
  try{
	if (frmStoryInput->muiStory_Only->Checked)
    {
      strSQLClause = (AnsiString)"select count(*) from lib_new"+strNodeID
                     +" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
                     +(AnsiString)"'";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

      strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(nCurrentYear)
                     +"_"+strNodeID+" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"
                     +strEndDate+(AnsiString)"'";
	  dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

	}
    else
    {
      strSQLClause = (AnsiString)"select count(*) from lib_new"+strNodeID
                     +" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
                     +(AnsiString)"'";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

      strSQLClause = (AnsiString)"select count(*) from lib_old"
                     +strNodeID+" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"
                     +strEndDate+(AnsiString)"'";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

	  for (i=1990;i<=nCurrentYear;i++)
//	  for (i=1990;i<=2010;i++)
	  {

		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
															   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

        strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(i)
					   +"_"+strNodeID+" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"
					   +strEndDate+(AnsiString)"'";
		dmdStory->daqCheck->SQL->Clear();
		dmdStory->daqCheck->SQL->Add(strSQLClause);
		dmdStory->daqCheck->Open();
		nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;
	  }
	}
    dmdStory->daqCheck->SQL->Clear();
    dmdStory->daqCheck->SQL->Add(strSQLClause);
    frmStoryInput->stbStory->Panels->Items[1]->Text="Story Numbers:"+(AnsiString)nRecords;

    if (frmStoryInput->muiStory_Only->Checked)
    {
      strSQLClause=(AnsiString)"select * from lib_new"+strNodeID+" where cast(WriteTime As Date)>='"
                   +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
                   +(AnsiString)"'";
      dmdStory->daqScript->SQL->Clear();
      dmdStory->daqScript->SQL->Add(strSQLClause);
      dmdStory->daqScript->Open();
      if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
      {
          frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
          delete strlstConfig;
          return true;
      }

      strSQLClause =(AnsiString)"select * from lib_"+IntToStr(nCurrentYear)+"_"+strNodeID+" where cast(WriteTime As Date)>='"
                    +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
                    +(AnsiString)"'";
      dmdStory->daqScript->SQL->Clear();
      dmdStory->daqScript->SQL->Add(strSQLClause);
      dmdStory->daqScript->Open();
      if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
      {
          frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
          delete strlstConfig;
          return true;
      }

    }
    else
    {
      strSQLClause=(AnsiString)"select * from lib_new"+strNodeID+" where cast(WriteTime As Date)>='"
                   +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
                   +(AnsiString)"'";
      dmdStory->daqScript->SQL->Clear();
      dmdStory->daqScript->SQL->Add(strSQLClause);
      dmdStory->daqScript->Open();
      if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
      {
          frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
          delete strlstConfig;
          return true;
      }

      strSQLClause =(AnsiString)"select * from lib_old"+strNodeID+" where cast(WriteTime As Date)>='"
                    +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
                    +(AnsiString)"'";
      dmdStory->daqScript->SQL->Clear();
      dmdStory->daqScript->SQL->Add(strSQLClause);
      dmdStory->daqScript->Open();
      if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
      {
          frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
          delete strlstConfig;
          return true;
	  }

	  for (i=1990;i<=nCurrentYear;i++)
//	  for (i=1990;i<=2010;i++)
	  {
		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
															   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		strSQLClause =(AnsiString)"select * from lib_"+IntToStr(i)+"_"+strNodeID+" where cast(WriteTime As Date)>='"
					  +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
                      +(AnsiString)"'";
        dmdStory->daqScript->SQL->Clear();
        dmdStory->daqScript->SQL->Add(strSQLClause);
        dmdStory->daqScript->Open();
        if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
        {
            frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
            delete strlstConfig;
            return true;
        }

      }

    }
  }
  catch(...)
  {
 //   frmStoryInput->lstvwStory->EndUpdate();
    frmStoryInput->stbStory->Panels->Items[2]->Text="";
    delete strlstConfig;
    return false;
  }

  dmdStory->daqScript->Close();
  delete strlstConfig;
//  frmStoryInput->lstvwStory->EndUpdate();
  frmStoryInput->stbStory->Panels->Items[2]->Text="";
  return true;
}
bool BrowseByID(AnsiString strNodeID,AnsiString strMaterialID)
{
  TListColumn* pColumn;
  TDateTime RegTime;
  AnsiString strSQLClause,strFieldName,strFieldType;
  int nCurrentYear;
  int i,nRecords=0;
  TStringList* strlstConfig;

  strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  dmdStory->daqCheck->Open();
  nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

  strlstConfig=new TStringList;

  strSQLClause=(AnsiString)"select FieldName,FieldType from lib_BaseFields";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
    dmdStory->daqCheck->Open();
  }
  catch(...)
  {
	frmStoryInput->stbStory->Panels->Items[1]->Text="";
	frmStoryInput->stbStory->Panels->Items[2]->Text="";
    delete strlstConfig;
    return false;
  }

//  frmStoryInput->lstvwStory->BeginUpdate();
  frmStoryInput->lstvwStory->Items->Clear();

  if (!frmStoryInput->m_bRefreshStory)
     frmStoryInput->lstvwStory->Columns->Clear();
  for (i=0;i<dmdStory->daqCheck->RecordCount;i++)
  {
    strFieldType=OrigStr(dmdStory->daqCheck->FieldValues["FieldType"]);
    if ((strFieldType!="image")&&(strFieldType!="text"))
    {
      if (!frmStoryInput->m_bRefreshStory)
      {
        pColumn=frmStoryInput->lstvwStory->Columns->Add();
        pColumn->Caption=OrigStr(dmdStory->daqCheck->FieldValues["FieldName"]);
      }
      strlstConfig->Add(strFieldType);
    }
    dmdStory->daqCheck->Next();
  }
  strSQLClause=(AnsiString)"select FieldName,FieldType from lib_ExtendFields where CategoryID='"
               +SQLStr(strNodeID)+(AnsiString)"'";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
    dmdStory->daqCheck->Open();
  }
  catch(...)
  {
 //   frmStoryInput->lstvwStory->EndUpdate();
    frmStoryInput->stbStory->Panels->Items[1]->Text="";
    frmStoryInput->stbStory->Panels->Items[2]->Text="";
    delete strlstConfig;
    return false;
  }

  for (i=0;i<dmdStory->daqCheck->RecordCount;i++)
  {
    strFieldType=OrigStr(dmdStory->daqCheck->FieldValues["FieldType"]);
    if ((strFieldType!="image")&&(strFieldType!="text"))
    {
      if (!frmStoryInput->m_bRefreshStory)
      {
        pColumn=frmStoryInput->lstvwStory->Columns->Add();
        pColumn->Caption=OrigStr(dmdStory->daqCheck->FieldValues["FieldName"]);
	  }
	  strlstConfig->Add(strFieldType);
    }
	dmdStory->daqCheck->Next();
  }

  if (!frmStoryInput->m_bRefreshStory)
  {
	for (i=0;i<frmStoryInput->lstvwStory->Columns->Count;i++)
	   frmStoryInput->lstvwStory->Columns->Items[i]->Width = 100;

 //   frmStoryInput->lstvwStory->SaveSettings->RegistryKey = (AnsiString)"TBSLibrary\\StoryInput\\"+strNodeID;
//    frmStoryInput->lstvwStory->LoadSettings();
  }
  frmStoryInput->lstvwStory->Columns->Items[0]->Width = 120;
  frmStoryInput->lstvwStory->Columns->Items[1]->Width = 120;
  try{
	if (frmStoryInput->muiStory_Only->Checked)
	{
	  strSQLClause = (AnsiString)"select count(*) from lib_new"+strNodeID
					 +" where MaterialID='"+SQLStr(strMaterialID)
					 +(AnsiString)"'";
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);
	  dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

	  strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(nCurrentYear)
					 +"_"+strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
					 +(AnsiString)"'";
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);
	  dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

	}
	else
	{
	  strSQLClause = (AnsiString)"select count(*) from lib_new"+strNodeID
					 +" where MaterialID='"+SQLStr(strMaterialID)
					 +(AnsiString)"'";
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);
	  dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

	  strSQLClause = (AnsiString)"select count(*) from lib_old"
					 +strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
					 +(AnsiString)"'";
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);
	  dmdStory->daqCheck->Open();
	  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

//	  for (i=1990;i<=2010;i++)
	  for (i=1990;i<=nCurrentYear;i++)
	  {

		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
															   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(i)
					   +"_"+strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
					   +(AnsiString)"'";
		dmdStory->daqCheck->SQL->Clear();
		dmdStory->daqCheck->SQL->Add(strSQLClause);
		dmdStory->daqCheck->Open();
		nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;
	  }
	}
	dmdStory->daqCheck->SQL->Clear();
	dmdStory->daqCheck->SQL->Add(strSQLClause);
	frmStoryInput->stbStory->Panels->Items[1]->Text="Story Numbers:"+(AnsiString)nRecords;

	if (frmStoryInput->muiStory_Only->Checked)
	{
	  strSQLClause=(AnsiString)"select * from lib_new"+strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
				   +(AnsiString)"'";
	  dmdStory->daqScript->SQL->Clear();
	  dmdStory->daqScript->SQL->Add(strSQLClause);
	  dmdStory->daqScript->Open();
	  if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
	  {
		  frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
		  delete strlstConfig;
		  return true;
	  }

	  strSQLClause =(AnsiString)"select * from lib_"+IntToStr(nCurrentYear)+"_"+strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
					+(AnsiString)"'";
	  dmdStory->daqScript->SQL->Clear();
	  dmdStory->daqScript->SQL->Add(strSQLClause);
	  dmdStory->daqScript->Open();
	  if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
	  {
		  frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
		  delete strlstConfig;
		  return true;
	  }

	}
	else
	{
	  strSQLClause=(AnsiString)"select * from lib_new"+strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
				   +(AnsiString)"'";
	  dmdStory->daqScript->SQL->Clear();
	  dmdStory->daqScript->SQL->Add(strSQLClause);
	  dmdStory->daqScript->Open();
	  if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
	  {
		  frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
		  delete strlstConfig;
		  return true;
	  }

	  strSQLClause =(AnsiString)"select * from lib_old"+strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
					+(AnsiString)"'";
	  dmdStory->daqScript->SQL->Clear();
	  dmdStory->daqScript->SQL->Add(strSQLClause);
	  dmdStory->daqScript->Open();
	  if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
	  {
		  frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
		  delete strlstConfig;
		  return true;
	  }

	  for (i=1990;i<=nCurrentYear;i++)
//	  for (i=1990;i<=2010;i++)
	  {
	  dmdStory->daqCheckTable->Close();
	  dmdStory->daqCheckTable->SQL->Clear();
	  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
														   String(i)+ "_"+ strNodeID+ "')");
	  dmdStory->daqCheckTable->Open();
	  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		strSQLClause =(AnsiString)"select * from lib_"+IntToStr(i)+"_"+strNodeID+" where MaterialID='"+SQLStr(strMaterialID)
					  +(AnsiString)"'";
		dmdStory->daqScript->SQL->Clear();
		dmdStory->daqScript->SQL->Add(strSQLClause);
		dmdStory->daqScript->Open();
		if (FillListView(dmdStory->daqScript,strlstConfig,frmStoryInput->lstvwStory)==1)
		{
			frmStoryInput->stbStory->Panels->Items[2]->Text="The Browsing list is too long";
			delete strlstConfig;
			return true;
		}
	  }
	}
  }
  catch(...)
  {
  //  frmStoryInput->lstvwStory->EndUpdate();
	frmStoryInput->stbStory->Panels->Items[2]->Text="";
	delete strlstConfig;
	return false;
  }

  dmdStory->daqScript->Close();
  delete strlstConfig;
 // frmStoryInput->lstvwStory->EndUpdate();
  frmStoryInput->stbStory->Panels->Items[2]->Text="";
  return true;

}


bool AddStory(AnsiString strNodeID,TInputInfo* pInfo)
{
  TListItem* pListItem;
  AnsiString strSQLClause;
  AnsiString strFileID,strWriteTime;
  AnsiString strFieldName,strFieldType;
  AnsiString strFieldValue;
  AnsiString strError;
  TDateTime time;
  TBlobStream *pStream;
  int i,nIndex;
  TStringList *strlstFieldName,*strlstFieldType;

  // call store procedure
  strSQLClause=(AnsiString)"GetNewFileID";
  dmdStory->daqScript->SQL->Clear();
  dmdStory->daqScript->SQL->Add(strSQLClause);

  try
  {
	dmdStory->daqScript->Open();
	dmdStory->daqScript->First();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error.",L"TBS Library",48);
	return false;
  }

  strFileID=dmdStory->daqScript->FieldValues["FileID"];
  strSQLClause=(AnsiString)"select GetDate()";
  dmdStory->daqScript->SQL->Clear();
  dmdStory->daqScript->SQL->Add(strSQLClause);
  try
  {
	dmdStory->daqScript->Open();
	dmdStory->daqScript->First();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error,the operation failed.",L"TBS Library",48);
	return false;
  }
  strWriteTime=dmdStory->daqScript->FieldValues["Column1"];

  if (dmdStory->tbStory->Active)
	 dmdStory->tbStory->Close();
  dmdStory->tbStory->TableName=(AnsiString)"lib_new"+strNodeID;
  strSQLClause=(AnsiString)"select FieldName,FieldType from lib_ExtendFields where CategoryID='"
			   +SQLStr(strNodeID)+(AnsiString)"'";
  dmdStory->daqFields->SQL->Clear();
  dmdStory->daqFields->SQL->Add(strSQLClause);

  strlstFieldName=new TStringList;
  strlstFieldType=new TStringList;

  try
  {
	dmdStory->daqFields->Open();

	for (i=0;i<dmdStory->daqFields->RecordCount;i++)
	{
	  strFieldName=OrigStr(dmdStory->daqFields->FieldValues["FieldName"]);
	  strFieldType=OrigStr(dmdStory->daqFields->FieldValues["FieldType"]);
	  strlstFieldName->Add(strFieldName);
	  strlstFieldType->Add(strFieldType);
	  dmdStory->daqFields->Next();
	}

	if (!dmdStory->tbStory->Active)
	   dmdStory->tbStory->Open();
	dmdStory->tbStory->Append();
	dmdStory->tbStory->FieldByName("StoryID")->AsString=SQLStr(strFileID);
	dmdStory->tbStory->FieldByName("WriteTime")->AsDateTime=(TDateTime)strWriteTime;

	for (i=0;i<pInfo->slInfoType->Count;i++)
	{
	  strFieldName=pInfo->slInfoType->Strings[i];
	  if ((strFieldName.IsEmpty())||(strFieldName.Trim()!=strFieldName))
		 continue;
	  strFieldValue=((AnsiString)((char*)pInfo->lInfo->Items[i])).Trim();

	  if (strFieldName=="MaterialID")
	  {
		if (!strFieldValue.IsEmpty())
		{
		  strSQLClause=(AnsiString)"select ResID from lib_Res where ResID='"
						+SQLStr(strFieldValue)+(AnsiString)"'";
		  dmdStory->daqCheck->SQL->Clear();
		  dmdStory->daqCheck->SQL->Add(strSQLClause);
		  dmdStory->daqCheck->Open();
		  if (dmdStory->daqCheck->RecordCount<=0)
		  {
			if ((Application->MessageBox(L"The material associated with the story is not exist in the library,will you want to continue?",L"TBS Library",MB_OKCANCEL))==IDCANCEL)
			{
			  delete strlstFieldName;
			  delete strlstFieldType;
			  return false;
			}
			dmdStory->tbStory->FieldByName("MaterialID")->AsString=strFieldValue+(AnsiString)".";
		  }
		  else
		  {
			strSQLClause=(AnsiString)"update lib_Res set AstLibs='"
						 +SQLStr(strNodeID)+(AnsiString)"'where ResID='"
						 +SQLStr(strFieldValue)+(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->ExecSQL();
			dmdStory->tbStory->FieldByName("MaterialID")->AsString=strFieldValue+(AnsiString)".";
		  }
		}
		else
		{
			Application->MessageBox(L"Please input material ID!",L"TBS Library Warning",MB_OK);
			delete strlstFieldName;
			delete strlstFieldType;
			return false;
		}
	  }
	  else
	  {
		strFieldType=strlstFieldType->Strings
		   [strlstFieldName->IndexOf(strFieldName)];
		if (strFieldType=="int")
		{
		   try{
				 if (!strFieldValue.IsEmpty())
					 dmdStory->tbStory->FieldByName(strFieldName)->AsInteger
						=strFieldValue.ToInt();
			  }
		   catch(...)
		   {
			  strError=strFieldName+(AnsiString)" must be integer,'"+strFieldValue
					  +(AnsiString)"' is not a valid integer,please input again.";
			  Application->MessageBox(UnicodeString(strError).c_str(),L"TBS Library",48);
			  delete strlstFieldName;
			  delete strlstFieldType;
			  return false;
		   }
		}
		if (strFieldType=="varchar")
		   dmdStory->tbStory->FieldByName(strFieldName)->AsString
				 =strFieldValue+(AnsiString)".";
		if (strFieldType=="text")
		{
		   dmdStory->tbStory->FieldByName(strFieldName)->AsString
				 =strFieldValue+(AnsiString)".";
		}
		if (strFieldType=="datetime")
		{

		   if (!strFieldValue.IsEmpty())
			   dmdStory->tbStory->FieldByName(strFieldName)->AsDateTime
					 =(TDateTime)(strFieldValue.ToInt());

		}
		if (strFieldType=="image")
		{
		   pStream=new TBlobStream((TBlobField*)dmdStory->tbStory->FieldByName(strFieldName),bmReadWrite);
		   html->GetImage(pStream,strFieldName);
		   delete pStream;
		}
	  }
	}
	dmdStory->tbStory->Post();
	dmdStory->tbStory->Close();

	CreateLog("Add Story",strFileID,"","Story Input");

	pListItem=frmStoryInput->lstvwStory->Items->Add();
	pListItem->Caption=strFileID;
	for (i=0;i<frmStoryInput->lstvwStory->Columns->Count;i++)
		pListItem->SubItems->Add("");
	time=(TDateTime)strWriteTime;
	pListItem->SubItems->Strings[0]=time.FormatString("yyyy-mm-dd hh:nn");
	for (i=0;i<pInfo->slInfoType->Count;i++)
	{
	  strFieldName=pInfo->slInfoType->Strings[i];
	  if ((strFieldName.IsEmpty())||(strFieldName.Trim()!=strFieldName))
		 continue;

	  for (nIndex=0;nIndex<frmStoryInput->lstvwStory->Columns->Count;nIndex++)
	  {
		strFieldValue=frmStoryInput->lstvwStory->Columns->Items[nIndex]->Caption;
		if (strFieldValue==strFieldName)
		   break;
	  }
	  if (nIndex<frmStoryInput->lstvwStory->Columns->Count)
	  {
		strFieldValue=(AnsiString)((char*)pInfo->lInfo->Items[i]);
		if (strFieldName!="MaterialID")
		{
		  strFieldType=strlstFieldType->Strings
			 [strlstFieldName->IndexOf(strFieldName)];
		  if (strFieldType=="datetime")
		  {
			time=(TDateTime)strFieldValue.ToInt();
			pListItem->SubItems->Strings[nIndex-1]
				  =time.FormatString("yyyy-mm-dd");
		  }
		  else
			pListItem->SubItems->Strings[nIndex-1]=strFieldValue;
		}
		else
			pListItem->SubItems->Strings[nIndex-1]=strFieldValue;
	  }
	}

  }
  catch(...)
  {
	Application->MessageBox(L"Database error,the operation failed.",L"TBS Library",48);
	dmdStory->tbStory->Close();
	delete strlstFieldName;
	delete strlstFieldType;
	return false;
  }

  delete strlstFieldName;
  delete strlstFieldType;
  return true;
}

bool ModifyStory(AnsiString strNodeID,AnsiString strStoryID,TInputInfo* pInfo)
{
  AnsiString strSQLClause;
  AnsiString strFieldName,strFieldType;
  AnsiString strFieldValue;
  AnsiString strError;
  AnsiString strTime;
  bool bModifyDate,bHasStory;
  TLocateOptions LocateOption;
  TBlobStream *pStream;
  int i,nIndex;
  TDateTime time,WriteTime;
  TStringList *strlstFieldName,*strlstFieldType;

  strSQLClause=(AnsiString)"select StoryID from lib_new"+strNodeID+(AnsiString)" where StoryID='"
			   +SQLStr(strStoryID)+(AnsiString)"'";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  try
  {
	dmdStory->daqCheck->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error,the operation is not succeeded.",L"TBS Library",48);
	return false;
  }
  if (dmdStory->tbStory->Active)
	 dmdStory->tbStory->Close();
  if (dmdStory->daqCheck->RecordCount<1)
  {
	 if (frmStoryInput->strDate.ToInt()<1990)
		 dmdStory->tbStory->TableName=(AnsiString)"lib_old"+strNodeID;
	 else
		 dmdStory->tbStory->TableName=(AnsiString)"lib_"+frmStoryInput->strDate+"_"+strNodeID;
  }
  else
	 dmdStory->tbStory->TableName=(AnsiString)"lib_new"+strNodeID;
  dmdStory->tbStory->Filter = "StoryID = '" + SQLStr(strStoryID) + "'";

  strSQLClause=(AnsiString)"select FieldName,FieldType from lib_ExtendFields where CategoryID='"
			   +SQLStr(strNodeID)+(AnsiString)"'";
  dmdStory->daqFields->SQL->Clear();
  dmdStory->daqFields->SQL->Add(strSQLClause);
  strlstFieldName=new TStringList;
  strlstFieldType=new TStringList;
  try
  {
	dmdStory->daqFields->Open();

	for (i=0;i<dmdStory->daqFields->RecordCount;i++)
	{
	  strFieldName=OrigStr(dmdStory->daqFields->FieldValues["FieldName"]);
	  strFieldType=OrigStr(dmdStory->daqFields->FieldValues["FieldType"]);
	  strlstFieldName->Add(strFieldName);
	  strlstFieldType->Add(strFieldType);
	  dmdStory->daqFields->Next();
	}
	if (!dmdStory->tbStory->Active)
	  dmdStory->tbStory->Open();
	dmdStory->tbStory->Filter = "StoryID = '" + SQLStr(strStoryID) + "'";

//	Application->MessageBox(IntToStr(dmdStory->tbStory->RecordCount).c_str(),L"TBS Library",48);


	LocateOption.Clear();
	bHasStory = dmdStory->tbStory->Locate("StoryID",SQLStr(strStoryID),LocateOption);
	if (!bHasStory)
	{
		Application->MessageBox(L"This story has been deleted by someone else,please refresh the story list.",L"TBS Library",48);
		if (dmdStory->tbStory->Active)
			dmdStory->tbStory->Close();
		delete strlstFieldName;
		delete strlstFieldType;
		return false;
	}
	WriteTime = dmdStory->tbStory->FieldByName("WriteTime")->AsDateTime;
	for (i=0;i<pInfo->slInfoType->Count;i++)
	{
		strFieldName = pInfo->slInfoType->Strings[i];
		if (strFieldName=="Date")
		{
			strFieldValue=((AnsiString)((char*)pInfo->lInfo->Items[i])).Trim();
			time = (TDateTime)strFieldValue.ToInt();
			strTime = time.FormatString("yyyy");
			if (strTime == frmStoryInput->strDate)
				bModifyDate = false;
			else
				bModifyDate = true;
		}
	}
	if ((!bModifyDate)||(dmdStory->tbStory->TableName=="lib_new"+strNodeID))
	{
	  dmdStory->tbStory->Edit();
	  for (i=0;i<pInfo->slInfoType->Count;i++)
	  {
		strFieldName=pInfo->slInfoType->Strings[i];
		if ((strFieldName.IsEmpty())||(strFieldName.Trim()!=strFieldName))
		   continue;
		strFieldValue=((AnsiString)((char*)pInfo->lInfo->Items[i])).Trim();
		if (strFieldName=="MaterialID")
		{

		  if (!strFieldValue.IsEmpty())
		  {
			  strSQLClause=(AnsiString)"select ResID from lib_Res where ResID='"
						   +SQLStr(strFieldValue)+(AnsiString)"'";
			  dmdStory->daqCheck->SQL->Clear();
			  dmdStory->daqCheck->SQL->Add(strSQLClause);
			  dmdStory->daqCheck->Open();
			  if (dmdStory->daqCheck->RecordCount<=0)
			  {
				if ((Application->MessageBox(L"The material associated with the story is not exist in the library,will you want to continue?",L"TBS Library",MB_OKCANCEL))==IDCANCEL)
				{
				  delete strlstFieldName;
				  delete strlstFieldType;
				  return false;
				}
				dmdStory->tbStory->FieldByName("MaterialID")->AsString=SQLStr(strFieldValue);
			  }
			  else
			  {
				strSQLClause=(AnsiString)"update lib_Res set AstLibs='"
							 +SQLStr(strNodeID)+(AnsiString)"'where ResID='"
							 +SQLStr(strFieldValue)+(AnsiString)"'";
				dmdStory->daqCheck->SQL->Clear();
				dmdStory->daqCheck->SQL->Add(strSQLClause);
				dmdStory->daqCheck->ExecSQL();
				dmdStory->tbStory->FieldByName("MaterialID")->AsString=strFieldValue+(AnsiString)".";
			  }
		  }
		  else
			dmdStory->tbStory->FieldByName("MaterialID")->AsString=strFieldValue+(AnsiString)".";
		}
		else
		{
		  strFieldType=strlstFieldType->Strings
			 [strlstFieldName->IndexOf(strFieldName)];
		  if (strFieldType=="int")
		  {
			 try{
				   if (!strFieldValue.IsEmpty())
					   dmdStory->tbStory->FieldByName(strFieldName)->AsInteger
						  =strFieldValue.ToInt();
				   else
					   dmdStory->tbStory->FieldByName(strFieldName)->Clear();
				}
			 catch(...)
			 {
				strError=strFieldName+(AnsiString)" must be integer,'"+strFieldValue
						+(AnsiString)"' is not a valid integer,please input again.";
				Application->MessageBox(UnicodeString(strError).c_str(),L"TBS Library",48);
				delete strlstFieldName;
				delete strlstFieldType;
				return false;
			 }
		  }
		  if (strFieldType=="varchar")
		  {
			 dmdStory->tbStory->FieldByName(strFieldName)->AsString
				   =strFieldValue+(AnsiString)".";
		  }
		  if (strFieldType=="text")
			 dmdStory->tbStory->FieldByName(strFieldName)->AsString
				   =strFieldValue+(AnsiString)".";
		  if (strFieldType=="datetime")
		  {
			 if (!strFieldValue.IsEmpty())
			 {
				 if (strFieldValue.ToInt()!=0)
					 dmdStory->tbStory->FieldByName(strFieldName)->AsDateTime
						   =(TDateTime)strFieldValue.ToInt();
			 }
		  }
		  if (strFieldType=="image")
		  {
			 pStream=new TBlobStream((TBlobField*)dmdStory->tbStory->FieldByName(strFieldName),bmReadWrite);
			 html->GetImage(pStream,strFieldName);
			 delete pStream;
		  }
		}
	  }
	  dmdStory->tbStory->Post();
	  dmdStory->tbStory->Close();
	}
	else
	{
	  dmdStory->tbStory->Delete();
	  if (dmdStory->tbStory->Active)
		  dmdStory->tbStory->Close();
	  dmdStory->tbStory->TableName = (AnsiString)"lib_new"+strNodeID;
	  if (!dmdStory->tbStory->Active)
		  dmdStory->tbStory->Open();
	  dmdStory->tbStory->Append();
	  dmdStory->tbStory->FieldByName("StoryID")->AsString
				  =SQLStr(strStoryID);
	  dmdStory->tbStory->FieldByName("WriteTime")->AsDateTime
				  =WriteTime;

	  for (i=0;i<pInfo->slInfoType->Count;i++)
	  {
		strFieldName=pInfo->slInfoType->Strings[i];
		if ((strFieldName.IsEmpty())||(strFieldName.Trim()!=strFieldName))
		   continue;
		strFieldValue=((AnsiString)((char*)pInfo->lInfo->Items[i])).Trim();
		if (strFieldName=="MaterialID")
		{
		  if (!strFieldValue.IsEmpty())
		  {
			  strSQLClause=(AnsiString)"select ResID from lib_Res where ResID='"
						   +SQLStr(strFieldValue)+(AnsiString)"'";
			  dmdStory->daqCheck->SQL->Clear();
			  dmdStory->daqCheck->SQL->Add(strSQLClause);
			  dmdStory->daqCheck->Open();
			  if (dmdStory->daqCheck->RecordCount<=0)
			  {
				if ((Application->MessageBox(L"The material associated with the story is not exist in the library,will you want to continue?",L"TBS Library",MB_OKCANCEL))==IDCANCEL)
				{
				  delete strlstFieldName;
				  delete strlstFieldType;
				  return false;
				}
				dmdStory->tbStory->FieldByName("MaterialID")->AsString=strFieldValue+(AnsiString)".";
			  }
			  else
			  {
				strSQLClause=(AnsiString)"update lib_Res set AstLibs='"
							 +SQLStr(strNodeID)+(AnsiString)"'where ResID='"
							 +SQLStr(strFieldValue)+(AnsiString)"'";
				dmdStory->daqCheck->SQL->Clear();
				dmdStory->daqCheck->SQL->Add(strSQLClause);
				dmdStory->daqCheck->ExecSQL();
				dmdStory->tbStory->FieldByName("MaterialID")->AsString=strFieldValue+(AnsiString)".";
			  }
		  }
		  else
			dmdStory->tbStory->FieldByName("MaterialID")->AsString=strFieldValue+(AnsiString)".";
		}
		else
		{
		  strFieldType=strlstFieldType->Strings
			 [strlstFieldName->IndexOf(strFieldName)];
		  if (strFieldType=="int")
		  {
			 try{
				   if (!strFieldValue.IsEmpty())
					   dmdStory->tbStory->FieldByName(strFieldName)->AsInteger
						  =strFieldValue.ToInt();
				   else
					   dmdStory->tbStory->FieldByName(strFieldName)->Clear();
				}
			 catch(...)
			 {
				strError=strFieldName+(AnsiString)" must be integer,'"+strFieldValue
						+(AnsiString)"' is not a valid integer,please input again.";
				Application->MessageBox(UnicodeString(strError).c_str(),L"TBS Library",48);
				delete strlstFieldName;
				delete strlstFieldType;
				return false;
			 }
		  }
		  if (strFieldType=="varchar")
			 dmdStory->tbStory->FieldByName(strFieldName)->AsString
				   =strFieldValue+(AnsiString)".";
		  if (strFieldType=="text")
			 dmdStory->tbStory->FieldByName(strFieldName)->AsString
				   =strFieldValue+(AnsiString)".";
		  if (strFieldType=="datetime")
		  {
			 if (!strFieldValue.IsEmpty())
			 {
				 if (strFieldValue.ToInt()!=0)
					 dmdStory->tbStory->FieldByName(strFieldName)->AsDateTime
						   =(TDateTime)strFieldValue.ToInt();
			 }
		  }
		  if (strFieldType=="image")
		  {
			 pStream=new TBlobStream((TBlobField*)dmdStory->tbStory->FieldByName(strFieldName),bmReadWrite);
			 html->GetImage(pStream,strFieldName);
			 delete pStream;
		  }
		}
	  }
	  dmdStory->tbStory->Post();
	  dmdStory->tbStory->Close();

	}
	CreateLog("Modify Story",strStoryID,"","Story Input");

	for (i=0;i<pInfo->slInfoType->Count;i++)
	{
	  strFieldName=pInfo->slInfoType->Strings[i];
	  if ((strFieldName.IsEmpty())||(strFieldName.Trim()!=strFieldName))
		 continue;

	  for (nIndex=0;nIndex<frmStoryInput->lstvwStory->Columns->Count;nIndex++)
	  {
		strFieldValue=frmStoryInput->lstvwStory->Columns->Items[nIndex]->Caption;
		if (strFieldValue==strFieldName)
		  break;
	  }
	  if ((nIndex<frmStoryInput->lstvwStory->Columns->Count)&&(nIndex>0))
	  {
		strFieldValue=(AnsiString)((char*)pInfo->lInfo->Items[i]);
		if (strFieldName!="MaterialID")
		{
		  strFieldType=strlstFieldType->Strings
			 [strlstFieldName->IndexOf(strFieldName)];
		  if (strFieldType=="datetime")
		  {
			if (!strFieldValue.IsEmpty())
			{
			  if (strFieldValue.ToInt()!=0)
			  {
				time=(TDateTime)strFieldValue.ToInt();
				frmStoryInput->lstvwStory->ItemFocused->SubItems->Strings[nIndex-1]
					=time.FormatString("yyyy-mm-dd");
			  }
			}
		  }
		  else
			frmStoryInput->lstvwStory->ItemFocused->SubItems->Strings[nIndex-1]
				 =strFieldValue;
		}
		else
			frmStoryInput->lstvwStory->ItemFocused->SubItems->Strings[nIndex-1]
				=strFieldValue;
	  }
	}
  }
  catch(...)
  {
	Application->MessageBox(L"Database error,the operation is not succeeded.",L"TBS Library",48);
	delete strlstFieldName;
	delete strlstFieldType;
	return false;
  }
  delete strlstFieldName;
  delete strlstFieldType;
  return true;

}


bool DeleteStory(AnsiString strNodeID)
{
  TListItem *pListItem,*pNextItem;
  TItemStates ItemState;
  AnsiString strBeginDate,strEndDate;
  int nSelected,i;
  int nCurrentYear;
  AnsiString strSQLClause;
  AnsiString strStoryID;
  AnsiString strYear;

  strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  dmdStory->daqCheck->Open();
  nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

  ItemState<<isSelected;
  pListItem=frmStoryInput->lstvwStory->Selected;
  nSelected=frmStoryInput->lstvwStory->SelCount;
  strYear = "";
  for (i=0;i<frmStoryInput->lstvwStory->Columns->Count;i++)
  {
	  if (frmStoryInput->lstvwStory->Columns->Items[i]->Caption == "Date")
	  {
		  strYear = frmStoryInput->lstvwStory->ItemFocused->SubItems->Strings[i-1].SubString(0,4);
		  break;
	  }
  }

  try
  {
	for (i=0;i<nSelected;i++)
	{
	  pNextItem=frmStoryInput->lstvwStory->GetNextItem(pListItem,sdBelow,ItemState);
	  strStoryID=pListItem->Caption;
	  if (strStoryID.IsEmpty())
		 return false;
	  dmdStory->daqScript->SQL->Clear();
	  strSQLClause=(AnsiString)"delete lib_new"+strNodeID+(AnsiString)" where StoryID='"
				   +SQLStr(strStoryID)+(AnsiString)"'";
	  dmdStory->daqScript->SQL->Add(strSQLClause);
	  if (!strYear.IsEmpty())
	  {
		if (strYear.ToInt()<=nCurrentYear)
		{
		  if (strYear.ToInt()<1990)
			  strSQLClause=(AnsiString)"delete lib_old"+strNodeID+(AnsiString)" where StoryID='"
						   +SQLStr(strStoryID)+(AnsiString)"'";
		  else
			  strSQLClause=(AnsiString)"delete lib_"+strYear+"_"+strNodeID+(AnsiString)" where StoryID='"
						   +SQLStr(strStoryID)+(AnsiString)"'";
		  dmdStory->daqScript->SQL->Add(strSQLClause);
		}
	  }
	  dmdStory->daqScript->ExecSQL();
	  CreateLog("Delete Story",strStoryID,"","Story Input");
	  frmStoryInput->lstvwStory->Items->Delete(frmStoryInput->lstvwStory->Items->IndexOf(pListItem));
	  pListItem=pNextItem;
	}
	strBeginDate = frmStoryInput->BeginDate.FormatString("yyyy-mm-dd");
	strEndDate = frmStoryInput->EndDate.FormatString("yyyy-mm-dd");
	frmStoryInput->ShowStoryNum();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error,the selected story can not be deleted.",L"TBS Library",48);
	return false;
  }

  frmStoryInput->stbStory->Panels->Items[2]->Text=IntToStr(nSelected)+(AnsiString)" story deleted.";
  return true;
}

bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module)
{
  AnsiString strSQLClause,strFileID;
  if (Operation=="Add Story")
	  strFileID=ObjID;
  else
  {
	  strSQLClause =(AnsiString)"exec GetNewFileID";
	  dmdStory->daqCheck->Close();
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);
	  try
	  {
		dmdStory->daqCheck->Open();
	  }
	  catch(...)
	  {
		Application->MessageBox(L"Failed update table lib_Log",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		return false;
	  }
	  dmdStory->daqCheck->First();
	  strFileID=dmdStory->daqCheck->FieldValues["FileID"];
  }

  strSQLClause = (AnsiString)"Insert into dbo.lib_Log (LogID,OpID,OpTime,Operation,ObjID,ObjStr,Module) values('"
				  +(AnsiString)SQLStr(strFileID)
				  +(AnsiString)"','"
//				  +SQLStr(dfrm->strUserID)
				  +SQLStr(dmdStory->strCurrentUserID)
				  +(AnsiString)"',GetDate(),'"
				  +SQLStr(Operation)
				  +(AnsiString)"','"
				  +SQLStr(ObjID)
				  +(AnsiString)"','"
				  +SQLStr(ObjStr)
				  +(AnsiString)"','"
				  +SQLStr(Module)
				  +(AnsiString)"')";
 dmdStory->daqCheck->SQL->Clear();
 dmdStory->daqCheck->SQL->Add(strSQLClause);
 try
 {
  dmdStory->daqCheck->ExecSQL();
 }
 catch(...)
 {
  Application->MessageBox(L"Failed update table lib_Log.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  return false;
 }
 return true;
}

bool StoryModify(AnsiString strNodeID,AnsiString strStoryID,AnsiString strDate)
{
   TDateTime time;
   int i;
   AnsiString strSQLClause;
   AnsiString strFieldName,strFieldType;
   AnsiString strTbname;
   int nCurrentYear;
   TInputInfo* pInfo;
   TStringList* strlstFields;
   TBlobStream* pStream;

   pInfo=new TInputInfo;

   strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
   dmdStory->daqCheck->SQL->Clear();
   dmdStory->daqCheck->SQL->Add(strSQLClause);
   dmdStory->daqCheck->Open();
   nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

   strSQLClause=(AnsiString)"select * from lib_ExtendFields where CategoryID='"
				+SQLStr(strNodeID)+(AnsiString)"'";
   dmdStory->daqFields->SQL->Clear();
   dmdStory->daqFields->SQL->Add(strSQLClause);
   try
   {
	 dmdStory->daqFields->Open();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error.",L"TBS Library",48);
	 delete pInfo;
	 return false;
   }
   strlstFields=new TStringList;
   while (!dmdStory->daqFields->Eof)
   {
	 strFieldName=OrigStr(dmdStory->daqFields->FieldValues["FieldName"]);
	 strFieldType=OrigStr(dmdStory->daqFields->FieldValues["FieldType"]);
	 strlstFields->Add(strFieldName+"="+strFieldType);
	 dmdStory->daqFields->Next();
   }

   html->GetControlText(pInfo);

   strSQLClause=(AnsiString)"select StoryID from lib_new"+strNodeID+" where StoryID='"
				+SQLStr(strStoryID)+(AnsiString)"'";
   dmdStory->daqScript->SQL->Clear();
   dmdStory->daqScript->SQL->Add(strSQLClause);
   try
   {
	 dmdStory->daqScript->Open();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error.",L"TBS Library",48);
	 delete pInfo;
	 delete strlstFields;
	 return false;
   }
   if (dmdStory->daqScript->RecordCount<1)
   {
	 if (frmStoryInput->strDate.IsEmpty())
	 {
		 Application->MessageBox(L"The story does not exist,please refresh the story list.",L"TBS Library",48);
		 delete pInfo;
		 delete strlstFields;
		 return false;
	 }
	 if (frmStoryInput->strDate.ToInt()>nCurrentYear)
	 {
		 Application->MessageBox(L"The story does not exist,please refresh the story list.",L"TBS Library",48);
		 delete pInfo;
		 delete strlstFields;
		 return false;
	 }
	 if (frmStoryInput->strDate.ToInt()<1990)
		  strSQLClause=(AnsiString)"select * from lib_old"+strNodeID+" where StoryID='"
				  +SQLStr(strStoryID)+(AnsiString)"'";
	 else
		  strSQLClause=(AnsiString)"select * from lib_"+frmStoryInput->strDate+"_"+strNodeID+" where StoryID='"
				  +SQLStr(strStoryID)+(AnsiString)"'";
	 dmdStory->daqScript->SQL->Clear();
	 dmdStory->daqScript->SQL->Add(strSQLClause);
	 try
	 {
	   dmdStory->daqScript->Open();
	   if ((frmStoryInput->strDate.IsEmpty())||(frmStoryInput->strDate.ToInt()<1990))
		 strTbname=(AnsiString)"lib_old"+strNodeID;
	   else
		 strTbname=(AnsiString)"lib_"+frmStoryInput->strDate+"_"+strNodeID;
	 }
	 catch(...)
	 {
	   Application->MessageBox(L"Database error.",L"TBS Library",48);
	   delete pInfo;
	   delete strlstFields;
	   return false;
	 }
   }
   else
   {
	 strSQLClause=(AnsiString)"select * from lib_new"+strNodeID+" where StoryID='"
				  +SQLStr(strStoryID)+(AnsiString)"'";
	 dmdStory->daqScript->SQL->Clear();
	 dmdStory->daqScript->SQL->Add(strSQLClause);
	 try
	 {
	   dmdStory->daqScript->Open();
	   strTbname=(AnsiString)"lib_new"+strNodeID;
	 }
	 catch(...)
	 {
	   Application->MessageBox(L"Database error.",L"TBS Library",48);
	   delete pInfo;
	   delete strlstFields;
	   return false;
	 }

   }
   i=0;
   while (!dmdStory->daqScript->Eof)
   {
	   i++;
	   dmdStory->daqScript->Next();
   }
   if (i<1)
   {
	   Application->MessageBox(L"This story has been deleted by someone else,please refresh the story list.",L"TBS Library",48);
	   delete pInfo;
	   delete strlstFields;
	   return false;
   }

   dmdStory->daqScript->First();
   for (i=0;i<pInfo->slInfoType->Count;i++)
   {
	 strFieldName = pInfo->slInfoType->Strings[i];
	 if ((strFieldName.IsEmpty())||(strFieldName.Trim()!=strFieldName))
		continue;
	 if (strFieldName == "MaterialID")
		strFieldType=(AnsiString)"varchar";
	 else
		strFieldType=strlstFields->Values[strFieldName];
	 if (strFieldType=="varchar")
		 html->SetSingleControlText(strFieldName,OrigStr(dmdStory->daqScript
			  ->FieldByName(strFieldName)->AsString).c_str());
	 if (strFieldType=="text")
		 html->SetSingleControlText(strFieldName,OrigStr(dmdStory->daqScript
			  ->FieldByName(strFieldName)->AsString).c_str());
	 if (strFieldType=="datetime")
	 {
		 time=dmdStory->daqScript->FieldByName(strFieldName)->AsDateTime;
		 html->SetSingleControlText(strFieldName,(AnsiString((int)time)).c_str());
	 }
	 if (strFieldType=="int")
	 {
		 if (!dmdStory->daqScript->FieldByName(strFieldName)->IsNull)
			 html->SetSingleControlText(strFieldName,((AnsiString)dmdStory->daqScript
				  ->FieldByName(strFieldName)->AsInteger).c_str());
	 }
	 if (strFieldType=="image")
	 {
		pStream=new TBlobStream((TBlobField*)dmdStory->daqScript->FieldByName(strFieldName),bmRead);
		if (pStream->Size>0)
		   html->SetImage(pStream,strFieldName);
		delete pStream;
	 }
   }
   delete strlstFields;
   delete pInfo;
   frmStoryInput->btnStoryAdd->Caption=(AnsiString)"&Modify";
 //  frmStoryInput->nbkStoryInput->PageIndex=1;
   frmStoryInput->btnClear->Visible=false;
   return true;
}

int FillListView(TQuery* pQuery,TStringList* pStringList,TListView* pListView)
{
  TListItem* pListItem;
  AnsiString strFieldName;
  TDateTime RegTime;
  int i;
  while (!pQuery->Eof)
  {
	if (pListView->Items->Count>=MAXROWS)
	{
	 //  pListView->EndUpdate();
	   pQuery->Close();
	   return 1;
	}

	pListItem=pListView->Items->Add();
	for (i=0;i<pStringList->Count;i++)
	{
	  strFieldName=pListView->Columns->Items[i]->Caption;
	  if (pStringList->Strings[i]=="varchar")
	  {
		if (strFieldName=="StoryID")
          pListItem->Caption=OrigStr(pQuery->FieldByName(strFieldName)->AsString);
        else
          pListItem->SubItems->Add(OrigStr(pQuery->FieldByName(strFieldName)->AsString));
        continue;
      }
      if (pStringList->Strings[i]=="datetime")
      {
		 if (pQuery->FieldByName(strFieldName)->IsNull)
           pListItem->SubItems->Add("");
         else
         {
           RegTime=pQuery->FieldByName(strFieldName)->AsDateTime;
		   if (strFieldName=="WriteTime")
             pListItem->SubItems->Add(RegTime.FormatString("yyyy-mm-dd hh:nn"));
           else
			 pListItem->SubItems->Add(RegTime.FormatString("yyyy-mm-dd"));
         }
         continue;
      }
      if (pStringList->Strings[i]=="int")
      {
        if (pQuery->FieldByName(strFieldName)->IsNull)
           pListItem->SubItems->Add("");
        else
           pListItem->SubItems->Add((AnsiString)pQuery->FieldByName(strFieldName)->AsInteger);
        continue;
      }
    }
    pQuery->Next();
  }
  return 0;
}
