//---------------------------------------------------------------------------
#ifndef DataProcessH
#define DataProcessH
//---------------------------------------------------------------------------
#define COMMONUSERGROUP 2
#define NODEIDSIZE 32
#ifndef htmlformH
#include "htmlform.h"
#endif
bool RefreshTree();
bool BrowseScript(AnsiString strNodeID);
bool BrowseByID(AnsiString strNodeID,AnsiString strMaterialID);
bool AddStory(AnsiString strNodeID,TInputInfo* pInfo);
bool ModifyStory(AnsiString strNodeID,AnsiString strStoryID,TInputInfo* pInfo);
bool StoryModify(AnsiString strNodeID,AnsiString strStoryID,AnsiString strDate);
bool DeleteStory(AnsiString strNodeID);
bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module);
int FillListView(TQuery* pQuery,TStringList* pStringList,TListView* pListView);
extern AnsiString strCurrentUserID;
extern int nGroupType;
#endif
