//---------------------------------------------------------------------------
#ifndef SourceTbFrmH
#define SourceTbFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TfrmSourceTbs : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblTable;
    TComboBox *cbbSourceTable;
    TLabel *lblDSN;
    TButton *btnOk;
    TButton *btnCancel;
    TEdit *edSourcePath;
    TButton *btnBrowse;
    TOpenDialog *OpenDialog1;
    void __fastcall btnBrowseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
    AnsiString browseForFolder(HWND hParentWnd);
public:		// User declarations
    __fastcall TfrmSourceTbs(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSourceTbs *frmSourceTbs;
//---------------------------------------------------------------------------
#endif
