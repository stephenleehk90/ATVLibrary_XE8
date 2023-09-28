//---------------------------------------------------------------------------
#ifndef ConfigFrmH
#define ConfigFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "AdvGrid.hpp"
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmConfig : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblCategory;
    TButton *btnOk;
    TButton *btnCancel;
    TEdit *edtCategory;
    TListView *lstvwValues;
    TPopupMenu *mnuPopConfig;
    TMenuItem *muiPopInsert;
    TMenuItem *muiPopDelete;
    TMenuItem *muiPopRename;
    TMenuItem *N1;void __fastcall btnOkClick(TObject *Sender);
    
    void __fastcall lstvwValuesDblClick(TObject *Sender);
    
    
    void __fastcall FormShow(TObject *Sender);
    void __fastcall lstvwValuesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall lstvwValuesChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
    void __fastcall lstvwValuesColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall muiPopRenameClick(TObject *Sender);
    void __fastcall muiPopInsertClick(TObject *Sender);
    void __fastcall muiPopDeleteClick(TObject *Sender);
	void __fastcall lstvwValuesEdited(TObject *Sender, TListItem *Item, UnicodeString &S);

    
private:	// User declarations
    void DeleteValues();
public:		// User declarations
    AnsiString strKey;
    __fastcall TfrmConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmConfig *frmConfig;
//---------------------------------------------------------------------------
#endif
