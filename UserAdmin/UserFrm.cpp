//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UserFrm.h"
#include "SelectUserFrm.h"
#include "SelectGroupFrm.h"
#include "UserData.h"
#include "CommFunc.h"
#include "DataProcess.h"
#include "frm_Main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvListV"
#pragma resource "*.dfm"
TfrmUserAdmin *frmUserAdmin;
int ColumnToSort = 0;
//---------------------------------------------------------------------------
__fastcall TfrmUserAdmin::TfrmUserAdmin(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool TfrmUserAdmin::Checkprivilege(AnsiString Title)
{
  AnsiString strSQLClause;
  int nEditType;
  if (nGroupType>=2)
  {
    strSQLClause=(AnsiString)"Select * from lib_Group Where GroupID In(Select GroupID from lib_GroupUser Where UserID='"
                  +(AnsiString)SQLStr(Title)+(AnsiString)"')";
    dmdUserAdmin->daqCheck->SQL->Clear();
    dmdUserAdmin->daqCheck->SQL->Add(strSQLClause);
    try
    {
      dmdUserAdmin->daqCheck->Open();
    }
    catch(...)
    {
	  Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      return false;
    }
    dmdUserAdmin->daqCheck->First();
    while (!dmdUserAdmin->daqCheck->Eof)
    {
      nEditType=dmdUserAdmin->daqCheck->FieldByName("GroupType")->AsInteger;
      if (nGroupType>=nEditType)
         return false;
      dmdUserAdmin->daqCheck->Next();
    }
  }
  return true;
}
void TfrmUserAdmin::AddUser()
{
  if (!bEditUser)      //添加用户
  {
    pnlUserInput->Visible=true;
    bEditUser=true;
    btnUserOk->Caption="&Add";

	tbbtnAddUser->Enabled=false;
	tbbtnModifyUser->Enabled=false;
	tbbtnDeleteUser->Enabled=false;
    muiPopAdd->Enabled=false;
    muiPopDelete->Enabled=false;
	muiPopModify->Enabled=false;
    muiPopRefresh->Enabled=false;
	tbbtnRefreshUser->Visible=false;

    muiEdit->Visible=false;
    lstCurrentGroups->Items->Clear();
	if ((cboUserGroups->Text!="everyone")&&(cboUserGroups->ItemIndex!=-1))
	   lstCurrentGroups->Items->Add(cboUserGroups->Text);
    cboUserGroups->Enabled=false;
	edtUserID->SetFocus();
  }
  bModifyUser=false;
  frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Add User";
}
void TfrmUserAdmin::AddGroup()
{
  if (!bEditGroup)
  {
    pnlGroupUsers->Visible=true;
    bEditGroup=true;
    btnOk->Caption="&Add";
    edtGroupID->SetFocus();
	cboGroupType->ItemIndex = 3;
	tbbtnAddGroup->Enabled=false;
    tbbtnModifyGroup->Enabled=false;
    tbbtnDeleteGroup->Enabled=false;

	muiPopAdd->Enabled=false;
    muiPopDelete->Enabled=false;
    muiPopModify->Enabled=false;
    muiPopRefresh->Enabled=false;

    tbbtnRefreshGroup->Visible=false;
    muiEdit->Visible=false;
    lstCurrentUsers->Items->Clear();
  }
  bModifyGroup=false;
  frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Add User Group";
}
void TfrmUserAdmin::ModifyGroup()
{
   TListItem *pListItem;
   AnsiString strSQLClause;
   int nEditType;
   pListItem=lstvwGroups->ItemFocused;
   if ((pListItem!=NULL)&&(pListItem->Selected))
   {
       strSQLClause=(AnsiString)"select GroupType,remark from lib_Group where GroupID='"
                    +SQLStr(pListItem->Caption)+(AnsiString)"'";
       dmdUserAdmin->daqGroups->SQL->Clear();
       dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
       try
       {
         dmdUserAdmin->daqGroups->Open();
       }
       catch(...)
       {
         Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
         return;
       }
       if (dmdUserAdmin->daqGroups->RecordCount<1)
       {
		 Application->MessageBox(L"This group's ID has been modified or this group has been deleted,please refresh the group list.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		 return;
	   }
	   nEditType=dmdUserAdmin->daqGroups->FieldByName("GroupType")->AsInteger;
	   cboGroupType->ItemIndex=nEditType;
	   pListItem->SubItems->Strings[0]=OrigStr(dmdUserAdmin->daqGroups->FieldByName("remark")->AsString);
	   pListItem->SubItems->Strings[1]=cboGroupType->Text;
	   if ((nGroupType>=nEditType)&&(nGroupType>=2))
		  Application->MessageBox(L"You have not the privilege.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
       else
       {
         if (!bEditGroup)
         {
           pnlGroupUsers->Visible=true;
           bEditGroup=true;
           btnOk->Caption="&Modify";

           tbbtnAddGroup->Enabled=false;
           tbbtnModifyGroup->Enabled=false;
           tbbtnDeleteGroup->Enabled=false;

           muiPopAdd->Enabled=false;
           muiPopDelete->Enabled=false;
           muiPopModify->Enabled=false;
           muiPopRefresh->Enabled=false;

           tbbtnRefreshGroup->Visible=false;

           muiEdit->Visible=false;
         }
         bModifyGroup=true;
         edtGroupID->Text=pListItem->Caption;
         edtGroupRemark->Text=pListItem->SubItems->Strings[0];
         cboGroupType->ItemIndex=nEditType;
         cboGroupType->Enabled=false;
         GroupUsersShow();
         strGroupID=edtGroupID->Text;
         edtGroupID->SetFocus();
         edtGroupID->SelectAll();
         frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Modify User Group";
       }
     }
}
void TfrmUserAdmin::ModifyUser()
{
   TListItem* pListItem;
   AnsiString strPassword,strSQLClause;
   pListItem=lstvwUsers->ItemFocused;
   if ((pListItem!=NULL)&&(pListItem->Selected))
   {
     strSQLClause=(AnsiString)"Select code,name,remark,department,phone,passwd from lib_User Where UserID='"
                 +(AnsiString)SQLStr(pListItem->Caption)+(AnsiString)"'";
     dmdUserAdmin->daqUsers->SQL->Clear();
     dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
     try
     {
       dmdUserAdmin->daqUsers->Open();
	 }
	 catch(...)
	 {
	   Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	   return;
	 }
	 if (dmdUserAdmin->daqUsers->RecordCount<1)
	 {
	   Application->MessageBox(L"This user's ID has been modified or this user has been deleted,please refresh the user list.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	   return;
	 }
	 pListItem->SubItems->Strings[0]=OrigStr(dmdUserAdmin->daqUsers->FieldByName("name")->AsString);
	 pListItem->SubItems->Strings[1]=OrigStr(dmdUserAdmin->daqUsers->FieldByName("code")->AsString);
	 pListItem->SubItems->Strings[2]=OrigStr(dmdUserAdmin->daqUsers->FieldByName("department")->AsString);
	 pListItem->SubItems->Strings[3]=OrigStr(dmdUserAdmin->daqUsers->FieldByName("phone")->AsString);
	 pListItem->SubItems->Strings[4]=OrigStr(dmdUserAdmin->daqUsers->FieldByName("remark")->AsString);
	 if (!Checkprivilege(pListItem->Caption))
		 Application->MessageBox(L"You have not the privilege.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
	 else
	 {
	   if (bEditUser==false)
	   {
		 pnlUserInput->Visible=true;
		 bEditUser=true;
		 btnUserOk->Caption="&Modify";

		 tbbtnAddUser->Enabled=false;
		 tbbtnModifyUser->Enabled=false;
		 tbbtnDeleteUser->Enabled=false;

		 muiPopAdd->Enabled=false;
		 muiPopDelete->Enabled=false;
		 muiPopModify->Enabled=false;
		 muiPopRefresh->Enabled=false;

		 tbbtnRefreshUser->Visible=false;

		 muiEdit->Visible=false;
		 cboUserGroups->Enabled=false;
	   }
	   bModifyUser=true;
	   edtUserID->Text=pListItem->Caption;
	   edtUserName->Text=pListItem->SubItems->Strings[0];
	   edtUserCode->Text=pListItem->SubItems->Strings[1];
	   edtUserDep->Text=pListItem->SubItems->Strings[2];
	   edtUserPhone->Text=pListItem->SubItems->Strings[3];
	   edtUserRemark->Text=pListItem->SubItems->Strings[4];
	   dmdUserAdmin->daqUsers->First();
	   strPassword=OrigStr(dmdUserAdmin->daqUsers->FieldValues["passwd"]);
	   DecodeString(strPassword);
	   edtUserPassword->Text=strPassword;
	   edtUserConfirm->Text=edtUserPassword->Text;
	   UserGroupsShow();
	   strUserID=edtUserID->Text;
	   strUserCode=edtUserCode->Text;
	   strUserName=edtUserName->Text;
	   edtUserID->SetFocus();
	   edtUserID->SelectAll();
	   frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Modify User";
	 }
   }
}
void TfrmUserAdmin::DeleteUser()
{
	TListItem *pListItem;
	int button;
	pListItem=lstvwUsers->ItemFocused;
	if ((pListItem==NULL)||(pListItem->Selected==false))
		Application->MessageBox(L"You must select users to delete.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	else
	{
	  if (!Checkprivilege(pListItem->Caption))
		   Application->MessageBox(L"You have not the privilege.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
	  else
	  {
		button=Application->MessageBox(L"Are you sure to delete the selected users?",L"TBS Library",MB_OKCANCEL|MB_ICONQUESTION);
		if (button==ID_OK)
        {
          if (UsersDelete())
          {
            if (lstvwUsers->ItemFocused!=NULL)
                lstvwUsers->ItemFocused->Selected=true;
          }
        }
      }
    }
}
void TfrmUserAdmin::DeleteGroup()
{
    TListItem *pListItem;
    int button;
    int nEditType;
    AnsiString strGroupType;
    pListItem=lstvwGroups->ItemFocused;
    if ((pListItem==NULL)||(pListItem->Selected==false))
		Application->MessageBox(L"You must select groups to delete.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    else
	{
       strGroupType=pListItem->SubItems->Strings[1];
       if(strGroupType=="Administrator")
           nEditType=0;
       else if(strGroupType=="Manager")
           nEditType=1;
       else if (strGroupType=="Operator")
           nEditType=2;
       else if (strGroupType=="User")
           nEditType=3;
       if ((nGroupType>=nEditType)&&(nGroupType>=2))
			Application->MessageBox(L"You have not the privilege.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
       else
       {
		 button=Application->MessageBox(L"Are you sure to delete the selected groups?",L"TBS Library",MB_OKCANCEL|MB_ICONQUESTION);
         if (button==ID_OK)
         {
		   if (GroupsDelete())
           {
             if (lstvwGroups->ItemFocused!=NULL)
                 lstvwGroups->ItemFocused->Selected=true;
           }
         }
       }
    }
}




void __fastcall TfrmUserAdmin::muiPopAddClick(TObject *Sender)
{
  if (nbkUsersAdmin->PageIndex==0)
     AddUser();
  else
     AddGroup();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::muiPopModifyClick(TObject *Sender)
{
  if (nbkUsersAdmin->PageIndex==0)
     ModifyUser();
  else
     ModifyGroup();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::muiPopDeleteClick(TObject *Sender)
{
  if (nbkUsersAdmin->PageIndex==0)
     DeleteUser();
  else
     DeleteGroup();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::muiPopRefreshClick(TObject *Sender)
{
  if (nbkUsersAdmin->PageIndex==0)
     RefreshUsers();
  else
     RefreshGroups();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::cboUserGroupsChange(TObject *Sender)
{
  AnsiString strSQLClause;
  int nType;
  if (cboUserGroups->Text.IsEmpty())
	  Application->MessageBox(L"Please select a category",
		 L"TBS Library",MB_OK|MB_ICONINFORMATION);
  else
  {
    if (cboUserGroups->ItemIndex!=0)
    {
      strSQLClause=(AnsiString)"select GroupType from lib_Group where GroupID='"
                   +(AnsiString)SQLStr(cboUserGroups->Text)+(AnsiString)"'";
      dmdUserAdmin->daqGroups->SQL->Clear();
      dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
      try
      {
        dmdUserAdmin->daqGroups->Open();
      }
      catch(...)
      {
		 Application->MessageBox(L"Database error",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      }
      dmdUserAdmin->daqGroups->First();
	  nType=dmdUserAdmin->daqGroups->FieldByName("GroupType")->AsInteger;
      if ((nGroupType<nType)||(nGroupType<2))
      {
         tbbtnAddUser->Enabled=true;
         muiPopAdd->Enabled=true;
         muiEdit_Add->Enabled=true;
      }
      else
      {
        tbbtnAddUser->Enabled=false;
        muiPopAdd->Enabled=false;
        muiEdit_Add->Enabled=false;
      }
    }
    if (cboUserGroups->ItemIndex==0)
    {
      tbbtnAddUser->Enabled=true;
      muiPopAdd->Enabled=true;
      muiEdit_Add->Enabled=true;
    }
    RefreshUsers();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::cboGroupTypeChange(TObject *Sender)
{
  if ((!bModifyGroup)&&(nGroupType>=2))
  {
    if (nGroupType>=cboGroupType->ItemIndex)
    {
	  Application->MessageBox(L"You have not the privilege.",L"TBS Library",MB_OK|MB_ICONEXCLAMATION);
	  cboGroupType->ItemIndex=3;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::btnUsersClick(TObject *Sender)
{
  frmSelectUsers->ShowModal();    //为一个用户组选择用户
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::btnGroupsClick(TObject *Sender)
{
  frmSelectGroups->ShowModal();    //为一个用户选择用户组
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwGroupsClick(TObject *Sender)
{
/*  TListItem *pListItem;
  AnsiString strGroupType;
  AnsiString strSQLClause;
  int nEditType;
  if (bModifyGroup==true)
  {
    pListItem=lstvwGroups->ItemFocused;
    strSQLClause=(AnsiString)"select GroupType,remark from lib_Group where GroupID='"
                 +SQLStr(pListItem->Caption)+(AnsiString)"'";
    dmdUserAdmin->daqGroups->SQL->Clear();
    dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
    try
    {
      dmdUserAdmin->daqGroups->Open();
    }
    catch(...)
    {
      Application->MessageBox("Database error.","TBS Library",MB_OK|MB_ICONINFORMATION);
      return;
    }
    if (dmdUserAdmin->daqGroups->RecordCount<1)
    {
      Application->MessageBox("This group's ID has been modified or this group has been deleted,please refresh the group list.","TBS Library",MB_OK|MB_ICONINFORMATION);
      return;
    }
    pListItem->SubItems->Strings[0]=OrigStr(dmdUserAdmin->daqGroups->FieldValues["remark"]);
    nEditType=dmdUserAdmin->daqGroups->FieldByName("GroupType")->AsInteger;
    cboGroupType->ItemIndex=nEditType;
    pListItem->SubItems->Strings[1]=cboGroupType->Text;
    edtGroupID->Text=pListItem->Caption;
    edtGroupRemark->Text=pListItem->SubItems->Strings[0];
    GroupUsersShow();
    strGroupID=edtGroupID->Text;
    if ((nGroupType<nEditType)||(nGroupType<2))
    {
      edtGroupID->Enabled=true;
      edtUserRemark->Enabled=true;
      btnUsers->Enabled=true;
      btnOk->Enabled=true;
      btnCancel->Enabled=true;
    }
    else
    {
      edtGroupID->Enabled=false;
      edtUserRemark->Enabled=false;
      cboGroupType->Enabled=false;
      btnUsers->Enabled=false;
      btnOk->Enabled=false;
    }
  }*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwUsersClick(TObject *Sender)
{
/*  TListItem *pListItem;
  AnsiString strSQLClause;
  if(bModifyUser==true)
  {
    pListItem=lstvwUsers->ItemFocused;
    strSQLClause=(AnsiString)"Select passwd,remark,code,department,name,phone from lib_User Where UserID='"
					+(AnsiString)SQLStr(pListItem->Caption)+(AnsiString)"'";
    dmdUserAdmin->daqUsers->SQL->Clear();
    dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
    try
    {
      dmdUserAdmin->daqUsers->Open();
    }
    catch(...)
    {
	  Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      return;
    }
    if (dmdUserAdmin->daqUsers->RecordCount<1)
    {
      Application->MessageBox("This user's ID has been modified or this user has been deleted,please refresh the user list.","TBS Library",MB_OK|MB_ICONINFORMATION);
      return;
    }
    pListItem->SubItems->Strings[0]=OrigStr(dmdUserAdmin->daqUsers->FieldValues["name"]);
    pListItem->SubItems->Strings[1]=OrigStr(dmdUserAdmin->daqUsers->FieldValues["code"]);
    pListItem->SubItems->Strings[2]=OrigStr(dmdUserAdmin->daqUsers->FieldValues["department"]);
    pListItem->SubItems->Strings[3]=OrigStr(dmdUserAdmin->daqUsers->FieldValues["phone"]);
    pListItem->SubItems->Strings[4]=OrigStr(dmdUserAdmin->daqUsers->FieldValues["remark"]);
    edtUserID->Text=pListItem->Caption;
    edtUserName->Text=pListItem->SubItems->Strings[0];
    edtUserCode->Text=pListItem->SubItems->Strings[1];
    edtUserDep->Text=pListItem->SubItems->Strings[2];
    edtUserPhone->Text=pListItem->SubItems->Strings[3];
    edtUserRemark->Text=pListItem->SubItems->Strings[4];
    if (Checkprivilege(pListItem->Caption))
    {
      dmdUserAdmin->daqUsers->First();
      edtUserPassword->Text=OrigStr(dmdUserAdmin->daqUsers->FieldValues["passwd"]);
      DecodeString(edtUserPassword->Text);
      edtUserConfirm->Text=edtUserPassword->Text;
      UserGroupsShow();
      strUserID=edtUserID->Text;
      strUserCode=edtUserCode->Text;
      strUserName=edtUserName->Text;
      edtUserPassword->Enabled=true;
      edtUserConfirm->Enabled=true;
      edtUserID->Enabled=true;
      edtUserName->Enabled=true;
      edtUserCode->Enabled=true;
      edtUserDep->Enabled=true;
      edtUserPhone->Enabled=true;
      edtUserRemark->Enabled=true;
      btnGroups->Enabled=true;
      btnUserOk->Enabled=true;
      btnUserCancel->Enabled=true;
    }
    else
    {
      edtUserPassword->Text=edtUserPassword->Text.StringOfChar('*',edtUserPassword->MaxLength);
      edtUserConfirm->Text=edtUserPassword->Text;
      edtUserPassword->Enabled=false;
      edtUserConfirm->Enabled=false;
      edtUserID->Enabled=false;
      edtUserName->Enabled=false;
      edtUserCode->Enabled=false;
      edtUserDep->Enabled=false;
      edtUserPhone->Enabled=false;
      edtUserRemark->Enabled=false;
      btnGroups->Enabled=false;
      btnUserOk->Enabled=false;
    }
  }*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwUsersKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if ((Key==VK_DELETE)&&(!bEditUser))
     DeleteUser();
  if ((Key==VK_RETURN)&&(!bEditUser))
     ModifyUser();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwGroupsKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if ((Key==VK_DELETE)&&(!bEditGroup))
     DeleteGroup();
  if ((Key==VK_RETURN)&&(!bEditUser))
     ModifyGroup();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwGroupsChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
  if (lstvwGroups->Focused())
  {
    if ((lstvwGroups->ItemFocused==NULL)||(!lstvwGroups->ItemFocused->Selected))
    {
      tbbtnModifyGroup->Enabled=false;
      tbbtnDeleteGroup->Enabled=false;

      muiEdit_Delete->Enabled=false;
      muiEdit_Modify->Enabled=false;
      muiPopDelete->Enabled=false;
      muiPopModify->Enabled=false;
    }
    else
    {
      if (!bEditGroup)
      {
        tbbtnModifyGroup->Enabled=true;
        tbbtnDeleteGroup->Enabled=true;

        muiEdit_Delete->Enabled=true;
        muiEdit_Modify->Enabled=true;
        muiPopDelete->Enabled=true;
        muiPopModify->Enabled=true;
      }
      else
      {
        lstvwGroupsClick(Sender);
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwUsersChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
  if (lstvwUsers->Focused())
  {
    if ((lstvwUsers->ItemFocused==NULL)||(!lstvwUsers->ItemFocused->Selected))
    {
      tbbtnModifyUser->Enabled=false;
      tbbtnDeleteUser->Enabled=false;

      muiEdit_Delete->Enabled=false;
      muiEdit_Modify->Enabled=false;
      muiPopDelete->Enabled=false;
      muiPopModify->Enabled=false;
    }
    else
    {
      if (!bEditUser)
      {
        tbbtnModifyUser->Enabled=true;
        tbbtnDeleteUser->Enabled=true;

        muiEdit_Delete->Enabled=true;
        muiEdit_Modify->Enabled=true;
        muiPopDelete->Enabled=true;
        muiPopModify->Enabled=true;
      }
      else
      {
        lstvwUsersClick(Sender);
      }
    }
  }    
}
//---------------------------------------------------------------------------
/*int __stdcall SortByName(int Item1,int Item2,int ParamSort)
{
  TListItem *pItem1=(TListItem *)Item1;
  TListItem *pItem2=(TListItem *)Item2;
  int intReturn,index;
  AnsiString tempstr;
  //if (ParamSort>=0) sign=true; else {sign=false; ParamSort=-ParamSort;}
  tempstr=IntToStr(ParamSort);
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
  intReturn= lstrcmpi(pItem1->SubItems->Strings[index-1].c_str(),pItem2->SubItems->Strings[index-1].c_str());
  }
  else {//item的处理
  if (ParamSort>=20000) {
  if (StrToDate(pItem1->Caption)<=StrToDate(pItem2->Caption))
  intReturn = -1; else intReturn = 1;
  }
  if (ParamSort>=10000) {
  if (StrToInt(pItem1->Caption)<=StrToInt(pItem2->Caption))
   intReturn = -1; else intReturn = 1;
  }
  intReturn= lstrcmp(pItem1->Caption.c_str(),pItem2->Caption.c_str());

  }
  if (ColumnSign==false) return intReturn;
  else return -(intReturn);
 }
*/

void __fastcall TfrmUserAdmin::btnOkClick(TObject *Sender)
{
   TListItem *pListItem;
   TItemStates ItemState;
   int nSelected,i;
   AnsiString strSQLClause,strSQLClause1;
   int nUserGroupIndex;
   if (edtGroupID->Text.Trim().IsEmpty())
   {
	 Application->MessageBox(L"Please input GroupID.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 edtGroupID->SetFocus();
   }
   else if (edtGroupID->Text.Trim()=="everyone")
   {
	 Application->MessageBox(L"The GroupID can not be 'everyone',please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 edtGroupID->SetFocus();
	 edtGroupID->SelectAll();
   }
   else if (cboGroupType->Text.IsEmpty())
   {
	 Application->MessageBox(L"Please select Group Type.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 cboGroupType->SetFocus();
   }
   else
   {
	   strSQLClause=(AnsiString)"select GroupID from lib_Group where GroupID='"
					+(AnsiString)SQLStr(edtGroupID->Text.Trim())+(AnsiString)"'";
	   if (bModifyGroup==false)
	   {
		 if (!CheckUnique(strSQLClause))
		 {
		   Application->MessageBox(L"GroupID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		   edtGroupID->SetFocus();
		   edtGroupID->SelectAll();
		 }
		 else
		 {
		   if (GroupsAdd())
		   {
			 if (GroupUsersAdd())
			 {
			   ItemState<<isSelected;
			   pListItem = lstvwGroups->Selected;
			   nSelected = lstvwGroups->SelCount;
			   for(i=0;i<nSelected;i++)
			   {
				 pListItem->Selected = false;
				 pListItem=lstvwGroups->GetNextItem(pListItem,sdBelow,ItemState);
			   }
			   pListItem=lstvwGroups->Items->Add();
			   pListItem->Caption=edtGroupID->Text.Trim();
			   pListItem->SubItems->Add(edtGroupRemark->Text.Trim());
			   pListItem->SubItems->Add(cboGroupType->Text.Trim());
			   lstvwGroups->ItemFocused = pListItem;
			   lstvwGroups->Selected = pListItem;

			   cboUserGroups->Items->Add(edtGroupID->Text.Trim());
			   frmMain->stbStory->Panels->Items[2]->Text
				 =(AnsiString)"User Group "+edtGroupID->Text.Trim()+(AnsiString)" has been added.";
			   ClearGroupInput();
			   edtGroupID->SetFocus();
			 }
		   }
		 }
	   }
	   else
	   {
		 strSQLClause1=(AnsiString)"select GroupID from lib_Group where GroupID='"
					  +(AnsiString)SQLStr(strGroupID)+(AnsiString)"'";

		 if (CheckUnique(strSQLClause1))
		 {
			Application->MessageBox(L"This group's ID has been modified or this group has been deleted, please refresh the group list.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
			edtGroupID->SetFocus();
			edtGroupID->SelectAll();
			return;
		 }
		 if ((!CheckUnique(strSQLClause))
			&&(edtGroupID->Text.Trim()!=strGroupID.Trim()))
		 {
			Application->MessageBox(L"GroupID is not unique, please input again",L"TBS Library",MB_OK|MB_ICONINFORMATION);
			edtGroupID->SetFocus();
			edtGroupID->SelectAll();
		 }
		 else
		 {
		   if (GroupsModify())
		   {
			 if (GroupUsersModify())
			 {
				ItemState<<isSelected;
				pListItem = lstvwGroups->Selected;
				nSelected = lstvwGroups->SelCount;
				for(i=0;i<nSelected;i++)
				{
				  pListItem->Selected = false;
				  pListItem=lstvwGroups->GetNextItem(pListItem,sdBelow,ItemState);
				}

				pListItem=lstvwGroups->FindCaption(0,strGroupID.Trim(),true,true,true);
				pListItem->Caption=edtGroupID->Text.Trim();
				pListItem->SubItems->Strings[0]=edtGroupRemark->Text.Trim();
				pListItem->SubItems->Strings[1]=cboGroupType->Text.Trim();
				lstvwGroups->ItemFocused = pListItem;
				lstvwGroups->Selected = pListItem;

				nUserGroupIndex=cboUserGroups->ItemIndex;
				cboUserGroups->Items->Insert(cboUserGroups->Items->IndexOf(strGroupID),edtGroupID->Text);
                cboUserGroups->Items->Delete(cboUserGroups->Items->IndexOf(strGroupID));
                cboUserGroups->ItemIndex=nUserGroupIndex;
                frmMain->stbStory->Panels->Items[2]->Text
                 =(AnsiString)"User Group "+strGroupID.Trim()+(AnsiString)"'s property has been modified.";
                strGroupID=pListItem->Caption.Trim();
                lstvwGroups->SetFocus();
                pnlGroupUsers->Visible=false;
                bEditGroup=false;
                btnOk->Caption="&Modify";

                tbbtnAddGroup->Enabled=true;
                tbbtnModifyGroup->Enabled=true;
                tbbtnDeleteGroup->Enabled=true;

                muiPopAdd->Enabled=true;
                muiPopDelete->Enabled=true;
                muiPopModify->Enabled=true;
                muiPopRefresh->Enabled=true;
                muiEdit_Add->Enabled=true;
                muiEdit_Modify->Enabled=true;
                muiEdit_Delete->Enabled=true;
                
                tbbtnRefreshGroup->Visible=true;

                muiEdit->Visible=true;
                ClearGroupInput();
                cboGroupType->Enabled = true;
                pListItem->MakeVisible(false);
              }
            }
         }
       }
     }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::btnCancelClick(TObject *Sender)
{
    bEditGroup=false;
    bModifyGroup=false;
    pnlGroupUsers->Visible=false;
    ClearGroupInput();
    lstvwGroups->SetFocus();

    tbbtnAddGroup->Enabled=true;
    muiPopAdd->Enabled=true;
    muiEdit_Add->Enabled=true;
    muiEdit->Visible=true;
    cboGroupType->Enabled=true;
    muiPopRefresh->Enabled=true;

    tbbtnRefreshGroup->Visible=true;
    frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"";
    if (lstvwGroups->ItemFocused!=NULL)
    {
      lstvwGroups->ItemFocused->Selected=true;
      tbbtnModifyGroup->Enabled=true;
      tbbtnDeleteGroup->Enabled=true;

      muiPopDelete->Enabled=true;
      muiPopModify->Enabled=true;
      muiEdit_Modify->Enabled=true;
      muiEdit_Delete->Enabled=true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::btnUserCancelClick(TObject *Sender)
{
    bEditUser=false;
    bModifyUser=false;
    pnlUserInput->Visible=false;
    edtUserPassword->Enabled=true;
    edtUserConfirm->Enabled=true;
    edtUserID->Enabled=true;
    edtUserName->Enabled=true;
    edtUserCode->Enabled=true;
    edtUserDep->Enabled=true;
    edtUserPhone->Enabled=true;
    edtUserRemark->Enabled=true;
    btnGroups->Enabled=true;
    btnUserOk->Enabled=true;
    btnUserCancel->Enabled=true;

    tbbtnAddUser->Enabled=true;
    muiPopAdd->Enabled=true;
    muiEdit_Add->Enabled=true;
    muiEdit->Visible=true;
    muiPopRefresh->Enabled=true;

    tbbtnRefreshUser->Visible=true;

	cboUserGroups->Enabled=true;
	frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"";
    ClearUserInput();
    lstvwUsers->SetFocus();
    if (lstvwUsers->ItemFocused!=NULL)
    {
      lstvwUsers->ItemFocused->Selected=true;
	  tbbtnModifyUser->Enabled=true;
      tbbtnDeleteUser->Enabled=true;

      muiPopDelete->Enabled=true;
      muiPopModify->Enabled=true;
      muiEdit_Modify->Enabled=true;
      muiEdit_Delete->Enabled=true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::btnUserOkClick(TObject *Sender)
{
   TListItem *pListItem;
   TItemStates ItemState;
   int nSelected,i;
   AnsiString strSQLClause,strSQLClause1,strSQLClause2;
   
   if (edtUserID->Text.Trim().IsEmpty())
   {
	 Application->MessageBox(L"Please input UserID.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 edtUserID->SetFocus();
     return;
   }
/*   if (edtUserPassword->Text.Trim().IsEmpty())
   {
      Application->MessageBox("Please input Password.","TBS Library",MB_OK|MB_ICONINFORMATION);
      edtUserPassword->SetFocus();
      return;
   }
*/   if(edtUserPassword->Text.Trim()!=edtUserConfirm->Text.Trim())
   {
	  Application->MessageBox(L"Please confirm Password.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      edtUserConfirm->SetFocus();
      edtUserConfirm->SelectAll();
      return;
   }
   strSQLClause=(AnsiString)"select UserID from lib_User where UserID='"
               +(AnsiString)SQLStr(edtUserID->Text.Trim())+(AnsiString)"'";
   strSQLClause1=(AnsiString)"select code from lib_User where code='"
               +(AnsiString)SQLStr(edtUserCode->Text.Trim())+(AnsiString)"'";
   if (bModifyUser==false)
   {
      if (!CheckUnique(strSQLClause))
      {
		Application->MessageBox(L"UserID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        edtUserID->SetFocus();
        edtUserID->SelectAll();
        return;
      }
      if (!edtUserCode->Text.Trim().IsEmpty())
      {
        if (!CheckUnique(strSQLClause1))
		{
		  Application->MessageBox(L"UserCode is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
          edtUserCode->SetFocus();
          edtUserCode->SelectAll();
          return;
        }
      }
      if (UsersAdd())
      {
        if (UserGroupsAdd())
        {
          ItemState<<isSelected;
          pListItem = lstvwUsers->Selected;
          nSelected = lstvwUsers->SelCount;
          for(i=0;i<nSelected;i++)
          {
            pListItem->Selected = false;
            pListItem=lstvwUsers->GetNextItem(pListItem,sdBelow,ItemState);
          }

          pListItem=lstvwUsers->Items->Add();
          pListItem->Caption=edtUserID->Text.Trim();
          pListItem->SubItems->Add(edtUserName->Text.Trim());
          pListItem->SubItems->Add(edtUserCode->Text.Trim());
          pListItem->SubItems->Add(edtUserDep->Text.Trim());
          pListItem->SubItems->Add(edtUserPhone->Text.Trim());
          pListItem->SubItems->Add(edtUserRemark->Text.Trim());
          lstvwUsers->ItemFocused = pListItem;
          lstvwUsers->Selected = pListItem;
          frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"User "+edtUserID->Text+(AnsiString)" has been added.";
          edtUserID->Clear();
          edtUserCode->Clear();
          edtUserName->Clear();
          edtUserDep->Clear();
          edtUserPassword->Clear();
          edtUserConfirm->Clear();
          edtUserPhone->Clear();
          edtUserRemark->Clear();
          lstCurrentGroups->Items->Clear();
		  if ((cboUserGroups->Text!="everyone")&&(cboUserGroups->ItemIndex!=-1))
              lstCurrentGroups->Items->Add(cboUserGroups->Text);
          edtUserID->SetFocus();
        }
      }
    }
    else
    {
      strSQLClause2=(AnsiString)"select UserID from lib_User where UserID='"
                  +(AnsiString)SQLStr(strUserID)+(AnsiString)"'";

      if (CheckUnique(strSQLClause2))
      {
		Application->MessageBox(L"This user's ID has been modified or this user has been deleted,please refresh the user list.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        edtUserID->SetFocus();
        edtUserID->SelectAll();
        return;
      }

      if ((!CheckUnique(strSQLClause))
         &&(edtUserID->Text.Trim()!=strUserID.Trim()))
      {
		Application->MessageBox(L"UserID is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        edtUserID->SetFocus();
        edtUserID->SelectAll();
        return;
      }

      if (!edtUserCode->Text.Trim().IsEmpty())
      {
        if ((!CheckUnique(strSQLClause1))
              &&(edtUserCode->Text.Trim()!=strUserCode.Trim()))
        {
		   Application->MessageBox(L"UserCode is not unique, please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
           edtUserCode->SetFocus();
		   edtUserCode->SelectAll();
           return;
        }
      }
      if (UsersModify())
      {
        if(UserGroupsModify())
        {
          ItemState<<isSelected;
          pListItem = lstvwUsers->Selected;
          nSelected = lstvwUsers->SelCount;
          for(i=0;i<nSelected;i++)
          {
            pListItem->Selected = false;
            pListItem=lstvwUsers->GetNextItem(pListItem,sdBelow,ItemState);
          }

          pListItem=lstvwUsers->FindCaption(0,strUserID.Trim(),true,true,true);
          pListItem->Caption=edtUserID->Text.Trim();
          pListItem->SubItems->Strings[0]=edtUserName->Text.Trim();
          pListItem->SubItems->Strings[1]=edtUserCode->Text.Trim();
          pListItem->SubItems->Strings[2]=edtUserDep->Text.Trim();
          pListItem->SubItems->Strings[3]=edtUserPhone->Text.Trim();
          pListItem->SubItems->Strings[4]=edtUserRemark->Text.Trim();
          lstvwUsers->ItemFocused = pListItem;
          lstvwUsers->Selected = pListItem;
          
          frmMain->stbStory->Panels->Items[2]->Text
              =(AnsiString)"User "+strUserID+(AnsiString)"'s property has been modified.";
          strUserID=pListItem->Caption.Trim();
          strUserCode=pListItem->SubItems->Strings[1].Trim();
          strUserName=pListItem->SubItems->Strings[0].Trim();
          pnlUserInput->Visible=false;
          bEditUser=false;
          tbbtnAddUser->Enabled=true;
          tbbtnModifyUser->Enabled=true;
          tbbtnDeleteUser->Enabled=true;

          muiPopAdd->Enabled=true;
          muiPopDelete->Enabled=true;
          muiPopModify->Enabled=true;
          muiEdit_Add->Enabled=true;
          muiEdit_Modify->Enabled=true;
          muiEdit_Delete->Enabled=true;

          muiPopRefresh->Enabled=true;

          tbbtnRefreshUser->Visible=true;

          muiEdit->Visible=true;

          edtUserID->Clear();
          edtUserCode->Clear();
          edtUserName->Clear();
          edtUserDep->Clear();
          edtUserPassword->Clear();
          edtUserConfirm->Clear();
          edtUserPhone->Clear();
          edtUserRemark->Clear();

		  cboUserGroups->Enabled=true;

          lstvwUsers->SetFocus();
          pListItem->MakeVisible(false);
        }
      }
    }
}
//---------------------------------------------------------------------------

void TfrmUserAdmin::UserPage()
{
   if (!bEditUser)
   {
     frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"";
     if (bFirstIntoUser)
     {
       InitUsers();
       bFirstIntoUser=false;
     }
     else
       RefreshUsers();
    }
    nbkUsersAdmin->ActivePage="pgUsers";
    if (!bEditUser)
    {
      muiEdit->Visible=true;

      tbbtnAddUser->Enabled=true;
      tbbtnModifyUser->Enabled=true;
      tbbtnDeleteUser->Enabled=true;

      muiPopAdd->Enabled=true;
      muiPopDelete->Enabled=true;
      muiPopModify->Enabled=true;
      muiEdit_Add->Enabled=true;
      muiEdit_Delete->Enabled=true;
      muiEdit_Modify->Enabled=true;
      muiPopRefresh->Enabled=true;

      tbbtnRefreshUser->Visible=true;

      lstvwUsers->SetFocus();
      if (lstvwUsers->ItemFocused!=NULL)
         lstvwUsers->ItemFocused->Selected=true;
      else
      {
        tbbtnModifyUser->Enabled=false;
        tbbtnDeleteUser->Enabled=false;

        muiPopModify->Enabled=false;
        muiEdit_Modify->Enabled=false;
        muiPopDelete->Enabled=false;
        muiEdit_Delete->Enabled=false;
      }
    }
    else
    {
      if (bModifyUser)
          frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Modify User";
      else
          frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Add User";
      muiEdit->Visible = false;

      tbbtnAddUser->Enabled=false;
      tbbtnModifyUser->Enabled=false;
      tbbtnDeleteUser->Enabled=false;

      muiPopAdd->Enabled=false;
      muiPopDelete->Enabled=false;
      muiPopModify->Enabled=false;
      muiEdit_Add->Enabled=false;
      muiEdit_Delete->Enabled=false;
      muiEdit_Modify->Enabled=false;
      muiPopRefresh->Enabled=false;

      tbbtnRefreshUser->Visible=false;

    }
    Application->MainForm->Caption=(AnsiString)"TBS Library - Users";
}
void TfrmUserAdmin::GroupPage()
{
    if (!bEditGroup)
    {
      frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"";
      RefreshGroups();
    }
    nbkUsersAdmin->ActivePage="pgUserGroup";
    if (!bEditGroup)
    {
      muiEdit->Visible=true;

      tbbtnAddGroup->Enabled=true;
      tbbtnModifyGroup->Enabled=true;
      tbbtnDeleteGroup->Enabled=true;

      muiPopAdd->Enabled=true;
      muiPopDelete->Enabled=true;
      muiPopModify->Enabled=true;
      muiEdit_Add->Enabled=true;
      muiEdit_Delete->Enabled=true;
      muiEdit_Modify->Enabled=true;
      muiPopRefresh->Enabled=true;
      tbbtnRefreshGroup->Visible=true;

      lstvwGroups->SetFocus();
      if (lstvwGroups->ItemFocused!=NULL)
         lstvwGroups->ItemFocused->Selected=true;
      else
      {
         tbbtnModifyGroup->Enabled=false;
         tbbtnDeleteGroup->Enabled=false;

         muiPopModify->Enabled=false;
         muiEdit_Modify->Enabled=false;
         muiPopDelete->Enabled=false;
         muiEdit_Delete->Enabled=false;
      }
    }
    else
    {
      if (bModifyGroup)
          frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Modify User Group";
      else
          frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Add User Group";
      tbbtnAddGroup->Enabled=false;
      tbbtnModifyGroup->Enabled=false;
      tbbtnDeleteGroup->Enabled=false;

      muiEdit->Visible = false;
      muiPopAdd->Enabled = false;
      muiPopDelete->Enabled = false;
      muiPopModify->Enabled = false;
      muiEdit_Add->Enabled = false;
      muiEdit_Delete->Enabled = false;
      muiEdit_Modify->Enabled = false;
      muiPopRefresh->Enabled = false;
      tbbtnRefreshGroup->Visible = false;

    }
    Application->MainForm->Caption=(AnsiString)"TBS Library - UserGroups";
}
void __fastcall TfrmUserAdmin::lstvwGroupsDblClick(TObject *Sender)
{
  if (!bEditGroup)
     ModifyGroup();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwUsersDblClick(TObject *Sender)
{
  if (!bEditUser)
     ModifyUser();
}
//---------------------------------------------------------------------------
void __fastcall TfrmUserAdmin::WndProc(TMessage &Message)
{
  if ( (Message.Msg!=WM_SYSCOMMAND)||
    ((Message.WParam!=SC_NEXTWINDOW)&&(Message.WParam!=SC_PREVWINDOW)) )
   TForm::WndProc(Message);
}


void __fastcall TfrmUserAdmin::FormCreate(TObject *Sender)
{
  bEditUser=false;
  bEditGroup=false;
  bModifyUser=false;
  bModifyGroup=false;
  bFirstIntoUser=true;

}
//---------------------------------------------------------------------------




void __fastcall TfrmUserAdmin::tbbtnAddGroupClick(TObject *Sender)
{
  AddGroup();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::tbbtnModifyGroupClick(TObject *Sender)
{
   ModifyGroup();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::tbbtnDeleteGroupClick(TObject *Sender)
{
    DeleteGroup();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::tbbtnAddUserClick(TObject *Sender)
{
    AddUser();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::tbbtnModifyUserClick(TObject *Sender)
{
    ModifyUser();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::tbbtnDeleteUserClick(TObject *Sender)
{
    DeleteUser();    
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::FormShow(TObject *Sender)
{
nbkUsersAdmin->PageIndex = 0;
RefreshGroups();
if (cboUserGroups->Items->Count > 0 )
	cboUserGroups->ItemIndex = 0;
RefreshUsers();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwUsersColumnClick(TObject *Sender, TListColumn *Column)

{
  ColumnToSort = Column->Index;
  dynamic_cast<TCustomListView *>(Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwUsersCompare(TObject *Sender, TListItem *Item1,
		  TListItem *Item2, int Data, int &Compare)
{
  if (ColumnToSort == 0)
	Compare = CompareText(Item1->Caption,Item2->Caption);
  else
  {
	int ix = ColumnToSort - 1;
	Compare =
	  CompareText(Item1->SubItems->Strings[ix], Item2->SubItems->Strings[ix]);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwGroupsCompare(TObject *Sender, TListItem *Item1,
		  TListItem *Item2, int Data, int &Compare)
{
  if (ColumnToSort == 0)
	Compare = CompareText(Item1->Caption,Item2->Caption);
  else
  {
	int ix = ColumnToSort - 1;
	Compare =
	  CompareText(Item1->SubItems->Strings[ix], Item2->SubItems->Strings[ix]);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmUserAdmin::lstvwGroupsColumnClick(TObject *Sender, TListColumn *Column)

{
  ColumnToSort = Column->Index;
  dynamic_cast<TCustomListView *>(Sender)->AlphaSort();

}
//---------------------------------------------------------------------------

