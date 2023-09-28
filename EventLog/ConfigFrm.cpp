//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LogFrm.h"
#include "DataProcess.h"
#include "ConfigFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "AdvGrid"
#pragma resource "*.dfm"
TfrmConfig *frmConfig;
//---------------------------------------------------------------------------
bool ColumnSign;
__fastcall TfrmConfig::TfrmConfig(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::btnOkClick(TObject *Sender)
{
  if (!lstvwValues->Focused())
  {
    if (!CommitValues(strKey,edtCategory->Text.Trim()))
    {
      ModalResult=mrNone;
      return;
    }
  }
  else
  {
    ModalResult=mrNone;
    return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::lstvwValuesDblClick(TObject *Sender)
{
  if ((lstvwValues->ItemFocused!=NULL)&&(lstvwValues->ItemFocused->Selected))
    lstvwValues->ItemFocused->EditCaption();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::FormShow(TObject *Sender)
{
  strKey=edtCategory->Text;
  edtCategory->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::lstvwValuesKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
  TListItem *pListItem;
  if ((Key==VK_DELETE)&&(lstvwValues->ItemFocused!=NULL)&&(lstvwValues->ItemFocused->Selected))
	DeleteValues();
  if (Key==VK_RETURN)
  {
	if (lstvwValues->Items->Count>=1)
	{
	  if (lstvwValues->ItemFocused->Index==lstvwValues->Items->Count-1)
	  {
		if (!lstvwValues->Items->Item[lstvwValues->Items->Count-1]->Caption.Trim().IsEmpty())
		{
		  for(int i=0;i<lstvwValues->Items->Count;i++)
			 lstvwValues->Items->Item[i]->Selected=false;
		  pListItem=lstvwValues->Items->Add();
		  pListItem->Focused=true;
		  pListItem->Selected=true;
		  pListItem->EditCaption();
		}
		else
		  lstvwValues->ItemFocused->EditCaption();
	  }
	  else
	  {
		for(int i=0;i<lstvwValues->Items->Count;i++)
		   lstvwValues->Items->Item[i]->Selected=false;
		lstvwValues->Items->Item[lstvwValues->ItemFocused->Index+1]->Focused=true;
		lstvwValues->ItemFocused->Selected=true;
		lstvwValues->ItemFocused->EditCaption();
	  }
	}
  }
  if (Key==VK_INSERT)
  {
	if ((lstvwValues->Items->Count>0)
	 &&(lstvwValues->ItemFocused!=NULL))
	{
	  for(int i=0;i<lstvwValues->Items->Count;i++)
		 lstvwValues->Items->Item[i]->Selected=false;
	   pListItem=lstvwValues->Items->Insert(lstvwValues->ItemFocused->Index);
	   pListItem->Focused=true;
	   pListItem->Selected=true;
	   pListItem->EditCaption();
	}
	else
	{
	  for(int i=0;i<lstvwValues->Items->Count;i++)
		 lstvwValues->Items->Item[i]->Selected=false;
	  pListItem=lstvwValues->Items->Add();
	  pListItem->Focused=true;
	  pListItem->Selected=true;
	  pListItem->EditCaption();
	}
  }
}
//---------------------------------------------------------------------------
void TfrmConfig::DeleteValues()
{
  TListItem *pListItem,*pNextItem;
  TItemStates ItemState;
  int button;
  int nSelected;
  pListItem=lstvwValues->ItemFocused;
  if ((pListItem==NULL)||(pListItem->Selected==false))
  {
	 Application->MessageBox(L"You must select Values to delete.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return;
  }
  button=Application->MessageBox(L"Are you sure to delete the selected values?",L"TBS Library",MB_OKCANCEL|MB_ICONQUESTION);
  if (button!=ID_OK)
     return;
  ItemState<<isSelected;
  pListItem=lstvwValues->Selected;
  nSelected=lstvwValues->SelCount;
  for (int i=0;i<nSelected;i++)
  {
    pNextItem=lstvwValues->GetNextItem(pListItem,sdBelow,ItemState);
    lstvwValues->Items->Delete(lstvwValues->Items->IndexOf(pListItem));
    pListItem=pNextItem;
  }
  lstvwValues->ItemFocused->Selected=true;
  lstvwValues->ItemFocused->MakeVisible(false);
}

void __fastcall TfrmConfig::lstvwValuesChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
  if ((lstvwValues->ItemFocused!=NULL)&&(lstvwValues->ItemFocused->Selected))
  {
	muiPopDelete->Enabled=true;
    muiPopRename->Enabled=true;
  }
  else
  {
    muiPopDelete->Enabled=false;
    muiPopRename->Enabled=false;
  }
}
/*
//---------------------------------------------------------------------------
int __stdcall SortByName(LPARAM Item1,LPARAM Item2,LPARAM ParamSort)
{
  TListItem *pItem1=(TListItem *)Item1;
  TListItem *pItem2=(TListItem *)Item2;
  int intReturn,index;
  AnsiString tempstr;
  //if (ParamSort>=0) sign=true; else {sign=false; ParamSort=-ParamSort;}
  tempstr=IntToStr((int)ParamSort);
  tempstr.Delete(0,1);
  index=StrToInt(tempstr);
  if (index>0){
  if (ParamSort>=20000) {
  if (StrToDate(pItem1->SubItems->Strings[index-1].c_str()) <= StrToDate(pItem2->SubItems->Strings[index-1].c_str()))
  intReturn = -1; else intReturn = 1;
  }
  if (ParamSort>=10000) {
  if (StrToInt(pItem1->SubItems->Strings[index-1].c_str()) <= StrToInt(pItem2->SubItems->Strings[index-1].c_str()))
   intReturn = -1;
  else intReturn = 1;
  }
  intReturn= lstrcmpi(AnsiString(pItem1->SubItems->Strings[index-1]).c_str(),AnsiString(pItem2->SubItems->Strings[index-1]).c_str());
  }
  else {//itemµÄ´¦Àí
  if (ParamSort>=20000) {
  if (StrToDate(pItem1->Caption)<=StrToDate(pItem2->Caption))
  intReturn = -1; else intReturn = 1;
  }
  if (ParamSort>=10000) {
  if (StrToInt(pItem1->Caption)<=StrToInt(pItem2->Caption))
   intReturn = -1; else intReturn = 1;
  }
  intReturn= lstrcmp(AnsiString(pItem1->Caption).c_str(),AnsiString(pItem2->Caption).c_str());

  }
  if (ColumnSign==false) return intReturn;
  else return -(intReturn);
 }
 */
void __fastcall TfrmConfig::lstvwValuesColumnClick(TObject *Sender,
	  TListColumn *Column)
{
  ColumnSign=!(ColumnSign);
 // lstvwValues->CustomSort(SortByName,Column->Index );
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::muiPopRenameClick(TObject *Sender)
{
  if ((lstvwValues->ItemFocused!=NULL)&&(lstvwValues->ItemFocused->Selected))
	 lstvwValues->ItemFocused->EditCaption();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::muiPopInsertClick(TObject *Sender)
{
  TListItem *pListItem;
  if (lstvwValues->Items->Count>0)
  {
	for(int i=0;i<lstvwValues->Items->Count;i++)
	  lstvwValues->Items->Item[i]->Selected=false;
	pListItem=lstvwValues->Items->Insert(lstvwValues->ItemFocused->Index);
	pListItem->Focused=true;
	pListItem->Selected=true;
	pListItem->EditCaption();
  }
  else
  {
    for(int i=0;i<lstvwValues->Items->Count;i++)
      lstvwValues->Items->Item[i]->Selected=false;
    pListItem=lstvwValues->Items->Add();
    pListItem->Focused=true;
    pListItem->Selected=true;
    pListItem->EditCaption();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::muiPopDeleteClick(TObject *Sender)
{
  DeleteValues();
}
//---------------------------------------------------------------------------


void __fastcall TfrmConfig::lstvwValuesEdited(TObject *Sender, TListItem *Item, UnicodeString &S)

{
   if (S.Trim().IsEmpty())
   {
	 Application->MessageBox(L"The value can not be empty,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 return;
   }
   if(S!=Item->Caption)
   {
	 if (CheckValues(S)>=1)
	 {
	   Application->MessageBox(L"The value is not unique,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
       S=Item->Caption;
	 }
   }
}
//---------------------------------------------------------------------------

