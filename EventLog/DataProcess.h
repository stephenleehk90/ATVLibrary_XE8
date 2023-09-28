//---------------------------------------------------------------------------
#ifndef DataProcessH
#define DataProcessH
//---------------------------------------------------------------------------
bool RefreshLogs(TDateTime tBegin,TDateTime tEnd,AnsiString strID);
bool LogDelete(int nDays);
bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module);
bool CommitValues(AnsiString Oldkey,AnsiString Newkey);
bool CommitTypes();
int CheckValues(AnsiString Caption);
int CheckTypes(AnsiString strTypeID,AnsiString strTypeName);
bool CheckUnique(AnsiString strSQLClause);
#endif
