//---------------------------------------------------------------------------
#ifndef utdmglobH
#define utdmglobH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\DB.hpp>
#include <vcl\DBTables.hpp>
#include <Db.hpp>
//---------------------------------------------------------------------------
class TdmGlobal : public TDataModule
{
__published:	// IDE-managed Components
	TDatabase *dbGlobal;
	TQuery *qeGlobal;
	TTable *tbUser;
	TTable *tbGroup;
	TTable *tbGroupUser;
	TTable *tbSysConfig;
	TTable *tbResTypes;
	TDatabase *dbMaster;
	TQuery *qeMaster;
    TDatabase *dbSearch;
    TQuery *qeSearch;
private:	// User declarations
public:		// User declarations
	__fastcall TdmGlobal(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TdmGlobal *dmGlobal;
//---------------------------------------------------------------------------
#endif
