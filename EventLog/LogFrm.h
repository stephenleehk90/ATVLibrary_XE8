//---------------------------------------------------------------------------
#ifndef LogFrmH
#define LogFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
//#include "AdvGrid.hpp"
#include "EditBtn.hpp"
#include <Db.hpp>
#include <DBTables.hpp>
#include <Mask.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <System.ImageList.hpp>
#include "AdvListV.hpp"

#define GLOBALMSGCAPTION    L"TBS Library"
#define NODEIDSIZE          32
#define DATABASEACCESSERROR L"Database Access Error"

#define strLIBBASEFIELDS   "lib_BaseFields"
#define strLIBEXTENDFIELDS "lib_ExtendFields"
#define strSTORYLIBTREE    "lib_StoryLibTree"
#define strSYSCONFIG       "lib_SysConfig"
#define strPERMISSION      "lib_Permission"

#define typeVARCHAR	  "varchar"
#define typeINT	      "int"
#define typeTEXT	  "text"
#define typeDATETIME  "datetime"
#define typeIMAGE	  "image"

#define FIELDSTORYID    "StoryID"
#define FIELDWRITETIME  "WriteTime"
#define FIELDMATERIALID "MaterialID"
#define FIELDDateID     "Date"

#define UPDATESTORYPROC "UpdateStoryLibrary"

#define LANGUAGE_BIG5 "B5"
#define LANGUAGE_GB   "GB"
//---------------------------------------------------------------------------
class TfrmLogs : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *mnuLogs;
    TMenuItem *muiLog;
    TMenuItem *muiLogQuery;
    TMenuItem *muiExpire;
    TMenuItem *N2;
    TMenuItem *muiClearLog;
    TNotebook *nbkDatabase;
    TPanel *pnlDest;
    TGroupBox *grbDestinate;
    TLabel *lblDestTb;
    TEdit *edtDestTb;
    TButton *btnDestTbs;
    TGroupBox *grbSource;
    TListView *lstvwFields;
    TLabel *lblSourceTb;
    TEdit *edtSourceTb;
    TButton *btnSourceTbs;
    TCheckBox *chkOld;
    TPopupMenu *mnuPopKey;
    TMenuItem *muiPopNew;
    TMenuItem *muiPopModify;
    TMenuItem *muiPopDelete;
    TMenuItem *N1;
    TButton *btnTransfer;
    TPopupMenu *mnuAssociate;
    TMenuItem *MenuItem1;
    TMenuItem *MenuItem2;
    TQuery *qeInternal;
    TGroupBox *GroupBox1;
    TButton *btnPerform;
    TGroupBox *GroupBox2;
    TPanel *pnCharSet;
    TCheckBox *ckbCheckIntegrity;
    TCheckBox *ckbClearEventLog;
    TMenuItem *N3;
    TMenuItem *muiRefresh;
	TLabel *lblLogBegin;
	TLabel *lblLogEnd;
	TLabel *lblOpID;
	TImageList *imglstUserAdmin;
	TToolBar *ToolBar1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TAdvListView *lstvwLogs;
	TLabel *lblDBName;
	void __fastcall FormShow(TObject *Sender);
    void __fastcall muiLogQueryClick(TObject *Sender);
    void __fastcall muiExpireClick(TObject *Sender);
    void __fastcall muiClearLogClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);

    void __fastcall WndProc(TMessage &Message);
    
    
    
    
    
    void __fastcall btnDestTbsMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    
    void __fastcall btnSourceTbsMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnTransferClick(TObject *Sender);
    void __fastcall MenuItem1Click(TObject *Sender);
    void __fastcall MenuItem2Click(TObject *Sender);

    void __fastcall btnPerformClick(TObject *Sender);
    void __fastcall chkOldClick(TObject *Sender);
    void __fastcall tbbtnRefreshClick(TObject *Sender);

    
private:	// User declarations
	AnsiString m_strDatabaseName,m_strNodeType;
	void OrganizeFieldsSQLClause(AnsiString  strNodeID,
								 AnsiString& strBaseFieldsSQLClause,
								 AnsiString& strExtFieldsSQLClause,
								 AnsiString& strBaseFieldsIndexClause,
								 AnsiString& strExtFieldsIndexClause);
	void UpdateStoryLibrary();
	void CreateTable(AnsiString strNodeID,AnsiString strTableType);
public:		// User declarations
	__fastcall TfrmLogs(TComponent* Owner);
	int nSaveDays,m_nDataType;
	AnsiString strOpID,strUserID;
	TDateTime LogBegin,LogEnd;
	AnsiString m_strCodeType;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogs *frmLogs;
//---------------------------------------------------------------------------
#endif
