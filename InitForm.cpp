//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "InitForm.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmInit *frmInit;
//---------------------------------------------------------------------------
__fastcall TfrmInit::TfrmInit(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmInit::btOkClick(TObject *Sender)
{
	strODBC = edtServer->Text;
	strUser = edtLogin->Text;
	strPass = edtPass->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInit::FormShow(TObject *Sender)
{
	edtServer->Text = strODBC;
	edtLogin->Text = strUser;
	edtPass->Text = strPass;

}
//---------------------------------------------------------------------------

