//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("lendmain.cpp", frmLendSys);
USERES("Lend.res");
USEFORM("login.cpp", frmLogin);
USEUNIT("LendLogin.cpp");
USEDATAMODULE("dmData.cpp", dmLendDM);
USEUNIT("lenddatas.cpp");
USEFORM("InfoReport.cpp", frmPreview);
USEFORM("options.cpp", frmOption);
USEFORM("find.cpp", frmFind);
USEFORM("AboutFrm.cpp", AboutBox);
USEUNIT("CommFunc.cpp");
USEFORM("password.cpp", frmPassword);
//---------------------------------------------------------------------------
//#include "LendLogin.h"
#include "lendmain.h"
//#include "login.h"
#include "lenddatas.h"
#include "commfunc.h"
#include "dmData.h"
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
/*		Application->Initialize();
		Application->CreateForm(__classid(TfrmLendSys), &frmLendSys);
		Application->CreateForm(__classid(TfrmLogin), &frmLogin);
		Application->CreateForm(__classid(TdmLendDM), &dmLendDM);
		CLdLogin = new TLdLogin();
		CLdSystemData = new TLdSystemData();
		CLdSystemData->strCaption = "Library Lend";
		DisableMenu(frmLendSys->mmMenu);
		if (frmLogin->ShowModal() == mrOk)
		 {
		  EnableMenu(frmLendSys->mmMenu);
		  Application->CreateForm(__classid(TfrmOption), &frmOption);
		  Application->CreateForm(__classid(TfrmFind), &frmFind);
		  Application->CreateForm(__classid(TAboutBox), &AboutBox);
		  Application->CreateForm(__classid(TfrmPassword), &frmPassword);
		 try
		 {
		  Application->CreateForm(__classid(TfrmPreview), &frmPreview);
		 }
		 catch(...)
		 {
		  frmLendSys->muiPreview1->Enabled = false;
		  frmLendSys->muiPrint1->Enabled   = false;
		  }
		Application->Run();
	   }
	   else
	   {
		delete dmLendDM;
		delete frmLendSys;
		delete frmLogin;
	   }
	  if (CLdLogin!=NULL)
	  delete CLdLogin;
	  if (CLdSystemData!=NULL)
	  delete CLdSystemData;
*/
	 }
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
