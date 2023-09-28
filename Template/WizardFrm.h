//---------------------------------------------------------------------------
#ifndef WizardFrmH
#define WizardFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "EditBtn.hpp"
//#include "AdvGrid.hpp"
#include <Db.hpp>
#include <DBTables.hpp>
//#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Graphics.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvUtil.hpp"
//---------------------------------------------------------------------------
class TfrmWizard : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlWizard;
    TImage *imgWizard;
    TButton *btnBack;
    TButton *btnNext;
    TButton *btnCancel;
    TBevel *bvlWizard;
	TNotebook *nbkWizard;
	TGroupBox *grbNode;
	TLabel *lblDirectory;
	TLabel *lblNodeName;
	TComboBox *cboDirectory;
	TEdit *edtNodeName;
	TGroupBox *grbNodeType;
	TCheckBox *chbInherit;
	TCheckBox *chkType;
	TMemo *Memo2;
	TAdvStringGrid *strgrdWizard;
	TPanel *pnlTemplate;
	TLabel *lblInput;
	TLabel *lblBrowse;
	TLabel *lblAdvanced;
	TLabel *lblSimple;
	TLabel *Label2;
	TEdit *edtInput;
	TEdit *edtBrowse;
	TEdit *edtSearch;
	TEdit *edtAdvance;
	TButton *btnInput;
	TButton *btnBrowse;
	TButton *btnSearch;
	TButton *btnAdvance;
	TEdit *edtList;
	TButton *Button1;
	TMemo *Memo3;
	TMemo *Memo1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnBackClick(TObject *Sender);
    void __fastcall btnNextClick(TObject *Sender);

	void __fastcall strgrdWizardGetEditorType(TObject *Sender, int aCol,
		  int aRow, TEditorType &aEditor);
    void __fastcall strgrdWizardCanEditCell(TObject *Sender, int Arow,
          int Acol, bool &canedit);
    
    void __fastcall btnInputClick(TObject *Sender);
	void __fastcall btnBrowseClick(TObject *Sender);
    void __fastcall btnSearchClick(TObject *Sender);
    void __fastcall btnAdvanceClick(TObject *Sender);
    void __fastcall edtNodeNameChange(TObject *Sender);

	void __fastcall cboDirectoryDropDown(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
    
     void __fastcall cboDirectoryChange(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall strgrdWizardClick(TObject *Sender);
	void __fastcall strgrdWizardKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall strgrdWizardKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall strgrdWizardCellValidate(TObject *Sender, int ACol, int ARow, UnicodeString &Value,
          bool &Valid);
	void __fastcall strgrdWizardGetEditMask(TObject *Sender, int ACol, int ARow, UnicodeString &Value);


private:	// User declarations
     int MatchField(AnsiString);
     bool CheckHtmlValid();
public:		// User declarations
    bool CheckFields(AnsiString);
    __fastcall TfrmWizard(TComponent* Owner);
    bool CheckType(AnsiString strType);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWizard *frmWizard;
//---------------------------------------------------------------------------
#endif
