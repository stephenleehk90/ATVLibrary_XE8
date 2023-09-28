//---------------------------------------------------------------------------
#ifndef BrowseTimeFrmH
#define BrowseTimeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmBrowseTime : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblBeginDate;
    TLabel *lblEndDate;
    TButton *btnOk;
    TButton *btnCancel;
    TDateTimePicker *dtpBegin;
    TDateTimePicker *dtpEnd;
    TLabel *lblIndicator;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    
private:	// User declarations
public:		// User declarations
    __fastcall TfrmBrowseTime(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBrowseTime *frmBrowseTime;
//---------------------------------------------------------------------------
#endif
  