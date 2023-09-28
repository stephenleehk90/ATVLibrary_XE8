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
    TGroupBox *grbDate;
    TLabel *lblFrom;
    TLabel *lblTo;
    TDateTimePicker *dtpBegin;
    TDateTimePicker *dtpEnd;
    TCheckBox *chkOpID;
    TEdit *edtOpID;
    TButton *btnCancel;
    TButton *btnOk;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall chkOpIDClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmQuery(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmQuery *frmQuery;
//---------------------------------------------------------------------------
#endif
