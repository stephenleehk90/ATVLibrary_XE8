//---------------------------------------------------------------------------
#ifndef InitFormH
#define InitFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmInit : public TForm
{
__published:	// IDE-managed Components
    TButton *btOk;
    TButton *btCancel;
    TEdit *edtServer;
	TEdit *edtLogin;
    TLabel *Label1;
    TLabel *Label2;
	TLabel *Label3;
	TEdit *edtPass;
	TLabel *Label4;
	TLabel *Label5;
    void __fastcall btOkClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmInit(TComponent* Owner);
	AnsiString strODBC, strUser, strPass;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmInit *frmInit;
//---------------------------------------------------------------------------
#endif
