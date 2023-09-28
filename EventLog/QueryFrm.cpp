//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "QueryFrm.h"
#include "LogFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmQuery *frmQuery;
//---------------------------------------------------------------------------
__fastcall TfrmQuery::TfrmQuery(TComponent* Owner)
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
void __fastcall TfrmQuery::btnOkClick(TObject *Sender)
{
  frmLogs->LogBegin=dtpBegin->Date;
  frmLogs->LogEnd=dtpEnd->Date+1;
  frmLogs->lblLogBegin->Caption=dtpBegin->Date.FormatString("yyyy-mm-dd");
  frmLogs->lblLogEnd->Caption=dtpEnd->Date.FormatString("yyyy-mm-dd");
  if (chkOpID->Checked)
  {
    frmLogs->strOpID=edtOpID->Text;
    frmLogs->lblOpID->Caption=edtOpID->Text;
  }
  else
  {
    frmLogs->strOpID="everyone";
    frmLogs->lblOpID->Caption="everyone";
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuery::FormShow(TObject *Sender)
{
  dtpBegin->Date=frmLogs->LogBegin;
  dtpEnd->Date=frmLogs->LogEnd-1;
  edtOpID->Text=frmLogs->strOpID;
  if (chkOpID->Checked==false)
  {
    edtOpID->Enabled=false;
    edtOpID->Color=clInactiveBorder;
  }
  else
  {
    edtOpID->Enabled=true;
    edtOpID->Color=clWindow;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmQuery::chkOpIDClick(TObject *Sender)
{
  if (chkOpID->Checked)
  {
    edtOpID->Enabled=true;
    edtOpID->Color=clWindow;
  }
  else
  {
    edtOpID->Enabled=false;
    edtOpID->Color=clInactiveBorder;
  }    
}
//---------------------------------------------------------------------------

