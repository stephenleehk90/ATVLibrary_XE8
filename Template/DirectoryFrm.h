//---------------------------------------------------------------------------
#ifndef DirectoryFrmH
#define DirectoryFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmDirectory : public TForm
{
__published:	// IDE-managed Components
    TTreeView *trvwStory;
    void __fastcall FormDeactivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmDirectory(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDirectory *frmDirectory;
//---------------------------------------------------------------------------
#endif
