//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma once
#include "PrintSetFrm.h"
//#include "PreviewFrm.h"
#include "frm_Main.h"
#include "CommFunc.h"
#include "StoryData.h"
//#include "DllHead.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "AdvGrid"
//#pragma link "AdvObj"
//#pragma link "BaseGrid"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "AdvGrid"
#pragma link "AdvUtil"
#pragma link "frxExportPDF"
#pragma resource "*.dfm"
#include "frm_StoryInput.h"
TfrmPrintSetting *frmPrintSetting;
//---------------------------------------------------------------------------
__fastcall TfrmPrintSetting::TfrmPrintSetting(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrintSetting::btnPreviewClick(TObject *Sender)
{

	PrintStory(0);

}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::FormShow(TObject *Sender)
{
  AnsiString strSQLClause;
  AnsiString strFieldName,strFieldType,strNodeID,strCaption;
  AnsiString strINIFileName;
  int i,j,nLength,nCount,nIndex;
  strNodeID=frmStoryInput->strCurrentNodeID;
  nLength=strNodeID.Length();
  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
					 (AnsiString)".ini";
  TIniFile* IniFile=new TIniFile(strINIFileName);
  strlstPrint->Clear();
  IniFile->ReadSectionValues("Print Config",strlstPrint);
  delete IniFile;
  strSQLClause=(AnsiString)"select FieldName,FieldType from lib_ExtendFields where CategoryID='"
			   +SQLStr(frmStoryInput->strCurrentNodeID)+(AnsiString)"'";
  dmdStory->daqFields->SQL->Clear();
  dmdStory->daqFields->SQL->Add(strSQLClause);
  try
  {
	dmdStory->daqFields->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error.",L"TBS Library",48);
	return;
  }

  strgrdFields->RemoveRows(2,strgrdFields->RowCount-2);
  strgrdFields->ClearRows(1,1);
//  AdvStringGrid1->ClearRows(1,1);
//  for (i=strgrdFields->RowCount-1 ;i>=2; i--)
//  {
//	strgrdFields->Rows[i]->Delete(0);
//  }
//   strgrdFields->RowCount=2;

//  strgrdFields->Rows[1]->Clear();

  for (i=0;i<frmStoryInput->strlstFieldName->Count;i++)
  {
	 strgrdFields->Cells[0][i+1]=frmStoryInput->strlstFieldName->Strings[i];
	 strgrdFields->Cells[1][i+1]=frmStoryInput->strlstFieldType->Strings[i];
	 strgrdFields->RowCount=i+2;
  }
  j=i+1;
  nCount = 0;
  for (i=0;i<dmdStory->daqFields->RecordCount;i++)
  {
	 strFieldName=OrigStr(dmdStory->daqFields->FieldValues["FieldName"]);
	 strFieldType=OrigStr(dmdStory->daqFields->FieldValues["FieldType"]);
	 if (strFieldType!="image")
	 {
		 strgrdFields->RowCount++;
		 strgrdFields->Cells[0][nCount+j]=strFieldName;
		 strgrdFields->Cells[1][nCount+j]=strFieldType;
		 nCount++;
	 }
	 dmdStory->daqFields->Next();
  }

  nCount=0;
  lstPrint->Clear();
  cboOrder->Clear();
  for (i=0;i<strlstPrint->Count;i++)
  {
	strFieldName=strlstPrint->Names[i].SubString(0,nLength);
	if (strFieldName==strNodeID)
	{
	  strFieldName=strlstPrint->Names[i];
	  nCount++;
	  strCaption=strFieldName.SubString(nLength+2,strFieldName.Length()-nLength-1);
	  if (strCaption!="Order By")
	  {
		lstPrint->Items->Add(strCaption);
		nIndex=strgrdFields->Cols[0]->IndexOf(strCaption);
		if (nIndex>=0)
		{
		  strgrdFields->Cells[2][nIndex]=strlstPrint->Values[strFieldName];
		  if (strgrdFields->Cells[1][nIndex]!="text")
			cboOrder->Items->Add(strCaption);
		}
	  }
	}
  }
  if (nCount<=0)
  {
	for (i=1;i<strgrdFields->RowCount;i++)
	{
		lstPrint->Items->Add(strgrdFields->Cells[0][i]);
		if (strgrdFields->Cells[1][i]!="text")
          cboOrder->Items->Add(strgrdFields->Cells[0][i]);
    }
  }
  for (i=1;i<strgrdFields->RowCount;i++)
  {
    if (strgrdFields->Cells[2][i].IsEmpty())
      strgrdFields->Cells[2][i]="100";
  }
  strgrdFields->SetFocus();
  strCaption = strlstPrint->Values[strNodeID+"_Order By"];
  if (!strCaption.IsEmpty())
      cboOrder->ItemIndex = cboOrder->Items->IndexOf(strCaption);
  else
  {
      if (cboOrder->Items->Count>=0)
          cboOrder->ItemIndex = 0;
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::FormCreate(TObject *Sender)
{
  strlstPrint=new TStringList;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::FormDestroy(TObject *Sender)
{
  if (strlstPrint!=NULL)
     delete strlstPrint;
}
//---------------------------------------------------------------------------


void __fastcall TfrmPrintSetting::btnAddClick(TObject *Sender)
{
  AnsiString str;
  str=strgrdFields->Cells[0][strgrdFields->Row];
  if (lstPrint->Items->IndexOf(str)<0)
  {
    lstPrint->Items->Add(str);
    if (strgrdFields->Cells[1][strgrdFields->Row]!="text")
       cboOrder->Items->Add(str);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::btnDeleteClick(TObject *Sender)
{
  int nIndex,nLocate;
  AnsiString strFieldName;
  nIndex=lstPrint->ItemIndex;
  if (nIndex>=0)
  {
	strFieldName=lstPrint->Items->Strings[nIndex];
	nLocate=cboOrder->Items->IndexOf(strFieldName);
	if (nLocate>=0)
	   cboOrder->Items->Delete(nLocate);
    lstPrint->Items->Delete(lstPrint->ItemIndex);
    lstPrint->SetFocus();
  }
  if (nIndex>0)
    lstPrint->ItemIndex=nIndex-1;
  if ((nIndex==0)&&(lstPrint->Items->Count>=1))
	lstPrint->ItemIndex=nIndex;

  if (cboOrder->Items->Count > 0)
	cboOrder->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::strgrdFieldsCanEditCell(TObject *Sender,
      int Arow, int Acol, bool &canedit)
{
  if (Acol==2)
    canedit=true;
  else
    canedit=false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::btnUpClick(TObject *Sender)
{
  int nIndex;
  nIndex=lstPrint->ItemIndex;
  if (nIndex>0)
  {
    lstPrint->Items->Move(nIndex,nIndex-1);
	lstPrint->ItemIndex=nIndex-1;
	lstPrint->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::btnDownClick(TObject *Sender)
{
  int nIndex;
  nIndex=lstPrint->ItemIndex;
  if ((nIndex<lstPrint->Items->Count-1)&&(nIndex>=0))
  {
	lstPrint->Items->Move(nIndex,nIndex+1);
	lstPrint->ItemIndex=nIndex+1;
	lstPrint->SetFocus();
  }
}
/*
//---------------------------------------------------------------------------
void __fastcall TfrmPrintSetting::Print(TObject* Sender,AnsiString& Value)
{
	AnsiString strFieldName,strFieldValue,strFieldType;
	AnsiString strTableName;
	AnsiString strSQLClause;
	AnsiString strStoryID;
	TDateTime  time;

	strFieldName = frmPreview->qrbDetail->Controls[m_nField-1]->Name;
	strFieldType = frmPreview->qrbDetail->Controls[m_nField-1]->Hint;
	strStoryID = Value.SubString(0,13);
	if (strFieldName == "StoryID")
	{
		Value = strStoryID;
		m_nField++;
		if (m_nField > lstPrint->Items->Count)
		   m_nField = 1;

		return;
	}
	strTableName = Value.SubString(15,Value.Length());
	m_nField++;
	if (m_nField > lstPrint->Items->Count)
		m_nField = 1;
	strSQLClause = (AnsiString)"select "+strFieldName+(AnsiString)" from lib_"
				   +strTableName+(AnsiString)frmStoryInput->strCurrentNodeID
				   +(AnsiString)" where StoryID='"+SQLStr(strStoryID)
				   +(AnsiString)"'";
	dmdStory->daqCheck->SQL->Clear();
	dmdStory->daqCheck->SQL->Add(strSQLClause);
	dmdStory->daqCheck->Open();
	if (!dmdStory->daqCheck->FieldByName(strFieldName)->IsNull)
	{
		if (strFieldType == "varchar")
			strFieldValue = OrigStr(dmdStory->daqCheck->FieldByName(strFieldName)->AsString);
		if (strFieldType == "text")
			strFieldValue = OrigStr(dmdStory->daqCheck->FieldByName(strFieldName)->AsString);
		if (strFieldType == "int")
			strFieldValue = (AnsiString)dmdStory->daqCheck->FieldByName(strFieldName)->AsInteger;
		if (strFieldType == "datetime")
		{
			time = dmdStory->daqCheck->FieldByName(strFieldName)->AsDateTime;
			if (strFieldName == "WriteTime")
				strFieldValue = time.FormatString("yyyy-mm-dd hh:nn");
			else
				strFieldValue = time.FormatString("yyyy-mm-dd");
		}
	}
	Value = strFieldValue;
}
*/
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::btnPrintClick(TObject *Sender)
{
	PrintStory(1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::btnExportClick(TObject *Sender)
{
   TStringList* strlstFile;
   AnsiString strFile,strWidth;
   int i,nIndex;
   strlstFile=new TStringList();
   for (i=0;i<lstPrint->Items->Count;i++)
   {
	 strFile=lstPrint->Items->Strings[i];
	 nIndex=strgrdFields->Cols[0]->IndexOf(strFile);
	 strWidth=strgrdFields->Cells[2][nIndex];
	 strFile+=(AnsiString)"="+strWidth;
	 strlstFile->Add(strFile);
   }
   if (cboOrder->ItemIndex>=0)
   {
	 strFile = (AnsiString)"Order By="+(AnsiString)cboOrder->ItemIndex;
	 strlstFile->Add(strFile);
   }
   else
   {
	 strFile = (AnsiString)"Order By=";
	 strlstFile->Add(strFile);
   }
   if (SaveDialog1->Execute())
	  strlstFile->SaveToFile(SaveDialog1->FileName);
   delete strlstFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmPrintSetting::btnImportClick(TObject *Sender)
{
  TStringList* strlstFile;
  AnsiString strFieldName;
  int i,nIndex;
  if (OpenDialog1->Execute())
  {
	 lstPrint->Clear();
	 cboOrder->Clear();

	 strlstFile=new TStringList();
	 try
	 {
	   strlstFile->LoadFromFile(OpenDialog1->FileName);

	   for (i=0;i<strlstFile->Count-1;i++)
	   {
		 strFieldName=strlstFile->Names[i];
		 lstPrint->Items->Add(strFieldName);
		 cboOrder->Items->Add(strFieldName);
		 nIndex=strgrdFields->Cols[0]->IndexOf(strFieldName);
		 strgrdFields->Cells[2][nIndex]=strlstFile->Values[strFieldName];
	   }
	   if (!strlstFile->Values["Order By"].IsEmpty())
		   cboOrder->ItemIndex = strlstFile->Values["Order By"].ToInt();
	   else
		   cboOrder->ItemIndex = -1;
	 }
	 catch(...)
	 {
	   Application->MessageBox(L"It is not a illegal print template file or it is not design for this node.",L"TBS Library",48);
	   delete strlstFile;
	   return;
	 }
	 delete strlstFile;
  }
}

//---------------------------------------------------------------------------
void TfrmPrintSetting::PrintStory(int nMode)
{

  AnsiString strINIFileName;
  AnsiString strWidth;
  int i,nIndex,nPosition,nYPosition,nCount;
  TList* pList;
  TStringList* strlstClassName;
//  TQRDBText* pText;
//  TQRLabel* pLabel;

  TFont* pFont;
  TFontStyles style;
  TDateTime time;
  AnsiString strSQLClause,strSQLClause1,strNodeID;
  AnsiString strBeginDate,strEndDate;
  AnsiString strOrderField,strOrderField1;
  int  nCurrentYear;

  m_nField = 1;
  strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  dmdStory->daqCheck->Open();
  nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

  strNodeID=frmStoryInput->strCurrentNodeID;
  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
					 (AnsiString)".ini";
  TIniFile* IniFile=new TIniFile(strINIFileName);
  for (i=1;i<strgrdFields->RowCount;i++)
	 IniFile->DeleteKey("Print Config",strNodeID+"_"+strgrdFields->Cells[0][i]);
  for (i=0;i<lstPrint->Items->Count;i++)
  {
	 nIndex=strgrdFields->Cols[0]->IndexOf(lstPrint->Items->Strings[i]);
	 if (nIndex>=0)
		 IniFile->WriteString("Print Config",strNodeID+"_"+lstPrint->Items->Strings[i],strgrdFields->Cells[2][nIndex]);
  }

  if (cboOrder->ItemIndex>=0)
	 strOrderField=cboOrder->Items->Strings[cboOrder->ItemIndex];
  else
	 strOrderField=(AnsiString)"StoryID";
  if (strOrderField=="StoryID")
	strOrderField1 = (AnsiString)"WriteTime";
  else
	strOrderField1 = strOrderField;
  IniFile->WriteString("Print Config",strNodeID+"_Order By",strOrderField);
  delete IniFile;

  try
  {
	strBeginDate=frmStoryInput->BeginDate.FormatString("yyyy-mm-dd");
	strEndDate=frmStoryInput->EndDate.FormatString("yyyy-mm-dd");

	strSQLClause = (AnsiString)"select name from tempdb..sysobjects where name like '#"
				   +frmStoryInput->strOpID+(AnsiString)"_pr%'";
	dmdStory->daqCheck->SQL->Clear();
	dmdStory->daqCheck->SQL->Add(strSQLClause);
	dmdStory->daqCheck->Open();
	if (dmdStory->daqCheck->RecordCount>0)
	{
	  strSQLClause=(AnsiString)"drop table #"+frmStoryInput->strOpID+(AnsiString)"_pr";
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);
	  dmdStory->daqCheck->ExecSQL();
	}
	if (frmStoryInput->muiBrowseTime->Checked)
	{
	  strSQLClause=(AnsiString)"select StoryID+'new' StoryID ,"+strOrderField1+" into #"+frmStoryInput->strOpID+(AnsiString)"_pr from lib_new"+strNodeID+" where cast(WriteTime As Date)>='"
				   +strBeginDate+"' and cast(WriteTime As Date)<='"+strEndDate
				   +(AnsiString)"' order by "
				   +strOrderField;
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);

	  if (frmStoryInput->muiStory_Only->Checked)
	  {
		strSQLClause=(AnsiString)"insert into #"+frmStoryInput->strOpID+(AnsiString)"_pr select StoryID+'"+IntToStr(nCurrentYear)+"'+'_' StoryID,"+strOrderField1+" from lib_"+IntToStr(nCurrentYear)+"_"+strNodeID+" where cast(WriteTime As Date)>='"
					 +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
					 +(AnsiString)"' order by "
					 +strOrderField;

		dmdStory->daqCheck->SQL->Add(strSQLClause);
	  }
	  else
	  {
		strSQLClause=(AnsiString)"insert into #"+frmStoryInput->strOpID+(AnsiString)"_pr select StoryID+'old' StoryID,"+strOrderField1+" from lib_old"+strNodeID+" where cast(WriteTime As Date)>='"
					 +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
					 +(AnsiString)"' order by "
					 +strOrderField;
		dmdStory->daqCheck->SQL->Add(strSQLClause);
		for (i=1990;i<=nCurrentYear;i++)
		{
		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
							   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		  strSQLClause=(AnsiString)"insert into #"+frmStoryInput->strOpID+(AnsiString)"_pr select StoryID+'"+IntToStr(i)+"'+'_' StoryID,"+strOrderField+" from lib_"+IntToStr(i)+"_"+strNodeID+" where cast(WriteTime As Date)>='"
					   +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
					   +(AnsiString)"' order by "
					   +strOrderField;
		  dmdStory->daqCheck->SQL->Add(strSQLClause);
		}

	  }
	}
	else
	{
	  strSQLClause=(AnsiString)"select StoryID+'new' StoryID,"+strOrderField1+" into #"+frmStoryInput->strOpID+(AnsiString)"_pr from lib_new"+strNodeID+" where MaterialID='"
				   +SQLStr(frmStoryInput->strMaterialID)
				   +(AnsiString)"' order by "
				   +strOrderField;
	  dmdStory->daqCheck->SQL->Clear();
	  dmdStory->daqCheck->SQL->Add(strSQLClause);

	  if (frmStoryInput->muiStory_Only->Checked)
	  {
		strSQLClause=(AnsiString)"insert into #"+frmStoryInput->strOpID+(AnsiString)"_pr select StoryID+'"+IntToStr(nCurrentYear)+"'+'_' StoryID,"+strOrderField1+" from lib_"+IntToStr(nCurrentYear)+"_"
					 +strNodeID+" where MaterialID='"
					 +SQLStr(frmStoryInput->strMaterialID)
					 +(AnsiString)"' order by "
					 +strOrderField;
		dmdStory->daqCheck->SQL->Add(strSQLClause);
	  }
	  else
	  {
		strSQLClause=(AnsiString)"insert into #"+frmStoryInput->strOpID+(AnsiString)"_pr select StoryID+'old' StoryID,"+strOrderField1+" from lib_old"+strNodeID+" where MaterialID='"
					 +SQLStr(frmStoryInput->strMaterialID)
					 +(AnsiString)"' order by "
					 +strOrderField;
		dmdStory->daqCheck->SQL->Add(strSQLClause);
		for (i=1990;i<=nCurrentYear;i++)
		{
		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
							   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		  strSQLClause=(AnsiString)"insert into #"+frmStoryInput->strOpID+(AnsiString)"_pr select StoryID+'"+IntToStr(i)+"'+'_' StoryID,"+strOrderField1+" from lib_"+IntToStr(i)+"_"
					   +strNodeID+" where MaterialID='"
					   +SQLStr(frmStoryInput->strMaterialID)
					   +(AnsiString)"' order by "
					   +strOrderField;
		  dmdStory->daqCheck->SQL->Add(strSQLClause);

		}

	  }
	}
	dmdStory->daqCheck->ExecSQL();
	strSQLClause = (AnsiString)"select StoryID from #"+frmStoryInput->strOpID+(AnsiString)"_pr Order By "
					+strOrderField;
	dmdStory->daqPrint->SQL->Clear();
	dmdStory->daqPrint->SQL->Add(strSQLClause);
	dmdStory->daqPrint->Open();
  }
  catch(...)
  {
	  Application->MessageBox(L"Database error,the operation is not succeeded.",L"TBS Library",48);
	  return;
  }

//  AnsiString strValue = "a'a";
//  strValue = StringReplace(strValue, "'", "''", TReplaceFlags());
//  strValue = "'" + strValue + "'";

//frxReport1->LoadFromFile("Untitled2.fr3");
frxReport1->Clear();

//  frxReport1->Variables->Items[frxReport1->Variables->IndexOf("vTitle")]->Value= strValue;
//frxReport1->ShowReport();
//frmPreview->ShowModal();
  //	PrintStory(0);
 // clear report
TfrxDataPage * DataPage;
TfrxReportPage * Page;
TfrxBand * Band;
TfrxMasterData * DataBand;
TfrxMemoView * Memo;
TfrxPageHeader * RHBand;
TfrxLineView * RHLine;
AnsiString strAllFields;
	dmdStory->daqPrint->SQL->Clear();

	strAllFields = "";
	  for (i=0;i<lstPrint->Items->Count;i++)
	  {
		nIndex=strgrdFields->Cols[0]->IndexOf(lstPrint->Items->Strings[i]);
		if (strgrdFields->Cells[1][nIndex]=="text" ||
			strgrdFields->Cells[1][nIndex]=="datetime" ||
			strgrdFields->Cells[1][nIndex]=="varchar" || strgrdFields->Cells[1][nIndex]=="int")
		{
			if (strgrdFields->Cells[1][nIndex]=="text")
			{
				if (i==0)
					strAllFields = "CONVERT(varchar(4000), "+lstPrint->Items->Strings[i]+" ) as " + lstPrint->Items->Strings[i];
				else
					strAllFields = strAllFields + "," + "CONVERT(varchar(4000), "+lstPrint->Items->Strings[i]+" ) as " + lstPrint->Items->Strings[i];

			}
			else
			{
				if (i==0)
					strAllFields = lstPrint->Items->Strings[i];
				else
					strAllFields = strAllFields + "," + lstPrint->Items->Strings[i];
			}
		}
	  }


	if (frmStoryInput->muiBrowseTime->Checked)
	{
	  if (frmStoryInput->muiStory_Only->Checked)
	  {
		//strSQLClause=(AnsiString)"select " + strAllFields + " from dbo.lib_2002_1998081900006 where WriteTime > '2006-11-07 18:33:01.000'";
	  //	dmdStory->daqPrint->SQL->Add(strSQLClause);
		strSQLClause=(AnsiString)"select " + strAllFields + " from lib_new"+strNodeID+" where cast(WriteTime As Date)>='"
					 +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate  + (AnsiString)"'"
					 +(AnsiString)" union select " + strAllFields + " from lib_"+IntToStr(nCurrentYear)+"_"+strNodeID+" where cast(WriteTime As Date)>='"
					 +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate
					 +(AnsiString)"' order by "
					 +strOrderField;
	  }
	  else
	  {
		strSQLClause=(AnsiString)"select " + strAllFields + " from lib_new"+strNodeID+" where cast(WriteTime As Date)>='"
					 +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate  + (AnsiString)"'"
					 +(AnsiString)" union select " + strAllFields + " from lib_old"+strNodeID+" where cast(WriteTime As Date)>='"
					 +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate + "'";

		for (i=1990;i<=nCurrentYear;i++)
		{
		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
							   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		  strSQLClause=strSQLClause+(AnsiString)" union select " + strAllFields + " from lib_"+IntToStr(i)+"_"+strNodeID+" where cast(WriteTime As Date)>='"
					   +strBeginDate+"'and cast(WriteTime As Date)<='"+strEndDate + "'";
		}
		strSQLClause=strSQLClause+(AnsiString)" order by "  +strOrderField;

	  }
	}
	else
	{
	  if (frmStoryInput->muiStory_Only->Checked)
	  {
		strSQLClause=(AnsiString)" select " + strAllFields + "  from lib_new"
					 +strNodeID+" where MaterialID='" +SQLStr(frmStoryInput->strMaterialID) + (AnsiString)"'"
					  +(AnsiString)" union select " + strAllFields + "  from lib_"+IntToStr(nCurrentYear)+"_"
					 +strNodeID+" where MaterialID='"
					 +SQLStr(frmStoryInput->strMaterialID)
					 +(AnsiString)"' order by "
					 +strOrderField;
	  }
	  else
	  {
		strSQLClause=(AnsiString)" select " + strAllFields + "  from lib_new"
					 +strNodeID+" where MaterialID='" +SQLStr(frmStoryInput->strMaterialID) + (AnsiString)"'"
					 +(AnsiString)" union select " + strAllFields + " from lib_old"+strNodeID+" where MaterialID='"
					 +SQLStr(frmStoryInput->strMaterialID) + "'";
		for (i=1990;i<=nCurrentYear;i++)
//		for (i=1990;i<=2010;i++)
		{
		  dmdStory->daqCheckTable->Close();
		  dmdStory->daqCheckTable->SQL->Clear();
		  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
							   String(i)+ "_"+ strNodeID+ "')");
		  dmdStory->daqCheckTable->Open();
		  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		 strSQLClause=strSQLClause+(AnsiString)" union select " + strAllFields + " from lib_"+IntToStr(i)+"_"
					   +strNodeID+" where MaterialID='"
					   +SQLStr(frmStoryInput->strMaterialID) + "'";
		}
		strSQLClause=strSQLClause+(AnsiString)" order by "  +strOrderField;

	  }
	}
 //	dmdStory->daqCheck->ExecSQL();

//	  strSQLClause=(AnsiString)"select * from lib_new"+strNodeID+" where MaterialID='"
  //				   +SQLStr(frmStoryInput->strMaterialID)
	//			   +(AnsiString)"' order by "
	  //			   +strOrderField;

//		strSQLClause=(AnsiString)"select * from dbo.lib_2002_1998081900006 where WriteTime > '2006-11-07 18:33:01.000'";


//	dmdStory->daqPrint->SQL->Clear();
	dmdStory->daqPrint->SQL->Add(strSQLClause);
	dmdStory->daqPrint->Open();


frxDBDataset1->DataSet = dmdStory->daqPrint;
frxReport1->DataSets->Add(frxDBDataset1);
// add "Data" page
DataPage = new TfrxDataPage(frxReport1);
// add page
Page = new TfrxReportPage(frxReport1);
// create unique name
Page->CreateUniqueName();
// set sizes of fields, paper and orientation to defaults
Page->SetDefaults();
// change paper orientation
Page->Orientation = poLandscape;
// add report title band

//TFont* pFont;
//TFontStyles style;
pFont=new TFont;
style<<fsBold;
pFont->Name=(AnsiString)"Arial";
pFont->Color=clNavy;
pFont->Style=style;
pFont->Size = 15;

Band = new TfrxReportTitle(Page);
Band->CreateUniqueName();
// only ：Top； coordinate and height of band need setting
//    both in pixels
Band->Top = 0;
Band->Height = 50;
// add object to report title band
Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = "View of the Story";
Memo->Font = pFont;
Memo->Height = 40;
// this object will be stretched to band・s width
Memo->Align = baWidth;

Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = (AnsiString)"Date "+time.CurrentDate().FormatString("yyyy-mm-dd");
Memo->SetBounds(600, 0, 200, 20);
//  frmPreview->qrlblPrinter->Caption=(AnsiString)"Printed By "+dfrm->strUserID;
Memo->HAlign = Frxclass::haRight;

Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = (AnsiString)(AnsiString)"Printed By "+dmdStory->strCurrentUserID;
Memo->SetBounds(820, 0, 200, 20);
Memo->HAlign = Frxclass::haRight;


RHBand = new TfrxPageHeader(Page) ;
RHBand->CreateUniqueName();
RHBand->Top = 10 ;
RHBand->Height = 20;

RHLine = new TfrxLineView(RHBand);
RHLine->Width = Page->Width - 100;
RHLine->Left = 3;
RHLine->Top = 20;
RHLine->Visible = true;


// add masterdata band
DataBand = new TfrxMasterData(Page);
DataBand->CreateUniqueName();
DataBand->DataSet = frxDBDataset1;
// ：Top； should be greater than previously added band・s top + height
DataBand->Top = 100;
DataBand->Height = 20;
DataBand->Stretched = true;
/*
// add object on masterdata
Memo = new TfrxMemoView(DataBand);
Memo->CreateUniqueName();
// connect to data
Memo->DataSet = frxDBDataset1;
Memo->DataField = "CustNo";
Memo->SetBounds(0, 0, 100, 20);
// align text to object・s right margin
Memo->HAlign = Advgrid::haRight;
*/
//frxReport1->ShowReport(true);

  pList=NULL;
//  pText=NULL;
//  pLabel=NULL;
  pFont=NULL;
  strlstClassName=NULL;
  pList=new TList;
  strlstClassName=new TStringList;
  pFont=new TFont;
  style<<fsUnderline;
  style<<fsBold;
  pFont->Name=(AnsiString)"Arial";
  pFont->Color=clNavy;
  pFont->Style=style;
  pFont->Size = 10;
  nPosition=0;
  nYPosition=0;

  for (i=0;i<lstPrint->Items->Count;i++)
  {
	nIndex=strgrdFields->Cols[0]->IndexOf(lstPrint->Items->Strings[i]);
	if (strgrdFields->Cells[1][nIndex]=="text" ||
		strgrdFields->Cells[1][nIndex]=="datetime" || strgrdFields->Cells[1][nIndex]=="varchar")
	{
		Memo = new TfrxMemoView(RHBand);
		Memo->CreateUniqueName();
		Memo->Font = pFont;
		Memo->SetBounds(nPosition, 0, strgrdFields->Cells[2][nIndex].Trim().ToInt(), 20);
		// align text to object・s right margin
		//Memo->AutoWidth = true;
		Memo->Text = lstPrint->Items->Strings[i];
//		Memo->StretchMode = smMaxHeight;
//		Memo->ShiftMode = smWhenOverlapped;
		Memo->HAlign = Frxclass::haLeft;

		Memo = new TfrxMemoView(DataBand);
		Memo->CreateUniqueName();
		// connect to data
		Memo->DataSet = frxDBDataset1;
		//Memo->Font = pFont;
		Memo->DataField = lstPrint->Items->Strings[i];
		Memo->SetBounds(nPosition, 0, strgrdFields->Cells[2][nIndex].Trim().ToInt(), 20);
		// align text to object・s right margin
		//Memo->AutoWidth = true;
		Memo->StretchMode = smMaxHeight;
		Memo->ShiftMode = smWhenOverlapped;
		Memo->HAlign = Frxclass::haLeft;

	  nPosition+= Memo->Width+20;
	  continue;

	}
	if (strgrdFields->Cells[1][nIndex]=="int")
	{
		Memo = new TfrxMemoView(DataBand);
		Memo->CreateUniqueName();
		// connect to data
		Memo->DataSet = frxDBDataset1;
		Memo->Font = pFont;
		Memo->DataField = lstPrint->Items->Strings[i];
		Memo->SetBounds(nPosition, 0, strgrdFields->Cells[2][nIndex].Trim().ToInt(), 20);

		Memo->ShiftMode = smWhenOverlapped;
		// align text to object・s right margin
		Memo->HAlign = Frxclass::haRight;
	  nPosition+= Memo->Width+20;
	  continue;
	}

  }

  if (nMode == 0)
	  frxReport1->ShowReport(true);
  else
	  frxReport1->Print();

  dmdStory->daqPrint->Close();

  nCount=pList->Count;
/*  for (nIndex=0;nIndex<nCount;nIndex++)
  {
	 if (strlstClassName->Strings[nIndex]=="TQRDBText")
	 {
	   pText=(TQRDBText*)pList->Items[nIndex];
	   pText->Parent=NULL;
	   delete pText;
	   pText=NULL;
	   continue;
	 }
	 if (strlstClassName->Strings[nIndex]=="TQRLabel")
	 {
	   pLabel=(TQRLabel*)pList->Items[nIndex];
	   pLabel->Parent=NULL;
	   delete pLabel;
	   pLabel=NULL;
	   continue;
	 }
  }

  if (pText!=NULL)
	delete pText;
  if (pLabel!=NULL)
	delete pLabel;
*/  if (pFont!=NULL)
	delete pFont;
  if (strlstClassName!=NULL)
	  delete strlstClassName;

  if (pList!=NULL)
	delete pList;

}
//---------------------------------------------------------------------------



void __fastcall TfrmPrintSetting::strgrdFieldsGetEditMask(TObject *Sender, int ACol,
          int ARow, UnicodeString &Value)
{
  if (ACol==2)
	Value="999";
}
//---------------------------------------------------------------------------

