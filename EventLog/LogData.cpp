//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LogData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdmdLog *dmdLog;
//---------------------------------------------------------------------------
__fastcall TdmdLog::TdmdLog(TComponent* Owner)
    : TDataModule(Owner)
{
    tbCategory->TableName   = strSTORYLIBTREE;
}
//---------------------------------------------------------------------------
