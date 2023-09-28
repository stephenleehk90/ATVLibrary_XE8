//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "InfoReport.h"
#include "lenddatas.h"
#include "PrevFrm.h"
#include "commfunc.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmPreview *frmPreview;
//---------------------------------------------------------------------------
__fastcall TfrmPreview::TfrmPreview(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::QRDBTextMaterialIDPrint(TObject *sender,
      AnsiString &Value)
{
 Value = OrigStr(Value);      
}
//---------------------------------------------------------------------------




void __fastcall TfrmPreview::QRDBTextLendOpIDPrint(TObject *sender,
      AnsiString &Value)
{
  Value = OrigStr(Value);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::QRDBTextBorrowIDPrint(TObject *sender,
      AnsiString &Value)
{
  Value = OrigStr(Value);    
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::QRDBTextReturnOpIDPrint(TObject *sender,
      AnsiString &Value)
{
  Value = OrigStr(Value);    
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::QRDBTextRemarkPrint(TObject *sender,
      AnsiString &Value)
{
  Value = OrigStr(Value);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPreview::qrpLendInfoPreview(TObject *Sender)
{
    frmPrev->Show();
 //   frmPrev->qrPreview->QRPrinter = dynamic_cast<TQRPrinter*>(Sender);
}
//---------------------------------------------------------------------------




void __fastcall TfrmPreview::QRDBTextReturnTimePrint(TObject *sender,
      AnsiString &Value)
{
    if (Value=="1899-12-30 00:00")
        Value=(AnsiString)"";    
}
//---------------------------------------------------------------------------



