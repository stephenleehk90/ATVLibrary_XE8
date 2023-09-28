//---------------------------------------------------------------------------
#ifndef QueryFrmH
#define QueryFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmQuery : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblIndicator;
	TLabel *lblBeginDate;
	TLabel *lblEndDate;
	TButton *btnOk;
	TButton *btnCancel;
    TDateTimePicker *dtpBegin;
    TDateTimePicker *dtpEnd;
	void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);


private:	// User declarations
public:		// User declarations
	__fastcall TfrmQuery(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfrmQuery *frmQuery;
//---------------------------------------------------------------------------
#endif
