//---------------------------------------------------------------------------
#ifndef PrintSetFrmH
#define PrintSetFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <checklst.hpp>
#include <ComCtrls.hpp>
//#include "AdvGrid.hpp"
#include <Grids.hpp>
#include <Dialogs.hpp>
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "AdvGrid.hpp"
#include "AdvUtil.hpp"
#include "frxExportPDF.hpp"
//---------------------------------------------------------------------------
class TfrmPrintSetting : public TForm
{
__published:	// IDE-managed Components
	TButton *btnCacel;
	TButton *btnPreview;
	TButton *btnPrint;
	TButton *btnImport;
	TButton *btnExport;
	TSaveDialog *SaveDialog1;
	TOpenDialog *OpenDialog1;
	TGroupBox *grbOption;
	TButton *btnAdd;
	TButton *btnDelete;
	TListBox *lstPrint;
	TComboBox *cboOrder;
	TButton *btnDown;
	TButton *btnUp;
	TLabel *lblOrder;
	TfrxDBDataset *frxDBDataset1;
	TfrxReport *frxReport1;
	TfrxReport *frxReport2;
	TAdvStringGrid *strgrdFields;
	void __fastcall btnPreviewClick(TObject *Sender);

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall strgrdFieldsCanEditCell(TObject *Sender, int Arow,
		  int Acol, bool &canedit);
	void __fastcall btnUpClick(TObject *Sender);
	void __fastcall btnDownClick(TObject *Sender);
	void __fastcall btnPrintClick(TObject *Sender);
	void __fastcall btnExportClick(TObject *Sender);
	void __fastcall btnImportClick(TObject *Sender);
	void __fastcall strgrdFieldsGetEditMask(TObject *Sender, int ACol, int ARow, UnicodeString &Value);



private:	// User declarations
public:		// User declarations
	__fastcall TfrmPrintSetting(TComponent* Owner);
	void PrintStory(int nMode);
	//void __fastcall Print(TObject* Sender,AnsiString& Value);
	TStringList *strlstPrint;
	int m_nField;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPrintSetting *frmPrintSetting;
//---------------------------------------------------------------------------
#endif
