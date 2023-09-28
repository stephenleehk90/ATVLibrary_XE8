//---------------------------------------------------------------------------
#include <vcl.h>
#include <vfw.h>
#include <clipbrd.hpp>
#pragma hdrstop

#include "Capture.h"
//#include "DllHead.h"
#include "frm_Main.h"
#include "frm_StoryInput.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImageEn"
#pragma link "ImageEnView"
#pragma link "hyiedefs"
#pragma link "hyieutils"
#pragma link "iesettings"
#pragma link "ieview"
#pragma link "iexBitmaps"
#pragma link "iexLayers"
#pragma link "iexRulers"
#pragma link "iexToolbars"
#pragma link "iexUserInteractions"
#pragma resource "*.dfm"
TfrmCapture *frmCapture;
//---------------------------------------------------------------------------
__fastcall TfrmCapture::TfrmCapture(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCapture::FormShow(TObject *Sender)
{
  TMemoryStream *pStream;
  int nReturn;
  bool bLocal;
  if (!m_bKeyFrame)
  {
    imgCapture->Clear();
  }
  else{
    pStream= new TMemoryStream;
    html->GetImage(pStream,strFieldName);
    pStream->Position=0;
    if (pStream->Size>0)
    {
      try{
		  imgCapture->IO->LoadFromStreamJpeg(pStream);
		  imgCapture->Proc->Resample(IMAGEWIDTH,IMAGEHEIGHT,rfNone);
      }
      catch(...)
      {
         imgCapture->Clear();
      }
    }
    else
      imgCapture->Clear();
    delete pStream;
  }
  pVTR->COMSetPort(5);
  nReturn = pVTR->VTRInit();
  if (nReturn!=0)
  {
	 Application->MessageBox(L"The VTR can not be successfully initialized,please check the device or connection.",L"TBS Library",48);
     btnBack->Enabled = false;
     btnForward->Enabled = false;
     btnPlay->Enabled = false;
     btnStop->Enabled = false;
     btnCapture->Enabled = false;
     btnEject->Enabled = false;
  }
  else
  {
     pVTR->VTRGetStatus();
     bLocal = pVTR->Status.Local;
     if (bLocal)
		Application->MessageBox(L"The VTR's control is on local,please change it to remote.",L"TBS Library",48);
     btnBack->Enabled = true;
     btnForward->Enabled = true;
     btnPlay->Enabled = true;
     btnStop->Enabled = true;
     btnCapture->Enabled = true;
     btnEject->Enabled = true;
  }

  capPreview(hwndCap,TRUE);
  capPreviewRate(hwndCap,1);
  capPreviewScale(hwndCap,true);
  Timer1->Enabled=true;
  m_bCapture = false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  pVTR->VTRRelease();
  Timer1->Enabled = false;
  m_bKeyFrame = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::btnPlayClick(TObject *Sender)
{
   int nReturn;
   nReturn = pVTR->VTRPlay();
   if (nReturn!=0)
	   Application->MessageBox(L"The VTR can not be successfully played,please check the device or connection.",L"TBS Library",48);
   else
   {
       btnPlay->Enabled = false;
       btnStop->Enabled = true;
       btnForward->Enabled = true;
       btnBack->Enabled = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::btnStopClick(TObject *Sender)
{
   int nReturn;
   nReturn = pVTR->VTRStop();
   if (nReturn!=0)
	   Application->MessageBox(L"The VTR can not be successfully stopped,please check the device or connection.",L"TBS Library",48);
   else
   {
       btnStop->Enabled = false;
       btnPlay->Enabled = true;
       btnForward->Enabled = true;
       btnBack->Enabled = true;
   }

}
//---------------------------------------------------------------------------


void __fastcall TfrmCapture::btnForwardClick(TObject *Sender)
{
    int nReturn;
    nReturn = pVTR->VTRFFwd();
    if (nReturn!=0)
	   Application->MessageBox(L"The VTR can not be successfully forwarded,please check the device or connection.",L"TBS Library",48);
    else
    {
       btnBack->Enabled = true;
       btnForward->Enabled = false;
       btnPlay->Enabled = true;
       btnStop->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::btnBackClick(TObject *Sender)
{
    int nReturn;
    nReturn = pVTR->VTRRewind();
    if (nReturn!=0)
	   Application->MessageBox(L"The VTR can not be successfully rewinded,please check the device or connection.",L"TBS Library",48);
    else
    {
       btnBack->Enabled = false;
       btnForward->Enabled = true;
       btnPlay->Enabled = true;
       btnStop->Enabled = true;
    }

}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::btnEjectClick(TObject *Sender)
{
    int nReturn;
    nReturn = pVTR->VTREject();
    if (nReturn!=0)
	   Application->MessageBox(L"The VTR can not be successfully ejected,please check the device or connection.",L"TBS Library",48);
    else
    {
       btnBack->Enabled = true;
       btnForward->Enabled = true;
       btnPlay->Enabled = true;
       btnStop->Enabled = true;
    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmCapture::Timer1Timer(TObject *Sender)
{
	AnsiString strTimeCode;
	pVTR->VTRGetLTC();
	strTimeCode=pVTR->Status.LTC;
	lblTimeCode->Caption=strTimeCode;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::btnCaptureClick(TObject *Sender)
{
	TClipboard *pCB = Clipboard();
	int nReturn;
	TMemoryStream *pStream;
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER* pInfo;
	int nBytes,TextHandle,nPallette;
	void* pText;

	capEditCopy(hwndCap);
	pCB->Open();
	if (pCB->HasFormat(CF_DIB))
	{
	  pStream = new TMemoryStream;
	  TextHandle = pCB->GetAsHandle(CF_DIB);
	  pText = GlobalLock((HGLOBAL)TextHandle);
	  pInfo = (BITMAPINFOHEADER*)pText;
	  if (pInfo->biBitCount <= 8)
	  {
	   if (pInfo->biClrUsed != 0)
		nPallette = pInfo->biClrUsed;
	   else
		nPallette = (1<<pInfo->biBitCount);
	  }
	  else
		nPallette = 0;

	  nBytes = ((pInfo->biWidth*pInfo->biBitCount+7)/8+3)/4*4;
	  header.bfType=0x4d42;
	  header.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*nPallette
					+nBytes*pInfo->biHeight;
	  header.bfReserved1=0;
	  header.bfReserved2=0;
	  header.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*nPallette;

	  pStream->WriteBuffer(&header,14);
	  pStream->Position = 14;
	  pStream->WriteBuffer((const void*)pText,sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*nPallette+nBytes*pInfo->biHeight);
	  GlobalUnlock((HGLOBAL)TextHandle);

      pStream->Position=0;
	  imgCapture->IO->LoadFromStreamBMP(pStream);
	  imgCapture->Proc->Resample(IMAGEWIDTH,IMAGEHEIGHT,rfNone);
      delete pStream;
	  m_bCapture = true;
      nReturn = pVTR->VTRStop();
	  if (nReturn==0)
	  {
		 btnBack->Enabled = true;
		 btnForward->Enabled = true;
		 btnPlay->Enabled = true;
		 btnStop->Enabled = false;
	  }
	}
	else
	   Application->MessageBox(L"There is no video signal to capture,please check the device or connection.",L"TBS Library",48);
	pCB->Close();
}
//---------------------------------------------------------------------------
/*LRESULT FAR PASCAL FrameCallbackProc(HWND hWnd,LPVIDEOHDR lpVHdr)
{
   DWORD mTime;
   TFileStream *pStream;
   if (!m_bCapture)
	  return (LRESULT) false;
   pStream = new TFileStream("c:\\test.avi",fmCreate);
   pStream->WriteBuffer(lpVHdr->lpData,lpVHdr->dwBytesUsed);
   mTime = lpVHdr->dwTimeCaptured;
   capCaptureStop(frmCapture->hwndCap);
   capEditCopy(frmCapture->hwndCap);
   frmCapture->imgCapture->Picture->LoadFromFile("g:\\jpeg\\ghost.jpeg");
   frmCapture->imgCapture->V

   TJPEGImage *jpegimage = dynamic_cast<TJPEGImage *>(frmCapture->imgCapture->Picture->Graphic);
   jpegimage->LoadFromStream(pStream);
    if (jpegimage){
			jpegimage->PixelFormat = TJPEGPixelFormat(0);
			jpegimage->Scale = TJPEGScale(0);
			jpegimage->Grayscale = bool(0);
			jpegimage->Performance = TJPEGPerformance(1);
			jpegimage->ProgressiveDisplay = true;

			frmCapture->imgCapture->IncrementalDisplay = true;

	}

   delete pStream;
   m_bCapture=false;
   return (LRESULT) true;
}
void __fastcall TfrmCapture::OpenFile(const AnsiString Filename)
{
	try{
		Image1->Picture->LoadFromFile(Filename);
	} catch(EInvalidGraphic &x){
		Image1->Picture->Graphic = NULL;
	}
	SetJPEGOptions(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::SetJPEGOptions(TObject *Sender)
{
	TJPEGImage *jpegimage = dynamic_cast<TJPEGImage *>(Image1->Picture->Graphic);
	if (jpegimage){
			jpegimage->PixelFormat = TJPEGPixelFormat(PixelFormat->ItemIndex);
			jpegimage->Scale = TJPEGScale(Scale->ItemIndex);
			jpegimage->Grayscale = bool(ColorSpace->ItemIndex);
			jpegimage->Performance = TJPEGPerformance(Performance->ItemIndex);
			jpegimage->ProgressiveDisplay = ProgressiveDisplay->Checked;

			Scale->Enabled = true;
			PixelFormat->Enabled = true;
			ColorSpace->Enabled = true;
			Performance->Enabled = true;
			ProgressiveDisplay->Enabled = jpegimage->ProgressiveEncoding;
			Image1->IncrementalDisplay = IncrementalDisplay->Checked;
	}
}
*/
void __fastcall TfrmCapture::FormDestroy(TObject *Sender)
{
   capDriverDisconnect(hwndCap);
}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::FormCreate(TObject *Sender)
{
  BITMAPINFOHEADER header;
  if (hwndCap==NULL)
  {
    hwndCap = capCreateCaptureWindow(
                        NULL,
                        WS_CHILD | WS_VISIBLE,
                        pnlPreview->Left, pnlPreview->Top, pnlPreview->Width, pnlPreview->Height,
                        Handle,
                        1
                        );
    capDriverConnect(hwndCap,0);
/*
    if (!bSuccess)
       Application->MessageBox("The video capture device can not be successfully connected,please check the configuration of the device.","TBS Library",48);
*/
//    nSize = capGetVideoFormat(hwndCap,&info,sizeof(BITMAPINFO));
    header.biSize = sizeof(BITMAPINFOHEADER);
//    info.bmiColors[1] = NULL;
/*    info.bmiColors[1].rgbBlue = 0;
    info.bmiColors[1].rgbGreen = 0;
    info.bmiColors[1].rgbRed = 0;
    info.bmiColors[1].rgbReserved = 0;
*/
    header.biWidth = 384;
    header.biHeight = 288;
    header.biPlanes = 1;
    header.biBitCount = 24;
    header.biCompression = BI_RGB;
    header.biSizeImage = 0;
    header.biXPelsPerMeter = 96;
    header.biYPelsPerMeter = 96;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
    capSetVideoFormat(hwndCap,(BITMAPINFO*)&header,sizeof(header));
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCapture::btnOkClick(TObject *Sender)
{
    TMemoryStream *pJpegStream;
    int nReturn;

    if (!m_bCapture)
        return;
    nReturn = pVTR->VTRStop();
    if (nReturn==0)
    {
       btnBack->Enabled = true;
       btnForward->Enabled = true;
       btnPlay->Enabled = true;
       btnStop->Enabled = false;
    }
    if (!m_bKeyFrame)
    {
        html->SetTimeCode(lblTimeCode->Caption,strFieldName);
        return;
    }

    pJpegStream = new TMemoryStream;
	imgCapture->IO->SaveToStreamJpeg(pJpegStream);
    html->SetImage(pJpegStream,strFieldName);
    delete pJpegStream;
}
//---------------------------------------------------------------------------



