//---------------------------------------------------------------------------
#ifndef DllFormH
#define DllFormH

#if !defined(INC_VCLDB_HEADERS)
#include <dbtables.hpp>
#endif

#if !defined(INC_VCLEXT_HEADERS)
#include <ComCtrls.hpp>
#endif

//---------------------------------------------------------------------------
typedef int (*FDllFormCreate) (void);
typedef int (*FDllFormActivate) (char *);
typedef int (*FDllFormDeactivate) (char *);
typedef int (*FDllFormCloseQuery) (void);
typedef int (*FDllFormDestroy) (void);
//---------------------------------------------------------------------------
bool KeepOnly1(const char* UniName);
void SwitchToFront(TForm* MDIChild);

//---------------------------------------------------------------------------
class TDllForm
{
protected:
    bool bMyFormExist;
public:
    TStatusBar* stbMain;
	TDock97* dkMain;
    TDatabase* dbMain;
    AnsiString strUserID;
    int nGroupType;

    FDllFormCreate OnFormCreate;
    FDllFormActivate OnFormActivate;
    FDllFormDeactivate OnFormDeactivate;
    FDllFormCloseQuery OnFormCloseQuery;
    FDllFormDestroy OnFormDestroy;

    TDllForm();
    ~TDllForm();

    int FormCreate(void);
    int FormActivate(char *);
    int FormDeactivate(char *);
    int FormCloseQuery(void);
    int FormDestroy(void);

    void MergeTb97(TToolbar97*);
    void UnmergeTb97(TToolbar97*);

__property bool bFormExist = { read = bMyFormExist };
};
//---------------------------------------------------------------------------

typedef TDllForm* (*FDllCreate) (TApplication*, TSessionList*, TSession*);
typedef int (*FDllDestroy) (void);

class TDllLoader
{
protected:
    HINSTANCE hDll;
    bool bMyLoadOK ;
    TDllForm* MyDllForm;
    AnsiString MyDllName;

    FDllCreate MyDllCreate;
    FDllDestroy MyDllDestroy;
public:
    TDllLoader (char*, AnsiString, int, TStatusBar*, TDatabase*, TDock97*);
    ~TDllLoader (void);

    int FormCreate(void);
    int FormActivate(char *);
    int FormDeactivate(char *);
    int FormCloseQuery(void);
    int FormDestroy(void);

__property bool bLoadOK = { read = bMyLoadOK };
__property TDllForm* dfrm = { read = MyDllForm };
__property AnsiString strDllName = { read = MyDllName };
};
//---------------------------------------------------------------------------

class TDllLoaderList
{
protected:
    TList* lstLoader;

    TStatusBar* stbMain;
    TDock97* dkMain;
    TDatabase* dbMain;
    AnsiString strUserID;
    int nGroupType;

    AnsiString ActiveDllName;
    AnsiString ActivePage;
    TDllLoader* GetLoader(int);
public:
    TDllLoaderList (AnsiString, int, TStatusBar*, TDatabase*, TDock97*);
    ~TDllLoaderList (void);

    TDllLoader* LoaderByName(AnsiString);
    int AddLoader(char*);
    int DeleteLoader(char*);
    int ActivateLoader(char*,char*);
    int DeactivateLoader(char*,char*);
    int CloseQuery(void);
    int ClearLoader(void);

__property TDllLoader* Loaders[int Index] = { read = GetLoader };
};

//---------------------------------------------------------------------------
#endif
