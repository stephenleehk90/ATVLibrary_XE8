//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <inifiles.hpp>

#include "frmWord.h"
#include "SearchForm.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "TB97"
#pragma resource "*.dfm"
TfrmWB *frmWB;
//---------------------------------------------------------------------------
__fastcall TfrmWB::TfrmWB(TComponent* Owner)
    : TForm(Owner)
{
	  TIniFile* IniFile=new TIniFile(IniName());
   AnsiString strLoginID=IniFile->ReadString("Database Connection","UserName","U_TDvn[l|cx");
   AnsiString strPassword=IniFile->ReadString("Database Connection","PassWord","dqymbibaeuo{}d");

   dbMain->Params->Add((AnsiString)"USER NAME="+strLoginID);
   dbMain->Params->Add((AnsiString)"PASSWORD="+strPassword);

   try
   {
	 dbMain->Open();
   }
   catch(...)
   {
	 Application->MessageBox(L"Database Error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmWB::dgdWordKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

// 当按下  INSERT或者Down时自动移到English列
   if  ( (Key==VK_INSERT) || (Key==VK_DOWN) )
       dgdWord->SelectedField=tbWord->FieldByName("English");
}
//---------------------------------------------------------------------------



void __fastcall TfrmWB::bt97SearchClick(TObject *Sender)
{
    TLocateOptions SearchOptions;
    AnsiString str;
/*
    TfrmSearch *frmSearch;

    frmSearch=new TfrmSearch(this);
    frmSearch->Parent=this;
    frmSearch->Visible=false;
    frmSearch->ActiveControl=NULL;
*/
    str=frmSearch->Execute();

//    delete frmSearch;

    if (str=="") return;

    SearchOptions.Clear();
    SearchOptions << loPartialKey <<loCaseInsensitive;
	if (!tbWord->Locate("English",str,SearchOptions))
        tbWord->Locate("Chinese",str,SearchOptions);

}
//---------------------------------------------------------------------------

void __fastcall TfrmWB::tbWordPostError(TDataSet *DataSet,
      EDatabaseError *E, TDataAction &Action)
{
   Action = daAbort;
   Application->MessageBox(L"The word is not unique,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TfrmWB::FormShow(TObject *Sender)
{
tbWord->Active = true;
}
//---------------------------------------------------------------------------

