//---------------------------------------------------------------------------
#ifndef lendmainH
#define lendmainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include <System.ImageList.hpp>
#include "AdvListV.hpp"
//---------------------------------------------------------------------------
class TfrmLendSys : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *mmMenu;
	TNotebook *ntbMain;
	TNotebook *ntbLend;
	TPanel *Panel1;
	TPanel *Panel2;
	TButton *btnReturnConfirm;
	TButton *btnReturnDiscard;
	TButton *btnReturnDelete;
	TGroupBox *grbLend;
	TLabel *lblLendMaterial;
	TEdit *edtLendMaterial;
	TLabel *lblLendUser;
	TButton *btnLendAdd;
	TButton *btnLendConfirm;
	TButton *btnLendDiscard;
	TButton *btnLendDelete;
	TGroupBox *grbReturn;
	TLabel *Label1;
	TEdit *edtReturnMaterial;
	TButton *btnReturnAdd;
	TImageList *imglRecord;
	TPopupMenu *pmLend;
	TMenuItem *muiLendDelete;
	TMenuItem *muiLendConfirm;
	TMenuItem *muiLendDiscard;
	TPopupMenu *pmReturn;
	TMenuItem *muiReturnConfirm;
	TMenuItem *muiReturnDiscard;
	TMenuItem *muiReturnDelete;
	TPopupMenu *pmQuery;
	TMenuItem *muiNormal;
	TMenuItem *muiBrowseOutOfTime;
	TComboBox *cmbMode;
	TLabel *lblMode;
	TMenuItem *muiLendOption;
	TMenuItem *N2;
	TMenuItem *muiLendFind;
	TMenuItem *N3;
	TMenuItem *muiBrowseFind;
	TMenuItem *muiBrowseOption;
	TMenuItem *N4;
	TMenuItem *muiReturnFind;
	TMenuItem *muiReturnOption;
	TLabel *lblLendRemark;
	TEdit *edtLendRemark;
	TLabel *lblReturnLabel;
	TEdit *edtReturnRemark;
	TButton *btnReturnClear;
	TButton *btnLendClear;
	TEdit *edtLendUser;
	TMenuItem *Operation1;
	TMenuItem *muiPreview1;
	TMenuItem *muiPrint1;
	TMenuItem *muiFind1;
	TMenuItem *muiOptions1;
	TMenuItem *N8;
	TMenuItem *muiRefresh;
	TMenuItem *muiBrowseRefresh;
    TMenuItem *muiUnLost;
    TMenuItem *muiBrowseNotReturn;
    TMenuItem *ItemPrint;
    TMenuItem *N1;
    TMenuItem *N5;
    TMenuItem *muiBrowseMID;
    TMenuItem *BrowsebyLosted1;
    TMenuItem *N6;
    TMenuItem *N7;
    TMenuItem *UserInformation1;
    TMenuItem *itemPrintPreview;
    TMenuItem *muiLost;
	TListView *lstvLend;
	TListView *lstvReturn;
	TPanel *Panel3;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TComboBox *cmbQueryMode;
	TLabel *Label2;
	TLabel *lblUserID;
	TEdit *edpage;
	TButton *Button1;
	TButton *Button2;
	TImageList *imglstUserAdmin;
	TfrxReport *frxReport1;
	TfrxDBDataset *frxDBDataset1;
	TfrxReport *frxReport2;
	TImageList *ImageListStatus;
	TAdvListView *lstvQuery;
    void __fastcall btnLendAddClick(TObject *Sender);
	void __fastcall btnLendDiscardClick(TObject *Sender);
	void __fastcall btnLendDeleteClick(TObject *Sender);
	void __fastcall btnLendConfirmClick(TObject *Sender);
	void __fastcall btnReturnAddClick(TObject *Sender);
	void __fastcall btnReturnDeleteClick(TObject *Sender);
	void __fastcall btnReturnDiscardClick(TObject *Sender);
	void __fastcall btnReturnConfirmClick(TObject *Sender);
	void __fastcall cmbQueryModeChange(TObject *Sender);
	void __fastcall muiPreviewClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall muiOptionClick(TObject *Sender);

	void __fastcall nuiFindClick(TObject *Sender);
	void __fastcall muiNormalClick(TObject *Sender);
	void __fastcall muiBrowseOutOfTimeClick(TObject *Sender);
	void __fastcall muiPrintClick(TObject *Sender);

	void __fastcall muiLendFindClick(TObject *Sender);
	void __fastcall muiBrowseFindClick(TObject *Sender);
	void __fastcall muiReturnFindClick(TObject *Sender);
	void __fastcall lstvLendDblClick(TObject *Sender);
	void __fastcall btnReturnClearClick(TObject *Sender);
	void __fastcall btnLendClearClick(TObject *Sender);
	void __fastcall lstvLendMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall lstvReturnMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall lstvQueryDblClick(TObject *Sender);
	void __fastcall lstvReturnDblClick(TObject *Sender);
	void __fastcall muiRefreshClick(TObject *Sender);
	void __fastcall lstvLendKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall lstvReturnKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtLendMaterialKeyPress(TObject *Sender, char &Key);
    void __fastcall edtLendUserKeyPress(TObject *Sender, char &Key);
    void __fastcall edtReturnMaterialKeyPress(TObject *Sender, char &Key);
    void __fastcall muiUnLostClick(TObject *Sender);
    
    
    
    
    void __fastcall muiBrowseNotReturnClick(TObject *Sender);
    
    
    
    void __fastcall tbbtnUserClick(TObject *Sender);
    void __fastcall pmQueryPopup(TObject *Sender);
    void __fastcall BrowsebyLosted1Click(TObject *Sender);
    void __fastcall muiBrowseMIDClick(TObject *Sender);
    void __fastcall itemPrintPreviewClick(TObject *Sender);
    void __fastcall ItemPrintClick(TObject *Sender);
    void __fastcall muiLostClick(TObject *Sender);
    
    void __fastcall btGoClick(TObject *Sender);
	void __fastcall lstvQueryColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lstvQueryCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall ntbLendPageChanged(TObject *Sender);
	void __fastcall UserInformation1Click(TObject *Sender);

