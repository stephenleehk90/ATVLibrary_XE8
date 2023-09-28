//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UserData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmdUserAdmin *dmdUserAdmin;
//---------------------------------------------------------------------------
__fastcall TdmdUserAdmin::TdmdUserAdmin(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
