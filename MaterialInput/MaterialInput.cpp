//---------------------------------------------------------------------------
#include <vcl.h>
#include <dbtables.hpp>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be perfomring new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------
USEFORM("MaterialFrm.cpp", frmMaterials);
USEFORM("PreviewFrm.cpp", frmPreview);
USEFORM("QueryFrm.cpp", frmQuery);
USEFORM("ShelfFrm.cpp", frmShelf);
//USEFORM("MaterialData.cpp", dmdMaterials); /* TDataModule: DesignClass */
USEUNIT("CommFunc.cpp");
USEUNIT("DataProcess.cpp");
USEUNIT("DllHead.cpp");
USEUNIT("DllForm.cpp");
USEFILE("DllHead.inc");
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}

//---------------------------------------------------------------------------


