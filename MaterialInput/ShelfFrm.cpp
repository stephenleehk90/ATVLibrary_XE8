//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ShelfFrm.h"
#include "MaterialFrm.h"
#include "CommFunc.h"
#include "MaterialData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmShelf *frmShelf;
//---------------------------------------------------------------------------
__fastcall TfrmShelf::TfrmShelf(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmShelf::btnOkClick(TObject *Sender)
{
  AnsiString strSQLClause;
  if (rdbShelf->Checked)
    frmMaterials->lblShelfID->Caption=edtShelf->Text;
  else
  {
    //MaterialID在习惯上是大写
    strSQLClause=(AnsiString)"select Position from dbo.lib_Res where ResID='"
                 +SQLStr(edtMaterial->Text.Trim().UpperCase())+(AnsiString)"'";
    dmdMaterials->daqCheck->SQL->Clear();
    dmdMaterials->daqCheck->SQL->Add(strSQLClause);
    try
    {
      dmdMaterials->daqCheck->Open();
    }
    catch(...)
    {
	  Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  ModalResult=mrNone;
	  return;
	}
	if (dmdMaterials->daqCheck->RecordCount<1)
	{
	   ModalResult=mrNone;
	   Application->MessageBox(L"The material can not be found in library,please confirm the MaterialID.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    }
    else
    {
      dmdMaterials->daqCheck->First();
      frmMaterials->lblShelfID->Caption=OrigStr(dmdMaterials->daqCheck->FieldByName("Position")->AsString.Trim());
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmShelf::rdbShelfClick(TObject *Sender)
{
  edtMaterial->Enabled=false;
  edtMaterial->Color=clInactiveBorder;
  edtShelf->Enabled=true;
  edtShelf->Color=clWindow;
  edtShelf->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmShelf::rdbMaterialClick(TObject *Sender)
{
  edtShelf->Enabled=false;
  edtShelf->Color=clInactiveBorder;
  edtMaterial->Enabled=true;
  edtMaterial->Color=clWindow;
  edtMaterial->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmShelf::FormShow(TObject *Sender)
{
  edtShelf->Text=frmMaterials->lblShelfID->Caption;
  if (rdbShelf->Checked)
  {
	 edtShelf->SetFocus();
	 edtShelf->Enabled=true;
	 edtShelf->Color=clWindow;
	 edtMaterial->Enabled=false;
	 edtMaterial->Color=clInactiveBorder;
  }
  else
  {
	 edtMaterial->SetFocus();
	 edtShelf->Enabled=false;
	 edtShelf->Color=clInactiveBorder;
	 edtMaterial->Enabled=true;
     edtMaterial->Color=clWindow;
  }
}
//---------------------------------------------------------------------------

