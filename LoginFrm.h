//---------------------------------------------------------------------------
#ifndef LoginFrmH
#define LoginFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
__published:	// IDE-managed Components
	TEdit *edtLoginID;
	TEdit *edtPassword;
	TLabel *lblLoginID;
	TLabel *lblPassword;
	TImage *imgLogin;
	TButton *btnOk;
	TButton *btnCancel;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
public:		// User declarations
    int GetPrivilege(AnsiString strUserID);
    AnsiString strLoginID,strPassword,strDataBaseName,strDataBaseServer,strLoginIDOld,strPasswordOld;
    int nLoginTimes;
	__fastcall TfrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif
