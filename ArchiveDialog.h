//---------------------------------------------------------------------------
#ifndef ArchiveDialogH
#define ArchiveDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmArchive : public TForm
{
__published:	// IDE-managed Components
    TButton *btnOK;
    TLabel *Label1;
    TProgressBar *prgIndexInfo;
    TTimer *tmrIndexInfo;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall tmrIndexInfoTimer(TObject *Sender);
    
private:	// User declarations
public:		// User declarations
    __fastcall TfrmArchive(TComponent* Owner);
    void ShowIndexInfo();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmArchive *frmArchive;
//---------------------------------------------------------------------------
#endif
