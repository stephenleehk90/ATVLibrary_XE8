//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "utGlDm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmGlobal *dmGlobal;
//---------------------------------------------------------------------------
__fastcall TdmGlobal::TdmGlobal(TComponent* Owner)
    : TDataModule(Owner)
{
    dmGlobal->tbCategory->TableName   = "lib_StoryLibTree";
//    dmTemplate->tbExtFields->TableName  = "lib_ExtendFields";
//    dmTemplate->tbBaseFields->TableName = "lib_BaseFields";
}
//---------------------------------------------------------------------------
