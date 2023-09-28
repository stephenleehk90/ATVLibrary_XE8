//---------------------------------------------------------------------------
#ifndef TemplateFormH
#define TemplateFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//#include "TB97.h"
#include <Menus.hpp>
//#include "ComboBox.h"
//#include "Menus97.h"
#include <Dialogs.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include "htmlformTemp.h"
//#include "EnhListView.hpp"
//#include "ExtListView.hpp"
#include <ImgList.hpp>
#include <System.ImageList.hpp>
#include <Vcl.Buttons.hpp>

#define GLOBALMSGCAPTION    L"TBS Library"
#define DATABASEACCESSERROR L"Database Access Error!"
#define strSYSMANAGER       "admins"
#define strADMINISTRATOR    "admin"
#define strFIELDNAME        "FieldName"
#define strFIELDTYPE        "FieldType"
#define strFIELDSIZE        "FieldSize"
#define strLIBBASEFIELDS    "lib_BaseFields"
#define strLIBEXTENDFIELDS  "lib_ExtendFields"
#define strSTORYLIBTREE     "lib_StoryLibTree"
#define strPERMISSION       "lib_Permission"
#define strGROUP            "lib_Group"

#define typeVARCHAR	  "varchar"
#define typeINT	      "int"
#define typeTEXT	  "text"
#define typeDATETIME  "datetime"
#define typeIMAGE	  "image"

#define MAXFIELDTYPES 5
#define NODEIDSIZE    32

#define INPUTTEMPLATE    1
#define BROWSETEMPLATE   2
#define SEARCHTEMPLATE   3
#define ADVANCEDTEMPLATE 4
#define LISTTEMPLATE     5

#define DATABASEERROR    1
#define LEAFNODE         2
#define MIDNODE          3

#define UPDATESTORYPROC "UpdateStoryLibrary"

#define FIELDMATERIALID  "MaterialID"
#define FIELDDATEID      "Date"
//---------------------------------------------------------------------------
class TfrmTemplate : public TForm
{
__published:	// IDE-managed Components
    TNotebook *ntbMain;
    TTreeView *trvwTemplate;
    TSplitter *splTemplate;
    TPanel *pnlTemplate;
    TImageList *imglstTemplate;
    TPopupMenu *popMain;
    TMenuItem *muiPopTemplate;
    TMenuItem *muiPopRename;
    TOpenDialog *OpenDialog1;
    TSaveDialog *SaveDialog1;
    TMenuItem *muiPopInput;
    TMenuItem *muiPopBrowse;
    TMenuItem *muiPopSearch;
    TMenuItem *muiPopAdvance;
    TQuery *qeInternal;
    TDataSource *dsInternal;
    TPopupMenu *pmFields;
    TMenuItem *mniNewField;
    TMenuItem *mniRemoveField;
    TMenuItem *mniModifyField;
    TMenuItem *N2;
    TQuery *qeInternal2;
    TMenuItem *muiPopList;
    TMenuItem *muiPopremove;
    TMainMenu *mnuTemplete;
    TMenuItem *muiTemplate;
    TMenuItem *muiNew;
    TMenuItem *muiremove;
    TMenuItem *N3;
    TMenuItem *muiInput;
    TMenuItem *muiBrowse;
    TMenuItem *muiSearch;
    TMenuItem *muiAdvance;
    TMenuItem *muiList;
    TMenuItem *N1;
    TMenuItem *muiRefresh;
    TMenuItem *N4;
    TMenuItem *muiPopRefresh;
    TMenuItem *muiRename;
	TListView *lstvwFields;
	TToolBar *ToolBar1;
	TToolButton *tbbtnInput;
	TToolButton *tbbtnBrowse;
	TToolButton *tbbtnSearch;
	TToolButton *tbbtnAdvance;
	TToolButton *tbbtnList;
	TToolButton *tbbtnAdd;
	TToolButton *tbbtnDelete;
	TToolButton *ToolButton1;
	TLabel *lblDBName;
    void __fastcall tbbtnAddClick(TObject *Sender);
    void __fastcall muiPopImportClick(TObject *Sender);
    
    void __fastcall muiPopRenameClick(TObject *Sender);
    void __fastcall tbbtnBrowseClick(TObject *Sender);
    void __fastcall tbbtnSearchClick(TObject *Sender);
    void __fastcall tbbtnAdvanceClick(TObject *Sender);
    void __fastcall trvwTemplateChange(TObject *Sender, TTreeNode *Node);
    
    
    void __fastcall FormCreate(TObject *Sender);
    
    void __fastcall mniRemoveFieldClick(TObject *Sender);
    
    void __fastcall mniNewFieldClick(TObject *Sender);
    void __fastcall mniModifyFieldClick(TObject *Sender);
    void __fastcall tbbtnDeleteClick(TObject *Sender);

	void __fastcall tbbtnInputClick(TObject *Sender);
    void __fastcall tbbtnListClick(TObject *Sender);
    void __fastcall tbbtnAuthorizeClick(TObject *Sender);

    void __fastcall trvwTemplateMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall RefreshTree();          
    void __fastcall tbbtnRefreshClick(TObject *Sender);
    
    
    void __fastcall trvwTemplateKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall trvwTemplateEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);



private:	// User declarations
    int m_nNodeID;
    int m_nAuthority;
	AnsiString m_strSysManager;
	AnsiString m_strLibExtFields;

    void RemoveNode(AnsiString strNodeID);
    void RemoveAssociatedTable(AnsiString strNodeID);
    void CreateTable(AnsiString strNodeID,bool bToCreate);
    void ModifyTemplate();
public:		// User declarations

    int m_nTemplateType;
    AnsiString m_strDatabaseName;
	AnsiString m_strCodeType;
 //	TDock97* dkMain;

    AnsiString GetDirectory(TTreeView *pTreeView);
    bool SetDirectory(TTreeView *pTreeview,TTreeNode *pTreeNode,AnsiString strDirectory);
    bool CheckStrings(TComboBox *pCombox,AnsiString str);
    __fastcall TfrmTemplate(TComponent* Owner);
    int IsLeafNode(AnsiString strNodeID);
    void UpdateStoryLibrary();    
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTemplate *frmTemplate;
//---------------------------------------------------------------------------
#endif
