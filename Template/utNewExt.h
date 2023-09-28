//---------------------------------------------------------------------------
#ifndef utNewExtH
#define utNewExtH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TfmEditExtFields : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *edFieldName;
    TLabel *Label2;
    TComboBox *cbbFieldType;
    TLabel *Label3;
    TMaskEdit *meFieldSize;
    TBitBtn *btnFunction;
    TBitBtn *BitBtn2;
    void __fastcall btnFunctionClick(TObject *Sender);
    void __fastcall cbbFieldTypeChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    int m_nEditState;
    
    __fastcall TfmEditExtFields(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmEditExtFields *fmEditExtFields;
//---------------------------------------------------------------------------
#endif
