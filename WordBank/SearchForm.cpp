//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SearchForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSearch *frmSearch;
//---------------------------------------------------------------------------
__fastcall TfrmSearch::TfrmSearch(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

AnsiString TfrmSearch::Execute(void)
{
    if (ShowModal()==mrOk)
        return edSearch->Text;
    else
        return "";
}

void __fastcall TfrmSearch::edSearchKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key==VK_RETURN)
    {
        Key=NULL;
        ModalResult=mrOk;
    }
    else if (Key==VK_ESCAPE)
    {
        Key=NULL;
        ModalResult=mrCancel;
    }
}
//---------------------------------------------------------------------------

