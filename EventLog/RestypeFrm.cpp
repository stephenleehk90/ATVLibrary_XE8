//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RestypeFrm.h"
#include "DataProcess.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "AdvGrid"
//#pragma link "AdvObj"
//#pragma link "BaseGrid"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvUtil"
#pragma link "BaseGrid"
#pragma resource "*.dfm"
TfrmResType *frmResType;
//---------------------------------------------------------------------------
__fastcall TfrmResType::TfrmResType(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmResType::FormShow(TObject *Sender)
{
  //RefreshTypes();
  strgrdResType->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmResType::btnCommitClick(TObject *Sender)
{
  CommitTypes();
}
//---------------------------------------------------------------------------

void __fastcall TfrmResType::btnAddClick(TObject *Sender)
{
  AnsiString strTypeID,strTypeName,strDays;
  strTypeID=strgrdResType->Cells[0][strgrdResType->RowCount-1];
  strTypeName=strgrdResType->Cells[1][strgrdResType->RowCount-1];
  strDays=strgrdResType->Cells[2][strgrdResType->RowCount-1];
  if ((strgrdResType->Row==strgrdResType->RowCount-1)
     &&(!strTypeID.Trim().IsEmpty())&&(!strTypeName.Trim().IsEmpty())
     &&(!strDays.Trim().IsEmpty()))
    strgrdResType->RowCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmResType::btnResetClick(TObject *Sender)
{
 // RefreshTypes();
}
//---------------------------------------------------------------------------


void __fastcall TfrmResType::btnDeleteClick(TObject *Sender)
{
  int i;
  i=strgrdResType->Row;
  if (strgrdResType->RowCount>2)
	strgrdResType->RemoveRows(i,1);
  else
	strgrdResType->ClearRows(i,1);
  if (i<strgrdResType->RowCount)
	strgrdResType->Row=i;
  else strgrdResType->Row=strgrdResType->RowCount-1;

}
//---------------------------------------------------------------------------

void __fastcall TfrmResType::strgrdResTypeKeyDown(TObject *Sender,
	  WORD &Key, TShiftState Shift)
{
  AnsiString strTypeID,strTypeName,strDays;
  strTypeID=strgrdResType->Cells[0][strgrdResType->RowCount-1];
  strTypeName=strgrdResType->Cells[1][strgrdResType->RowCount-1];
  strDays=strgrdResType->Cells[2][strgrdResType->RowCount-1];
  if ((Key==VK_DOWN)&&(strgrdResType->Row==strgrdResType->RowCount-1)
	 &&(!strTypeID.Trim().IsEmpty())&&(!strTypeName.Trim().IsEmpty())
	 &&(!strDays.Trim().IsEmpty()))
	strgrdResType->RowCount++;
}
//---------------------------------------------------------------------------



void __fastcall TfrmResType::strgrdResTypeKeyPress(TObject *Sender,
	  char &Key)
{
  if (strgrdResType->Col!=1)
  {
	if ((!isdigit(Key))&&(Key!=VK_BACK))
	  Key=NULL;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmResType::strgrdResTypeCellValidate(TObject *Sender, int ACol,
		  int ARow, UnicodeString &Value, bool &Valid)
{
   if (ACol==2)
  {
	try
	{
	  Value.ToInt();
	}
	catch(...)
	{
	  Application->MessageBox(L"The value you input is not a valid integer,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  btnCommit->Enabled=false;
	  Valid=false;
	  return;
	}
	btnCommit->Enabled=true;
	Valid=true;
  }
  if (ACol==0)
  {
	if (CheckTypes(Value,strgrdResType->Cells[1][ARow])==1)
	{
	  Application->MessageBox(L"The TypeID is not unique,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  Valid=false;
	  btnCommit->Enabled=false;
	  return;
	}
  }
  if (ACol==1)
  {
	if (CheckTypes(strgrdResType->Cells[0][ARow],Value)==2)
	{
	  Application->MessageBox(L"The TypeName is not unique,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  Valid=false;
	  btnCommit->Enabled=false;
	  return;
	}
  }

}
//---------------------------------------------------------------------------

