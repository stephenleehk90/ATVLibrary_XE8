//---------------------------------------------------------------------------
#ifndef UserFrmH
#define UserFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <System.ImageList.hpp>
#include "AdvListV.hpp"
//---------------------------------------------------------------------------
class TfrmUserAdmin : public TForm
{
__published:	// IDE-managed Components
    TNotebook *nbkUsersAdmin;
    TPanel *pnlUserInput;
    TLabel *lblUserName;
    TLabel *lblUserDep;
    TLabel *lblUserID;
    TLabel *lblUserRemark;
    TLabel *lblUserCode;
    TLabel *lblUserPhone;
    TLabel *lblConfirm;
    TLabel *lbledtUserPassword;
    TEdit *edtUserName;
    TEdit *edtUserDep;
    TEdit *edtUserID;
    TEdit *edtUserRemark;
    TEdit *edtUserCode;
    TEdit *edtUserPhone;
    TButton *btnUserOk;
    TButton *btnUserCancel;
    TButton *btnGroups;
    TListBox *lstCurrentGroups;
    TEdit *edtUserConfirm;
    TEdit *edtUserPassword;
    TPanel *pnlGroupUsers;
    TLabel *lblGroupID;
    TLabel *lblGroupType;
    TLabel *lblGroupRemark;
    TEdit *edtGroupRemark;
    TComboBox *cboGroupType;
    TEdit *edtGroupID;
    TButton *btnUsers;
    TButton *btnOk;
    TButton *btnCancel;
    TListBox *lstCurrentUsers;
    TPopupMenu *popUserAdmin;
    TMenuItem *muiPopAdd;
    TMenuItem *muiPopModify;
    TMenuItem *N6;
    TMenuItem *muiPopDelete;
    TMenuItem *N7;
    TMenuItem *muiPopRefresh;
    TMainMenu *mnuMain;
    TMenuItem *muiEdit;
    TMenuItem *muiEdit_Add;
    TMenuItem *muiEdit_Modify;
    TMenuItem *muiEdit_Delete;
	TPanel *Panel1;
	TToolBar *ToolBar1;
	TToolButton *tbbtnAddUser;
	TToolButton *tbbtnModifyUser;
	TToolButton *tbbtnDeleteUser;
	TToolButton *tbbtnRefreshUser;
	TComboBox *cboUserGroups;
	TImageList *imglstUserAdmin;
	TPanel *Panel2;
	TToolBar *ToolBar2;
	TToolButton *tbbtnAddGroup;
	TToolButton *tbbtnModifyGroup;
	TToolButton *tbbtnDeleteGroup;
	TToolButton *tbbtnRefreshGroup;
	TAdvListView *lstvwUsers;
	TAdvListView *lstvwGroups;
    void __fastcall muiPopAddClick(TObject *Sender);
    void __fastcall muiPopModifyClick(TObject *Sender);
    void __fastcall muiPopDeleteClick(TObject *Sender);
    void __fastcall muiPopRefreshClick(TObject *Sender);
    void __fastcall cboUserGroupsChange(TObject *Sender);
    void __fastcall cboGroupTypeChange(TObject *Sender);
    void __fastcall btnUsersClick(TObject *Sender);
    void __fastcall btnGroupsClick(TObject *Sender);
    void __fastcall lstvwGroupsClick(TObject *Sender);
    void __fastcall lstvwUsersClick(TObject *Sender);
    void __fastcall lstvwUsersKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall lstvwGroupsKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall lstvwGroupsChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
    void __fastcall lstvwUsersChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnUserCancelClick(TObject *Sender);
    void __fastcall btnUserOkClick(TObject *Sender);
    
    void __fastcall lstvwGroupsDblClick(TObject *Sender);
    void __fastcall lstvwUsersDblClick(TObject *Sender);
    void __fastcall WndProc(TMessage &Message);
    void __fastcall FormCreate(TObject *Sender);
    
    
    
    void __fastcall tbbtnAddGroupClick(TObject *Sender);
    void __fastcall tbbtnModifyGroupClick(TObject *Sender);
    void __fastcall tbbtnDeleteGroupClick(TObject *Sender);
    void __fastcall tbbtnAddUserClick(TObject *Sender);
    void __fastcall tbbtnModifyUserClick(TObject *Sender);
    void __fastcall tbbtnDeleteUserClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall lstvwUsersColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lstvwUsersCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall lstvwGroupsCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall lstvwGroupsColumnClick(TObject *Sender, TListColumn *Column);
private:	// User declarations
public:		// User declarations
    int nGroupType;
    __fastcall TfrmUserAdmin(TComponent* Owner);
    bool Checkprivilege(AnsiString Title);
    void AddUser();
    void AddGroup();
    void ModifyGroup();
    void ModifyUser();
    void DeleteUser();
    void DeleteGroup();
    void GroupPage();
    void UserPage();
    AnsiString strCurrentUserID,strGroupID,strUserID,strUserCode,strUserName;
    bool bEditUser,bEditGroup,bModifyUser,bModifyGroup;
    bool bFirstIntoUser;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmUserAdmin *frmUserAdmin;
//---------------------------------------------------------------------------
#endif
