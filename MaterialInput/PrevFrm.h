//---------------------------------------------------------------------------
#ifndef PrevFrmH
#define PrevFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmPrev : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlPrev;
    TBitBtn *btnHome;
    TBitBtn *btnPrev;
    TBitBtn *btnNext;
    TBitBtn *btnEnd;
    TBitBtn *btnZoomIn;
    TBitBtn *btnZoomOut;
    TBitBtn *btnClose;
    TBitBtn *btnSetup;
    TBitBtn *btnPrint;
    TPrintDialog *PrintDialog1;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    
    void __fastcall btnHomeClick(TObject *Sender);
    void __fastcall btnPrevClick(TObject *Sender);
    void __fastcall btnNextClick(TObject *Sender);
    void __fastcall btnEndClick(TObject *Sender);
    void __fastcall btnZoomInClick(TObject *Sender);
    void __fastcall btnZoomOutClick(TObject *Sender);
    void __fastcall btnSetupClick(TObject *Sender);
    void __fastcall btnPrintClick(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);


    
private:	// User declarations
public:		// User declarations
    __fastcall TfrmPrev(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPrev *frmPrev;
//---------------------------------------------------------------------------
#endif
