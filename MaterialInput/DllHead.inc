TApplication* orig_App;
TSessionList* orig_Sessions;
TSession* orig_Session;

int MyFormCreate(void);
int MyFormActivate(char *);
int MyFormDeactivate(char *);
int MyFormCloseQuery(void);
int MyFormDestroy(void);


void SetGlobalVars(TApplication* AOwner, TSessionList* LOwner, TSession* SOwner)
{
    orig_App=Application;
    orig_Sessions=Sessions;
    orig_Session=Session;
    Application=AOwner;
    Sessions=LOwner;
    Session=SOwner;
}

void RestoreGlobalVars(void)
{
    Application=orig_App;
    Sessions=orig_Sessions;
    Session=orig_Session;
};

extern "C" __declspec(dllexport) TDllForm* DllCreate
    (TApplication* AOwner, TSessionList* LOwner, TSession* SOwner)
{
    SetGlobalVars(AOwner, LOwner, SOwner);
    dfrm=new TDllForm();
    if (dfrm!=NULL)
        {
        dfrm->OnFormCreate= MyFormCreate;
        dfrm->OnFormActivate= MyFormActivate;
        dfrm->OnFormDeactivate= MyFormDeactivate;
        dfrm->OnFormCloseQuery= MyFormCloseQuery;
        dfrm->OnFormDestroy= MyFormDestroy;
        }
    return dfrm;
}

extern "C" __declspec(dllexport) int DllDestroy(void)
{
    if (dfrm!=NULL)
        {
        try {
            delete dfrm;
            }
        catch(...)
            {
            return 0;
            } //<TBC>
        }
    RestoreGlobalVars();
    return 1;
}
