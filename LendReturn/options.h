//---------------------------------------------------------------------------
#ifndef optionsH
#define optionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "cspin.h"
//---------------------------------------------------------------------------
class TfrmOption : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TCSpinEdit *edtTapeDays;
    TCSpinEdit *edtBookDays;
    TCSpinEdit *edtMagazineDays;
    TCSpinEdit *edtFileDays;
    TCSpinEdit *edtQueryDays;
    TButton *btnDaysOK;
    TButton *btnDaysCancel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall spbQueryDaysUpClick(TObject *Sender);
	void __fastcall spbQueryDaysDownClick(TObject *Sender);
	void __fastcall btnDaysOKClick(TObject *Sender);

    void __fastcall edtTapeDaysKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtTapeDaysKeyPress(TObject *Sender, char &Key);
    
    void __fastcall edtBookDaysKeyPress(TObject *Sender, char &Key);
    void __fastcall edtMagazineDaysKeyPress(TObject *Sender, char &Key);
    void __fastcall edtFileDaysKeyPress(TObject *Sender, char &Key);
    void __fastcall edtQueryDaysKeyPress(TObject *Sender, char &Key);
private:	// User declarations
    void __fastcall SetDays();
public:		// User declarations
	__fastcall TfrmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfrmOption *frmOption;
//---------------------------------------------------------------------------
#endif
 