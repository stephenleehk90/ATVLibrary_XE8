//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("UserAdmin\UserData.cpp", dmdUserAdmin); /* TDataModule: File Type */
USEFORM("LendReturn\gotopage.cpp", fmGotoPage);
USEFORM("EventLog\SourceTbFrm.cpp", frmSourceTbs);
USEFORM("LendReturn\options.cpp", frmOption);
USEFORM("LendReturn\find.cpp", frmFind);
USEFORM("UserAdmin\UserFrm.cpp", frmUserAdmin);
USEFORM("frm_StoryInput.cpp", frmStoryInput);
USEFORM("LendReturn\Borrower.cpp", frmBorrower);
USEFORM("EventLog\SaveFrm.cpp", frmSave);
USEFORM("EventLog\DestTbFrm.cpp", frmDestTbs);
USEFORM("EventLog\DirectoryFrm.cpp", frmDirectory);
USEFORM("EventLog\LogFrm.cpp", frmLogs);
USEFORM("EventLog\LogData.cpp", dmdLog); /* TDataModule: File Type */
USEFORM("EventLog\SelectFieldFrm.cpp", frmSelectFields);
USEFORM("EventLog\RestypeFrm.cpp", frmResType);
USEFORM("Template\TemplateForm.cpp", frmTemplate);
USEFORM("Template\ImportFrm.cpp", frmInout);
USEFORM("Template\utAuth.cpp", fmAuthority);
USEFORM("Template\WizardFrm.cpp", frmWizard);
USEFORM("Template\utGlobDm.cpp", dmTemplate); /* TDataModule: File Type */
USEFORM("Template\utNewExt.cpp", fmEditExtFields);
USEFORM("LendReturn\lendmain.cpp", frmLendSys);
USEFORM("LendReturn\InfoReport.cpp", frmPreview);
USEFORM("LendReturn\FindMaterail.cpp", frmMaterial);
USEFORM("MaterialInput\ShelfFrm.cpp", frmShelf);
USEFORM("MaterialInput\QueryFrm.cpp", frmQuery);
USEFORM("LendReturn\PrevFrm.cpp", frmPrev);
USEFORM("LendReturn\dmData.cpp", dmLendDM); /* TDataModule: File Type */
USEFORM("LendReturn\FindUser.cpp", frmFindUser);
USEFORM("WordBank\SearchForm.cpp", frmSearch);
USEFORM("frm_Main.cpp", frmMain);
USEFORM("MaterialIDFrm.cpp", frmMaterialID);
USEFORM("StoryData.cpp", dmdStory); /* TDataModule: File Type */
USEFORM("UserAdmin\SelectUserFrm.cpp", frmSelectUsers);
USEFORM("ArchiveDialog.cpp", frmArchive);
USEFORM("BrowseTimeFrm.cpp", frmBrowseTime);
USEFORM("Capture.cpp", frmCapture);
USEFORM("UserAdmin\SelectGroupFrm.cpp", frmSelectGroups);
USEFORM("EventLog\ConfigFrm.cpp", frmConfig);
USEFORM("EventLog\utGlDm.cpp", dmGlobal); /* TDataModule: File Type */
USEFORM("EventLog\QueryFrmLog.cpp", frmQueryLog);
USEFORM("ChangeMaterialIDForm.cpp", frmChangeM);
USEFORM("MaterialInput\MaterialData.cpp", dmdMaterials); /* TDataModule: File Type */
USEFORM("MaterialInput\MaterialFrm.cpp", frmMaterials);
USEFORM("WordBank\frmWord.cpp", frmWB);
USEFORM("LoginFrm.cpp", frmLogin);
USEFORM("AboutFrm.cpp", AboutBox);
USEFORM("PwdFrm.cpp", frmPassword);
//---------------------------------------------------------------------------
#include "LoginFrm.h"
#include "PrintSetFrm.h"
#include "lenddatas.h"

WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TdmdStory), &dmdStory);
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfrmLogin), &frmLogin);
		if (frmLogin->ShowModal()==mrOk){
		Application->CreateForm(__classid(TfrmPrintSetting), &frmPrintSetting);
		Application->CreateForm(__classid(TfrmBrowseTime), &frmBrowseTime);
		Application->CreateForm(__classid(TfrmMaterialID), &frmMaterialID);
		Application->CreateForm(__classid(TfrmCapture), &frmCapture);
		Application->CreateForm(__classid(TfrmArchive), &frmArchive);
		Application->CreateForm(__classid(TfrmChangeM), &frmChangeM);
		Application->CreateForm(__classid(TfrmSelectGroups), &frmSelectGroups);
		Application->CreateForm(__classid(TfrmSelectUsers), &frmSelectUsers);
		Application->CreateForm(__classid(TdmdUserAdmin), &dmdUserAdmin);
		Application->CreateForm(__classid(TfrmConfig), &frmConfig);
		Application->CreateForm(__classid(TfrmDestTbs), &frmDestTbs);
		Application->CreateForm(__classid(TfrmDirectory), &frmDirectory);
		Application->CreateForm(__classid(TdmdLog), &dmdLog);
		Application->CreateForm(__classid(TfrmResType), &frmResType);
		Application->CreateForm(__classid(TfrmSave), &frmSave);
		Application->CreateForm(__classid(TfrmSelectFields), &frmSelectFields);
		Application->CreateForm(__classid(TfrmSourceTbs), &frmSourceTbs);
		Application->CreateForm(__classid(TdmGlobal), &dmGlobal);
		Application->CreateForm(__classid(TfrmSearch), &frmSearch);
		Application->CreateForm(__classid(TfrmBorrower), &frmBorrower);
		Application->CreateForm(__classid(TdmLendDM), &dmLendDM);

		CLdSystemData = new TLdSystemData();
		CLdSystemData->strCaption = "Library Lend";
		Application->CreateForm(__classid(TfrmFind), &frmFind);
		Application->CreateForm(__classid(TfrmMaterial), &frmMaterial);
		Application->CreateForm(__classid(TfrmFindUser), &frmFindUser);
		Application->CreateForm(__classid(TfmGotoPage), &fmGotoPage);
		Application->CreateForm(__classid(TfrmPreview), &frmPreview);
		Application->CreateForm(__classid(TfrmOption), &frmOption);
		Application->CreateForm(__classid(TfrmPrev), &frmPrev);
		Application->CreateForm(__classid(TdmdMaterials), &dmdMaterials);
		Application->CreateForm(__classid(TfrmShelf), &frmShelf);
		Application->CreateForm(__classid(TfrmQueryLog), &frmQueryLog);
		Application->CreateForm(__classid(TfrmQuery), &frmQuery);
		Application->CreateForm(__classid(TfrmInout), &frmInout);
		Application->CreateForm(__classid(TfmAuthority), &fmAuthority);
		Application->CreateForm(__classid(TdmTemplate), &dmTemplate);
		Application->CreateForm(__classid(TfmEditExtFields), &fmEditExtFields);
		Application->CreateForm(__classid(TfrmWizard), &frmWizard);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TfrmPassword), &frmPassword);
		Application->Run();
		}
		else
		{
//			delete frmMain;
//			delete frmLogin;
//			delete dmdStory;
		}
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	try
	{
	return 0;
	}
	catch (Exception &exception)
	{
	}
}
//---------------------------------------------------------------------------
