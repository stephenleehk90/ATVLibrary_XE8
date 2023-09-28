//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "utGlobDm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmTemplate *dmTemplate;
//---------------------------------------------------------------------------
__fastcall TdmTemplate::TdmTemplate(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
