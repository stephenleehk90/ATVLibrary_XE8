//---------------------------------------------------------------------------
#ifndef ShelfFrmH
#define ShelfFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmShelf : public TForm
{
__published:	// IDE-managed Components
    TRadioButton *rdbShelf;
    TRadioButton *rdbMaterial;
    TButton *btnOk;
    TButton *btnCancel;
    TEdit *edtShelf;
    TEdit *edtMaterial;
    TLabel *lblIndicator;
    void __fastcall btnOkClick(TObject *Sender);
    
    void __fastcall rdbShelfClick(TObject *Sender);
    void __fastcall rdbMaterialClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmShelf(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmShelf *frmShelf;
//---------------------------------------------------------------------------
#endif
