//---------------------------------------------------------------------------
#ifndef utGlDmH
#define utGlDmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TdmGlobal : public TDataModule
{
__published:	// IDE-managed Components
	TTable *tbCategory;
private:	// User declarations
public:		// User declarations
    __fastcall TdmGlobal(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TdmGlobal *dmGlobal;
//---------------------------------------------------------------------------
#endif
