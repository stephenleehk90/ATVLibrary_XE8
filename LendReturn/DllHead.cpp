//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DllForm.h"
#include "DllHead.h"

#include "lendmain.h"
#include "lenddatas.h"
#include "PrevFrm.h"
#include "commfunc.h"
#include "dmData.h"
#include "Borrower.h"
#include "find.h"
#include "Finduser.h"
#include "options.h"
#include "InfoReport.h"
#include "findmaterail.h"
#include "tb97.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDllForm* dfrm;

#include "DllHead.inc"

int MyFormCreate(void)
{
    try {
         CLdSystemData = new TLdSystemData();
         sbStatus = dfrm->stbMain;

        Application->CreateForm(__classid(TdmLendDM), &dmLendDM);
        Application->CreateForm(__classid(TfrmLendSys), &frmLendSys);
        Application->CreateForm(__classid(TfrmBorrower), &frmBorrower);
        Application->CreateForm(__classid(TfrmOption), &frmOption);
        Application->CreateForm(__classid(TfrmFind), &frmFind);
        Application->CreateForm(__classid(TfrmFindUser), &frmFindUser);
        Application->CreateForm(__classid(TfrmPrev), &frmPrev);
        // updated 2002
        Application->CreateForm(__classid(TfrmMaterial), &frmMaterial);
        //
        try{
            Application->CreateForm(__classid(TfrmPreview), &frmPreview);
        }
         catch(...)
         {
          frmLendSys->muiPreview1->Enabled = false;
          frmLendSys->muiPrint1->Enabled   = false;
         }

         CLdSystemData->strCurrentUser = dfrm->strUserID;
         CLdSystemData->strCaption = "Library Lend";
         CLdSystemData->UserRight = dfrm->nGroupType;
         dmLendDM->dbLendDB=dfrm->dbMain;
        }
       catch (...)
        {
        return 0;
        }
     return 1;
}

int MyFormActivate(char * ActivatePage)
{
    try {
         Application->MainForm->Caption="TBS Library - "+(AnsiString)ActivatePage;
         if (((AnsiString)ActivatePage) == "Browse")
             {
              Tb97Merge(frmLendSys->Toolbar971,dfrm->dkMain);
              frmLendSys->Operation1->Visible = true;
              frmLendSys->BrowsePage(NULL);
             }
         else
         {
             Tb97Unmerge(frmLendSys->Toolbar971);
             frmLendSys->Operation1->Visible = false;
             frmLendSys->ntbLend->ActivePage=ActivatePage;
         }
         SwitchToFront(frmLendSys);
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

int MyFormDeactivate(char * ActivePage)
{
    Tb97Unmerge(frmLendSys->Toolbar971);
    return 1; // 1 means that Form is deactivated
}

int MyFormCloseQuery(void)
{
    return 1; // 1 means that Form can be closed
}

int MyFormDestroy(void)
{
    try {
         if (frmMaterial!=NULL)
          delete frmMaterial;
         if (frmLendSys!=NULL)
         delete frmLendSys;
         if (dmLendDM!=NULL)
         delete dmLendDM;
         if (frmOption!=NULL)
         delete frmOption;
         if (frmFind!=NULL)
         delete frmFind;
         if (frmFindUser!=NULL)
         delete frmFindUser;
         if (CLdSystemData!=NULL)
         delete CLdSystemData;
         if (frmPrev!=NULL)
         delete frmPrev;
         if (frmPreview!=NULL)
         delete frmPreview;
         if (frmBorrower!=NULL)
         delete frmBorrower;
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

