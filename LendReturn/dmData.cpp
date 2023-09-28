//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dmData.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TdmLendDM *dmLendDM;
TStatusBar* sbStatus;
//---------------------------------------------------------------------------
__fastcall TdmLendDM::TdmLendDM(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------

