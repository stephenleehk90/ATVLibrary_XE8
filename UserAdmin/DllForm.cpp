//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DllForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool KeepOnly1(const char* UniName)
{
    HANDLE hOnly1, h;
    char* cstr1;
    AnsiString str1;

    hOnly1=CreateMutex(NULL,true,UniName);
    if (hOnly1!=0)
        {
        if (GetLastError() == ERROR_ALREADY_EXISTS )
            {
  	        CloseHandle(hOnly1);
            str1=Application->ExeName;
            str1.SetLength(str1.Length()-4);
            cstr1=strrchr(str1.c_str(),'\\');
            if (cstr1==NULL)
                cstr1=str1.c_str();
            else
                cstr1++;
            //   find the current window's handle
			h = FindWindow(NULL, cstr1);
            // change the current window's name to something else (literally)<g>
            SetWindowText(h, "something else");
            // now, we can find the previous window's handle (the one we really want!)
			h = FindWindow(NULL, cstr1);
            // bring the first instance to the front
            if (h != 0 )
  			    {
        	    if (IsIconic(h))
          	        ShowWindow(h,SW_RESTORE);
                else
                    SetForegroundWindow(h);
                }
            return false;
            }
        else{
            return true;
            }
        }
   	else
        return true;
}

void SwitchToFront(TForm* MDIChild)
{
    int j, WindowFrameThick, WindowCaptionHeight;

    WINDOWPLACEMENT w1;
    RECT r1;
    HWND h1;

    h1=GetWindow(MDIChild->Handle,GW_HWNDFIRST);
    if (h1==MDIChild->Handle) return;

    GetClientRect(Application->MainForm->ClientHandle,&r1);

    w1.length=sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(h1,&w1);

    WindowFrameThick=-w1.ptMaxPosition.x;
    WindowCaptionHeight=-w1.ptMaxPosition.y;
    w1.rcNormalPosition.left=-WindowFrameThick;
    w1.rcNormalPosition.top=-WindowCaptionHeight;
    w1.rcNormalPosition.right=r1.right+WindowFrameThick;
    w1.rcNormalPosition.bottom=r1.bottom+WindowFrameThick;
    SetWindowPlacement(h1,&w1);

    MDIChild->Left=-WindowFrameThick;
    MDIChild->Width=r1.right+2*WindowFrameThick;
    MDIChild->Top=-WindowCaptionHeight;
    MDIChild->Height=r1.bottom+WindowFrameThick+WindowCaptionHeight;

    j=GetWindowLong(MDIChild->Handle,GWL_STYLE);
    SetWindowLong(MDIChild->Handle,GWL_STYLE,j|WS_MAXIMIZE);
    MDIChild->BringToFront();
    return;
}

//---------------------------------------------------------------------------
void Tb97Merge(TToolbar97* tb, TDock97* td)
{
    try{
        tb->DockPos=1;
        tb->DockedTo=td;
        tb->Visible=true;;
        }
    catch(...)
    {} //<TBC>
}

void Tb97Unmerge(TToolbar97* tb)
{
    tb->Visible=false;
    tb->DockedTo=NULL;
}

void Tb97UnmergeAll(TDock97* td)
{
    int i,j,s=td->ToolbarCount;
    TCustomToolWindow97* tb;
    for (i=0,j=0;i<s; i++)
        {
        tb=td->Toolbars[j];
        if (tb->Owner!=td->Owner)
            {
            tb->Visible=false;
            tb->Parent=(TWinControl*)tb->Owner;
            }
        else j++;
        }
}
//---------------------------------------------------------------------------
TDllForm::TDllForm()
{
    bMyFormExist=false;
}

TDllForm::~TDllForm()
{
    if (bMyFormExist) FormDestroy();
    //<TBC>
}

int TDllForm::FormCreate(void)
{
    int i=0;
    if (OnFormCreate!=NULL)
        {
        try{
            i=OnFormCreate();
            }
        catch(...)
        {
            return 0;
        } //<TBC>
       }
    if (i==1) bMyFormExist = true;
    return i;
}

