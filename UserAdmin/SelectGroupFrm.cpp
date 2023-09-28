//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SelectGroupFrm.h"
#include "UserData.h"
#include "UserFrm.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmSelectGroups *frmSelectGroups;
//---------------------------------------------------------------------------
__fastcall TfrmSelectGroups::TfrmSelectGroups(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGroups::FormShow(TObject *Sender)
{
  AnsiString strSQLClause;
   int i;
   lstvwCurrent->Items->Clear();
   for (i=0;i<frmUserAdmin->lstCurrentGroups->Items->Count;i++)
   {
     lstvwCurrent->Items->Add();
     lstvwCurrent->Items->Item[i]->Caption
                   =frmUserAdmin->lstCurrentGroups->Items->Strings[i];
   }
   strSQLClause=(AnsiString)"select GroupID from lib_Group";
   dmdUserAdmin->daqGroups->SQL->Clear();
   dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
   try
   {
     dmdUserAdmin->daqGroups->Open();
     dmdUserAdmin->daqGroups->First();
     lstvwAllGroups->Items->Clear();
     for (i=0;i<dmdUserAdmin->daqGroups->RecordCount;i++)
     {
        lstvwAllGroups->Items->Add();
        lstvwAllGroups->Items->Item[i]->Caption=
               OrigStr(dmdUserAdmin->daqGroups->FieldValues["GroupID"]);
        dmdUserAdmin->daqGroups->Next();
     }
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONSTOP);
   }
   btnAdd->Enabled=false;
   btnDelete->Enabled=false;
   lstvwCurrent->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGroups::btnAddClick(TObject *Sender)
{
  if (!Checkprivilege())
	 Application->MessageBox(L"You have not the privilege.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
  else
  {
    bool IfCurrentGroup;
    int nSelected,i,j;
    TListItem *pNextItem;
    TItemStates ItemState;
    ItemState<<isSelected;
    pNextItem=lstvwAllGroups->Selected;
    nSelected=lstvwAllGroups->SelCount;
    for(i=0;i<nSelected;i++)
    {
      IfCurrentGroup=false;
      if (lstvwCurrent->Items->Count>0)
      {
        for (j=0;j<lstvwCurrent->Items->Count;j++)
        {
          if (lstvwCurrent->Items->Item[j]->Caption
                   ==pNextItem->Caption)
          {
            IfCurrentGroup=true;
            break;
          }

        }
        if (!IfCurrentGroup)
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
      pNextItem=lstvwAllGroups->GetNextItem(pNextItem,sdBelow,ItemState);
    }
  }
  lstvwAllGroups->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectGroups::btnDeleteClick(TObject *Sender)
{
   	TListItem *pListItem,* pNextItem;
	TItemStates ItemState;
    int i,nSelected;
    ItemState<<isSelected;
   	pListItem = lstvwCurrent->Selected;
    nSelected = lstvwCurrent->SelCount;
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
void __fastcall TfrmSelectGroups::lstvwCurrentEnter(TObject *Sender)
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
void __fastcall TfrmSelectGroups::lstvwAllGroupsEnter(TObject *Sender)
{
  btnDelete->Enabled=false;
  if (lstvwAllGroups->Items->Count>0)
  {
    if (lstvwAllGroups->ItemFocused!=NULL)
       lstvwAllGroups->ItemFocused->Selected=true;
    else
    {
      lstvwAllGroups->Items->Item[0]->Selected=true;
      lstvwAllGroups->Items->Item[0]->Focused=true;
    }
    btnAdd->Enabled=true;
  }
}
//---------------------------------------------------------------------------
bool __fastcall TfrmSelectGroups::Checkprivilege()
{
  if (frmUserAdmin->nGroupType>=2)
  {
    int nSelected,i;
    TListItem *pNextItem;
    TItemStates ItemState;
    ItemState<<isSelected;
    pNextItem=lstvwAllGroups->Selected;
    nSelected=lstvwAllGroups->SelCount;
    for(i=0;i<nSelected;i++)
    {
      if (!CheckSingle(pNextItem->Caption))
         return false;
      pNextItem=lstvwAllGroups->GetNextItem(pNextItem,sdBelow,ItemState);
    }
  }
  return true;
}
bool __fastcall TfrmSelectGroups::CheckSingle(AnsiString Title)
{
   AnsiString strSQLClause;
   int nType;
   strSQLClause=(AnsiString)"select * from lib_Group where GroupID='"
                  +(AnsiString)SQLStr(Title)+(AnsiString)"'";
   dmdUserAdmin->daqGroups->SQL->Clear();
   dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
   try
   {
     dmdUserAdmin->daqGroups->Open();
   }
   catch(...)
   {
     Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return false;
   }
   dmdUserAdmin->daqGroups->First();
   nType=dmdUserAdmin->daqGroups->FieldByName("GroupType")->AsInteger;
   if (frmUserAdmin->nGroupType>=nType)
        return false;
   return true;
}
void __fastcall TfrmSelectGroups::btnOkClick(TObject *Sender)
{
  frmUserAdmin->lstCurrentGroups->Items->Clear();
  for (int i=0;i<lstvwCurrent->Items->Count;i++)
      frmUserAdmin->lstCurrentGroups->Items->Add(lstvwCurrent->Items->Item[i]->Caption);
}
//---------------------------------------------------------------------------

