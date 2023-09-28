//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DllForm.h"
#include "DllHead.h"

#include "CommFunc.h"
#include "DataProcess.h"
#include "DirectoryFrm.h"
#include "LogFrm.h"
#include "SaveFrm.h"
#include "QueryFrm.h"
#include "SourceTbFrm.h"
#include "SelectFieldFrm.h"
#include "LogData.h"
#include "tb97.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDllForm* dfrm;

#include "utGlDm.h"
#include "DllHead.inc"

int MyFormCreate(void)
{
   try {
        Application->CreateForm(__classid(TdmdLog), &dmdLog);
        Application->CreateForm(__classid(TfrmDirectory), &frmDirectory);
        dmdLog->stbLog=dfrm->stbMain;
        Application->CreateForm(__classid(TfrmLogs), &frmLogs);
        Application->CreateForm(__classid(TfrmQuery), &frmQuery);
        Application->CreateForm(__classid(TfrmSave), &frmSave);
        Application->CreateForm(__classid(TfrmSourceTbs), &frmSourceTbs);
        Application->CreateForm(__classid(TfrmSelectFields), &frmSelectFields);
        frmLogs->Caption="EventLog [Log Browse]";
        frmLogs->strUserID=dfrm->strUserID;
        }
    catch (...)
        {
        return 0;
        }
    if ((frmLogs!=NULL)&&(frmQuery!=NULL)&&(frmSave!=NULL)&&(dmdLog!=NULL)&&(frmSourceTbs!=NULL)
       &&(frmSelectFields!=NULL)&&(frmDirectory != NULL))
         return 1;
    else return 0;
}

int MyFormActivate(char * ActivatePage)
{
    AnsiString strCaption;
    try {
        strCaption=ActivatePage;
        if (strCaption=="Log Browse")
        {
           Tb97Merge(frmLogs->Toolbar971,dfrm->dkMain);
           RefreshLogs(frmLogs->LogBegin,frmLogs->LogEnd,frmLogs->strOpID);
           frmLogs->nbkDatabase->PageIndex=0;
           Application->MainForm->Caption="TBS Library - Log Browse";
           frmLogs->muiLog->Visible=true;
        }
        if (strCaption=="Database Transfer")
        {
           Tb97Unmerge(frmLogs->Toolbar971);
           frmLogs->nbkDatabase->PageIndex=1;
           Application->MainForm->Caption="TBS Library - Database Transfer";
           dmdLog->stbLog->Panels->Items[1]->Text = "";
           frmLogs->muiLog->Visible=false;

           frmLogs->edtDestTb->Clear();
           frmLogs->edtSourceTb->Clear();
           frmLogs->chkOld->Checked = false;
           frmLogs->btnTransfer->Enabled = false;
           frmLogs->lstvwFields->Items->Clear();
        }
        if (strCaption=="System Settings")
        {
           Tb97Unmerge(frmLogs->Toolbar971);
           frmLogs->nbkDatabase->PageIndex=2;
           Application->MainForm->Caption="TBS Library - System Settings";
           dmdLog->stbLog->Panels->Items[1]->Text = "";
           frmLogs->muiLog->Visible=false;

           if (frmLogs->m_strCodeType == LANGUAGE_BIG5)
               frmLogs->pnCharSet->Caption = "BIG5";
           else
               frmLogs->pnCharSet->Caption = "GB";
        }

        SwitchToFront(frmLogs);
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

int MyFormDeactivate(char * ActivePage)
{
    Tb97Unmerge(frmLogs->Toolbar971);
    dmdLog->stbLog->Panels->Items[1]->Text = "";
    dmdLog->stbLog->Panels->Items[2]->Text = "";
    return 1; // 1 means that Form is deactivated
}

int MyFormCloseQuery(void)
{
    return 1; // 1 means that Form can be closed
}

int MyFormDestroy(void)
{
    try {
        if (frmLogs!=NULL)
          delete frmLogs;
        if (frmQuery!=NULL)
          delete frmQuery;
        if (frmSave!=NULL)
          delete frmSave;
        if (frmSourceTbs!=NULL)
          delete frmSourceTbs;
        if (frmSelectFields!=NULL)
          delete frmSelectFields;
        if (dmdLog!=NULL)
          delete dmdLog;
        if (frmDirectory!=NULL)
          delete frmDirectory;
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

