//---------------------------------------------------------------------------
#ifndef MaterialIDFrmH
#define MaterialIDFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmMaterialID : public TForm
{
__published:	// IDE-managed Components
    TButton *btnOk;
    TButton *btnCancel;
    TLabel *lblMaterialID;
    TEdit *edtMaterialID;
    TLabel *lblIndicator;
    void __fastcall btnOkClick(TObject *Sender);
    
    
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmMaterialID(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMaterialID *frmMaterialID;
//---------------------------------------------------------------------------
#endif
