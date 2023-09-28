//---------------------------------------------------------------------------
#ifndef RestypeFrmH
#define RestypeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "AdvGrid.hpp"
#include <Grids.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvUtil.hpp"
#include "BaseGrid.hpp"
//#include "AdvObj.hpp"
//#include "BaseGrid.hpp"
//---------------------------------------------------------------------------
class TfrmResType : public TForm
{
__published:	// IDE-managed Components
    TButton *btnAdd;
    TButton *btnDelete;
    TButton *btnCommit;
	TButton *btnReset;
    TButton *btnClose;
    TLabel *lblIndicator;
	TAdvStringGrid *strgrdResType;
	void __fastcall FormShow(TObject *Sender);
    void __fastcall btnCommitClick(TObject *Sender);
    void __fastcall btnAddClick(TObject *Sender);
    void __fastcall btnResetClick(TObject *Sender);
    void __fastcall btnDeleteClick(TObject *Sender);
    
    void __fastcall strgrdResTypeKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
    void __fastcall strgrdResTypeKeyPress(TObject *Sender, char &Key);
	void __fastcall strgrdResTypeCellValidate(TObject *Sender, int ACol, int ARow, UnicodeString &Value,
          bool &Valid);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmResType(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmResType *frmResType;
//---------------------------------------------------------------------------
#endif
