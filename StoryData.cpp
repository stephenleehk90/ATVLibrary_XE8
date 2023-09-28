//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StoryData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmdStory *dmdStory;
//---------------------------------------------------------------------------
__fastcall TdmdStory::TdmdStory(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------

