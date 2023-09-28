//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#pragma hdrstop

USEDATAMODULE("dmData.cpp", dmLendDM);
USEUNIT("lenddatas.cpp");
USEFORM("InfoReport.cpp", frmPreview);
USEFORM("options.cpp", frmOption);
USEFORM("find.cpp", frmFind);
USEUNIT("CommFunc.cpp");
USEUNIT("DllForm.cpp");
USEUNIT("DllHead.cpp");
USEFORM("lendmain.cpp", frmLendSys);
USEFORM("Borrower.cpp", frmBorrower);
USEFORM("FindUser.cpp", frmFindUser);
USEFORM("PrevFrm.cpp", frmPrev);
USEFORM("FindMaterail.cpp", frmMaterial);
USERES("LendReturn.res");
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}

