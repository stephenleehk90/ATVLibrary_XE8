//---------------------------------------------------------------------------
#ifndef ImportFrmH
#define ImportFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//#include <NMHTML.hpp>
#include <OleCtrls.hpp>

#include "htmlformTemp.h"
//---------------------------------------------------------------------------
class TfrmInout : public TForm
{
__published:	// IDE-managed Components
    TButton *btnCancel;
    TButton *btnImport;
    TButton *btnOk;
    TButton *btnExport;
    TScrollBox *sbPreview;
	void __fastcall btnImportClick(TObject *Sender);
    void __fastcall btnExportClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnOkClick(TObject *Sender);
    
private:	// User declarations
  ThtmlformTemp * m_pHtmlFormTemp;
  bool        m_bModified;
  AnsiString m_strNewTemplate;

  bool CheckHtmlValid(ThtmlformTemp * pHtmlFormTemp,AnsiString strNodeID);
public:		// User declarations
    __fastcall TfrmInout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmInout *frmInout;
//---------------------------------------------------------------------------
#endif
