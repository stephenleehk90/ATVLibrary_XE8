//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "MaterialFrm.h"
#include "PreviewFrm.h"
#include "PrevFrm.h"
#include "ShelfFrm.h"
#include "QueryFrm.h"
#include "DataProcess.h"
#include "CommFunc.h"
#include "MaterialData.h"
#include "frm_Main.h"
#include "StoryData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "AdvListV"
#pragma resource "*.dfm"
TfrmMaterials *frmMaterials;
int ColumnToSort = 0;

//---------------------------------------------------------------------------
__fastcall TfrmMaterials::TfrmMaterials(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TfrmMaterials::RegistrationPage()
{
  muiEdit->Visible=false;
  muiMaterial->Visible=false;
  muiPopRefresh->Enabled=false;
  muiPopTime->Enabled = false;
  muiPopShelf->Enabled = false;
  nbkMaterials->PageIndex=0;
  edtMaterialID->SetFocus();

  stbMaterial->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialAdd->Items->Count;
  stbMaterial->Panels->Items[2]->Text=(AnsiString)"";
  Application->MainForm->Caption=(AnsiString)"TBS Library - Material Registration";
}
//---------------------------------------------------------------------------
void TfrmMaterials::PositionPage()
{
  muiEdit->Visible=false;
  muiMaterial->Visible=false;
  nbkMaterials->ActivePage="pgBatchInput";
  edtPosFile->SetFocus();
  stbMaterial->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialPos->Items->Count;
  stbMaterial->Panels->Items[2]->Text=(AnsiString)"";
  Application->MainForm->Caption=(AnsiString)"TBS Library - Shelf Registration";
}
//---------------------------------------------------------------------------

void TfrmMaterials::BrowsePage()
{

  if (!bEditMaterial)
  {
    frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"";
    if (muiBrowse->Checked)
	  BrowseMaterials();
	else
	  ShelfMaterials();

	muiEdit->Visible=true;
	muiMaterial->Visible=true;
	muiPopDelete->Enabled=true;
	muiPopModify->Enabled=true;
	muiPopRefresh->Enabled=true;
	muiPopTime->Enabled = true;
	muiPopShelf->Enabled = true;

	tbbtnRefresh->Visible = true;
	if (lstvwMaterials->ItemFocused!=NULL)
	   lstvwMaterials->ItemFocused->Selected=true;
	else
	{
	   muiEdit->Visible = false;

	   muiPopModify->Enabled=false;
	   muiPopDelete->Enabled=false;
	}
  }
  else
  {
	frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Modify Material";

	muiEdit->Visible = false;
	muiMaterial->Visible = false;
	muiPopDelete->Enabled=false;
	muiPopModify->Enabled=false;
	muiPopRefresh->Enabled=false;
	muiPopTime->Enabled = false;
	muiPopShelf->Enabled = false;
	tbbtnRefresh->Visible = false;
  }

  Application->MainForm->Caption=(AnsiString)"TBS Library - Material Browse";
  nbkMaterials->PageIndex=1;
  if (!bEditMaterial)
  {
	lstvwMaterials->SetFocus();
  }
}
void __fastcall TfrmMaterials::cboInputTypeChange(TObject *Sender)
{
 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
  BeginDate = StrToDateTime(lblBeginDate->Caption, FS );
  EndDate = StrToDateTime(lblEndDate->Caption, FS );

  if (muiBrowse->Checked)
	 BrowseMaterials();
   else
	 ShelfMaterials();
}
void TfrmMaterials::ModifyRes()
{
  TListItem *pListItem;
  AnsiString strResType,strResState;
  AnsiString strSQLClause;
  int nResType,nResState;
  pListItem=lstvwMaterials->ItemFocused;
  if ((pListItem!=NULL)&&(pListItem->Selected))
  {
	strSQLClause=(AnsiString)"Select ResType,Position,remark,state from dbo.lib_Res where ResID='"
				 +(AnsiString)SQLStr(pListItem->Caption)+(AnsiString)"'";
	dmdMaterials->daqCheck->SQL->Clear();
	dmdMaterials->daqCheck->SQL->Add(strSQLClause);
	try
    {
      dmdMaterials->daqCheck->Open();
      dmdMaterials->daqCheck->First();
    }
    catch(...)
    {
	  Application->MessageBox(L"Database error.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  return;
	}
	if (dmdMaterials->daqCheck->RecordCount<1)
	{
	  Application->MessageBox(L"This material's ID has been modified or this material has been deleted,please refresh the material list.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      return;
    }
    else
    {
      nResState=dmdMaterials->daqCheck->FieldByName("state")->AsInteger;
      nResType=dmdMaterials->daqCheck->FieldByName("ResType")->AsInteger;
      for (int i=0;i<strlstResType->Count;i++)
      {
        if (nResType==strlstResType->Values[strlstResType->Names[i]].ToInt())
        {
          strResType=strlstResType->Names[i];
          break;
        }
      }
      pListItem->SubItems->Strings[0]=strResType;
      pListItem->SubItems->Strings[1]=OrigStr(dmdMaterials->daqCheck->FieldValues["remark"]);
      pListItem->SubItems->Strings[3]=OrigStr(dmdMaterials->daqCheck->FieldValues["Position"]);
      switch (nResState)
      {
        case -1:
             pListItem->SubItems->Strings[4]="Lost";
             break;
        case 0:
             pListItem->SubItems->Strings[4]="On Shelf";
             break;
        case 1:
             pListItem->SubItems->Strings[4]="Borrowed";
             break;
      }
    }
    if (nResState==1)
    {
	   Application->MessageBox(L"The material has been borrowed,it can not be modified.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
       return;
    }
    if (!bEditMaterial)
    {
      pnlMaterialEdit->Visible=true;
      btnMaterialOk->Caption="&Modify";
      bEditMaterial=true;
      muiPopDelete->Enabled=false;
      muiPopModify->Enabled=false;
      muiEdit->Visible=false;
      muiMaterial->Visible=false;
      muiPopRefresh->Enabled=false;
      muiPopTime->Enabled = false;
	  muiPopShelf->Enabled = false;
	  tbbtnRefresh->Visible = false;
	}
	cboState->Items->Delete(2);
	edtResID->Text=pListItem->Caption;
	strResType=pListItem->SubItems->Strings[0];
	cboResType->ItemIndex=strlstResType->IndexOfName(strResType);
	cboInputType->Enabled=false;
	edtResRemark->Text=pListItem->SubItems->Strings[1];
	edtPosition->Text=pListItem->SubItems->Strings[3];
	strResID=pListItem->Caption;
	strRemark=edtResRemark->Text;
	cboState->ItemIndex=nResState+1;
	frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Modify Material";
	edtResID->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::muiBrowseClick(TObject *Sender)
{
 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
  BeginDate = StrToDateTime(lblBeginDate->Caption, FS );
  EndDate = StrToDateTime(lblEndDate->Caption, FS );

  if (frmQuery->ShowModal()==mrOk)
   {

 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
  BeginDate = StrToDateTime(lblBeginDate->Caption, FS );
  EndDate = StrToDateTime(lblEndDate->Caption, FS );

	 BrowseMaterials();
	 lblFrom->Visible=true;
	 lblBeginDate->Visible=true;
	 lblTo->Visible=true;
	 lblEndDate->Visible=true;
	 lblShelf->Visible=false;
	 lblShelfID->Visible=false;
	 muiBrowse->Checked=true;
	 muiBrowseShelf->Checked=false;
	 muiPopTime->Checked = true;
	 muiPopShelf->Checked = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::muiBrowseShelfClick(TObject *Sender)
{
  TListItem *pListItem;
  if (frmShelf->ShowModal()==mrOk)
  {
	lblBeginDate->Visible=false;
	lblEndDate->Visible=false;
	lblFrom->Visible=false;
	lblTo->Visible=false;
	lblShelf->Visible=true;
	lblShelfID->Visible=true;
	muiBrowse->Checked=false;
	muiBrowseShelf->Checked=true;
	muiPopTime->Checked = false;
	muiPopShelf->Checked = true;
    ShelfMaterials();
    if (lstvwMaterials->Items->Count>0)
	{
	  pListItem=lstvwMaterials->FindCaption(0,frmShelf->edtMaterial->Text,true,true,true);
	  if (pListItem!=NULL)
	  {
		pListItem->Focused=true;
		pListItem->Selected=true;
		pListItem->MakeVisible(false);
	  }
	}
  }
}
//---------------------------------------------------------------------------


void __fastcall TfrmMaterials::muiPreviewClick(TObject *Sender)
{
	print_Report(true);
//  MaterialReport(true,Sender);
//  frmPreview->qrpReport->Preview();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::muiEdit_ModifyClick(TObject *Sender)
{
  ModifyRes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnMaterialOkClick(TObject *Sender)
{
   TListItem *pListItem;
   AnsiString strSQLClause;
   if (edtResID->Text.Trim().IsEmpty())
   {
	 Application->MessageBox(L"Please input MaterialID.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     edtResID->SetFocus();
     return;
   }
   if (edtResID->Text.Trim().UpperCase()[edtResID->Text.Trim().Length()]=='S')
   {
	 Application->MessageBox(L"The Material ID's last char can NOT be 's' or 'S',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     edtResID->SetFocus();
     edtResID->SelectAll();
     return;
   }
   if (cboResType->Text.Trim().IsEmpty())
   {
	 Application->MessageBox(L"Please input Type.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 cboResType->SetFocus();
	 return;
   }
   if (!edtPosition->Text.Trim().IsEmpty())
   {
	 if (edtPosition->Text.Trim().UpperCase()[edtPosition->Text.Trim().Length()]!='S')
	 {
	   Application->MessageBox(L"The position's last char must be 's' or 'S',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	   edtPosition->SetFocus();
	   edtPosition->SelectAll();
	   return;
	 }
   }
   strSQLClause=(AnsiString)"select state from dbo.lib_Res where ResID='"
			   +(AnsiString)SQLStr(strResID.Trim())+(AnsiString)"'";
   dmdMaterials->daqCheck->SQL->Clear();
   dmdMaterials->daqCheck->SQL->Add(strSQLClause);
   try
   {
	 dmdMaterials->daqCheck->Open();
	 dmdMaterials->daqCheck->First();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return;
   }
   if (dmdMaterials->daqCheck->RecordCount<1)
   {
	 Application->MessageBox(L"This material's ID has been modified or this material has been deleted, please refresh the material list.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return;
   }
   else
   {
     if (dmdMaterials->daqCheck->FieldByName("state")->AsInteger==1)
     {
		Application->MessageBox(L"The material has been borrowed, it's property can not be modified.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        return;
     }
   }
   strSQLClause=(AnsiString)"select ResID from dbo.lib_Res where ResID='"
               +(AnsiString)SQLStr(edtResID->Text.Trim())+(AnsiString)"'";
   if ((!CheckUnique(strSQLClause))
         &&(edtResID->Text.Trim()!=strResID.Trim()))
   {
	  Application->MessageBox(L"MaterialID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      edtResID->SetFocus();
      edtResID->SelectAll();
   }
   else
   {
      if (cboState->Text.Trim().IsEmpty())
		  Application->MessageBox(L"Please input Status.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      else if (MaterialsModify())
      {
        pListItem=lstvwMaterials->FindCaption(0,strResID.Trim(),true,true,true);
        pListItem->Caption=edtResID->Text.Trim();
		pListItem->SubItems->Strings[0]=cboResType->Text.Trim();
		pListItem->SubItems->Strings[1]=edtResRemark->Text.Trim();
		pListItem->SubItems->Strings[3]=edtPosition->Text.Trim();
		pListItem->SubItems->Strings[4]=cboState->Text.Trim();
		lstvwMaterials->ItemFocused = pListItem;
		lstvwMaterials->Selected = pListItem;

		frmMain->stbStory->Panels->Items[2]->Text
			  =(AnsiString)"Material "+strResID.Trim()+(AnsiString)"'s property has been modified.";
		strResID=pListItem->Caption.Trim();
		strRemark=pListItem->SubItems->Strings[1].Trim();
		muiPopDelete->Enabled=true;
		muiPopModify->Enabled=true;
        muiEdit->Visible=true;
        muiMaterial->Visible=true;
        muiPopRefresh->Enabled=true;
        muiPopTime->Enabled = true;
        muiPopShelf->Enabled = true;
        tbbtnRefresh->Visible = true;
		cboInputType->Enabled=true;

        lstvwMaterials->SetFocus();
        pnlMaterialEdit->Visible = false;
        bEditMaterial=false;
        pListItem->MakeVisible(false);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::lstvwMaterialsChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
  if (lstvwMaterials->Focused())
  {
    if ((lstvwMaterials->ItemFocused==NULL)||(!lstvwMaterials->ItemFocused->Selected))
    {
      muiEdit->Visible = false;
      muiPopDelete->Enabled=false;
      muiPopModify->Enabled=false;
    }
    else
    {
      if (!bEditMaterial)
      {
        muiEdit->Visible = true;
        muiPopDelete->Enabled=true;
        muiPopModify->Enabled=true;
      }
      else
      {
        lstvwMaterialsClick(Sender);
      }
    }
  }
    
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnMaterialCancelClick(TObject *Sender)
{
    bEditMaterial=false;
    pnlMaterialEdit->Visible=false;
    lblState->Visible=true;
    cboState->Visible=true;
    cboInputType->Enabled=true;
    edtResID->Enabled=true;
    edtPosition->Enabled=true;
    cboState->Enabled=true;
    cboResType->Enabled=true;
    edtResRemark->Enabled=true;
    btnMaterialOk->Enabled=true;
    muiEdit->Visible=true;
    muiMaterial->Visible=true;
    ClearMaterialInput();
    lstvwMaterials->SetFocus();
    muiPopRefresh->Enabled=true;
    muiPopTime->Enabled = true;
    muiPopShelf->Enabled = true;

    tbbtnRefresh->Visible = true;
    frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"";
    if (lstvwMaterials->ItemFocused!=NULL)
    {
      lstvwMaterials->ItemFocused->Selected=true;
	  muiPopDelete->Enabled=true;
	  muiPopModify->Enabled=true;
	  muiEdit->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnFileClick(TObject *Sender)
{
  if (dlgPosFile->Execute()==true)
	  edtPosFile->Text=dlgPosFile->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnInputClick(TObject *Sender)
{
  TFileStream *PosFile;
   char buffer[64];
   TListItem *p;
   AnsiString strPosID, strResID;
   int count;
   try {
	  PosFile=new TFileStream(edtPosFile->Text,
			 fmOpenRead | fmShareDenyWrite);
   }
   catch (...)
   {
	  Application->MessageBox(L"I can't read the file, please confirm its position.",
			L"Shelf Registration",48);
	  edtPosFile->SetFocus();
	  return;
   }

   strPosFileName=edtPosFile->Text;
   lstvwMaterialPos->Items->Clear();
   count=0;
 // lstvwMaterialPos->BeginUpdate();
   try
   {
	   while ( (PosFile->Read(buffer,44)==44 )&&(count<5002) )
	   {
    	  buffer[32]='\0';
	      strResID=Trim((AnsiString)buffer);
	      if ( *(strResID.LowerCase().AnsiLastChar()) =='s' )
    	     {
        	 strPosID=strResID;
	         }
    	  else
        	 {
	           p=lstvwMaterialPos->Items->Add();
    	       p->Caption=strResID;
        	   p->SubItems->Add(strPosID);
	           count++;
			 }
	   }
	   frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)count;
       if (p!=NULL)
       {
         lstvwMaterialPos->ItemFocused = p;
         lstvwMaterialPos->Selected = p;
       }
   }
   catch (...)
   {
		 Application->MessageBox(L"I can't read the file, please confirm its position.",
			L"Shelf Registration",48);
         delete PosFile;
         return;
   }
 //  lstvwMaterialPos->EndUpdate();

   if ( count >5000 )
   {
	  Application->MessageBox(L"There are records more than 5000 which is the Maxium number.",
			L"Shelf Registration",48);
   }

   btnPosDiscard->Enabled=true;
   btnPosConfirm->Enabled=true;
   btnPosConfirm->SetFocus();
   delete PosFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnPosConfirmClick(TObject *Sender)
{
  AnsiString strSQLClause;
  int i;
  try
  {
    for (i=0;i<lstvwMaterialPos->Items->Count;i++)
    {
     dmdMaterials->daqMaterials->SQL->Clear();
     strSQLClause=(AnsiString)"update dbo.lib_Res set ResID='"+
                  SQLStr(lstvwMaterialPos->Items->Item[i]->Caption)+
                  (AnsiString)"',Position='"+
                  SQLStr(lstvwMaterialPos->Items->Item[i]->SubItems->Strings[0])+
                  (AnsiString)"' where ResID='"+
                  SQLStr(lstvwMaterialPos->Items->Item[i]->Caption)+(AnsiString)"'";
     dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
     dmdMaterials->daqMaterials->Prepare();
     dmdMaterials->daqMaterials->ExecSQL();
    }
  }
  catch(...)
  {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  }
  frmMain->stbStory->Panels->Items[2]->Text
      =(AnsiString)lstvwMaterialPos->Items->Count+(AnsiString)" material's position has been Inputed.";
  lstvwMaterialPos->Items->Clear();
  frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialAdd->Items->Count;
  btnPosDiscard->Enabled=false;
  btnPosConfirm->Enabled=false;
  btnInput->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnPosDiscardClick(TObject *Sender)
{
  lstvwMaterialPos->Items->Clear();
  frmMain->stbStory->Panels->Items[1]->Text="Material numbers: 0";
  btnPosDiscard->Enabled=false;
  btnPosConfirm->Enabled=false;
  btnInput->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnAddClick(TObject *Sender)
{
  TListItem *pListItem;
  AnsiString strSQLClause;
  if (edtMaterialID->Text.Trim().IsEmpty())
  {
	Application->MessageBox(L"Please input MaterialID.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	edtMaterialID->SetFocus();
    return;
  }
  if (edtMaterialID->Text.Trim().UpperCase()[edtMaterialID->Text.Trim().Length()]=='S')
  {
	Application->MessageBox(L"The MaterialID's last char can NOT be 's' or 'S',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    edtMaterialID->SetFocus();
    edtMaterialID->SelectAll();
    return;
  }

  if (cboMaterialType->Text.IsEmpty())
  {
	Application->MessageBox(L"Please select Type.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    cboMaterialType->SetFocus();
    return;
  }
  if (!edtMaterialPos->Text.Trim().IsEmpty())
  {
    if (edtMaterialPos->Text.Trim().UpperCase()[edtMaterialPos->Text.Trim().Length()]!='S')
    {
	  Application->MessageBox(L"The position's last char must be 's' or 'S',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      edtMaterialPos->SetFocus();
      edtMaterialPos->SelectAll();
      return;
    }
  }
  strSQLClause=(AnsiString)"select ResID from dbo.lib_Res where ResID='"
              +(AnsiString)SQLStr(edtMaterialID->Text.Trim())+(AnsiString)"'";
  if (!CheckUnique(strSQLClause))
  {
	 Application->MessageBox(L"MaterialID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     edtMaterialID->SetFocus();
     edtMaterialID->SelectAll();
  }
  else
  {
    if (CheckMaterial(edtMaterialID->Text.Trim()))
    {
      pListItem=lstvwMaterialAdd->Items->Add();
	  lstvwMaterialAdd->ItemFocused = pListItem;
      lstvwMaterialAdd->Selected = pListItem;
      pListItem->Caption=edtMaterialID->Text.Trim();
      pListItem->SubItems->Add(cboMaterialType->Text);
      pListItem->SubItems->Add(edtMaterialTitle->Text.Trim());
      pListItem->SubItems->Add(edtMaterialPos->Text.Trim());
      frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialAdd->Items->Count;
      edtMaterialID->SetFocus();
//      edtMaterialID->SelectAll();
      edtMaterialID->Clear();
      edtMaterialPos->Clear();
      edtMaterialTitle->Clear();
      btnConfirm->Enabled=true;
      btnDiscard->Enabled=true;
    }
    else
    {
	  Application->MessageBox(L"MaterialID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      edtMaterialID->SetFocus();
      edtMaterialID->SelectAll();
	}
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnConfirmClick(TObject *Sender)
{
  AnsiString strSQLClause,strResType;
  int nResType;
  for (int i=0;i<lstvwMaterialAdd->Items->Count;i++)
  {
    strResType=lstvwMaterialAdd->Items->Item[i]->SubItems->Strings[0];
    nResType=strlstResType->Values[strResType].ToInt();
	strSQLClause = "Insert into dbo.lib_Res(ResID,ResType,RegTime,Position,state,remark) values('"
				   +SQLStr(lstvwMaterialAdd->Items->Item[i]->Caption)
                   +"',"
                   +nResType
                   +",GetDate(),'"
                   +SQLStr(lstvwMaterialAdd->Items->Item[i]->SubItems->Strings[2])
                   +"',0,'"
                   +SQLStr(lstvwMaterialAdd->Items->Item[i]->SubItems->Strings[1])
                   +"')";
    dmdMaterials->daqMaterials->SQL->Clear();
    dmdMaterials->daqMaterials->SQL->Add(strSQLClause);
    strSQLClause=(AnsiString)"select ResID from dbo.lib_Res where ResID='"
                 +(AnsiString)SQLStr(lstvwMaterialAdd->Items->Item[i]->Caption)+(AnsiString)"'";
    if (CheckUnique(strSQLClause))
    {
      try
      {
        dmdMaterials->daqMaterials->ExecSQL();
        CreateLog("Add Materials",lstvwMaterialAdd->Items->Item[i]->Caption,lstvwMaterialAdd->Items->Item[i]->SubItems->Strings[1],"Materials Input");
      }
      catch(...)
      {
		Application->MessageBox(L"Database Error",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        return;
      }
    }
 }
 frmMain->stbStory->Panels->Items[2]->Text
      =(AnsiString)lstvwMaterialAdd->Items->Count+(AnsiString)" materials have been added.";
 lstvwMaterialAdd->Items->Clear();
 btnConfirm->Enabled=false;
 btnDiscard->Enabled=false;
 frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialAdd->Items->Count;
 edtMaterialID->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::btnDiscardClick(TObject *Sender)
{
  int nButton;
  nButton=Application->MessageBox(L"Are you sure to delete all of these records to discard the operation?",L"TBS Library",MB_OKCANCEL|MB_ICONQUESTION);
  if (nButton==ID_OK)
  {
	lstvwMaterialAdd->Items->Clear();
	btnConfirm->Enabled=false;
	btnDiscard->Enabled=false;
	frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialAdd->Items->Count;
	edtMaterialID->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::lstvwMaterialsClick(TObject *Sender)
{
/*  TListItem *pListItem;
  AnsiString strResType,strResState;
  AnsiString strSQLClause;
  int nResType,nResState;
  if (bEditMaterial==true)
  {
	pListItem=lstvwMaterials->ItemFocused;
	strSQLClause=(AnsiString)"select ResType,remark,Position,state from dbo.lib_Res where ResID='"
				 +SQLStr(pListItem->Caption)+(AnsiString)"'";
	dmdMaterials->daqCheck->SQL->Clear();
	dmdMaterials->daqCheck->SQL->Add(strSQLClause);
	try
	{
	  dmdMaterials->daqCheck->Open();
	  dmdMaterials->daqCheck->First();
	}
	catch(...)
	{
	  Application->MessageBox("Database error!","TBS Library",MB_OK|MB_ICONINFORMATION);
	  return;
	}
	if (dmdMaterials->daqCheck->RecordCount<1)
	{
	  Application->MessageBox("This material's ID has been modified or this material has been deleted,please refresh the material list.","TBS Library",MB_OK|MB_ICONINFORMATION);
	  return;
	}
	else
	{
	  nResState=dmdMaterials->daqCheck->FieldByName("state")->AsInteger;
	  nResType=dmdMaterials->daqCheck->FieldByName("ResType")->AsInteger;
	  for (int i=0;i<strlstResType->Count;i++)
	  {
		if (nResType==strlstResType->Values[strlstResType->Names[i]].ToInt())
		{
		  strResType=strlstResType->Names[i];
		  break;
		}
	  }
	  pListItem->SubItems->Strings[0]=strResType;
	  pListItem->SubItems->Strings[1]=OrigStr(dmdMaterials->daqCheck->FieldValues["remark"]);
	  pListItem->SubItems->Strings[3]=OrigStr(dmdMaterials->daqCheck->FieldValues["Position"]);
	  switch (nResState)
	  {
		case -1:
			pListItem->SubItems->Strings[4]="Lost";
			break;
		case 0:
			pListItem->SubItems->Strings[4]="On Shelf";
			break;
		case 1:
			pListItem->SubItems->Strings[4]="Borrowed";
			break;
	  }
	  if (nResState==1)
	  {
		edtResID->Enabled=false;
		edtResRemark->Enabled=false;
		edtPosition->Enabled=false;
		cboState->Enabled=false;
		cboResType->Enabled=false;
		btnMaterialOk->Enabled=false;
	  }
	  else
      {
        edtResID->Enabled=true;
        edtResRemark->Enabled=true;
        edtPosition->Enabled=true;
        cboState->Enabled=true;
        cboResType->Enabled=true;
        btnMaterialOk->Enabled=true;
      }
    }
    cboResType->ItemIndex=strlstResType->IndexOfName(strResType);
    edtResID->Text=pListItem->Caption;
    edtResRemark->Text=pListItem->SubItems->Strings[1];
    edtPosition->Text=pListItem->SubItems->Strings[3];
    cboState->ItemIndex=nResState+1;
    strResID=edtResID->Text.Trim();
    strRemark=edtResRemark->Text.Trim();
  }*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::lstvwMaterialAddKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if (Key==VK_DELETE)
     DeleteMaterial();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::lstvwMaterialPosChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
  frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialPos->Items->Count;
}
//---------------------------------------------------------------------------
void TfrmMaterials::DeleteMaterial()
{
    TListItem *pListItem,*pNextItem;
    TItemStates ItemState;
    int nSelected;
    int button;
    pListItem=lstvwMaterialAdd->ItemFocused;
    if ((pListItem==NULL)||(pListItem->Selected==false))
		Application->MessageBox(L"You must select a material to delete.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
	else
    {
	  button=Application->MessageBox(L"Are you sure to delete the selected material?",L"TBS Library",MB_OKCANCEL|MB_ICONQUESTION);
      if (button==ID_OK)
      {
        ItemState<<isSelected;
        pListItem=lstvwMaterialAdd->Selected;
        nSelected=lstvwMaterialAdd->SelCount;
        for (int i=0;i<nSelected;i++)
        {
          pNextItem=lstvwMaterials->GetNextItem(pListItem,sdBelow,ItemState);
          lstvwMaterialAdd->Items->
                      Delete(lstvwMaterialAdd->Items->IndexOf(pListItem));

          pListItem=pNextItem;
        }
        if (lstvwMaterialAdd->ItemFocused!=NULL)
            lstvwMaterialAdd->ItemFocused->Selected=true;
        frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialAdd->Items->Count;
        if (lstvwMaterialAdd->Items->Count<1)
        {
           btnConfirm->Enabled=false;
           btnDiscard->Enabled=false;
        }
	  }
	}
}
bool TfrmMaterials::CheckMaterial(AnsiString strMaterialID)
{
  for(int i=0;i<lstvwMaterialAdd->Items->Count;i++)
  {
	if (strMaterialID==lstvwMaterialAdd->Items->Item[i]->Caption)
		return false;
  }
  return true;
}


void __fastcall TfrmMaterials::lstvwMaterialsKeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
  if ((Key==VK_DELETE)&&(!bEditMaterial))
	 DeleteRes();
  if ((Key==VK_RETURN)&&(!bEditMaterial))
     ModifyRes();

}
//---------------------------------------------------------------------------
void TfrmMaterials::DeleteRes()
{
    int button;
    TListItem *pListItem;
    pListItem=lstvwMaterials->ItemFocused;
    if ((pListItem==NULL)||(pListItem->Selected==false))
		Application->MessageBox(L"You must select a material to delete.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
    else
    {
	  button=Application->MessageBox(L"Are you sure to delete the selected material?",L"TBS Library",MB_OKCANCEL|MB_ICONQUESTION);
      if (button==ID_OK)
      {
        if (MaterialsDelete())
        {
          if (lstvwMaterials->ItemFocused!=NULL)
            lstvwMaterials->ItemFocused->Selected=true;
        }
      }
    }
}

void __fastcall TfrmMaterials::lstvwMaterialsDblClick(TObject *Sender)
{
  if (!bEditMaterial)
      ModifyRes();    
}
//---------------------------------------------------------------------------
/*int __stdcall SortByName(int Item1,int Item2,int ParamSort)
{
  TListItem *pItem1=(TListItem *)Item1;
  TListItem *pItem2=(TListItem *)Item2;
  int intReturn,index;
  AnsiString tempstr;
  //if (ParamSort>=0) sign=true; else {sign=false; ParamSort=-ParamSort;}
  tempstr=IntToStr(ParamSort);
  tempstr.Delete(0,1);
  index=StrToInt(tempstr);
  if (index>0){
  if (ParamSort>=20000) {
  if (StrToDate(pItem1->SubItems->Strings[index-1].c_str()) <= StrToDate(pItem2->SubItems->Strings[index-1].c_str()))
  intReturn = -1; else intReturn = 1;
  }
  if (ParamSort>=10000) {
  if (StrToInt(pItem1->SubItems->Strings[index-1].c_str()) <= StrToInt(pItem2->SubItems->Strings[index-1].c_str()))
   intReturn = -1;
  else intReturn = 1;
  }
  intReturn= lstrcmpi(pItem1->SubItems->Strings[index-1].c_str(),pItem2->SubItems->Strings[index-1].c_str());
  }
  else {//item議侃尖
  if (ParamSort>=20000) {
  if (StrToDate(pItem1->Caption)<=StrToDate(pItem2->Caption))
  intReturn = -1; else intReturn = 1;
  }
  if (ParamSort>=10000) {
  if (StrToInt(pItem1->Caption)<=StrToInt(pItem2->Caption))
   intReturn = -1; else intReturn = 1;
  }
  intReturn= lstrcmp(pItem1->Caption.c_str(),pItem2->Caption.c_str());

  }
  if (ColumnSign==false) return intReturn;
  else return -(intReturn);
 }

*/
void __fastcall TfrmMaterials::muiPopModifyClick(TObject *Sender)
{
  ModifyRes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::muiPopDeleteClick(TObject *Sender)
{
  DeleteRes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::muiPopRefreshClick(TObject *Sender)
{
 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
  BeginDate = StrToDateTime(lblBeginDate->Caption, FS );
  EndDate = StrToDateTime(lblEndDate->Caption, FS );

  if (muiBrowse->Checked)
	 BrowseMaterials();
  else
     ShelfMaterials();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaterials::WndProc(TMessage &Message)
{
  if ( (Message.Msg!=WM_SYSCOMMAND)||
    ((Message.WParam!=SC_NEXTWINDOW)&&(Message.WParam!=SC_PREVWINDOW)) )
   TForm::WndProc(Message);
}

void __fastcall TfrmMaterials::edtMaterialIDKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if (Key==VK_RETURN)
  {
    TListItem *pListItem;
    AnsiString strSQLClause;
    if (edtMaterialID->Text.Trim().IsEmpty())
    {
	  Application->MessageBox(L"Please input MaterialID.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      edtMaterialID->SetFocus();
	  return;
    }
    if (edtMaterialID->Text.Trim().UpperCase()[edtMaterialID->Text.Trim().Length()]=='S')
    {
	  Application->MessageBox(L"The MaterialID's last char can NOT be 's' or 'S',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      edtMaterialID->SetFocus();
      edtMaterialID->SelectAll();
      return;
    }
    if (cboMaterialType->Text.IsEmpty())
    {
	  Application->MessageBox(L"Please select Type.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  cboMaterialType->SetFocus();
	  return;
	}
	if (!edtMaterialPos->Text.Trim().IsEmpty())
	{
	  if (edtMaterialPos->Text.Trim().UpperCase()[edtMaterialPos->Text.Trim().Length()]!='S')
	  {
		Application->MessageBox(L"The position's last char must be 's' or 'S',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		edtMaterialPos->SetFocus();
		edtMaterialPos->SelectAll();
		return;
	  }
	}
	strSQLClause=(AnsiString)"select ResID from dbo.lib_Res where ResID='"
				+(AnsiString)SQLStr(edtMaterialID->Text.Trim())+(AnsiString)"'";
	if (!CheckUnique(strSQLClause))
	{
	   Application->MessageBox(L"MaterialID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	   edtMaterialID->SetFocus();
	   edtMaterialID->SelectAll();
	}
	else
	{
	  if (CheckMaterial(edtMaterialID->Text.Trim()))
      {
        pListItem=lstvwMaterialAdd->Items->Add();
        pListItem->Caption=edtMaterialID->Text.Trim();
        pListItem->SubItems->Add(cboMaterialType->Text);
        pListItem->SubItems->Add(edtMaterialTitle->Text.Trim());
        pListItem->SubItems->Add(edtMaterialPos->Text.Trim());
		frmMain->stbStory->Panels->Items[1]->Text="Material numbers: "+(AnsiString)lstvwMaterialAdd->Items->Count;
        edtMaterialID->SetFocus();
        edtMaterialID->Clear();
        edtMaterialPos->Clear();
        edtMaterialTitle->Clear();
        
        btnConfirm->Enabled=true;
        btnDiscard->Enabled=true;
      }
      else
      {
		Application->MessageBox(L"MaterialID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        edtMaterialID->SetFocus();
		edtMaterialID->SelectAll();
      }
	}
  }
}
//---------------------------------------------------------------------------


void __fastcall TfrmMaterials::muiPrintClick(TObject *Sender)
{
	print_Report(false);
}
//---------------------------------------------------------------------------

void TfrmMaterials::print_Report(bool b_Preview)
{
  AnsiString strSQLClause;
  AnsiString strBeginDate,strEndDate;
  TDateTime time;

 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
  BeginDate = StrToDateTime(lblBeginDate->Caption, FS );
  EndDate = StrToDateTime(lblEndDate->Caption, FS );


  strBeginDate=BeginDate.FormatString("yyyy-mm-dd");
  strEndDate=EndDate.FormatString("yyyy-mm-dd");

  strSQLClause=(AnsiString)"select ResID, CASE ResType WHEN 1 then 'Tapes' WHEN 2 then 'Books' WHEN 3 then 'Magazines' WHEN 4 then 'Files' else '.' end as ResType, remark, RegTime, Position, CASE state WHEN -1 then 'Lost' WHEN 0 then 'On Shelf' WHEN 1 then 'Borrowed' else '.' end as state, BorrowerID from lib_Res ";

  if (frmMaterials->muiBrowse->Checked)
  {
	 if (frmMaterials->cboInputType->ItemIndex==0)
					 strSQLClause = strSQLClause + (AnsiString)" where cast(RegTime As Date)>='"+
					 (AnsiString)strBeginDate+(AnsiString)"'"+
					 (AnsiString)"and cast(RegTime As Date)<='"+
					 (AnsiString)strEndDate+(AnsiString)"'";
	 else
					 strSQLClause = strSQLClause + (AnsiString)" Where ResType="
					 +(AnsiString)frmMaterials->cboInputType->ItemIndex
					 +(AnsiString)"and cast(RegTime As Date)>='"
					 +(AnsiString)strBeginDate+(AnsiString)"'"
					 +(AnsiString)"and cast(RegTime As Date)<='"
					 +(AnsiString)strEndDate+(AnsiString)"'";
  }
  else
  {
	if (frmMaterials->cboInputType->ItemIndex==0)
	{
	  if (frmMaterials->lblShelfID->Caption=="All")
	  {

	  }
//		  strSQLClause=(AnsiString)"select ResID, ResType, remark, RegTime, Position, CASE state WHEN -1 then 'Lost' WHEN 0 then 'On Shelf' WHEN 1 then 'Borrowed' else '.' end as state, BorrowerID from lib_Res";
	  else
					strSQLClause = strSQLClause + (AnsiString)" where Position='"+
					   (AnsiString)SQLStr(frmMaterials->lblShelfID->Caption)+
					   (AnsiString)"'";
	}
	else
	{
	  if (frmMaterials->lblShelfID->Caption=="All")
					strSQLClause = strSQLClause + (AnsiString)" Where ResType="
					   +(AnsiString)frmMaterials->cboInputType->ItemIndex;
	  else
					strSQLClause = strSQLClause + (AnsiString)" Where ResType="
					   +(AnsiString)frmMaterials->cboInputType->ItemIndex
					   +(AnsiString)"and Position='"
					   +(AnsiString)SQLStr(frmMaterials->lblShelfID->Caption)
					   +(AnsiString)"'";
	}
  }

  AnsiString strOrderList;
  GetOrderColumn(strSQLClause,strOrderList);


  dmdMaterials->daqPrint->SQL->Clear();
  dmdMaterials->daqPrint->SQL->Add(strSQLClause);
  try
  {
	dmdMaterials->daqPrint->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  }

//  frxReport1->LoadFromFile("Untitled2.fr3");
frxReport1->Clear();
TfrxDataPage * DataPage;
TfrxReportPage * Page;
TfrxBand * Band;
TfrxMasterData * DataBand;
TfrxMemoView * Memo;
TfrxPageHeader * RHBand;
TfrxLineView * RHLine;
frxDBDataset1->DataSet = dmdMaterials->daqPrint;
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
Band = new TfrxReportTitle(Page);
Band->CreateUniqueName();
// only ：Top； coordinate and height of band need setting
//    both in pixels
Band->Top = 0;
Band->Height = 20;
// add object to report title band

TFont* pFont;
TFontStyles style;
pFont=new TFont;
style<<fsBold;
pFont->Name=(AnsiString)"Arial";
pFont->Color=clNavy;
pFont->Style=style;
pFont->Size = 15;

Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = "View of Material Information";
Memo->Font = pFont;
Memo->Height = 50;
// this object will be stretched to band・s width
Memo->Align = baWidth;

Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = (AnsiString)"Date "+time.CurrentDate().FormatString("yyyy-mm-dd");
Memo->SetBounds(600, 0, 200, 20);
//  frmPreview->qrlblPrinter->Caption=(AnsiString)"Printed By "+dfrm->strUserID;
Memo->HAlign = haRight;

Memo = new TfrxMemoView(Band);
Memo->CreateUniqueName();
Memo->Text = (AnsiString)(AnsiString)"Printed By "+dmdStory->strCurrentUserID;
Memo->SetBounds(820, 0, 200, 20);
Memo->HAlign = haRight;


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
DataBand->Top = 150;
DataBand->Height = 20;
DataBand->Stretched = true;

//  TFont* pFont;
//  TFontStyles style;
  int nPosition;

  pFont=new TFont;
  style<<fsUnderline;
  style<<fsBold;
  pFont->Name=(AnsiString)"Arial";
  pFont->Color=clNavy;
  pFont->Style=style;
  pFont->Size = 10;
  nPosition=0;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Material ID";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "ResID";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

  nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Type";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "ResType";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

  nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 200, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Title";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "remark";
	Memo->SetBounds(nPosition, 0, 200, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Register Time";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "RegTime";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Position";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "Position";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Status";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "state";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

	Memo = new TfrxMemoView(RHBand);
	Memo->CreateUniqueName();
	Memo->Font = pFont;
	Memo->SetBounds(nPosition, 0, 100, 20);
	// align text to object・s right margin
	//Memo->AutoWidth = true;
	Memo->Text = "Borrower";
	Memo->HAlign = haLeft;

	Memo = new TfrxMemoView(DataBand);
	Memo->CreateUniqueName();
	// connect to data
	Memo->DataSet = frxDBDataset1;
	//Memo->Font = pFont;
	Memo->DataField = "BorrowerID";
	Memo->SetBounds(nPosition, 0, 100, 20);
	//Memo->AutoWidth = true;
	Memo->StretchMode = smMaxHeight;
	Memo->ShiftMode = smWhenOverlapped;
	Memo->HAlign = haLeft;

   nPosition+= Memo->Width+20;

  if (b_Preview)
	  frxReport1->ShowReport(true);
  else
	  frxReport1->Print();
  dmdMaterials->daqPrint->Close();

if (pFont!=NULL)
	delete pFont;

/*
  frmPreview->qlOrder->Caption =  strOrderList;
  frmPreview->qrdbtxtMaterialID->DataField=(AnsiString)"ResID";
  frmPreview->qrdbtxtType->DataField=(AnsiString)"ResType";
  frmPreview->qrdbtxtTitle->DataField=(AnsiString)"remark";
  frmPreview->qrdbtxtRegTime->DataField=(AnsiString)"RegTime";
  frmPreview->qrdbtxtPosition->DataField=(AnsiString)"Position";
  frmPreview->qrdbtxtStatus->DataField=(AnsiString)"state";
  frmPreview->qrdbtxtBorrower->DataField=(AnsiString)"BorrowID";

  frmPreview->qrlblDate->Caption=(AnsiString)"Date "+time.CurrentDate().FormatString("yyyy-mm-dd");
  frmPreview->qrlblPrinter->Caption=(AnsiString)"Printed by "+frmMaterials->strUserID;

  if ( bpreview )
	frmPreview->qrpReport->Preview();
  else
	frmPreview->qrpReport->Print();
 */

//  frmPreview->qrpReport->Print();
}

void __fastcall TfrmMaterials::btGoClick(TObject *Sender)
{
	muiPopRefreshClick(Sender);
}



//---------------------------------------------------------------------------





void __fastcall TfrmMaterials::FormShow(TObject *Sender)
{
   TDateTime time;
   AnsiString strINIFileName;

   strINIFileName=Application->ExeName;
   strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
					 (AnsiString)".ini";
   TIniFile* IniFile=new TIniFile(strINIFileName);
   edtPosFile->Text=IniFile->ReadString("PosFile","FileName","");
   time=time.CurrentDate();
   EndDate=IniFile->ReadInteger("Material Browse","EndDate",35912);
   BeginDate=IniFile->ReadInteger("Material Browse","BeginDate",35911);
   delete IniFile;
   BeginDate=BeginDate-EndDate+time;
   EndDate=time;
   lblBeginDate->Caption=BeginDate.FormatString("yyyy-mm-dd");
   lblEndDate->Caption=EndDate.FormatString("yyyy-mm-dd");
   lblFrom->Visible=true;
   lblBeginDate->Visible=true;
   lblTo->Visible=true;
   lblEndDate->Visible=true;
   lblShelf->Visible=false;
   lblShelfID->Visible=false;
   muiBrowse->Checked=true;
   muiPopTime->Checked = true;

   btnConfirm->Enabled = false;
   btnDiscard->Enabled = false;
   btnPosConfirm->Enabled = false;
   btnPosDiscard->Enabled = false;
   strlstResType=new TStringList();
   cboMaterialType->ItemIndex=0;
   cboResType->ItemIndex=0;
   cboInputType->ItemIndex=0;
   BrowseMaterialType(strlstResType);
   cboInputType->ItemIndex=0;
   nbkMaterials->PageIndex=0;
   muiEdit->Visible=false;
   muiMaterial->Visible=false;
   edtMaterialID->SetFocus();

   BrowsePage();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::FormDestroy(TObject *Sender)
{

 TFormatSettings FS;
FS.DateSeparator = '-';
FS.ShortDateFormat = "yyyy-mm-dd";
FS.LongTimeFormat = "hh:nn:ss";
FS.TimeSeparator = ':';
  BeginDate = StrToDateTime(lblBeginDate->Caption, FS );
  EndDate = StrToDateTime(lblEndDate->Caption, FS );

  AnsiString strINIFileName;
  strINIFileName=Application->ExeName;
  strINIFileName = strINIFileName.SubString(1,strINIFileName.Length()-4)+
    				 (AnsiString)".ini";
  TIniFile* IniFile=new TIniFile(strINIFileName);
  IniFile->WriteString("PosFile","FileName",strPosFileName);
  IniFile->WriteInteger("Material Browse","BeginDate",(int)BeginDate);
  IniFile->WriteInteger("Material Browse","EndDate",(int)EndDate);
  delete IniFile;
  delete strlstResType;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::lstvwMaterialsColumnClick(TObject *Sender, TListColumn *Column)

{
  ColumnToSort = Column->Index;
  dynamic_cast<TCustomListView *>(Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaterials::lstvwMaterialsCompare(TObject *Sender, TListItem *Item1,
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

void __fastcall TfrmMaterials::nbkMaterialsPageChanged(TObject *Sender)
{
 // if (frmMaterials->nbkMaterials->PageIndex==1)
  //{
  //	BrowsePage();
 // }
  muiPopRefreshClick(Sender);
}
//---------------------------------------------------------------------------

