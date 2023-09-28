//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DirectoryFrm.h"
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
//---------------------------------------------------------------------------
