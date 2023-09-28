//---------------------------------------------------------------------------
#ifndef DataProcessH
#define DataProcessH
//---------------------------------------------------------------------------
bool InitUsers();
bool RefreshUsers();
bool RefreshGroups();
bool UsersAdd();
bool GroupsAdd();
bool UsersDelete();
bool GroupsDelete();
bool UserGroupsShow();
bool GroupUsersShow();
bool UsersModify();
bool GroupsModify();
bool GroupUsersAdd();
bool UserGroupsAdd();
bool UserGroupsModify();
bool GroupUsersModify();
bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module);
bool CheckUnique(AnsiString strSQLClause);
void ClearUserInput();
void ClearGroupInput();
#endif
