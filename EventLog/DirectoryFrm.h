//---------------------------------------------------------------------------
#ifndef DirectoryFrmH
#define DirectoryFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <System.ImageList.hpp>

#define DATABASEERROR    1
#define LEAFNODE         2
#define MIDNODE          3
//---------------------------------------------------------------------------
class TfrmDirectory : public TForm
{
__published:	// IDE-managed Components
    TTreeView *trvwStory;
    TImageList *imglstTemplate;
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall trvwStoryChange(TObject *Sender, TTreeNode *Node);
private:	// User declarations
    int IsLeafNode(AnsiString strNodeID);
public:		// User declarations
    __fastcall TfrmDirectory(TComponent* Owner);
	void InitForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDirectory *frmDirectory;
//---------------------------------------------------------------------------
#endif
