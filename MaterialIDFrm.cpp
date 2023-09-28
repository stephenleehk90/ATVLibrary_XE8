//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MaterialIDFrm.h"
#include "frm_Main.h"
#include "StoryData.h"
#include "CommFunc.h"
#include "frm_StoryInput.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMaterialID *frmMaterialID;
//---------------------------------------------------------------------------
__fastcall TfrmMaterialID::TfrmMaterialID(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaterialID::btnOkClick(TObject *Sender)
{
/*  AnsiString strSQLClause;
  strSQLClause=(AnsiString)"select AstLibs from lib_Res where ResID='"
               +SQLStr(edtMaterialID->Text.Trim())+"'";
  dmdStory->daqCheck->SQL->Clear();
  dmdStory->daqCheck->SQL->Add(strSQLClause);
  dmdStory->daqCheck->Open();
  if (dmdStory->daqCheck->RecordCount<=0)
  {
    Application->MessageBox("Material dose not exist in the library.","TBS Library",48);
    ModalResult=mrNone;
    edtMaterialID->SetFocus();
    edtMaterialID->SelectAll();
    return;
  }*/
  frmStoryInput->strMaterialID=edtMaterialID->Text.Trim();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMaterialID::FormShow(TObject *Sender)
{
  edtMaterialID->SetFocus();
  edtMaterialID->SelectAll();
}
//---------------------------------------------------------------------------

