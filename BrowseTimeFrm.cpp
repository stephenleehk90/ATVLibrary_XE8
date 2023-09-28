//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "BrowseTimeFrm.h"
#include "frm_Main.h"
#include "frm_StoryInput.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmBrowseTime *frmBrowseTime;
//---------------------------------------------------------------------------
__fastcall TfrmBrowseTime::TfrmBrowseTime(TComponent* Owner)
    : TForm(Owner)
{
  NMDATETIMECHANGE dtc;
  SYSTEMTIME now;
  GetSystemTime(&now);
  dtc.nmhdr.hwndFrom = dtpBegin->Handle;
  dtc.nmhdr.idFrom = NULL;
  dtc.nmhdr.code = DTN_DATETIMECHANGE;
  dtc.dwFlags = GDT_VALID;
  dtc.st = now;

  dtpBegin->Perform(WM_NOTIFY, NULL, (int)&dtc);

  dtc.nmhdr.hwndFrom = dtpEnd->Handle;
  dtc.nmhdr.idFrom = NULL;
  dtc.nmhdr.code = DTN_DATETIMECHANGE;
  dtc.dwFlags = GDT_VALID;
  dtc.st = now;

  dtpEnd->Perform(WM_NOTIFY, NULL, (int)&dtc);

}
//---------------------------------------------------------------------------
void __fastcall TfrmBrowseTime::btnOkClick(TObject *Sender)
{
  frmStoryInput->BeginDate=dtpBegin->Date;
  frmStoryInput->EndDate=dtpEnd->Date;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBrowseTime::FormShow(TObject *Sender)
{
  dtpBegin->Date=frmStoryInput->BeginDate;
  dtpEnd->Date=frmStoryInput->EndDate;
}
//---------------------------------------------------------------------------

