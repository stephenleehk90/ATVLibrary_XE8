//---------------------------------------------------------------------------
#ifndef SelectUserFrmH
#define SelectUserFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class TfrmSelectUsers : public TForm
{
__published:	// IDE-managed Components
	TListView *lstvwCurrent;
	TListView *lstvwAllUsers;
	TLabel *lblCurrent;
	TLabel *lblAllUsers;
	TButton *btnAdd;
	TButton *btnDelete;
	TButton *btnOk;
	TButton *btnCancel;
	TImageList *imglstUsers;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	
	
	void __fastcall lstvwCurrentEnter(TObject *Sender);
	void __fastcall lstvwAllUsersEnter(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    bool __fastcall TfrmSelectUsers::CheckSingle(AnsiString Title);
    bool __fastcall TfrmSelectUsers::Checkprivilege();
	__fastcall TfrmSelectUsers(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfrmSelectUsers *frmSelectUsers;
//---------------------------------------------------------------------------
#endif
