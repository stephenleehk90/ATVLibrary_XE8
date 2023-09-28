//---------------------------------------------------------------------------
#ifndef DestTbFrmH
#define DestTbFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmDestTbs : public TForm
{
__published:	// IDE-managed Components
    TTreeView *trvwTables;
    void __fastcall FormDeactivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmDestTbs(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDestTbs *frmDestTbs;
//---------------------------------------------------------------------------
#endif
