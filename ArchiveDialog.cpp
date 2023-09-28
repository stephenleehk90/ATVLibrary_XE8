//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ArchiveDialog.h"
#include "ArchiveThread.h"
#include "StoryData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmArchive *frmArchive;
//---------------------------------------------------------------------------
__fastcall TfrmArchive::TfrmArchive(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmArchive::FormShow(TObject *Sender)
{
    btnOK->Enabled=false;
    thdArchive=new TThdArchive(false);
    Screen->Cursor = (TCursor)-11;
    Label1->Caption="System is archiving, Please wait.";
    prgIndexInfo->Position=0;
    ShowIndexInfo();
    tmrIndexInfo->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmArchive::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    tmrIndexInfo->Enabled=false;
    delete thdArchive;
}
//---------------------------------------------------------------------------
void __fastcall TfrmArchive::tmrIndexInfoTimer(TObject *Sender)
{
//    ShowIndexInfo();
    if (prgIndexInfo->Position < prgIndexInfo->Max)
        prgIndexInfo->Position++;
    else
        prgIndexInfo->Position = 0;
}
//---------------------------------------------------------------------------
void TfrmArchive::ShowIndexInfo()
{
//    prgIndexInfo->Position++;
/*    int n=0,max=0;
    try{
        dmdStory->qryIndexInfo->Close();
        dmdStory->qryIndexInfo->SQL->Clear();
        dmdStory->qryIndexInfo->SQL->Add("exec master.dbo.xpGetBuildingStatus");
        dmdStory->qryIndexInfo->Open();
        if (dmdStory->qryIndexInfo->FindFirst())
            {
            do{
              n+=dmdStory->qryIndexInfo->FieldByName("CurrentNumber")->AsInteger;
              max+=dmdStory->qryIndexInfo->FieldByName("TotalNumber")->AsInteger;
              }
            while(dmdStory->qryIndexInfo->FindNext());
            }
        dmdStory->qryIndexInfo->Close();
        }
    catch(...)
        {
        }
    if(prgIndexInfo->Max!=max) prgIndexInfo->Max=max;
    prgIndexInfo->Position=n;*/
}
//---------------------------------------------------------------------------