private:	// User declarations
    TDateTime   m_dtTimer;
	AnsiString  m_strCurrentTime,m_CaptionHead,m_CaptionDescription;
	void print_Report(bool b_Preview);
public:		// User declarations
    __fastcall TfrmLendSys(TComponent* Owner);
    void NormalQuery(AnsiString strUserID);
    void BrowseOutOfTime(AnsiString strUserID);
    void BrowseNotReturn(AnsiString strUserID);
    // updated 20027
    int  GetLimitNum(int &imaxpage,int &ipage, int &istart,int &iend);
    void DisplayResultHint(int imaxpage,int ipage, int istart,int iend);
    void LostQuery(AnsiString strUserID);
    bool GetOrderColumn(AnsiString &SQLClause,AnsiString &strOrderList);
//    void MaterailIDQuery(AnsiString strUserID);

    bool GetMaterialQuery(AnsiString &SQLClause, AnsiString strUserID);
    bool GetLostQuery(AnsiString &SQLClause, AnsiString strUserID);
    //
    bool GetNormalQuery(AnsiString &SQLClause, AnsiString strUserID);
    bool GetOutofTime(AnsiString &SQLClause, AnsiString strUserID);
    bool GetNotReturn(AnsiString &SQLClause, AnsiString strUserID);
    
    void __fastcall WriteINIFile();
    void __fastcall LendDelete();
    void __fastcall ReturnDelete();
    void __fastcall UpdateDays();
    void __fastcall BrowsePage(TObject * Sender);
};
//---------------------------------------------------------------------------
extern TfrmLendSys *frmLendSys;
//---------------------------------------------------------------------------
#endif

