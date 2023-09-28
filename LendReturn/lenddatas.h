//---------------------------------------------------------------------------

#ifndef lenddatasH
#define lenddatasH
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TLdSystemData
{
 public:
  AnsiString strCurrentUser,strCaption;
  int 		 UserRight;
  int        nLendOrReturnCount,nMaxRecord,nTapeDays,nBookDays,nMagazineDays,nFileDays;
  TDateTime  dtTimer;
// TColor     tcNormal,tcUnReturned,tcOutOfTime;
  int        nNormalQueryDays,nQueryMode;
   __fastcall TLdSystemData();
  bool __fastcall SearchInListBox(AnsiString strBeSearched,TObject *SearchedBox);
  bool __fastcall CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module,AnsiString Remark);
 };
extern TLdSystemData *CLdSystemData;
#endif
