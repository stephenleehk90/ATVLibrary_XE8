//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "utConfm.h"
#include "utInst.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfmConfirm *fmConfirm;
#define AUTHORIZATION "FOUNDERTBS"
//---------------------------------------------------------------------------
__fastcall TfmConfirm::TfmConfirm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmConfirm::BitBtn1Click(TObject *Sender)
{
	AnsiString strPassword;

    strPassword = edPassword->Text.Trim();

    if (strPassword != (AnsiString)AUTHORIZATION)
    {
    	Application->MessageBox("Incorrect Authorized Password!",MSGBOXCAPTION,48);
        edPassword->SetFocus();
        ModalResult = mrNone;
        return;
    }
}
//---------------------------------------------------------------------------