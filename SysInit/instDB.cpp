//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("utInst.cpp", fmInit);
USERES("instDB.res");
USEDATAMODULE("utdmglob.cpp", dmGlobal);
USEUNIT("CommFunc.cpp");
USEFORM("utConfm.cpp", fmConfirm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TfmInit), &fmInit);
        Application->CreateForm(__classid(TdmGlobal), &dmGlobal);
        Application->CreateForm(__classid(TfmConfirm), &fmConfirm);
        Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
