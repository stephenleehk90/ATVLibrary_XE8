//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DllForm.h"
#include "DllHead.h"

#include "CommFunc.h"
#include "tb97.h"

#include "frmWord.h"
#include "SearchForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDllForm* dfrm;

#include "DllHead.inc"

int MyFormCreate(void)
{
   try {
        Application->CreateForm(__classid(TfrmWB), &frmWB);
        Application->CreateForm(__classid(TfrmSearch), &frmSearch);
        frmWB->Caption="Welcome";
        }
    catch (...)
        {
        return 0;
        }
    if ( (frmWB!=NULL) && (frmSearch!=NULL) )
         return 1;
    else return 0;
}

int MyFormActivate(char * ActivatePage)
{
    int w1,w2;
    try {
        Application->MainForm->Caption="TBS Library - Word Bank";
        dfrm->stbMain->Panels->Items[1]->Text ="";
        if (frmWB->tbWord->Active==false)
           {
           frmWB->tbWord->Active=true;

           w1=GetIniInt("WordBank","FirstWidth",300);
           if ( w1< 100) w1=100;
           w2=GetIniInt("WordBank","SecondWidth",300);
           if ( w2< 100) w2=100;

           if  ( GetIniInt("WordBank","Sequence",0)==1 )
             {
             frmWB->dgdWord->Columns->Items[0]->FieldName="Chinese";
             frmWB->dgdWord->Columns->Items[1]->FieldName="English";
             }
           frmWB->dgdWord->Columns->Items[0]->Width=w1;
           frmWB->dgdWord->Columns->Items[1]->Width=w2;
           }
        Tb97Merge(frmWB->tb97Word,dfrm->dkMain);

        SwitchToFront(frmWB);
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

int MyFormDeactivate(char * ActivePage)
{
    try {
        dfrm->stbMain->Panels->Items[1]->Text ="";
        dfrm->UnmergeTb97(frmWB->tb97Word);
        }
    catch (...)
        {
        return 0;
        }
    return 1; // 1 means that Form is deactivated
}

int MyFormCloseQuery(void)
{
    return 1; // 1 means that Form can be closed
}

int MyFormDestroy(void)
{
    try {
        SetIniInt("WordBank","FirstWidth",
            frmWB->dgdWord->Columns->Items[0]->Width);
        SetIniInt("WordBank","SecondWidth",
            frmWB->dgdWord->Columns->Items[1]->Width);

        if (frmWB->dgdWord->Columns->Items[0]->FieldName=="Chinese")
            SetIniInt("WordBank","Sequence",1);
        else
            SetIniInt("WordBank","Sequence",0);

        if (frmWB->tbWord->Active==true)
           frmWB->tbWord->Active=false;
        if (frmWB!=NULL)
          delete frmWB;
        if (frmSearch!=NULL)
            delete frmSearch;
        }
    catch (...)
        {
        return 0;
        }
    return 1;
}

