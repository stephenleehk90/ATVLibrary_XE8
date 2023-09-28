//---------------------------------------------------------------------------
#ifndef findH
#define findH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmFind : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblMaterialID;
	TEdit *edtMaterialID;
	TButton *btnFind;
	TButton *btnClose;
    TButton *btnClear;
	TListView *lstvFind;
	void __fastcall btnFindClick(TObject *Sender);
    
    void __fastcall btnClearClick(TObject *Sender);
    
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmFind(TComponent* Owner);
    void __fastcall StartFind();
};
//---------------------------------------------------------------------------
extern TfrmFind *frmFind;
//---------------------------------------------------------------------------
#endif
