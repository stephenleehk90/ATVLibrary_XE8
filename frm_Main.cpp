//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frm_Main.h"
#include "StoryData.h"
#include "CommFunc.h"
#include "DataProcess.h"
#include "PrintSetFrm.h"
#include "BrowseTimeFrm.h"
#include "MaterialIDFrm.h"
#include "htmlform.h"
#include "Capture.h"
#include "ArchiveDialog.h"
#include "ChangeMaterialIDForm.h"
#include "frm_StoryInput.h"
#include "UserFrm.h"
#include "frmWord.h"
#include "LogFrm.h"
#include "lendmain.h"
#include "MaterialFrm.h"
#include "TemplateForm.h"
#include "AboutFrm.h"
#include "PwdFrm.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "hyiedefs"
#pragma link "hyieutils"
#pragma link "iesettings"
#pragma link "ieview"
#pragma link "iexBitmaps"
#pragma link "iexLayers"
#pragma link "iexRulers"
#pragma link "iexToolbars"
#pragma link "iexUserInteractions"
#pragma link "imageenview"
//#pragma link "AdvGrid"
//#pragma link "AdvObj"
//#pragma link "AdvUtil"
//#pragma link "BaseGrid"
#pragma link "frxClass"
#pragma link "InspectorBar"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::Button1Click(TObject *Sender)
{
//AddictSpell1->CheckWinControl(RichEdit1, ctAll);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::CreateChildForm( const AnsiString childName )
{
 // frmStoryInput = new TfrmStoryInput( Application );
 // frmStoryInput ->Caption = childName;
}

void __fastcall TfrmMain::muiStoryClick(TObject *Sender)
{
//frmStoryInput = new TfrmStoryInput(Application);
//  CreateChildForm( "Child " + IntToStr( MDIChildCount + 1 ) );
//Application->CreateForm(__classid(TfrmStoryInput), &frmStoryInput);
	if (frmStoryInput==NULL)
	{
		Application->CreateForm(__classid(TfrmStoryInput), &frmStoryInput);
		frmStoryInput->Show();
	}
	else
		frmStoryInput->BringToFront();

	frmStoryInput->WindowState = wsMaximized;

}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::UserAdmin1Click(TObject *Sender)
{
	if (frmUserAdmin==NULL)
	{
		Application->CreateForm(__classid(TfrmUserAdmin), &frmUserAdmin);
		frmUserAdmin->Show();
	}
	else
		frmUserAdmin->BringToFront();

	frmUserAdmin->WindowState = wsMaximized;
	frmUserAdmin->nbkUsersAdmin->PageIndex = 0;

}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::UserGroups1Click(TObject *Sender)
{
	if (frmUserAdmin==NULL)
	{
		Application->CreateForm(__classid(TfrmUserAdmin), &frmUserAdmin);
		frmUserAdmin->Show();
	}
	else
		frmUserAdmin->BringToFront();

	frmUserAdmin->WindowState = wsMaximized;
	frmUserAdmin->nbkUsersAdmin->PageIndex = 1;

}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::Wo1Click(TObject *Sender)
{
	if (frmWB==NULL)
	{
		Application->CreateForm(__classid(TfrmWB), &frmWB);
		frmWB->Show();
	}
	else
		frmWB->BringToFront();

	frmWB->WindowState = wsMaximized;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::LogBrowse1Click(TObject *Sender)
{
	if (frmLogs==NULL)
	{
		Application->CreateForm(__classid(TfrmLogs), &frmLogs);
		frmLogs->Show();
	}
	else
		frmLogs->BringToFront();

	frmLogs->WindowState = wsMaximized;
	frmLogs->nbkDatabase->PageIndex=0;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::DataTransfer1Click(TObject *Sender)
{
	if (frmLogs==NULL)
	{
		Application->CreateForm(__classid(TfrmLogs), &frmLogs);
		frmLogs->Show();
	}
	else
		frmLogs->BringToFront();

	frmLogs->WindowState = wsMaximized;
	frmLogs->nbkDatabase->PageIndex=1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::SystemSettings1Click(TObject *Sender)
{
	if (frmLogs==NULL)
	{
		Application->CreateForm(__classid(TfrmLogs), &frmLogs);
		frmLogs->Show();
	}
	else
		frmLogs->BringToFront();

	frmLogs->WindowState = wsMaximized;
	frmLogs->nbkDatabase->PageIndex=2;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Lend1Click(TObject *Sender)
{
	if (frmLendSys==NULL)
	{
		Application->CreateForm(__classid(TfrmLendSys), &frmLendSys);
		frmLendSys->Show();
	}
	else
		frmLendSys->BringToFront();

	frmLendSys->WindowState = wsMaximized;
	frmLendSys->ntbLend->PageIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Return1Click(TObject *Sender)
{
	if (frmLendSys==NULL)
	{
		Application->CreateForm(__classid(TfrmLendSys), &frmLendSys);
		frmLendSys->Show();
	}
	else
		frmLendSys->BringToFront();

	frmLendSys->WindowState = wsMaximized;
	frmLendSys->ntbLend->PageIndex=1;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Browse1Click(TObject *Sender)
{
	if (frmLendSys==NULL)
	{
		Application->CreateForm(__classid(TfrmLendSys), &frmLendSys);
		frmLendSys->Show();
	}
	else
		frmLendSys->BringToFront();

	frmLendSys->WindowState = wsMaximized;
	frmLendSys->ntbLend->PageIndex=2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::MaterialRegistration1Click(TObject *Sender)
{
	if (frmMaterials==NULL)
	{
		Application->CreateForm(__classid(TfrmMaterials), &frmMaterials);
		frmMaterials->Show();
	}
	else
		frmMaterials->BringToFront();

	frmMaterials->WindowState = wsMaximized;
	frmMaterials->nbkMaterials->PageIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::MaterialBrowse1Click(TObject *Sender)
{
	if (frmMaterials==NULL)
	{
		Application->CreateForm(__classid(TfrmMaterials), &frmMaterials);
		frmMaterials->Show();
	}
	else
		frmMaterials->BringToFront();

	frmMaterials->WindowState = wsMaximized;
	frmMaterials->nbkMaterials->PageIndex=1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ShelfRegistration1Click(TObject *Sender)
{
	if (frmMaterials==NULL)
	{
		Application->CreateForm(__classid(TfrmMaterials), &frmMaterials);
		frmMaterials->Show();
	}
	else
		frmMaterials->BringToFront();

	frmMaterials->WindowState = wsMaximized;
	frmMaterials->nbkMaterials->PageIndex=2;


}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Exit1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::emplate1Click(TObject *Sender)
{
	if (frmTemplate==NULL)
	{
		Application->CreateForm(__classid(TfrmTemplate), &frmTemplate);
		frmTemplate->Show();
	}
	else
		frmTemplate->BringToFront();

	frmTemplate->WindowState = wsMaximized;
	frmTemplate->ntbMain->PageIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::About1Click(TObject *Sender)
{
	AboutBox->ShowModal();

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ChangePasswords1Click(TObject *Sender)
{
	frmPassword->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ListView1Click(TObject *Sender)
{
	if (ListView1->ItemIndex<0) return;

	if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "Material")
		MaterialRegistration1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "Users")
		UserAdmin1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "User Groups")
		UserGroups1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "Log Browse")
		LogBrowse1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "Word Bank")
		Wo1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "Story Input")
		muiStoryClick(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "LendReturn")
		Lend1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "System Settings")
		SystemSettings1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "Data Transfer")
		DataTransfer1Click(Sender);
	else if (ListView1->Items->Item[ListView1->ItemIndex]->Caption == "Template")
		emplate1Click(Sender);




}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ShowModules1Click(TObject *Sender)
{
	ShowModules1->Checked=!ShowModules1->Checked;
//	Splitter1->Visible=ShowModules1->Checked;
//	ListView1->Visible=ShowModules1->Checked;
	PageControl1->Visible=ShowModules1->Checked;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
 if (dmdStory->nGroupType>=2)
 {
		for (int i = ListView1->Items->Count-1; i >=0 ; i--) {
			  if (ListView1->Items->Item[i]->Caption == "User Groups" ||
			  ListView1->Items->Item[i]->Caption == "Users" ||
			  ListView1->Items->Item[i]->Caption == "Log Browse" ||
			  ListView1->Items->Item[i]->Caption == "System Settings" ||
			  ListView1->Items->Item[i]->Caption == "Data Transfer" ||
			  ListView1->Items->Item[i]->Caption == "Template")
			   ListView1->Items->Delete(i);
		}


	   PageControl1->Pages[1]->Free();
	   Maintenance1->Visible = false;
	   UserAdmin1->Visible = false;
	   emplate1->Visible = false;
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::InspectorBar1ItemClick(TObject *Sender, TInspectorPanel *AInspectorPanel,
		  TInspectorItem *AInspectorItem)
{
//	 Application->MessageBox(UnicodeString(AInspectorItem->Name).c_str(),L"TBS Library",MB_OK|MB_ICONINFORMATION);

	if (AInspectorItem->Name == "m1")
		MaterialRegistration1Click(Sender);
	else if (AInspectorItem->Name == "m2")
		MaterialBrowse1Click(Sender);
	else if (AInspectorItem->Name == "m3")
		ShelfRegistration1Click(Sender);
	else if (AInspectorItem->Name == "l1")
		Lend1Click(Sender);
	else if (AInspectorItem->Name == "l2")
		Return1Click(Sender);
	else if (AInspectorItem->Name == "l3")
		Browse1Click(Sender);
	else if (AInspectorItem->Name == "s1")
		muiStoryClick(Sender);
	else if (AInspectorItem->Name == "s2")
		Wo1Click(Sender);
	else if (AInspectorItem->Name == "main1")
		LogBrowse1Click(Sender);
	else if (AInspectorItem->Name == "main2")
		DataTransfer1Click(Sender);
	else if (AInspectorItem->Name == "main3")
		SystemSettings1Click(Sender);
	else if (AInspectorItem->Name == "u1")
		UserAdmin1Click(Sender);
	else if (AInspectorItem->Name == "u2")
		UserGroups1Click(Sender);
	else if (AInspectorItem->Name == "t1")
		emplate1Click(Sender);

}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   int mButton=Application->MessageBox(L"Are you sure to exit?",L"TBS Library",
					MB_OKCANCEL|MB_ICONQUESTION);
   if (mButton==ID_CANCEL)
		CanClose=false;
}
//---------------------------------------------------------------------------


