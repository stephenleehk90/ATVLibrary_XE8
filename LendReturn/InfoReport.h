//---------------------------------------------------------------------------
#ifndef InfoReportH
#define InfoReportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmPreview : public TForm
{
__published:	// IDE-managed Components
    void __fastcall QRDBTextMaterialIDPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall QRDBTextLendOpIDPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall QRDBTextBorrowIDPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall QRDBTextReturnOpIDPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall QRDBTextRemarkPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall qrpLendInfoPreview(TObject *Sender);
    
    
    
    void __fastcall QRDBTextReturnTimePrint(TObject *sender,
          AnsiString &Value);
    
    
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPreview(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfrmPreview *frmPreview;
//---------------------------------------------------------------------------
#endif
