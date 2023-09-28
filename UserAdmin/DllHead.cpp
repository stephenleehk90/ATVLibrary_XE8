//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DllForm.h"
#include "DllHead.h"

#include "UserData.h"
#include "UserFrm.h"
#include "SelectUserFrm.h"
#include "SelectGroupFrm.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDllForm* dfrm;

#include "DllHead.inc"

int MyFormCreate(void)
{
    try {
        Application->CreateForm(__classid(TdmdUserAdmin), &dmdUserAdmin);
        dmdUserAdmin->stbUserAdmin=dfrm->stbMain;
        Application->CreateForm(__classid(TfrmUserAdmin), &frmUserAdmin);
        Application->CreateForm(__classid(TfrmSelectUsers), &frmSelectUsers);
        Application->CreateForm(__classid(TfrmSelectGroups), &frmSelectGroups);
        frmUserAdmin->strCurrentUserID=dfrm->strUserID;
        frmUserAdmin->nGroupType=dfrm->nGroupType;
        }
    catch (...)
        {
        return 0;
        }
    if ((frmUserAdmin!=NULL)&&(frmSelectUsers!=NULL)&&(frmSelectGroups!=NULL)&&(dmdUserAdmin!=NULL))
         return 1;
    else return 0;

}

int MyFormActivate(char * ActivatePage)
{
    AnsiString strCaption;
    try {
        strCaption=ActivatePage;

        if (strCaption=="Users")
        {
          Tb97Unmerge(frmUserAdmin->Toolbar972);
          Tb97Merge(frmUserAdmin->Toolbar971,dfrm->dkMain);
          frmUserAdmin->UserPage();
        }
        else if (strCaption=="UserGroups")
        {
          Tb97Unmerge(frmUserAdmin->Toolbar971);
          Tb97Merge(frmUserAdmin->Toolbar972,dfrm->dkMain);
          frmUserAdmin->GroupPage();
        }
        SwitchToFront(frmUserAdmin);
        }
    catch (...)
        {
        return 0;
        }
    return 1;

}

int MyFormDeactivate(char * ActivePage)
{
    Tb97Unmerge(frmUserAdmin->Toolbar971);
    Tb97Unmerge(frmUserAdmin->Toolbar972);
    dmdUserAdmin->stbUserAdmin->Panels->Items[1]->Text="";
    dmdUserAdmin->stbUserAdmin->Panels->Items[2]->Text="";
    return 1; // 1 means that Form is deactivated
}

int MyFormCloseQuery(void)
{
    return 1; // 1 means that Form can be closed
}

int MyFormDestroy(void)
{
    try {
        if (frmUserAdmin!=NULL)
          delete frmUserAdmin;
        if (frmSelectUsers!=NULL)
          delete frmSelectUsers;
        if (frmSelectGroups!=NULL)
          delete frmSelectGroups;
        if (dmdUserAdmin!=NULL)
          delete dmdUserAdmin;
        }
    catch (...)
        {
        return 0;
        }
    return 1;

}

