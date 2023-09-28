//---------------------------------------------------------------------------
#ifndef gotopageH
#define gotopageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfmGotoPage : public TForm
{
__published:	// IDE-managed Components
    TEdit *edPage;
    TLabel *lbtotalrecords;
    TLabel *Label1;
    TButton *btGoto;
    TLabel *lbPer;
    TEdit *edrecord;
    TLabel *lbRecord;
private:	// User declarations
public:		// User declarations
    __fastcall TfmGotoPage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmGotoPage *fmGotoPage;
//---------------------------------------------------------------------------
#endif
