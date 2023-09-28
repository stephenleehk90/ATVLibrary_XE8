//---------------------------------------------------------------------------
#ifndef PreviewFrmH
#define PreviewFrmH
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
    void __fastcall qrdbtxtMaterialIDPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall qrdbtxtTypePrint(TObject *sender, AnsiString &Value);
    void __fastcall qrdbtxtTitlePrint(TObject *sender, AnsiString &Value);
    void __fastcall qrdbtxtPositionPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall qrdbtxtStatusPrint(TObject *sender, AnsiString &Value);
    void __fastcall qrdbtxtBorrowerPrint(TObject *sender,
          AnsiString &Value);
    void __fastcall qrpReportPreview(TObject *Sender);
    
private:	// User declarations
public:		// User declarations
    __fastcall TfrmPreview(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPreview *frmPreview;
//---------------------------------------------------------------------------
#endif
