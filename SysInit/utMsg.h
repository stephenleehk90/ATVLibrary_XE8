//---------------------------------------------------------------------------
#ifndef utMsgH
#define utMsgH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include "RXCtrls.hpp"
//---------------------------------------------------------------------------
class TfmMsg : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TRxLabel *rxlMsg;
	TImage *Image1;
private:	// User declarations
public:		// User declarations
	__fastcall TfmMsg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfmMsg *fmMsg;
//---------------------------------------------------------------------------
#endif
