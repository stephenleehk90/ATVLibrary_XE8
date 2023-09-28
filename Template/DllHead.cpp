//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DllForm.h"
#include "DllHead.h"

#include "utNewExt.h"
#include "utGlobDm.h"
#include "CommFunc.h"
#include "TemplateForm.h"
#include "WizardFrm.h"
#include "ImportFrm.h"
#include "utAuth.h"
#include "tb97.h"

//USEFORM("utGlobDm.cpp", dmTemplate); /* TDataModule: DesignClass */
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDllForm* dfrm;

#include "DllHead.inc"
bool bFirst;
int MyFormCreate(void)
{
    try {
        Application->CreateForm(__classid(TdmTemplate), &dmTemplate);
        Application->CreateForm(__classid(TfrmWizard), &frmWizard);
        Application->CreateForm(__classid(TfmEditExtFields), &fmEditExtFields);
        Application->CreateForm(__classid(TfrmTemplate), &frmTemplate);
        Application->CreateForm(__classid(TfrmInout), &frmInout);
        Application->CreateForm(__classid(TfmAuthority), &fmAuthority);
        dmTemplate->m_stbTemplate = dfrm->stbMain;
        bFirst = true;
//        frmTemplate->dkMain=dfrm->dkMain;

        }
    catch (...)
        {
        return 0;
        }
    if ((frmTemplate!=NULL)&&(frmWizard!=NULL)&&(frmInout!=NULL)&&
        (dmTemplate!=NULL)&&(fmAuthority!=NULL))
         return 1;
    else return 0;
}

int MyFormActivate(char * ActivatePage)
{
    try {
        Application->MainForm->Caption="TBS Library - Tempalte Tree";
        dmTemplate->m_stbTemplate->Panels->Items[1]->Text = "Template Management";
//        Tb97Merge(frmTemplate->Toolbar971,frmTemplate->dkMain);
        dfrm->MergeTb97(frmTemplate->Toolbar971);
        if (bFirst)
        {
           frmTemplate->RefreshTree();
           bFirst = false;
        }
        SwitchToFront(frmTemplate);
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

int MyFormDeactivate(char * ActivePage)
{
    Tb97Unmerge(frmTemplate->Toolbar971);
    return 1; // 1 means that Form is deactivated
}

int MyFormCloseQuery(void)
{
    return 1; // 1 means that Form can be closed
}

int MyFormDestroy(void)
{
    try {
        if (frmTemplate!=NULL)
          delete frmTemplate;
        if (frmWizard!=NULL)
          delete frmWizard;
        if (frmInout!=NULL)
          delete frmInout;
        if (dmTemplate != NULL)
            delete dmTemplate;
        if (fmEditExtFields != NULL)
            delete fmEditExtFields;
        if (fmAuthority != NULL)
            delete fmAuthority;
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

