//---------------------------------------------------------------------------
#ifndef UserDataH
#define UserDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <comctrls.hpp>
//---------------------------------------------------------------------------
class TdmdUserAdmin : public TDataModule
{
__published:	// IDE-managed Components
    TQuery *daqUsers;
    TQuery *daqGroups;
    TQuery *daqCheck;
    TQuery *daqLogs;
	TDatabase *dbMain;
private:	// User declarations
public:		// User declarations
    __fastcall TdmdUserAdmin(TComponent* Owner);
    TStatusBar* stbUserAdmin;    
};
//---------------------------------------------------------------------------
extern PACKAGE TdmdUserAdmin *dmdUserAdmin;
//---------------------------------------------------------------------------
#endif
