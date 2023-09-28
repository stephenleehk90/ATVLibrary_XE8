//---------------------------------------------------------------------------
#ifndef utInstH
#define utInstH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Mask.hpp>

#define MSGBOXCAPTION "Database Initializing"
#define LANGUAGE_B5 "B5"
#define LANGUAGE_GB "GB"
//---------------------------------------------------------------------------
class TfmInit : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *BitBtn2;
	TBitBtn *btnConnect;
	TGroupBox *gbFunc3;
	TGroupBox *gbFunc1;
	TLabel *Label4;
	TEdit *edPassword;
	TLabel *Label5;
	TEdit *edDbName;
	TLabel *Label6;
	TEdit *edServerName;
	TGroupBox *gbFunc2;
	TRadioButton *rbBIG5;
	TRadioButton *rbGB;
	TRadioButton *rbTarget2;
    TLabel *Label2;
    TEdit *edSearchDb;
    TLabel *Label1;
    TRadioButton *rbRetrieve;
    TMaskEdit *meCacheSize;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnConnectClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rbTarget2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	
	void __fastcall cbInitialFullTextClick(TObject *Sender);
	
	void __fastcall meCacheSizeExit(TObject *Sender);
private:	// User declarations
	int m_nState;
    AnsiString m_strServerName,m_strServerIP,m_strDbName;

	AnsiString IniName(void);
	AnsiString browseForFolder(HWND hParentWnd);        
public:		// User declarations
	__fastcall TfmInit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfmInit *fmInit;
//---------------------------------------------------------------------------
#endif
