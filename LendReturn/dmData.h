//---------------------------------------------------------------------------
#ifndef dmDataH
#define dmDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <Db.hpp>
#include <vcl.h>
#include <comctrls.hpp>
//---------------------------------------------------------------------------
class TdmLendDM : public TDataModule
{
__published:	// IDE-managed Components
	TQuery *daqUser;
	TQuery *daqResource;
	TQuery *daqLendInfo;
	TQuery *daqQuery;
	TQuery *daqTime;
	TQuery *daqNewID;
	TQuery *daqLog;
	TQuery *daqGroup;
    TQuery *daqLendDays;
	TDatabase *dbMain;
private:	// User declarations
public:		// User declarations
	__fastcall TdmLendDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TdmLendDM *dmLendDM;
extern TStatusBar* sbStatus;

//---------------------------------------------------------------------------
#endif
