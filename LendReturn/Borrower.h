//---------------------------------------------------------------------------
#ifndef BorrowerH
#define BorrowerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmBorrower : public TForm
{
__published:	// IDE-managed Components
    TComboBox *cboMode;
    TLabel *lblUser;
    TEdit *edtUser;
    TButton *btnOk;
    TButton *btnCancel;
    TLabel *lblMode;
    TCheckBox *chkEveryone;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    
    void __fastcall chkEveryoneClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmBorrower(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBorrower *frmBorrower;
//---------------------------------------------------------------------------
#endif
