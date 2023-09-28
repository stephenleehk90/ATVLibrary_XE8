//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PrevFrm.h"
#include "InfoReport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmPrev *frmPrev;
//---------------------------------------------------------------------------
__fastcall TfrmPrev::TfrmPrev(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::FormClose(TObject *Sender, TCloseAction &Action)
{
 // qrPreview->PageNumber = 1;
 // qrPreview->QRPrinter = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnHomeClick(TObject *Sender)
{
 // qrPreview->PageNumber = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnPrevClick(TObject *Sender)
{
 // qrPreview->PageNumber -= 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnNextClick(TObject *Sender)
{
//  qrPreview->PageNumber += 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnEndClick(TObject *Sender)
{
//  qrPreview->PageNumber = qrPreview->QRPrinter->PageCount;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnZoomInClick(TObject *Sender)
{
//  qrPreview->Zoom += 20;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnZoomOutClick(TObject *Sender)
{
 // qrPreview->Zoom -= 20;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnSetupClick(TObject *Sender)
{
//  frmPreview->qrpLendInfo->QRPrinter = qrPreview->QRPrinter;
 // frmPreview->qrpLendInfo->PrinterSetup();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnPrintClick(TObject *Sender)
{
 // frmPreview->qrpLendInfo->QRPrinter = qrPreview->QRPrinter;
 // frmPreview->qrpLendInfo->Print();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

