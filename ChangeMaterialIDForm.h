//---------------------------------------------------------------------------
#ifndef ChangeMaterialIDFormH
#define ChangeMaterialIDFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TfrmChangeM : public TForm
{
__published:	// IDE-managed Components
    TEdit *edtOldID;
    TEdit *edtNewID;
    TLabel *Label1;
    TLabel *Label2;
    TButton *btnOk;
    TButton *btnCancel;
    TQuery *qryChange;void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmChangeM(TComponent* Owner);

    AnsiString strLib;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmChangeM *frmChangeM;
//---------------------------------------------------------------------------
#endif
