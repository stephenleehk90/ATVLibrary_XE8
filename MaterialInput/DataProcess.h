//---------------------------------------------------------------------------
#ifndef DataProcessH
#define DataProcessH
//---------------------------------------------------------------------------
bool BrowseMaterialType(TStringList *strResType);
bool BrowseMaterials();
bool ShelfMaterials();
bool MaterialsAdd();
bool MaterialsDelete();
bool MaterialsModify();
void ClearMaterialInput();
bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module);
bool MaterialReport(bool bpreview,TObject *sender);
bool CheckUnique(AnsiString strSQLClause);
// updated huchen 2002.7
int  GetLimitNum(int &imaxpage,int &ipage, int &istart,int &iend);
bool GetOrderColumn(AnsiString &SQLClause,AnsiString &strOrderList);
#endif
