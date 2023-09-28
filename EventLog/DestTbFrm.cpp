//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DestTbFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmDestTbs *frmDestTbs;
//---------------------------------------------------------------------------
__fastcall TfrmDestTbs::TfrmDestTbs(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDestTbs::FormDeactivate(TObject *Sender)
{
  Close();    
}
//---------------------------------------------------------------------------

