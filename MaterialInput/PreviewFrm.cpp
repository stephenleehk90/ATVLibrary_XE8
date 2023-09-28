//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PreviewFrm.h"
#include "PrevFrm.h"
#include "CommFunc.h"
#include "MaterialData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmPreview *frmPreview;
//---------------------------------------------------------------------------
__fastcall TfrmPreview::TfrmPreview(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrdbtxtMaterialIDPrint(TObject *sender,
      AnsiString &Value)
{
  Value=OrigStr(Value);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrdbtxtTypePrint(TObject *sender,
      AnsiString &Value)
{
  int nType;
  nType=Value.ToInt();
  switch (nType)
  {
    case 1:
         Value="Tapes";
         break;
    case 2:
         Value="Books";
         break;
    case 3:
         Value="Magazines";
         break;
    case 4:
         Value="Files";
         break;
    default:
         Value=".";
         break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrdbtxtTitlePrint(TObject *sender,
      AnsiString &Value)
{
  Value=OrigStr(Value);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrdbtxtPositionPrint(TObject *sender,
      AnsiString &Value)
{
  Value=OrigStr(Value);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrdbtxtStatusPrint(TObject *sender,
      AnsiString &Value)
{
  int nState;
  nState=Value.ToInt();
  switch (nState)
  {
    case -1:
         Value="Lost";
         break;
    case 0:
         Value="On Shelf";
         break;
    case 1:
         Value="Borrowed";
         break;
    default:
        Value = ".";
        break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrdbtxtBorrowerPrint(TObject *sender,
      AnsiString &Value)
{
  Value=OrigStr(Value);    
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrpReportPreview(TObject *Sender)
{
    frmPrev->Show();
    frmPrev->qrPreview->QRPrinter = dynamic_cast<TQRPrinter*>(Sender);
}
//---------------------------------------------------------------------------


