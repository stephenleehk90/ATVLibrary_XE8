//---------------------------------------------------------------------------
#ifndef SelectFieldFrmH
#define SelectFieldFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSelectFields : public TForm
{
__published:	// IDE-managed Components
    TListView *lstvwFields;
    TLabel *lblIndicator;
    TButton *btnOk;
    TButton *btnCancel;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmSelectFields(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectFields *frmSelectFields;
//---------------------------------------------------------------------------
#endif