int TDllForm::FormActivate(char * strPage)
{
    int i=0;
    if (!bFormExist) return 0;
    if (OnFormActivate!=NULL)
        {
        try{
            i=OnFormActivate(strPage);
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    return i;
}

int TDllForm::FormDeactivate(char* strPage)
{
    int i=1;
    if (!bFormExist) return 1;
    if (OnFormDeactivate!=NULL)
        {
        try{
            i=OnFormDeactivate(strPage);
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    return i;
}

int TDllForm::FormCloseQuery(void)
{
    int i=1;
    if (OnFormCloseQuery!=NULL)
        {
        try{
            i=OnFormCloseQuery();
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    return i;
}

int TDllForm::FormDestroy(void)
{
    int i=1;
    if (OnFormDestroy!=NULL)
        {
        try{
            i=OnFormDestroy();
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    if (i==1) bMyFormExist=false;
    return i;
}

void TDllForm::MergeTb97(TToolbar97* tb)
{
    if ( (dkMain!=NULL)&&(tb!=NULL) )
        {
        try{
            Tb97Merge(tb,dkMain);
            }
        catch(...){} //<TBC>
        }
}

void TDllForm::UnmergeTb97(TToolbar97* tb)
{
    if (tb!=NULL)
        {
        try{
            Tb97Unmerge(tb);
            }
        catch(...){} //<TBC>
        }
}
//---------------------------------------------------------------------------

AnsiString FileName(char* pathname)
{
    char* p;
    AnsiString str1;
    p=strrchr(pathname,'\\');
    p++;
    str1=AnsiString(p);
    str1.Delete(str1.Length()-3,4);
    return str1;
}

TDllLoader::TDllLoader(char* DllName,AnsiString strUserID,int nGroupType,
                  TStatusBar* stbMain, TDatabase* dbMain, TDock97* dkMain )
{
    try {
        hDll=LoadLibrary(DllName);
        MyDllName=FileName(DllName);
        if (hDll != NULL) {
            MyDllCreate=FDllCreate(GetProcAddress(hDll,"_DllCreate"));
            MyDllDestroy=FDllDestroy(GetProcAddress(hDll,"_DllDestroy"));
            }
        bMyLoadOK=(hDll!=NULL)&&(MyDllCreate!=NULL)&&(MyDllDestroy!=NULL);
        }
    catch (...) {
        bMyLoadOK=false;
        }
    if (bMyLoadOK)
        {
        try{
            MyDllForm=MyDllCreate(Application,Sessions,Session);
            MyDllForm->strUserID=strUserID;
            MyDllForm->nGroupType=nGroupType;
            MyDllForm->stbMain=stbMain;
            MyDllForm->dbMain=dbMain;
            MyDllForm->dkMain=dkMain;
            MyDllForm->FormCreate();
            }
        catch(...)
            {
            bMyLoadOK=false;
            }//<TBC>
        }
}

TDllLoader::~TDllLoader(void)
{
    try {
        if (bMyLoadOK) MyDllDestroy();
        }
    catch(...){}//<TBC>
    try {
        if (hDll!=NULL) FreeLibrary(hDll);
        }
    catch(...){}//<TBC>
}

int TDllLoader::FormCreate(void)
{
    int i=0;
    if  ( bMyLoadOK )
        {
        try{
            i=MyDllForm->FormCreate();
            }
        catch(...)
        {
            return 0;
        } //<TBC>
       }
    return i;
}

int TDllLoader::FormActivate(char * strPage)
{
    int i=0;
    if (bMyLoadOK)
        {
        try{
            i=MyDllForm->FormActivate(strPage);
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    return i;
}

int TDllLoader::FormDeactivate(char * strPage)
{
    int i=1;
    if (bMyLoadOK)
        {
        try{
            i=MyDllForm->FormDeactivate(strPage);
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    return i;
}

int TDllLoader::FormCloseQuery(void)
{
    int i=1;
    if (bMyLoadOK)
        {
        try{
            i=MyDllForm->FormCloseQuery();
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    return i;
}

int TDllLoader::FormDestroy(void)
{
    int i=1;
    if (bMyLoadOK)
        {
        try{
            i=MyDllForm->FormDestroy();
            }
        catch(...)
        {
            return 0;
        } //<TBC>
        }
    return i;
}
//---------------------------------------------------------------------------

TDllLoaderList::TDllLoaderList(AnsiString strUserID1, int nGroupType1,
               TStatusBar* stbMain1, TDatabase* dbMain1, TDock97* dkMain1 )
{
    strUserID=strUserID1;
    nGroupType=nGroupType1;
    stbMain=stbMain1;
    dbMain=dbMain1;
    dkMain=dkMain1;
    lstLoader=new TList();
}

TDllLoaderList::~TDllLoaderList(void)
{
    ClearLoader();
    delete lstLoader;
}

TDllLoader* TDllLoaderList::GetLoader(int n)
{
    if (n<lstLoader->Count) return (TDllLoader*)(lstLoader->Items[n]);
        else return NULL;
}

TDllLoader* TDllLoaderList::LoaderByName(AnsiString str1)
{
    int i;
    TDllLoader* p;
    for (i=0; i<lstLoader->Count; i++)
        {
        p=(TDllLoader*)(lstLoader->Items[i]);
        if ( (p!=NULL) && (p->strDllName==str1 ) )
            return p;
        }
    return NULL;
}


int TDllLoaderList::AddLoader(char* DllName)
{
    TDllLoader* DllLoader;
    try{
        DllLoader=new TDllLoader(DllName,strUserID,nGroupType,
                            stbMain,dbMain,dkMain);
        lstLoader->Add(DllLoader);
        }
    catch(...)
        {
        return 0;
        }//<TBC>
    if (DllLoader!=NULL) return 1;
        else return 0;

}

int TDllLoaderList::DeleteLoader(char* DllName)
{
    int i;
    TDllLoader* p;
    for (i=0; i<lstLoader->Count; i++)
        {
        p=(TDllLoader*)(lstLoader->Items[i]);
        if ( (p!=NULL) && (p->strDllName==(AnsiString)DllName ) )
            {
            try{
                lstLoader->Delete(i);
                delete p;
                }
            catch(...)
                {
                return 0;
                } //<TBC>
            }
        }
    return 1;
}

int TDllLoaderList::ClearLoader(void)
{
    int i;
    TDllLoader* p;
    for (i=0; i<lstLoader->Count; i++)
        {
        p=(TDllLoader*)(lstLoader->Items[i]);
        if (p!=NULL)
            {
            try{
                lstLoader->Delete(i);
                delete p;
                }
            catch(...)
                {
                return 0;
                } //<TBC>
            }
        }
    return 1;
}

int TDllLoaderList::ActivateLoader(char* DllName,char* PageName)
{
    TDllLoader* p;
    if ((ActiveDllName==(AnsiString)DllName)&&
         (ActivePage==(AnsiString)PageName))
         return 1;

    p=LoaderByName(ActiveDllName);
    if ( (p!=NULL) && (p->FormDeactivate(ActivePage.c_str())==0) )
            return 0;

    p=LoaderByName(DllName);
    if ((p!=NULL) && (p->FormActivate(PageName)==1))
        {
        ActiveDllName=(AnsiString)DllName;
        ActivePage=(AnsiString)PageName;
        return 1;
        }
        else return 0;
}

int TDllLoaderList::DeactivateLoader(char* DllName, char* PageName)
{
    TDllLoader * p;
    p=LoaderByName(DllName);
    if (p!=NULL) return p->FormDeactivate(PageName);
        else return 1; // ²»´æÔÚ¼´inactive
}

int TDllLoaderList::CloseQuery(void)
{
    int i;
    TDllLoader* p;
    for (i=0;i<lstLoader->Count;i++)
        {
        p=(TDllLoader*)(lstLoader->Items[i]);
        if  ( (p!=NULL)&&(p->FormCloseQuery()==0) )
                return 0;
        }
    return 1;
}

