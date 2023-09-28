//---------------------------------------------------------------------------
#ifndef LogDataH
#define LogDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <comctrls.hpp>

#define strSTORYLIBTREE "lib_StoryLibTree"
//---------------------------------------------------------------------------
class TdmdLog : public TDataModule
{
__published:	// IDE-managed Components
    TQuery *daqLogs;
    TQuery *daqKeywords;
    TQuery *daqResType;
    TQuery *daqCheck;
    TTable *tbCategory;
    TQuery *qeGlobal;
    TQuery *qeBaseFields;
    TQuery *qeExtFields;
    TTable *tbSource;
    TTable *tbDest;
    TDatabase *dbOldData;
    TTable *tbOldData;
    TTable *tbSysConfig;
    TQuery *qeSourceFields;
    TQuery *qeDestFields;
	TDatabase *dbMain;
private:	// User declarations
public:		// User declarations
    TStatusBar *stbLog;
    __fastcall TdmdLog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmdLog *dmdLog;
//---------------------------------------------------------------------------
#endif
