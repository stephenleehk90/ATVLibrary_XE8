//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DllForm.h"
#include "DllHead.h"

#include "MaterialData.h"
#include "MaterialFrm.h"
#include "PreviewFrm.h"
#include "PrevFrm.h"
#include "ShelfFrm.h"
#include "QueryFrm.h"
#include "CommFunc.h"
#include "tb97.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDllForm* dfrm;

#include "DllHead.inc"

int MyFormCreate(void)
{

    try {
        Application->CreateForm(__classid(TdmdMaterials), &dmdMaterials);
        Application->CreateForm(__classid(TfrmMaterials), &frmMaterials);
        Application->CreateForm(__classid(TfrmQuery), &frmQuery);
        Application->CreateForm(__classid(TfrmShelf), &frmShelf);
        Application->CreateForm(__classid(TfrmPrev), &frmPrev);
        try{
        Application->CreateForm(__classid(TfrmPreview), &frmPreview);
        }
        catch(...)
        {
          frmMaterials->muiPreview->Enabled=false;
        }
        frmMaterials->strUserID=dfrm->strUserID;
        frmMaterials->stbMaterial=dfrm->stbMain;
        }
    catch (...)
        {
        return 0;
        }
    if ((frmMaterials!=NULL)&&(frmQuery!=NULL)&&(frmShelf!=NULL)
         &&(frmPrev!=NULL)&&(dmdMaterials!=NULL))
         return 1;
    else return 0;
}

int MyFormActivate(char * ActivatePage)
{
    AnsiString strPage;
    strPage=(AnsiString)ActivatePage;
    try {
        if (strPage=="Material Registration")
        {
            Tb97Unmerge(frmMaterials->Toolbar971);
            frmMaterials->RegistrationPage();
        }
        if (strPage=="Material Browse")
        {
            Tb97Merge(frmMaterials->Toolbar971,dfrm->dkMain);
            frmMaterials->BrowsePage();
        }
        if (strPage=="Shelf Registration")
        {
            Tb97Unmerge(frmMaterials->Toolbar971);
            frmMaterials->PositionPage();
        }
        SwitchToFront(frmMaterials);
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

int MyFormDeactivate(char * ActivePage)
{
    Tb97Unmerge(frmMaterials->Toolbar971);
    frmMaterials->stbMaterial->Panels->Items[1]->Text="";
    frmMaterials->stbMaterial->Panels->Items[2]->Text="";
    return 1; // 1 means that Form is deactivated
}

int MyFormCloseQuery(void)
{
    return 1; // 1 means that Form can be closed
}

int MyFormDestroy(void)
{
    try {
        if (frmMaterials!=NULL)
           delete frmMaterials;
        if (frmQuery!=NULL)
          delete frmQuery;
        if (frmShelf!=NULL)
          delete frmShelf;
        if (frmPreview!=NULL)
          delete frmPreview;
        if (frmPrev!=NULL)
          delete frmPrev;

        if (dmdMaterials!=NULL)
          delete dmdMaterials;
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

