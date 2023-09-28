//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "QueryFrmLog.h"
#include "LogFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmQueryLog *frmQueryLog;
//---------------------------------------------------------------------------
__fastcall TfrmQueryLog::TfrmQueryLog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmQueryLog::FormShow(TObject *Sender)
{
  dtpBegin->Date=frmLogs->LogBegin;
//  dtpEnd->Date=frmLogs->LogEnd-1;
  dtpEnd->Date=frmLogs->LogEnd;
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
void __fastcall TfrmQueryLog::btnOkClick(TObject *Sender)
{
  frmLogs->LogBegin=dtpBegin->Date;
  frmLogs->LogEnd=dtpEnd->Date;
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
void __fastcall TfrmQueryLog::chkOpIDClick(TObject *Sender)
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
