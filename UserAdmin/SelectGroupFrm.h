//---------------------------------------------------------------------------
#ifndef SelectGroupFrmH
#define SelectGroupFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class TfrmSelectGroups : public TForm
{
__published:	// IDE-managed Components
	TListView *lstvwCurrent;
	TListView *lstvwAllGroups;
	TButton *btnAdd;
	TButton *btnDelete;
	TButton *btnCancel;
	TButton *btnOk;
	TLabel *lblCurrentGroups;
	TLabel *lblAllGroups;
	TImageList *imglstGroups;
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	
	
	void __fastcall lstvwCurrentEnter(TObject *Sender);
	void __fastcall lstvwAllGroupsEnter(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    bool __fastcall TfrmSelectGroups::CheckSingle(AnsiString Title);
    bool __fastcall TfrmSelectGroups::Checkprivilege();
	__fastcall TfrmSelectGroups(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfrmSelectGroups *frmSelectGroups;
//---------------------------------------------------------------------------
#endif
