//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PrevFrm.h"
#include "PreviewFrm.h"
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
  qrPreview->PageNumber = 1;
  qrPreview->QRPrinter = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnHomeClick(TObject *Sender)
{
  qrPreview->PageNumber = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnPrevClick(TObject *Sender)
{
  qrPreview->PageNumber -= 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnNextClick(TObject *Sender)
{
  qrPreview->PageNumber += 1;    
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnEndClick(TObject *Sender)
{
  qrPreview->PageNumber = qrPreview->QRPrinter->PageCount;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnZoomInClick(TObject *Sender)
{
  qrPreview->Zoom += 20;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnZoomOutClick(TObject *Sender)
{
  qrPreview->Zoom -= 20;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnSetupClick(TObject *Sender)
{
  frmPreview->qrpReport->QRPrinter = qrPreview->QRPrinter;
  frmPreview->qrpReport->PrinterSetup();
/*  PrintDialog1->Copies = frmPreview->qrpReport->PrinterSettings->Copies;
  PrintDialog1->MinPage = 1;
  PrintDialog1->MaxPage = frmPreview->qrpReport->PageNumber;
  PrintDialog1->FromPage = frmPreview->qrpReport->PrinterSettings->FirstPage;
  PrintDialog1->ToPage = frmPreview->qrpReport->PrinterSettings->LastPage;
  PrintDialog1->Collate = frmPreview->qrpReport->PrinterSettings->Duplex;
  if (PrintDialog1->Execute())
  {
    frmPreview->qrpReport->PrinterSettings->Copies = PrintDialog1->Copies;
    frmPreview->qrpReport->PrinterSettings->FirstPage = PrintDialog1->FromPage;
    frmPreview->qrpReport->PrinterSettings->LastPage = PrintDialog1->ToPage;
    frmPreview->qrpReport->PrinterSettings->Duplex = PrintDialog1->Collate;

  }*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnPrintClick(TObject *Sender)
{
  frmPreview->qrpReport->QRPrinter = qrPreview->QRPrinter;
  frmPreview->qrpReport->Print();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPrev::btnCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

