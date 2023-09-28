//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PreviewFrm.h"
#include "StoryData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "frxClass"
#pragma link "frxPreview"
#pragma resource "*.dfm"
TfrmPreview *frmPreview;
//---------------------------------------------------------------------------
__fastcall TfrmPreview::TfrmPreview(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPreview::ShowReport()
{
	// frxReport1->Preview = frxPreview1;
			//  frxReport1->ShowReport(true);
}
//---------------------------------------------------------------------------

