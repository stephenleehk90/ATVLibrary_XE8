//---------------------------------------------------------------------------
#ifndef CaptureH
#define CaptureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "ImageEn.hpp"
#include "ImageEnView.hpp"
#include "hyiedefs.hpp"
#include "hyieutils.hpp"
#include "iesettings.hpp"
#include "ieview.hpp"
#include "iexBitmaps.hpp"
#include "iexLayers.hpp"
#include "iexRulers.hpp"
#include "iexToolbars.hpp"
#include "iexUserInteractions.hpp"
//---------------------------------------------------------------------------
class TfrmCapture : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlPreview;
    TButton *btnOk;
    TButton *btnCancel;
    TBitBtn *btnPlay;
    TBitBtn *btnStop;
    TBitBtn *btnForward;
    TBitBtn *btnBack;
    TBitBtn *btnCapture;
    TBitBtn *btnEject;
    TLabel *lblTimeCode;
    TTimer *Timer1;
    TPanel *pnlCapture;
    TImageEn *imgCapture;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall btnPlayClick(TObject *Sender);
    void __fastcall btnStopClick(TObject *Sender);
    void __fastcall btnForwardClick(TObject *Sender);
    void __fastcall btnBackClick(TObject *Sender);
    void __fastcall btnEjectClick(TObject *Sender);

    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btnCaptureClick(TObject *Sender);

    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    
    void __fastcall btnOkClick(TObject *Sender);
    
private:	// User declarations
public:		// User declarations
    __fastcall TfrmCapture(TComponent* Owner);
    HWND hwndCap;
    bool m_bKeyFrame;
    bool m_bCapture;
    AnsiString strFieldName;    
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCapture *frmCapture;
//---------------------------------------------------------------------------
#endif
