//---------------------------------------------------------------------------
#ifndef MaterialFrmH
#define MaterialFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include <System.ImageList.hpp>
#include "AdvListV.hpp"
//---------------------------------------------------------------------------
class TfrmMaterials : public TForm
{
__published:	// IDE-managed Components
    TNotebook *nbkMaterials;
	TPanel *pnlMaterialAdd;
    TButton *btnDiscard;
	TButton *btnConfirm;
    TGroupBox *grbMaterialAdd;
    TLabel *lblMaterialID;
    TLabel *lblAddType;
    TLabel *lblMaterialPosition;
    TLabel *lblMaterialTitle;
    TEdit *edtMaterialID;
    TComboBox *cboMaterialType;
    TEdit *edtMaterialPos;
    TEdit *edtMaterialTitle;
    TButton *btnAdd;
	TPanel *pnlMaterialEdit;
	TButton *btnMaterialOk;
	TButton *btnMaterialCancel;
	TGroupBox *grbMaterialBrowse;
	TLabel *lblResID;
	TLabel *lblPosition;
	TLabel *lblMaterialRemark;
	TLabel *lblResType;
	TLabel *lblState;
	TComboBox *cboResType;
	TComboBox *cboState;
	TPanel *pnlMaterialPos;
	TGroupBox *grbMaterialPos;
	TLabel *lblPosFile;
    TButton *btnFile;
    TButton *btnInput;
    TButton *btnPosConfirm;
    TButton *btnPosDiscard;
    TFontDialog *dlgFont;
    TOpenDialog *dlgPosFile;
    TMainMenu *mnuMaterial;
    TMenuItem *muiEdit;
    TMenuItem *muiEdit_Modify;
    TMenuItem *muiEdit_Delete;
    TMenuItem *muiMaterial;
    TMenuItem *muiBrowse;
    TMenuItem *muiBrowseShelf;
    TMenuItem *N2;
    TMenuItem *muiPreview;
    TPopupMenu *popMaterial;
    TMenuItem *muiPopModify;
    TMenuItem *N6;
    TMenuItem *muiPopDelete;
    TMenuItem *N7;
    TMenuItem *muiPopRefresh;
    TEdit *edtResRemark;
    TEdit *edtPosition;
	TEdit *edtResID;
    TEdit *edtPosFile;
	TMenuItem *muiPrint;
	TMenuItem *muiRefresh;
	TMenuItem *muiPopTime;
	TMenuItem *muiPopShelf;
	TComboBox *cboInputType;
	TListView *lstvwMaterialAdd;
	TListView *lstvwMaterialPos;
	TEdit *edpage;
	TImageList *imglstUserAdmin;
	TToolBar *ToolBar1;
	TToolButton *tbbtnRefresh;
	TLabel *lblShelfID;
	TLabel *lblBeginDate;
	TLabel *lblEndDate;
	TLabel *lblFrom;
	TLabel *lblTo;
	TLabel *lblShelf;
	TfrxReport *frxReport1;
	TfrxDBDataset *frxDBDataset1;
	TfrxReport *frxReport2;
	TAdvListView *lstvwMaterials;
	void __fastcall cboInputTypeChange(TObject *Sender);
	void __fastcall muiBrowseClick(TObject *Sender);
	void __fastcall muiBrowseShelfClick(TObject *Sender);

	void __fastcall muiPreviewClick(TObject *Sender);
	void __fastcall muiEdit_ModifyClick(TObject *Sender);
	void __fastcall btnMaterialOkClick(TObject *Sender);
	void __fastcall lstvwMaterialsChange(TObject *Sender, TListItem *Item,
		  TItemChange Change);
	void __fastcall btnMaterialCancelClick(TObject *Sender);
	void __fastcall btnFileClick(TObject *Sender);
	void __fastcall btnInputClick(TObject *Sender);
	void __fastcall btnPosConfirmClick(TObject *Sender);
	void __fastcall btnPosDiscardClick(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnConfirmClick(TObject *Sender);
	void __fastcall btnDiscardClick(TObject *Sender);
	void __fastcall lstvwMaterialsClick(TObject *Sender);
	void __fastcall lstvwMaterialAddKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
	void __fastcall lstvwMaterialPosChange(TObject *Sender,
		  TListItem *Item, TItemChange Change);
	void __fastcall lstvwMaterialsKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
	void __fastcall lstvwMaterialsDblClick(TObject *Sender);
	void __fastcall muiPopModifyClick(TObject *Sender);
	void __fastcall muiPopDeleteClick(TObject *Sender);
	void __fastcall muiPopRefreshClick(TObject *Sender);
	void __fastcall WndProc(TMessage &Message);
	void __fastcall edtMaterialIDKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);




	void __fastcall muiPrintClick(TObject *Sender);

	void __fastcall btGoClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall lstvwMaterialsColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lstvwMaterialsCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall nbkMaterialsPageChanged(TObject *Sender);

private:	// User declarations
	void print_Report(bool b_Preview);
public:		// User declarations
	__fastcall TfrmMaterials(TComponent* Owner);
	TDateTime BeginDate,EndDate;
	AnsiString strUserID,strResID,strRemark;
	TStringList *strlstResType;
	AnsiString strPosFileName;
	TStatusBar* stbMaterial;
    bool bEditMaterial;
    void BrowsePage();
    void RegistrationPage();
    void PositionPage();
    void ModifyRes();
    bool CheckMaterial(AnsiString strMaterialID);
    void DeleteMaterial();
    void DeleteRes();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMaterials *frmMaterials;
//---------------------------------------------------------------------------
#endif
