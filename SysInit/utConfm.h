//---------------------------------------------------------------------------
#ifndef utConfmH
#define utConfmH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TfmConfirm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *edPassword;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfmConfirm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfmConfirm *fmConfirm;
//---------------------------------------------------------------------------
#endif
