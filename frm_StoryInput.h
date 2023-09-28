//---------------------------------------------------------------------------

#ifndef frm_StoryInputH
#define frm_StoryInputH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <System.ImageList.hpp>
#include "TMSSpellCheck.hpp"
#include "TMSSpellCheckCorrectLinesForm.hpp"
#include "AdvListV.hpp"
//#include "TMSSpellCheck.hpp"
//#include "TMSSpellCheckCorrectForm.hpp"
//#include "TMSSpellCheckCorrectLinesForm.hpp"
#ifndef htmlformH
#include "htmlform.h"
#endif
#ifndef VTRH
#include "VTR.h"
#endif
extern THtmlForm* html;
extern CVTR* pVTR;
//---------------------------------------------------------------------------
class TfrmStoryInput : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *stbStory;
	TMainMenu *mnuStory;
	TMenuItem *muiEdit;
	TMenuItem *muiEdit_Add;
	TMenuItem *muiEdit_Modify;
	TMenuItem *muiEdit_Delete;
	TMenuItem *N5;
	TMenuItem *Modify1;
	TMenuItem *muiStory;
	TMenuItem *muiPrint;
	TMenuItem *N2;
	TMenuItem *muiBrowseTime;
	TMenuItem *muiBrowseID;
	TMenuItem *N3;
	TMenuItem *ChangeMaterialID1;
	TMenuItem *ImportFromTBSNews1;
	TMenuItem *N8;
	TMenuItem *Refresh1;
	TMenuItem *muiOption;
	TMenuItem *muiOption_Archive;
	TMenuItem *N4;
	TMenuItem *RefreshTree1;
	TMenuItem *N7;
	TMenuItem *muiStory_Only;
	TPopupMenu *mnuPopStory;
	TMenuItem *muiPopAdd;
	TMenuItem *muiPopModify;
	TMenuItem *muiPopDelete;
	TMenuItem *N1;
	TMenuItem *muiPopTime;
	TMenuItem *muiPopID;
	TMenuItem *N6;
	TMenuItem *muiPopRefresh;
	TImageList *imglstStory;
	TNotebook *nbkStoryInput;
	TSplitter *Splitter1;
	TTreeView *trvwStory;
	TToolBar *ToolBar1;
	TToolButton *tbbtnAdd;
	TToolButton *tbbtnModify;
	TToolButton *tbbtnDelete;
	TToolButton *tbbtnSearch;
	TToolButton *tbbtnRefresh;
	TToolButton *ToolButton1;
	TLabel *lblFrom;
	TLabel *lblBegin;
	TLabel *lblTo;
	TLabel *lblEnd;
	TPanel *Panel2;
	TScrollBox *ScrollBox1;
	TPanel *Panel3;
	TButton *btnStoryAdd;
	TButton *btnClear;
	TButton *Button1;
	TButton *Button2;
	TPanel *pnlList;
	TAdvSpellCheck *AdvSpellCheck1;
	TAdvSpellCheckCorrectLinesDialog *AdvSpellCheckCorrectLinesDialog1;
	TAdvListView *lstvwStory;
	void __fastcall Capture(TObject *Sender);
	void __fastcall TimeCode(TObject *Sender);
	bool __fastcall ShowStoryNum();
	void __fastcall Refresh1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall tbbtnModifyClick(TObject *Sender);
	void __fastcall tbbtnDeleteClick(TObject *Sender);
	void __fastcall tbbtnAddClick(TObject *Sender);
	void __fastcall lstvwStoryKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall muiBrowseTimeClick(TObject *Sender);
	void __fastcall muiBrowseIDClick(TObject *Sender);
	void __fastcall btnStoryAddClick(TObject *Sender);
	void __fastcall btnCheckClick(TObject *Sender);
	void __fastcall lstvwStoryChange(TObject *Sender, TListItem *Item, TItemChange Change);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall tbbtnSearchClick(TObject *Sender);
	void __fastcall trvwStoryChange(TObject *Sender, TTreeNode *Node);
	void __fastcall ChangeMaterialID1Click(TObject *Sender);
	void __fastcall ImportFromTBSNews1Click(TObject *Sender);
	void __fastcall lstvwStoryMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall muiStory_OnlyClick(TObject *Sender);
	void __fastcall muiPrintClick(TObject *Sender);
	void __fastcall RefreshTree1Click(TObject *Sender);
	void __fastcall muiOption_ArchiveClick(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall lstvwStoryColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lstvwStoryCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);






private:	// User declarations
public:		// User declarations
	__fastcall TfrmStoryInput(TComponent* Owner);

	bool SetRowCount(int nCount);
	int m_nAdded;
	TStringList *strlstFieldName,*strlstFieldType;
	AnsiString strCurrentNodeID,strMaterialID,strStoryID,strPasteNodeID,strDate;
	AnsiString strOpID;
	TDateTime BeginDate,EndDate;
	bool bModifySearched;
	bool m_bRefreshTree,m_bRefreshStory;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStoryInput *frmStoryInput;
extern AnsiString strCurrentNodeID;
//---------------------------------------------------------------------------
#endif
