//---------------------------------------------------------------------------
#ifndef PwdFrmH
#define PwdFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmPassword : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblOld;
    TEdit *edtOld;
    TLabel *lblNew;
    TLabel *lblConfirm;
    TEdit *edtNew;
    TEdit *edtConfirm;
    TButton *btnOk;
    TButton *btnCancel;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmPassword(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPassword *frmPassword;
//---------------------------------------------------------------------------
#endif
