//---------------------------------------------------------------------------
#ifndef utAuthH
#define utAuthH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfmAuthority : public TForm
{
__published:	// IDE-managed Components
    TTreeView *trvwTemplate;
    TListBox *lbGroup;
    TListBox *lbAuthority;
    TButton *btnGrant;
    TButton *btnRevoke;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TButton *btnClose;
    void __fastcall trvwTemplateChange(TObject *Sender, TTreeNode *Node);
    
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnGrantClick(TObject *Sender);
    void __fastcall btnRevokeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfmAuthority(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmAuthority *fmAuthority;
//---------------------------------------------------------------------------
#endif
