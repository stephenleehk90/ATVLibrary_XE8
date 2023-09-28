//---------------------------------------------------------------------------

#ifndef QueryFrmLogH
#define QueryFrmLogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmQueryLog : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblIndicator;
	TGroupBox *grbDate;
	TLabel *lblFrom;
	TLabel *lblTo;
	TDateTimePicker *dtpBegin;
	TDateTimePicker *dtpEnd;
	TCheckBox *chkOpID;
	TEdit *edtOpID;
	TButton *btnCancel;
	TButton *btnOk;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall chkOpIDClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmQueryLog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmQueryLog *frmQueryLog;
//---------------------------------------------------------------------------
#endif
