//---------------------------------------------------------------------------
#ifndef frmWordH
#define frmWordH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class TfrmWB : public TForm
{
__published:	// IDE-managed Components
    TDBGrid *dgdWord;
    TTable *tbWord;
    TDataSource *DataSource1;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TImageList *imglstUserAdmin;
	TDatabase *dbMain;
	TDBNavigator *DBNavigator1;
    void __fastcall dgdWordKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    
    void __fastcall bt97SearchClick(TObject *Sender);
    
    
    
    
    void __fastcall tbWordPostError(TDataSet *DataSet, EDatabaseError *E,
          TDataAction &Action);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmWB(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWB *frmWB;
//---------------------------------------------------------------------------
#endif
