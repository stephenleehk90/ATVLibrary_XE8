//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frm_StoryInput.h"
#include "StoryData.h"
#include "CommFunc.h"
#include "DataProcess.h"
#include "PrintSetFrm.h"
#include "BrowseTimeFrm.h"
#include "MaterialIDFrm.h"
#include "htmlform.h"
#include "Capture.h"
#include "ArchiveDialog.h"
#include "ChangeMaterialIDForm.h"
#include "InitForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "TMSSpellCheck"
//#pragma link "TMSSpellCheckCorrectForm"
//#pragma link "TMSSpellCheckCorrectLinesForm"
#pragma link "TMSSpellCheck"
#pragma link "TMSSpellCheckCorrectLinesForm"
#pragma link "AdvListV"
#pragma resource "*.dfm"
TfrmStoryInput *frmStoryInput;
THtmlForm* html;
CVTR* pVTR;

int ColumnToSort = 0;

//---------------------------------------------------------------------------
__fastcall TfrmStoryInput::TfrmStoryInput(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmStoryInput::FormShow(TObject *Sender)
{
  m_bRefreshTree = true;
  RefreshTree();

  m_bRefreshTree = false;
   AnsiString strSQLClause;
   AnsiString strFieldName,strFieldType;
   strSQLClause=(AnsiString)"select FieldName,FieldType from lib_BaseFields";
   dmdStory->daqFields->SQL->Clear();
   dmdStory->daqFields->SQL->Add(strSQLClause);

   nbkStoryInput->PageIndex=0;
   muiBrowseTime->Checked=true;
   muiBrowseID->Checked=false;
   muiPopTime->Checked=true;
   muiPopID->Checked=false;
   lblBegin->Caption=(AnsiString)"  "+BeginDate.FormatString("yyyy-mm-dd");
   lblEnd->Caption=(AnsiString)"  "+EndDate.FormatString("yyyy-mm-dd");
   strSQLClause=(AnsiString)"Set ROWCOUNT 300";
   dmdStory->daqCheck->SQL->Clear();
   dmdStory->daqCheck->SQL->Add(strSQLClause);

   try
   {
	 dmdStory->daqCheck->ExecSQL();
	 dmdStory->daqFields->Open();

	 for (int i=0;i<dmdStory->daqFields->RecordCount;i++)
	 {
	   strFieldName=OrigStr(dmdStory->daqFields->FieldValues["FieldName"]);
	   strFieldType=OrigStr(dmdStory->daqFields->FieldValues["FieldType"]);
	   strlstFieldName->Add(strFieldName);
	   strlstFieldType->Add(strFieldType);
	   dmdStory->daqFields->Next();
	 }
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error.",L"TBS Library",48);
   }
   trvwStory->Selected = NULL;
 // Application->MessageBox(trvwStory->Items->GetFirstNode()->Text.c_str() ,L"TBS Library",48);
//  trvwStory->Items->GetFirstNode()->Selected = true;
  trvwStory->Select(trvwStory->Items->GetFirstNode());
  trvwStory->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStoryInput::FormCreate(TObject *Sender)
{
	html=new THtmlForm();
	html->OnReset=Capture;
	html->OnSubmit=TimeCode;
	html->m_bInputTem = true;
	pVTR=new CVTR();

  AnsiString strINIFileName;
  AnsiString strSQLClause;
  TDateTime time;

  strSQLClause = (AnsiString)"GetNewFileID";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  dmdStory->daqCheck->Open();


  strOpID = dmdStory->daqCheck->FieldValues["FileID"];
  dmdStory->tbCategory->TableName="lib_StoryLibTree";
  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
					 (AnsiString)".ini";
  TIniFile* IniFile=new TIniFile(strINIFileName);
  BeginDate=(TDateTime)IniFile->ReadInteger("Story Browse","BeginDate",(int)time.CurrentDate());
  EndDate=(TDateTime)IniFile->ReadInteger("Story Browse","EndDate",(int)time.CurrentDate());
  BeginDate=BeginDate-EndDate+time.CurrentDate();
  EndDate=time.CurrentDate();
  bModifySearched=false;
  m_bRefreshTree = false;
  strDate = (AnsiString)"";
  strlstFieldName = new TStringList;
  strlstFieldType = new TStringList;

  delete IniFile;
}
//---------------------------------------------------------------------------
void __fastcall TfrmStoryInput::FormDestroy(TObject *Sender)
{
  AnsiString strINIFileName;
  TDateTime time;

  if (strlstFieldName!=NULL)
	delete strlstFieldName;
  if (strlstFieldType!=NULL)
	delete strlstFieldType;

  if (!strCurrentNodeID.IsEmpty())
  {
 //     lstvwStory->SaveSettings->RegistryKey=(AnsiString)"TBSLibrary\\StoryInput\\"+strCurrentNodeID;
  //    lstvwStory->StoreSettings();
  }
  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
					 (AnsiString)".ini";
  TIniFile* IniFile=new TIniFile(strINIFileName);
  IniFile->WriteInteger("Story Browse","BeginDate",(int)BeginDate);
  IniFile->WriteInteger("Story Browse","EndDate",(int)EndDate);
  delete IniFile;

  if (pVTR !=NULL)
	delete pVTR;
  if (html !=NULL)
	delete html;


}
//---------------------------------------------------------------------------
void __fastcall TfrmStoryInput::tbbtnModifyClick(TObject *Sender)
{
  TListItem* pListItem;
  int i;

  pListItem=lstvwStory->ItemFocused;
  if ((pListItem!=NULL)&&(pListItem->Selected))
  {
	strStoryID=pListItem->Caption;
	if (!strStoryID.IsEmpty())
	{
	  bModifySearched=false;
//	  Tb97Unmerge(Toolbar971);
//	  PageControl1->ActivePage = TabSheet2;
//	  TabSheet2->Enabled = true;
//	  TabSheet1->Enabled = false;
//	  muiEdit->Visible=false;
  //	  muiStory->Visible=false;
	//  muiOption->Visible=false;
	  nbkStoryInput->PageIndex=1;
	  muiEdit->Enabled=false;
	  muiStory->Enabled=false;
	  muiOption->Enabled=false;
	  for (i=0;i<lstvwStory->Columns->Count;i++)
	  {
		  if (lstvwStory->Columns->Items[i]->Caption == "Date")
		  {
			 strDate = lstvwStory->ItemFocused->SubItems->Strings[i-1].SubString(0,4);
			 break;
		  }
	  }

	  StoryModify(strCurrentNodeID,strStoryID,strDate);
	}
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::tbbtnDeleteClick(TObject *Sender)
{
  AnsiString strNodeID;
  TListItem* pListItem;
  if (trvwStory->Selected!=NULL)
  {
	pListItem=lstvwStory->ItemFocused;
	if (pListItem==NULL)
	   return;
	strStoryID=pListItem->Caption;
	if (!strStoryID.IsEmpty())
	{
	   if (Application->MessageBox(L"Are you sure to delete this story?",L"TBS Library",MB_OKCANCEL)==IDOK)
	   {
		 strNodeID=(AnsiString)((char*)trvwStory->Selected->Data);
		 DeleteStory(strNodeID);
		 if (lstvwStory->ItemFocused!=NULL)
			 lstvwStory->ItemFocused->Selected = true;
	   }
	}
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::tbbtnAddClick(TObject *Sender)
{
  if (!strCurrentNodeID.IsEmpty())
  {
	Caption="Add Story";
//    Tb97Unmerge(Toolbar971);
	btnStoryAdd->Caption=(AnsiString)"&Add";
	btnClear->Visible=true;
	stbStory->Panels->Items[1]->Text="";
	nbkStoryInput->PageIndex=1;
   //	PageControl1->ActivePage = TabSheet2;
   //	TabSheet2->Enabled = true;
   //	TabSheet1->Enabled = false;
	muiEdit->Enabled=false;
	muiStory->Enabled=false;
	muiOption->Enabled=false;
	m_nAdded=0;
  }
  else
	Application->MessageBox(L"You must select the category for the script that you want to add.",L"TBS Library",48);

}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::lstvwStoryKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
  AnsiString strStoryID,strNodeID;
  TListItem* pListItem;
  int i;

  if (Key==VK_DELETE)
  {
	if (trvwStory->Selected!=NULL)
	{
	  pListItem=lstvwStory->ItemFocused;
	  if (pListItem==NULL)
		  return;
	  strStoryID=pListItem->Caption;
	  if (!strStoryID.IsEmpty())
	  {
		 if (Application->MessageBox(L"Are you sure to delete this story?",L"TBS Library",MB_OKCANCEL)==IDOK)
		 {
		   strNodeID=(AnsiString)((char*)trvwStory->Selected->Data);
		   DeleteStory(strNodeID);
		   if (lstvwStory->ItemFocused!=NULL)
			   lstvwStory->ItemFocused->Selected = true;
		 }
	  }
	}
  }
  if (Key==VK_RETURN)
  {
	pListItem=lstvwStory->ItemFocused;
	if ((pListItem!=NULL)&&(pListItem->Selected))
	{
	  strStoryID=pListItem->Caption;
	  if (!strStoryID.IsEmpty())
	  {
		bModifySearched=false;
		for (i=0;i<lstvwStory->Columns->Count;i++)
		{
			if (lstvwStory->Columns->Items[i]->Caption == "Date")
			{
			   strDate = lstvwStory->ItemFocused->SubItems->Strings[i-1].SubString(0,4);
			   break;
			}
		}

		StoryModify(strCurrentNodeID,strStoryID,strDate);
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::muiBrowseTimeClick(TObject *Sender)
{
  if (frmBrowseTime->ShowModal()==mrOk)
  {
	 lblFrom->Caption=(AnsiString)"From";
	 lblEnd->Visible=true;
	 lblTo->Visible=true;
	 lblBegin->Caption=(AnsiString)"  "+BeginDate.FormatString("yyyy-mm-dd");
	 lblEnd->Caption=(AnsiString)"  "+EndDate.FormatString("yyyy-mm-dd");
	 BrowseScript(strCurrentNodeID);
	 muiPopID->Checked=false;
	 muiPopTime->Checked=true;
	 muiBrowseTime->Checked=true;
	 muiBrowseID->Checked=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::muiBrowseIDClick(TObject *Sender)
{
  if (frmMaterialID->ShowModal()==mrOk)
  {
	 if (BrowseByID(strCurrentNodeID,strMaterialID))
	 {
	   muiPopID->Checked=true;
	   muiPopTime->Checked=false;
	   muiBrowseTime->Checked=false;
	   muiBrowseID->Checked=true;
	   lblFrom->Caption=(AnsiString)"MaterialID:";
	   lblBegin->Caption=(AnsiString)"    "+strMaterialID;
	   lblTo->Visible=false;
	   lblEnd->Visible=false;
	 }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::btnStoryAddClick(TObject *Sender)
{
  TInputInfo* pInfo;
  AnsiString strNodeID;
  AnsiString strMaterialID;

  if (! html->CheckValues())
  {
	Application->MessageBox(L"A story like this means nothing,please input some value.",L"TBS Library",48);
	return;
  }
  pInfo = new TInputInfo();
  html->GetControlText(pInfo);

  if (btnStoryAdd->Caption=="&Add")
  {
	strNodeID=(AnsiString)((char*)trvwStory->Selected->Data);
	if (AddStory(strNodeID,pInfo))
	{
	   html->ClearControlText(pInfo);
	   m_nAdded++;
	   stbStory->Panels->Items[2]->Text=IntToStr(m_nAdded)+(AnsiString)" story successfully added.";
	}
	else
	   stbStory->Panels->Items[2]->Text=(AnsiString)"Operation failed.";
  }
  else
  {
	if (bModifySearched)
		strNodeID=strPasteNodeID;
	else
		strNodeID=strCurrentNodeID;

	if (ModifyStory(strNodeID,strStoryID,pInfo))
	{
		nbkStoryInput->PageIndex=0;
		strDate=(AnsiString)"";
		html->ClearControlText();
 //       Tb97Merge(Toolbar971,dkMain);
		if (lstvwStory->Visible)
		{
	  //	   PageControl1->ActivePage = TabSheet1;
	   //	   TabSheet1->Enabled = true;
		//   TabSheet2->Enabled = false;

		   lstvwStory->SetFocus();
		   muiEdit->Enabled = true;
		   muiStory->Enabled = true;
		   muiOption->Enabled = true;
		   if ((lstvwStory->ItemFocused!=NULL)&&(lstvwStory->ItemFocused->Selected))
		   {
			 tbbtnModify->Enabled = true;
			 tbbtnDelete->Enabled = true;
			 muiEdit_Delete->Enabled = true;
			 muiEdit_Modify->Enabled = true;
		   }
		   else
		   {
			 tbbtnModify->Enabled = false;
			 tbbtnDelete->Enabled = false;
			 muiEdit_Delete->Enabled=false;
			 muiEdit_Modify->Enabled=false;
		   }

		   if (bModifySearched)
		   {
			  html->CloseTemplate();
			  html->OpenTemplate(ScrollBox1,"Library","lib_StoryLibTree","NodeID","InputTem",SQLStr(strCurrentNodeID));
		   }
		}
		else
		   trvwStory->SetFocus();

	}
  }
  delete pInfo;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::btnCheckClick(TObject *Sender)
{
/*  ControlInfo* pInfo;
  pInfo = html->m_pControlInfoHeader;
  while (pInfo)
  {
	  if (pInfo->strClassName == "TEdit")
	  {
		  if ( ((TEdit*)pInfo->pControl)->Text.Length() != 0 )
		  {
			 UnicodeString strInput = ((TEdit*)pInfo->pControl)->Text;
			if (AdvSpellCheck1->Validate(strInput)==wvrNotValidated)
			{
				if (AdvSpellCheckCorrectLinesDialog1->Execute(strInput)== mrOk)
				((TEdit*)pInfo->pControl)->Text = strInput;

			}
				//AdvSpellCheck1->AddValidationRequest(); ( ((TEdit*)pInfo->pControl)->Text, ctAll);
		  }
	  }
	  if (pInfo->strClassName == "TMemo")
	  {         UnicodeString strInput = ((TMemo*)pInfo->pControl)->Text;
			if (AdvSpellCheck1->Validate(strInput)==wvrNotValidated)
			{
				if (AdvSpellCheckCorrectLinesDialog1->Execute(strInput)== mrOk)
				((TMemo*)pInfo->pControl)->Text = strInput;
			}
	   //		AddictSpell1->CheckWinControl(((TMemo*)pInfo->pControl), ctAll);
	  }
	  pInfo = pInfo->pNextControl;
  }
*/	Application->MessageBox(L"Not Available Now, update will be delivered very soon.",L"TBS Library",48);

}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::lstvwStoryChange(TObject *Sender, TListItem *Item,
		  TItemChange Change)
{
 if (lstvwStory->Focused())
  {
	if ((lstvwStory->ItemFocused==NULL)||(!lstvwStory->ItemFocused->Selected))
	{
	  tbbtnModify->Enabled=false;
	  tbbtnDelete->Enabled=false;
	  muiEdit_Delete->Enabled=false;
	  muiEdit_Modify->Enabled=false;
	  muiPopDelete->Enabled=false;
	  muiPopModify->Enabled=false;
	}
	else
	{
	  tbbtnModify->Enabled=true;
	  tbbtnDelete->Enabled=true;
	  muiEdit_Delete->Enabled=true;
	  muiEdit_Modify->Enabled=true;
	  muiPopDelete->Enabled=true;
	  muiPopModify->Enabled=true;
	}
  }
  else
  {
	tbbtnModify->Enabled=false;
	tbbtnDelete->Enabled=false;
	muiEdit_Delete->Enabled=false;
	muiEdit_Modify->Enabled=false;
	muiPopDelete->Enabled=false;
	muiPopModify->Enabled=false;
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::btnCancelClick(TObject *Sender)
{
 AnsiString strSQLClause;
   AnsiString strBeginDate,strEndDate;

   //Tb97Merge(Toolbar971,dkMain);
   nbkStoryInput->PageIndex=0;
   strDate=(AnsiString)"";
   html->ClearControlText();

   if (lstvwStory->Visible)
   {
	  if (bModifySearched)
	  {
		html->CloseTemplate();
		html->OpenTemplate(ScrollBox1,"Library","lib_StoryLibTree","NodeID","InputTem",SQLStr(strCurrentNodeID));
	  }

   //	   PageControl1->ActivePage = TabSheet1;
  //	   TabSheet1->Enabled = true;
	//   TabSheet2->Enabled = false;

	  lstvwStory->SetFocus();
	  ShowStoryNum();
   }
   else
   {
	  trvwStory->SetFocus();
	  stbStory->Panels->Items[1]->Text=(AnsiString)"";
   }
   muiEdit->Enabled = true;
   muiStory->Enabled = true;
   muiOption->Enabled = true;
   if ((lstvwStory->ItemFocused!=NULL)&&(lstvwStory->ItemFocused->Selected))
   {
	 tbbtnModify->Enabled = true;
	 tbbtnDelete->Enabled = true;
	 muiEdit_Delete->Enabled=true;
	 muiEdit_Modify->Enabled=true;
   }
   else
   {
	 tbbtnModify->Enabled = false;
	 tbbtnDelete->Enabled = false;
	 muiEdit_Delete->Enabled=false;
	 muiEdit_Modify->Enabled=false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::tbbtnSearchClick(TObject *Sender)
{
 TClipboard* pClip;
  AnsiString strURL,strSQLClause;
  AnsiString strTableName,strNodeID;
  int nPos;
  int nPos1;
//  int nYear;

  pClip=new TClipboard;
  strURL=pClip->AsText;
  nPos=strURL.AnsiPos("TableName");
  if (nPos <= 0)
	 return;
  strURL=strURL.SubString(nPos+10,strURL.Length());
  nPos=strURL.AnsiPos("&");
  if (nPos <= 0)
	 return;
  strTableName=strURL.SubString(0,nPos-1).Trim();
  strURL=strURL.SubString(nPos+1,strURL.Length());
  nPos=strURL.AnsiPos("StoryID");
  if (nPos <= 0)
	 return;
  strStoryID=strURL.SubString(nPos+8,13).Trim();
  delete pClip;
  nPos=strTableName.AnsiPos("lib_old");
  nPos1=strTableName.AnsiPos("lib_new");
  if (nPos > 0)
  {
	  strPasteNodeID=strTableName.SubString(nPos+7,strTableName.Length());
	  strDate = (AnsiString)"1989";
  }
  else if (nPos1 > 0)
  {
	 TDateTime tm;

	  strPasteNodeID=strTableName.SubString(nPos1+7,strTableName.Length());
	  strDate = (AnsiString)tm.CurrentDate().FormatString("yyyy");
  }
  else
  {
	  nPos = strTableName.AnsiPos("lib_");
	  if (nPos==0) return;
	  strPasteNodeID = strTableName.SubString(nPos+9,strTableName.Length());
	  if (strPasteNodeID.Length()>31) return;
	  strDate = strTableName.SubString(nPos+4,4);
	  try{
//        nYear=strDate.ToInt();
		}
	  catch(...)
		{
		return;
		}
  }
  if ((!strPasteNodeID.IsEmpty())&&(!strStoryID.IsEmpty()))
  {
	 bModifySearched=true;
 //    Tb97Unmerge(Toolbar971);
	 nbkStoryInput->PageIndex=1;
	 muiEdit->Enabled=false;
	 muiStory->Enabled=false;
	 muiOption->Enabled=false;

  //	   PageControl1->ActivePage = TabSheet2;
  //	   TabSheet2->Enabled = true;
   //	   TabSheet1->Enabled = false;


	 html->CloseTemplate();
	 html->OpenTemplate(ScrollBox1,"Library","lib_StoryLibTree","NodeID","InputTem",SQLStr(strPasteNodeID));

	 StoryModify(strPasteNodeID,strStoryID,strDate);
  }
  else
	 Application->MessageBox(L"There are no searched story.",L"TBS Library",48);
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::trvwStoryChange(TObject *Sender, TTreeNode *Node)

{
   AnsiString strNodeID,strFieldName;
   AnsiString strSQLClause;
   int nAttr;
   if (m_bRefreshTree)
	   return;
   if (trvwStory->Selected!=NULL)
   {
	 strNodeID=(AnsiString)((char*)trvwStory->Selected->Data);
	 strSQLClause=(AnsiString)"select NodeAttr from lib_StoryLibTree where NodeID='"
				  +SQLStr(strNodeID)+(AnsiString)"'";
	 dmdStory->daqCheck->SQL->Clear();
	 dmdStory->daqCheck->SQL->Add(strSQLClause);
	 try
	 {
	   dmdStory->daqCheck->Open();
	 }
	 catch(...)
	 {
	   Application->MessageBox(L"Database error.",L"TBS Library",48);
	   return ;
	 }
	 nAttr=dmdStory->daqCheck->FieldByName("NodeAttr")->AsInteger;
	 if (nAttr==1)
	 {
	   muiEdit->Enabled=true;
	   muiStory->Enabled=true;
	   muiOption->Enabled=true;
	   tbbtnAdd->Enabled=true;
	   tbbtnRefresh->Enabled = true;

	   if (strCurrentNodeID!=strNodeID)
	   {
		 html->CloseTemplate();
		 html->OpenTemplate(ScrollBox1,"Library","lib_StoryLibTree","NodeID","InputTem",SQLStr(strNodeID));
	  //	 lstvwStory->SaveSettings->RegistryKey=(AnsiString)"TBSLibrary\\StoryInput\\"+strCurrentNodeID;
		// lstvwStory->StoreSettings();
		 if (muiBrowseTime->Checked)
		 {
			if (!BrowseScript(strNodeID))
			{
			   lstvwStory->Visible=false;
			   pnlList->Caption=(AnsiString)"There are some error on database.";
			   stbStory->Panels->Items[1]->Text=(AnsiString)"";
			   stbStory->Panels->Items[2]->Text=(AnsiString)"";
			   muiEdit->Enabled=false;
			   muiStory->Enabled=false;
			   tbbtnAdd->Enabled=false;
			   tbbtnModify->Enabled=false;
			   tbbtnDelete->Enabled=false;
			   tbbtnRefresh->Enabled = false;
			   return;
			}
		 }
		 else
		 {
			if (!BrowseByID(strNodeID,strMaterialID))
			{
			   lstvwStory->Visible=false;
			   pnlList->Caption=(AnsiString)"There are some error on database..";
			   stbStory->Panels->Items[1]->Text=(AnsiString)"";
			   stbStory->Panels->Items[2]->Text=(AnsiString)"";
			   muiEdit->Enabled=false;
			   muiStory->Enabled=false;
			   tbbtnAdd->Enabled=false;
			   tbbtnModify->Enabled=false;
			   tbbtnDelete->Enabled=false;
			   tbbtnRefresh->Enabled = false;
			   return;
			 }
		  }
	   }
	   lstvwStory->Visible=true;
	   lstvwStory->Color=clWindow;

	   if ((lstvwStory->ItemFocused!=NULL)&&(lstvwStory->ItemFocused->Selected))
	   {
		   tbbtnModify->Enabled=true;
		   tbbtnDelete->Enabled=true;
		   muiEdit_Delete->Enabled=true;
		   muiEdit_Modify->Enabled=true;
	   }
	   else
	   {
		   tbbtnModify->Enabled=false;
		   tbbtnDelete->Enabled=false;
		   muiEdit_Delete->Enabled=false;
		   muiEdit_Modify->Enabled=false;
	   }
	   strCurrentNodeID=strNodeID;
	 }
	 else
	 {
	   lstvwStory->Visible=false;
	   pnlList->Caption=(AnsiString)"It is not a leaf node.";
	   stbStory->Panels->Items[1]->Text=(AnsiString)"";
	   stbStory->Panels->Items[2]->Text=(AnsiString)"";
	   muiEdit->Enabled=false;
	   muiStory->Enabled=false;
	   tbbtnAdd->Enabled=false;
	   tbbtnModify->Enabled=false;
	   tbbtnDelete->Enabled=false;
	   tbbtnRefresh->Enabled = false;

	 }
   }
   else
   {
	   lstvwStory->Visible=false;
	   stbStory->Panels->Items[1]->Text=(AnsiString)"";
	   pnlList->Caption=(AnsiString)"There are no selected node.";
	   muiEdit->Enabled=false;
	   muiStory->Enabled=false;
	   tbbtnAdd->Enabled=false;
	   tbbtnModify->Enabled=false;
	   tbbtnDelete->Enabled=false;
	   tbbtnRefresh->Enabled = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::ChangeMaterialID1Click(TObject *Sender)
{
	frmChangeM->strLib=strCurrentNodeID;
	if (frmChangeM->ShowModal()==mrOk)
		{
		Application->MessageBox(L"CHANGE MATERIAL ID SUCCESS",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		Refresh1Click(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::ImportFromTBSNews1Click(TObject *Sender)
{
  AnsiString strPasswd;
   TField* f1;
   strPasswd="`*%hyh}=:jug-m$cymg9+9$fu";
   DecodeString(strPasswd);

   try{
   if (dmdStory->tbImport->Active)
		dmdStory->tbImport->Close();
   if (dmdStory->tbExport->Active)
		dmdStory->tbExport->Close();
   if (dmdStory->dbTBSNews->Connected)
		dmdStory->dbTBSNews->Connected=false;
   if (dmdStory->qryStoryID->Active)
		dmdStory->qryStoryID->Close();
   }
   catch(...)
   {}// do nothing;

   dmdStory->dbTBSNews->Params->Clear();

   TIniFile* IniFile=new TIniFile(IniName());
   frmInit->strODBC="TBS";
   frmInit->strUser=IniFile->ReadString("Database Connection","TBSUserName","");
   frmInit->strPass=IniFile->ReadString("Database Connection","TBSPassWord","");
   delete IniFile;
   frmInit->Caption = "TBS Database Login Information";
	if (frmInit->ShowModal()==mrOk)
	{
	   dmdStory->dbTBSNews->Params->Add((AnsiString)"USER NAME="+frmInit->strUser);
	   dmdStory->dbTBSNews->Params->Add((AnsiString)"PASSWORD="+frmInit->strPass);

		WritePrivateProfileString("Database Connection","TBSUserName",
					AnsiString(frmInit->strUser).c_str(),IniName().c_str());
		WritePrivateProfileString("Database Connection","TBSPassword",
					AnsiString(frmInit->strPass).c_str(),IniName().c_str());

	}
	else
	return;


//   dmdStory->dbTBSNews->Params->Add("DATABASE NAME=ATV");
//   dmdStory->dbTBSNews->Params->Add("SERVER NAME=atvnews");
//   dmdStory->dbTBSNews->Params->Add("USER NAME=TBSInternalLogin");
//   dmdStory->dbTBSNews->Params->Add("PASSWORD="+strPasswd);
//   dmdStory->dbTBSNews->Params->Add("LANGDRIVER=DBWINUS0");

   try{
//	dmdStory->dbTBSNews->Open();
	dmdStory->dbTBSNews->Connected=true;
   }
   catch(...)
   {


	Application->MessageBox(L"I can NOT connect to TBS News >_<",L"TBS Library",48);
	return;
   }

   try{
	dmdStory->tbExport->Active=true;
   }
   catch(...)
   {
	Application->MessageBox(L"I can NOT open TBS News's Export Table >_<",L"TBS Library",48);
	return;
   }

   dmdStory->tbImport->TableName="dbo.lib_new"+strCurrentNodeID;
   try{
	dmdStory->tbImport->Active=true;
   }
   catch(...)
   {
	Application->MessageBox(L"I can NOT open TBS Library's Import Table >_<",L"TBS Library",48);
	return;
   }

   dmdStory->qryStoryID->SQL->Clear();
   dmdStory->qryStoryID->SQL->Add("exec GetNewFileID");

   dmdStory->tbExport->First();
   while(!dmdStory->tbExport->Eof)
   {
	try{
	dmdStory->tbImport->Append();
	dmdStory->tbImport->Edit();

	dmdStory->qryStoryID->Open();
	dmdStory->tbImport->FieldByName("StoryID")->AsString=
		SQLStr(dmdStory->qryStoryID->Fields->Fields[0]->AsString);
	dmdStory->qryStoryID->Close();

	f1=dmdStory->tbImport->FieldByName("Date");
	if (f1!=NULL)
		f1->AsDateTime=dmdStory->tbExport->FieldValues["AirDate"];
/*
	f1=dmdStory->tbImport->FieldByName("Duration");
	if (f1!=NULL)
		f1->AsString=dmdStory->tbExport->FieldValues["TapeDuration"];
*/
	f1=dmdStory->tbImport->FieldByName("MaterialID");
	if (f1!=NULL)
		dmdStory->tbImport->FieldByName("MaterialID")->AsString
		   =SQLStr(dmdStory->tbExport->FieldByName("TapeNum")->AsString);
	f1=dmdStory->tbImport->FieldByName("Reporter");
	if (f1!=NULL)
		dmdStory->tbImport->FieldByName("Reporter")->AsString
		   =SQLStr(dmdStory->tbExport->FieldByName("AuthorID")->AsString);
	f1=dmdStory->tbImport->FieldByName("Slug");
	if (f1!=NULL)
		dmdStory->tbImport->FieldByName("Slug")->AsString
			=SQLStr(dmdStory->tbExport->FieldByName("Abstract")->AsString);
	f1=dmdStory->tbImport->FieldByName("WriteTime");
	if (f1!=NULL)
		dmdStory->tbImport->FieldByName("WriteTime")->AsDateTime
		   =dmdStory->tbExport->FieldValues["ModifyDate"];
	f1=dmdStory->tbImport->FieldByName("Content");
	if (f1!=NULL)
		 dmdStory->tbImport->FieldByName("Content")->AsString
		   =SQLStr(dmdStory->tbExport->FieldByName("Content")->AsString);
	dmdStory->tbImport->Post();
	dmdStory->tbExport->Delete();
	}
	catch(...)
    {
	 Application->MessageBox(L"Something is wrong when importing >_<",L"TBS Library",48);
	 return;
	}
   }

   try{
   if (dmdStory->tbImport->Active)
		dmdStory->tbImport->Close();
   if (dmdStory->tbExport->Active)
		dmdStory->tbExport->Close();
   if (dmdStory->dbTBSNews->Connected)
		dmdStory->dbTBSNews->Connected=false;
   }
   catch(...)
   {}// do nothing;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::lstvwStoryMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
 if (lstvwStory->Focused())
  {
	if ((lstvwStory->ItemFocused==NULL)||(!lstvwStory->ItemFocused->Selected))
	{
	  tbbtnModify->Enabled=false;
	  tbbtnDelete->Enabled=false;
	  muiEdit_Delete->Enabled=false;
	  muiEdit_Modify->Enabled=false;
	  muiPopDelete->Enabled=false;
	  muiPopModify->Enabled=false;
	}
	else
	{
	  tbbtnModify->Enabled=true;
	  tbbtnDelete->Enabled=true;
	  muiEdit_Delete->Enabled=true;
	  muiEdit_Modify->Enabled=true;
	  muiPopDelete->Enabled=true;
	  muiPopModify->Enabled=true;
	}
  }
  else
  {
	tbbtnModify->Enabled=false;
	tbbtnDelete->Enabled=false;
	muiEdit_Delete->Enabled=false;
	muiEdit_Modify->Enabled=false;
	muiPopDelete->Enabled=false;
	muiPopModify->Enabled=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::muiStory_OnlyClick(TObject *Sender)
{
	muiStory_Only->Checked = !muiStory_Only->Checked;
}
//---------------------------------------------------------------------------


bool TfrmStoryInput::SetRowCount(int nCount)
{
	AnsiString strSQLClause;
	strSQLClause=(AnsiString)"Set ROWCOUNT "+(AnsiString)nCount;
	dmdStory->daqCheck->SQL->Clear();
	dmdStory->daqCheck->SQL->Add(strSQLClause);
	try{
		dmdStory->daqCheck->ExecSQL();
	}
	catch(...)
	{
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmStoryInput::ShowStoryNum()
{
	 AnsiString strSQLClause;
	 AnsiString strBeginDate,strEndDate;
	 int nCurrentYear,i;
	 int nRecords=0;

	 try{
	   strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
	   dmdStory->daqCheck->SQL->Clear();
	   dmdStory->daqCheck->SQL->Add(strSQLClause);
	   dmdStory->daqCheck->Open();
	   nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

	   if (muiBrowseTime->Checked)
	   {
		  strBeginDate=BeginDate.FormatString("yyyy-mm-dd");
		  strEndDate=EndDate.FormatString("yyyy-mm-dd");

		  if (muiStory_Only->Checked)
		  {
			strSQLClause = (AnsiString)"select count(*) from lib_new"+strCurrentNodeID
						   +" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
						   +(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

			strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(nCurrentYear)
						   +"_"+strCurrentNodeID+" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"
						   +strEndDate+(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;
		  }
		  else
		  {
			strSQLClause = (AnsiString)"select count(*) from lib_new"+strCurrentNodeID
						   +" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
						   +(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

			strSQLClause = (AnsiString)"select count(*) from lib_old"+strCurrentNodeID+" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"
						   +strEndDate+(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

			for (i=1990;i<=nCurrentYear;i++)
			{
				dmdStory->daqCheckTable->Close();
			  dmdStory->daqCheckTable->SQL->Clear();
			  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
																   String(i)+ "_"+ strCurrentNodeID + "')");
			  dmdStory->daqCheckTable->Open();
			  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

			  strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(i)
							 +"_"+strCurrentNodeID+" where cast(WriteTime As Date)>='"+strBeginDate+"'and cast(WriteTime As Date)<='"
							 +strEndDate+(AnsiString)"'";
			  dmdStory->daqCheck->SQL->Clear();
			  dmdStory->daqCheck->SQL->Add(strSQLClause);
			  dmdStory->daqCheck->Open();
			  nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;
			}
		  }

	   }
	   else
	   {
		  if (muiStory_Only->Checked)
		  {
			strSQLClause = (AnsiString)"select count(*) from lib_new"+strCurrentNodeID
						   +" where MaterialID='"+SQLStr(strMaterialID)
						   +(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

			strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(nCurrentYear)
						   +"_"+strCurrentNodeID+" where MaterialID='"+SQLStr(strMaterialID)+(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

		  }
		  else
		  {
			strSQLClause = (AnsiString)"select count(*) from lib_new"+strCurrentNodeID
						   +" where MaterialID='"+SQLStr(strMaterialID)
						   +(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

			strSQLClause = (AnsiString)"select count(*) from lib_old"+strCurrentNodeID
						   +" where MaterialID='"+SQLStr(strMaterialID)+(AnsiString)"'";
			dmdStory->daqCheck->SQL->Clear();
			dmdStory->daqCheck->SQL->Add(strSQLClause);
			dmdStory->daqCheck->Open();
			nRecords = nRecords+dmdStory->daqCheck->Fields->Fields[0]->AsInteger;

			for (i = 1990;i <= nCurrentYear;i++)
			{
				dmdStory->daqCheckTable->Close();
			  dmdStory->daqCheckTable->SQL->Clear();
			  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
																   String(i)+ "_"+ strCurrentNodeID + "')");
			  dmdStory->daqCheckTable->Open();
			  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

			  strSQLClause = (AnsiString)"select count(*) from lib_"+IntToStr(i)
							 +"_"+strCurrentNodeID+" where MaterialID='"+SQLStr(strMaterialID)+(AnsiString)"'";
			  dmdStory->daqCheck->SQL->Clear();
			  dmdStory->daqCheck->SQL->Add(strSQLClause);
			  dmdStory->daqCheck->Open();
			  nRecords = nRecords + dmdStory->daqCheck->Fields->Fields[0]->AsInteger;
			}
		  }
	   }
	 }
	 catch(...)
	 {
		stbStory->Panels->Items[1]->Text=(AnsiString)"";
		stbStory->Panels->Items[2]->Text=(AnsiString)"";
	 }
	 stbStory->Panels->Items[1]->Text=(AnsiString)"Story Numbers: "+(AnsiString)(nRecords);
	 if (dmdStory->daqCheck->RecordCount>MAXROWS)
		stbStory->Panels->Items[2]->Text=(AnsiString)"The Browsing list is too long";
	 return true;
}

void __fastcall TfrmStoryInput::muiPrintClick(TObject *Sender)
{
  SetRowCount(0);
  frmPrintSetting->ShowModal();
  SetRowCount(300);
}

void __fastcall  TfrmStoryInput::Refresh1Click(TObject *Sender)
{
  m_bRefreshStory = true;
  if (muiBrowseTime->Checked)
	 BrowseScript(strCurrentNodeID);
  else
	 BrowseByID(strCurrentNodeID,strMaterialID);
  m_bRefreshStory = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::RefreshTree1Click(TObject *Sender)
{
  m_bRefreshTree = true;
  RefreshTree();
  m_bRefreshTree = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::muiOption_ArchiveClick(TObject *Sender)
{
/*    AnsiString strSQLClause;
	try{
	strSQLClause=(AnsiString)"msdb..sp_runtask archive";
	dmdStory->daqCheck->Close();
	dmdStory->daqCheck->SQL->Clear();
	dmdStory->daqCheck->SQL->Add(strSQLClause);

	dmdStory->daqCheck->ExecSQL();

  }
  catch(...)
  {
	// failed
	Application->MessageBox("Database error,please contact with administrator.","TBS Library",48);
  }
  Application->MessageBox("Archive is successfully started.","TBS Library",48);
*/    frmArchive->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::btnClearClick(TObject *Sender)
{
  html->ClearControlText();
}
//---------------------------------------------------------------------------
void __fastcall TfrmStoryInput::Capture(TObject *Sender)
{
  frmCapture->m_bKeyFrame = true;
  frmCapture->strFieldName = html->GetImageName();
  frmCapture->ShowModal();
}

void __fastcall TfrmStoryInput::TimeCode(TObject *Sender)
{
  frmCapture->m_bKeyFrame = false;
  frmCapture->strFieldName = html->GetTimeCodeName();
  frmCapture->ShowModal();
}

void __fastcall TfrmStoryInput::lstvwStoryColumnClick(TObject *Sender, TListColumn *Column)

{
  ColumnToSort = Column->Index;
  dynamic_cast<TCustomListView *>(Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TfrmStoryInput::lstvwStoryCompare(TObject *Sender, TListItem *Item1,
		  TListItem *Item2, int Data, int &Compare)
{
  if (ColumnToSort == 0)
	Compare = CompareText(Item1->Caption,Item2->Caption);
  else
  {
	int ix = ColumnToSort - 1;
	Compare =
	  CompareText(Item1->SubItems->Strings[ix], Item2->SubItems->Strings[ix]);
  }
}
//---------------------------------------------------------------------------

