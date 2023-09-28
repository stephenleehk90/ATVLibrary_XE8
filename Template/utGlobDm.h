//---------------------------------------------------------------------------
#ifndef utGlobDmH
#define utGlobDmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <comctrls.hpp>
//---------------------------------------------------------------------------
class TdmTemplate : public TDataModule
{
__published:	// IDE-managed Components
    TTable *tbCategory;
    TQuery *qeGlobal;
    TTable *tbUser;
    TTable *tbGroupUser;
    TTable *tbPermission;
    TTable *tbGroup;
    TTable *tbBaseFields;
    TTable *tbExtFields;
    TTable *tbTemp;
	TDatabase *dbMain;
private:	// User declarations
public:		// User declarations
    TStatusBar * m_stbTemplate;
    __fastcall TdmTemplate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmTemplate *dmTemplate;
//---------------------------------------------------------------------------
#endif
