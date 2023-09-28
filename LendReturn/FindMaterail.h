//---------------------------------------------------------------------------
#ifndef FindMaterailH
#define FindMaterailH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmMaterial : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblMaterialID;
    TEdit *edtMaterialID;
    TButton *btnFind;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmMaterial(TComponent* Owner);
    //
    inline __fastcall AnsiString GetMaterialID(){return edtMaterialID->Text;};

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMaterial *frmMaterial;
//---------------------------------------------------------------------------
#endif
