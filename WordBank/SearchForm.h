//---------------------------------------------------------------------------
#ifndef SearchFormH
#define SearchFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSearch : public TForm
{
__published:	// IDE-managed Components
    TEdit *edSearch;
    TButton *btOK;
    TButton *btCancel;
    TLabel *Label1;void __fastcall edSearchKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmSearch(TComponent* Owner);

    AnsiString Execute(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSearch *frmSearch;
//---------------------------------------------------------------------------
#endif
