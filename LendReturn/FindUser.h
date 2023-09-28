//---------------------------------------------------------------------------
#ifndef FindUserH
#define FindUserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmFindUser : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblUser;
    TEdit *edtUser;
    TButton *btnFind;
    TButton *btnClear;
    TButton *btnClose;
	TListView *lstvwUser;
    void __fastcall btnClearClick(TObject *Sender);

    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnFindClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmFindUser(TComponent* Owner);
    AnsiString strUserID;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFindUser *frmFindUser;
//---------------------------------------------------------------------------
#endif
