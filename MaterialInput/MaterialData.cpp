//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MaterialData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmdMaterials *dmdMaterials;
//---------------------------------------------------------------------------
__fastcall TdmdMaterials::TdmdMaterials(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
