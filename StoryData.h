//---------------------------------------------------------------------------
#ifndef StoryDataH
#define StoryDataH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TdmdStory : public TDataModule
{
__published:	// IDE-managed Components
	TTable *tbExport;
    TTable *tbImport;
    TQuery *qryStoryID;
	TDatabase *dbMain;
	TTable *tbCategory;
	TTable *tbStory;
	TQuery *daqScript;
	TQuery *daqPrint;
	TQuery *daqFields;
	TQuery *daqCheck;
	TQuery *daqOption;
	TDatabase *dbTBSNews;
	TQuery *daqCheckTable;
private:	// User declarations
public:		// User declarations
	__fastcall TdmdStory(TComponent* Owner);
	AnsiString strCurrentUserID;
	int nGroupType;
};
//---------------------------------------------------------------------------
extern PACKAGE TdmdStory *dmdStory;
//---------------------------------------------------------------------------
#endif
