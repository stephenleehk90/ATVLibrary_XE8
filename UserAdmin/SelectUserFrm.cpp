//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectUserFrm.h"
#include "UserData.h"
#include "UserFrm.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmSelectUsers *frmSelectUsers;
//---------------------------------------------------------------------------
__fastcall TfrmSelectUsers::TfrmSelectUsers(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectUsers::FormShow(TObject *Sender)
{
  AnsiString strSQLClause;
  int i;
  lstvwCurrent->Items->Clear();
  for (i=0;i<frmUserAdmin->lstCurrentUsers->Items->Count;i++)
  {
     lstvwCurrent->Items->Add();
     lstvwCurrent->Items->Item[i]->Caption
                   =frmUserAdmin->lstCurrentUsers->Items->Strings[i];
  }
  strSQLClause=(AnsiString)"select UserID from lib_User";
  dmdUserAdmin->daqUsers->SQL->Clear();
  dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
  try
  {
    dmdUserAdmin->daqUsers->Open();
    dmdUserAdmin->daqUsers->First();
    lstvwAllUsers->Items->Clear();
    for (i=0;i<dmdUserAdmin->daqUsers->RecordCount;i++)
    {
      lstvwAllUsers->Items->Add();
      lstvwAllUsers->Items->Item[i]->Caption=
              OrigStr(dmdUserAdmin->daqUsers->FieldValues["UserID"]);
      dmdUserAdmin->daqUsers->Next();
    }
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  }
  btnAdd->Enabled=false;
  btnDelete->Enabled=false;
  lstvwCurrent->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectUsers::btnOkClick(TObject *Sender)
{
    frmUserAdmin->lstCurrentUsers->Items->Clear();
    for (int i=0;i<lstvwCurrent->Items->Count;i++)
      frmUserAdmin->lstCurrentUsers->Items->Add(lstvwCurrent->Items->Item[i]->Caption);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectUsers::btnAddClick(TObject *Sender)
{
    bool IfCurrentUser;
    int nSelected,i,j;
    TListItem *pNextItem;
    TItemStates ItemState;
    ItemState<<isSelected;
    pNextItem=lstvwAllUsers->Selected;
    nSelected=lstvwAllUsers->SelCount;
    for(i=0;i<nSelected;i++)
    {
      IfCurrentUser=false;
      if (lstvwCurrent->Items->Count>0)
      {
        for (j=0;j<lstvwCurrent->Items->Count;j++)
        {
          if (lstvwCurrent->Items->Item[j]->Caption
                    ==pNextItem->Caption)
          {
            IfCurrentUser=true;
            break;
          }

        }
        if (!IfCurrentUser)
        {
          lstvwCurrent->Items->Add();
          lstvwCurrent->Items->Item[lstvwCurrent->Items->Count-1]->Caption=pNextItem->Caption;
        }
      }
      else
      {
        lstvwCurrent->Items->Add();
        lstvwCurrent->Items->Item[0]->Caption=pNextItem->Caption;
      }
      pNextItem=lstvwAllUsers->GetNextItem(pNextItem,sdBelow,ItemState);
    }
    lstvwAllUsers->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectUsers::btnDeleteClick(TObject *Sender)
{
    TListItem *pListItem,*pNextItem;
	TItemStates ItemState;
    int i,nSelected;
    ItemState<<isSelected;
   	pListItem=lstvwCurrent->Selected;
    nSelected=lstvwCurrent->SelCount;
    for (i=0;i<nSelected;i++)
    {
       pNextItem=lstvwCurrent->GetNextItem(pListItem,sdBelow,ItemState);
       lstvwCurrent->Items->Delete(lstvwCurrent->Items->IndexOf(pListItem));
       pListItem=pNextItem;
    }
    lstvwCurrent->Arrange(arAlignLeft);
    if (lstvwCurrent->Items->Count<1)
       btnDelete->Enabled=false;
    lstvwCurrent->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectUsers::lstvwCurrentEnter(TObject *Sender)
{
  btnAdd->Enabled=false;
  if (lstvwCurrent->Items->Count>0)
  {
    if (lstvwCurrent->ItemFocused!=NULL)
       lstvwCurrent->ItemFocused->Selected=true;
    else
    {
      lstvwCurrent->Items->Item[0]->Selected=true;
      lstvwCurrent->Items->Item[0]->Focused=true;
    }
    btnDelete->Enabled=true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectUsers::lstvwAllUsersEnter(TObject *Sender)
{
  btnDelete->Enabled=false;
  if (lstvwAllUsers->Items->Count>0)
  {
    if (lstvwAllUsers->ItemFocused!=NULL)
       lstvwAllUsers->ItemFocused->Selected=true;
    else
    {
      lstvwAllUsers->Items->Item[0]->Selected=true;
      lstvwAllUsers->Items->Item[0]->Focused=true;
    }
    btnAdd->Enabled=true;
  }
}
//---------------------------------------------------------------------------
bool __fastcall TfrmSelectUsers::Checkprivilege()
{
  if (frmUserAdmin->nGroupType>=2)
  {
    int nSelected,i;
    TListItem *pNextItem;
    TItemStates ItemState;
    ItemState<<isSelected;
    pNextItem=lstvwAllUsers->Selected;
    nSelected=lstvwAllUsers->SelCount;
    for(i=0;i<nSelected;i++)
    {
      if (!CheckSingle(pNextItem->Caption))
         return false;
      pNextItem=lstvwAllUsers->GetNextItem(pNextItem,sdBelow,ItemState);
    }
  }
  return true;
}
bool __fastcall TfrmSelectUsers::CheckSingle(AnsiString Title)
{
  AnsiString strSQLClause;
  int nType;
  strSQLClause=(AnsiString)"Select * from lib_Group Where GroupID In(Select GroupID from lib_GroupUser Where UserID='"
                   +(AnsiString)Title+(AnsiString)"')";
  dmdUserAdmin->daqUsers->SQL->Clear();
  dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
  try
  {
    dmdUserAdmin->daqUsers->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    return false;
  }
  dmdUserAdmin->daqUsers->First();
  while(!dmdUserAdmin->daqUsers->Eof)
  {
    nType=dmdUserAdmin->daqUsers->FieldByName("GroupType")->AsInteger;
    if (nType<frmUserAdmin->nGroupType)
       return false;
    dmdUserAdmin->daqUsers->Next();
  }
  return true;
}

