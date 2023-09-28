//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChangeMaterialIDForm.h"
#include "StoryData.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmChangeM *frmChangeM;
//---------------------------------------------------------------------------
__fastcall TfrmChangeM::TfrmChangeM(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------/*void __fastcall TfrmChangeM::btnOkClick(TObject *Sender)
/*void __fastcall TfrmChangeM::btnOkClick(TObject *Sender)
{
   AnsiString strSQLClause;
   AnsiString strError;
   bool bOldExist,bNewExist;
   bool bOldOnShelf,bNewOnShelf;
   int  nRowsAffected;
   int  nState;
   int  i;
   int  nCurrentYear;

   if (edtOldID->Text.Trim().IsEmpty())
    {
      Application->MessageBox("Please fill the Old ID.","TBS Library",
         MB_OK);
      edtOldID->SetFocus();
      ModalResult = mrNone;
      return;
    }
   if (edtNewID->Text.Trim().IsEmpty())
    {
      Application->MessageBox("Please fill the New ID.","TBS Library",
         MB_OK);
      edtNewID->SetFocus();
      ModalResult = mrNone;
      return;
    }
   if (edtNewID->Text.Trim().UpperCase()[edtNewID->Text.Trim().Length()]=='S')
   {
     Application->MessageBox("The Material ID's last char can NOT be 's' or 'S',please input again.","TBS Library",MB_OK|MB_ICONINFORMATION);
     edtNewID->SetFocus();
     edtNewID->SelectAll();
     ModalResult = mrNone;
     return;
   }

   try{
      strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
      nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

      strSQLClause = (AnsiString)"select ResID,state from lib_Res where ResID='"
                     +SQLStr(edtOldID->Text.Trim())+(AnsiString)"'";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
      if (dmdStory->daqCheck->RecordCount<=0)
         bOldExist = false;
      else
      {
         bOldExist = true;
         nState = dmdStory->daqCheck->FieldByName("state")->AsInteger;
         if (nState==0)
             bOldOnShelf = true;
         else
             bOldOnShelf = false;
      }
      strSQLClause = (AnsiString)"select ResID,state from lib_Res where ResID='"
                     +SQLStr(edtNewID->Text.Trim())+(AnsiString)"'";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
      if (dmdStory->daqCheck->RecordCount<=0)
         bNewExist = false;
      else
      {
         bNewExist = true;
         nState = dmdStory->daqCheck->FieldByName("state")->AsInteger;
         if (nState==0)
             bNewOnShelf = true;
         else
             bNewOnShelf = false;
      }
      if ((bOldExist)&&(!bOldOnShelf))
      {
          strError = (AnsiString)"'"+edtOldID->Text+(AnsiString)"' is not on shelf.";
          Application->MessageBox(strError.c_str(),"TBS Library",MB_OK|MB_ICONINFORMATION);
          edtOldID->SetFocus();
          edtOldID->SelectAll();
          ModalResult = mrNone;
          return;
      }
      else
      {
         if ((bOldExist)&&(bNewExist))
         {
            strError = (AnsiString)"'"+edtNewID->Text+(AnsiString)"' already exists.";
            Application->MessageBox(strError.c_str(),"TBS Library",MB_OK|MB_ICONINFORMATION);
            edtNewID->SetFocus();
            edtNewID->SelectAll();
            ModalResult = mrNone;
            return;
         }
         if ((bNewExist)&&(!bNewOnShelf))
         {
            strError = (AnsiString)"'"+edtNewID->Text+(AnsiString)"' is not on shelf.";
            Application->MessageBox(strError.c_str(),"TBS Library",MB_OK|MB_ICONINFORMATION);
            edtNewID->SetFocus();
            edtNewID->SelectAll();
            ModalResult = mrNone;
            return;
         }
      }

      if (qryChange->Active) qryChange->Close();

      qryChange->SQL->Clear();
      qryChange->SQL->Add("update dbo.lib_Res set ResID = '"
            + SQLStr(edtNewID->Text.Trim()) +"' where ResID = '" +
            SQLStr(edtOldID->Text.Trim()) +
            "'");
      qryChange->ExecSQL();
      qryChange->Close();

      qryChange->SQL->Clear();
      qryChange->SQL->Add("update dbo.lib_new"+strLib+" set MaterialID = '"
            + SQLStr(edtNewID->Text) +"' where MaterialID = '" +
            SQLStr(edtOldID->Text) +
            "'");
      qryChange->SQL->Add("update dbo.lib_old"+strLib+" set MaterialID = '"
            + SQLStr(edtNewID->Text) +"' where MaterialID = '" +
            SQLStr(edtOldID->Text) +
            "'");
	  for (i=1990;i<nCurrentYear+1;i++)
      {
        qryChange->SQL->Add("update dbo.lib_"+IntToStr(i)+"_"+strLib+" set MaterialID = '"
              + SQLStr(edtNewID->Text) +"' where MaterialID = '" +
              SQLStr(edtOldID->Text) +
              "'");

      }
      qryChange->ExecSQL();
      nRowsAffected = qryChange->RowsAffected;
      qryChange->Close();
      if (nRowsAffected<0)
          ModalResult = mrCancel;
   }
   catch(...)
   {
      Application->MessageBox("Something wrong with Database.","TBS Library",
         MB_OK);
      ModalResult = mrNone;
      return;
   }
} */
//---------------------------------------------------------------------------
void __fastcall TfrmChangeM::FormShow(TObject *Sender)
{
    edtOldID->Text="";
    edtNewID->Text="";
    edtOldID->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmChangeM::btnOkClick(TObject *Sender)
{
   AnsiString strSQLClause;
   AnsiString strError;
   int  nRowsAffected;
   int  i;
   int  nCurrentYear;

   if (edtOldID->Text.Trim().IsEmpty())
    {
	  Application->MessageBox(L"Please fill the Old ID.",L"TBS Library",
         MB_OK);
      edtOldID->SetFocus();
      ModalResult = mrNone;
      return;
    }
   if (edtNewID->Text.Trim().IsEmpty())
    {
	  Application->MessageBox(L"Please fill the New ID.",L"TBS Library",
         MB_OK);
      edtNewID->SetFocus();
      ModalResult = mrNone;
      return;
    }
   if (edtNewID->Text.Trim().UpperCase()[edtNewID->Text.Trim().Length()]=='S')
   {
	 Application->MessageBox(L"The Material ID's last char can NOT be 's' or 'S',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     edtNewID->SetFocus();
     edtNewID->SelectAll();
     ModalResult = mrNone;
     return;
   }

   try{
      strSQLClause = (AnsiString)"select year = datepart(yy,getdate())";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
      nCurrentYear = dmdStory->daqCheck->FieldValues["year"];

      strSQLClause = (AnsiString)"select ResID,state from lib_Res where ResID='"
                     +SQLStr(edtNewID->Text.Trim())+(AnsiString)"'";
      dmdStory->daqCheck->SQL->Clear();
      dmdStory->daqCheck->SQL->Add(strSQLClause);
      dmdStory->daqCheck->Open();
      if (dmdStory->daqCheck->RecordCount<=0)
      {
		Application->MessageBox(L"The New Material does not exist in the library,please change another.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        edtNewID->SetFocus();
        edtNewID->SelectAll();

        ModalResult = mrNone;
        return;
      }
      if (qryChange->Active) qryChange->Close();

      qryChange->SQL->Clear();
      qryChange->SQL->Add("update dbo.lib_Res set AstLibs = '"
            + SQLStr(strLib) +"' where ResID = '" +
            SQLStr(edtNewID->Text.Trim()) +
            "'");
      qryChange->SQL->Add("update dbo.lib_Res set AstLibs = '.' where ResID = '" +
            SQLStr(edtNewID->Text.Trim()) +
            "'");
      qryChange->ExecSQL();
      qryChange->Close();

      qryChange->SQL->Clear();
      qryChange->SQL->Add("update dbo.lib_new"+strLib+" set MaterialID = '"
            + SQLStr(edtNewID->Text) +"' where MaterialID = '" +
            SQLStr(edtOldID->Text) +
            "'");
      qryChange->SQL->Add("update dbo.lib_old"+strLib+" set MaterialID = '"
            + SQLStr(edtNewID->Text) +"' where MaterialID = '" +
            SQLStr(edtOldID->Text) +
            "'");
	  for (i=1990;i<nCurrentYear+1;i++)
	  {

	  dmdStory->daqCheckTable->Close();
	  dmdStory->daqCheckTable->SQL->Clear();
	  dmdStory->daqCheckTable->SQL->Add((AnsiString)"select * from sysobjects where id = object_id('lib_"+
														   String(i)+ "_"+ strLib+ "')");
	  dmdStory->daqCheckTable->Open();
	  if (dmdStory->daqCheckTable->RecordCount <= 0) continue;

		qryChange->SQL->Add("update dbo.lib_"+IntToStr(i)+"_"+strLib+" set MaterialID = '"
              + SQLStr(edtNewID->Text) +"' where MaterialID = '" +
              SQLStr(edtOldID->Text) +
              "'");

      }
      qryChange->ExecSQL();
      nRowsAffected = qryChange->RowsAffected;
	  qryChange->Close();
      if (nRowsAffected<0)
          ModalResult = mrCancel;
   }
   catch(...)
   {
	  Application->MessageBox(L"Something wrong with Database.",L"TBS Library",
		 MB_OK);
      ModalResult = mrNone;
      return;
   }

}
//---------------------------------------------------------------------------

