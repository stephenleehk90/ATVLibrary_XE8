//---------------------------------------------------------------------------
#ifndef MaterialDataH
#define MaterialDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TdmdMaterials : public TDataModule
{
__published:	// IDE-managed Components
    TQuery *daqPrint;
    TQuery *daqMaterials;
    TQuery *daqCheck;
    TQuery *daqLogs;
	TDatabase *dbMain1;
private:	// User declarations
public:		// User declarations
    __fastcall TdmdMaterials(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmdMaterials *dmdMaterials;
//---------------------------------------------------------------------------
#endif
