//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "QueryFrm.h"
#include "MaterialFrm.h"
#include "MaterialData.h"
//---------------------------------------------------------------------------
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
  frmMaterials->BeginDate=dtpBegin->Date;
  frmMaterials->EndDate=dtpEnd->Date;
  frmMaterials->lblBeginDate->Caption=frmMaterials->BeginDate.FormatString("yyyy-mm-dd");
  frmMaterials->lblEndDate->Caption=(frmMaterials->EndDate).FormatString("yyyy-mm-dd");
}
//---------------------------------------------------------------------------
void __fastcall TfrmQuery::FormShow(TObject *Sender)
{
  dtpBegin->Date=frmMaterials->BeginDate;
  dtpEnd->Date=frmMaterials->EndDate;
}
//---------------------------------------------------------------------------

