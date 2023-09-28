//---------------------------------------------------------------------------
#ifndef SaveFrmH
#define SaveFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TfrmSave : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblIndicator;
    TCSpinEdit *spnedtDays;
    TButton *btnOk;
    TButton *btnCancel;
    void __fastcall spnedtDaysKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall spnedtDaysKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmSave(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSave *frmSave;
//---------------------------------------------------------------------------
#endif
