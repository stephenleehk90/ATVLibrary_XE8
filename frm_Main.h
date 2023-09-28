//---------------------------------------------------------------------------
#ifndef frm_MainH
#define frm_MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "hyiedefs.hpp"
#include "hyieutils.hpp"
#include "iesettings.hpp"
#include "ieview.hpp"
#include "iexBitmaps.hpp"
#include "iexLayers.hpp"
#include "iexRulers.hpp"
#include "iexToolbars.hpp"
#include "iexUserInteractions.hpp"
#include <ComCtrls.hpp>
//#include "AdvGrid.hpp"
//#include "AdvObj.hpp"
//#include "AdvUtil.hpp"
//#include "BaseGrid.hpp"
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "frxClass.hpp"
#include <System.ImageList.hpp>
#include "InspectorBar.hpp"

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *stbStory;
	TMainMenu *MainMenu1;
	TMenuItem *Go1;
	TMenuItem *Settin1;
	TMenuItem *Help1;
	TMenuItem *UserAdmin1;
	TMenuItem *Users1;
	TMenuItem *UserGroups1;
	TMenuItem *Wo1;
	TMenuItem *Maintenance1;
	TMenuItem *LogBrowse1;
	TMenuItem *DataTransfer1;
	TMenuItem *SystemSettings1;
	TMenuItem *LendReturn1;
	TMenuItem *Lend1;
	TMenuItem *Return1;
	TMenuItem *Browse1;
	TMenuItem *Material1;
	TMenuItem *MaterialRegistration1;
	TMenuItem *MaterialBrowse1;
	TMenuItem *ShelfRegistration1;
	TMenuItem *N1;
	TMenuItem *StoryInput2;
	TMenuItem *WordBank1;
	TMenuItem *emplate1;
	TMenuItem *Exit1;
	TMenuItem *N2;
	TListView *ListView1;
	TImageList *imglstUserAdmin;
	TMenuItem *About1;
	TMenuItem *ChangePasswords1;
	TMenuItem *ShowModules1;
	TMenuItem *N3;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TInspectorBar *InspectorBar1;
	TInspectorBar *InspectorBar2;
	TImageList *ImageListToolBar;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall muiStoryClick(TObject *Sender);
	void __fastcall UserAdmin1Click(TObject *Sender);
	void __fastcall UserGroups1Click(TObject *Sender);
	void __fastcall Wo1Click(TObject *Sender);
	void __fastcall LogBrowse1Click(TObject *Sender);
	void __fastcall DataTransfer1Click(TObject *Sender);
	void __fastcall SystemSettings1Click(TObject *Sender);
	void __fastcall Lend1Click(TObject *Sender);
	void __fastcall Return1Click(TObject *Sender);
	void __fastcall Browse1Click(TObject *Sender);
	void __fastcall MaterialRegistration1Click(TObject *Sender);
	void __fastcall MaterialBrowse1Click(TObject *Sender);
	void __fastcall ShelfRegistration1Click(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall emplate1Click(TObject *Sender);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall ChangePasswords1Click(TObject *Sender);
	void __fastcall ListView1Click(TObject *Sender);
	void __fastcall ShowModules1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall InspectorBar1ItemClick(TObject *Sender, TInspectorPanel *AInspectorPanel,
          TInspectorItem *AInspectorItem);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

private:	// User declarations
	void __fastcall CreateChildForm( const AnsiString childName );

public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
