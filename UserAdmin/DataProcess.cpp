//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DataProcess.h"
#include "UserData.h"
#include "UserFrm.h"
#include "frm_Main.h"
#include "CommFunc.h"
#include "StoryData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
bool RefreshUsers()
{
   AnsiString strSQLClause;
   int j;
   TListItem *pListItem;
   if (frmUserAdmin->cboUserGroups->ItemIndex==0)
	  strSQLClause=(AnsiString)"Select UserID,code,name,department,phone,remark from lib_User";
   else
	  strSQLClause=(AnsiString)"Select UserID,code,name,department,phone,remark from lib_User Where UserID In(Select UserID from lib_GroupUser Where GroupID='"
				   +(AnsiString)SQLStr(frmUserAdmin->cboUserGroups->Text)+(AnsiString)"')";
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
   j=0;
//   frmUserAdmin->lstvwUsers->BeginUpdate();
   frmUserAdmin->lstvwUsers->Items->Clear();
   while (!dmdUserAdmin->daqUsers->Eof)
   {
      if (j>=MAXROWS)
      {
//         Application->MessageBox("The Browsing list is too long,please change query condition to reduce the number of records being browsed!","TBS Library",MB_OK|MB_ICONINFORMATION);
		 frmMain->stbStory->Panels->Items[2]->Text="The Browsing list is too long!";
//         frmUserAdmin->lstvwUsers->EndUpdate();
		 frmMain->stbStory->Panels->Items[1]->Text="User numbers: "+(AnsiString)dmdUserAdmin->daqUsers->RecordCount;
		 return true;
	  }
	  else
      {
        pListItem=frmUserAdmin->lstvwUsers->Items->Add();
        pListItem->Caption=OrigStr(dmdUserAdmin->daqUsers->FieldByName("UserID")->AsString);
        pListItem->SubItems->Add(OrigStr(dmdUserAdmin->daqUsers->FieldByName("name")->AsString));
        pListItem->SubItems->Add(OrigStr(dmdUserAdmin->daqUsers->FieldByName("code")->AsString));
        pListItem->SubItems->Add(OrigStr(dmdUserAdmin->daqUsers->FieldByName("department")->AsString));
        pListItem->SubItems->Add(OrigStr(dmdUserAdmin->daqUsers->FieldByName("phone")->AsString));
        pListItem->SubItems->Add(OrigStr(dmdUserAdmin->daqUsers->FieldByName("remark")->AsString));
        dmdUserAdmin->daqUsers->Next();
        j++;
      }
   }
//   frmUserAdmin->lstvwUsers->EndUpdate();
   frmMain->stbStory->Panels->Items[1]->Text="User numbers: "+(AnsiString)frmUserAdmin->lstvwUsers->Items->Count;
   return true;
}
bool RefreshGroups()
{
   int nItemIndex,nType,j;
   AnsiString strSQLClause;
   TListItem *pListItem;
   nItemIndex=frmUserAdmin->cboUserGroups->ItemIndex;
   strSQLClause=(AnsiString)"select GroupID,remark,GroupType from lib_Group";
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
   j=0;
   frmUserAdmin->cboUserGroups->Items->Clear();
   frmUserAdmin->cboUserGroups->Items->Add("everyone");
 //  frmUserAdmin->lstvwGroups->BeginUpdate();
   frmUserAdmin->lstvwGroups->Items->Clear();
   while (!dmdUserAdmin->daqGroups->Eof)
   {
      if (j>=MAXROWS)
      {
//         Application->MessageBox("The Browsing list is too long,please change query condition to reduce the number of records being browsed!","TBS Library",MB_OK|MB_ICONINFORMATION);
         if (nItemIndex<=dmdUserAdmin->daqGroups->RecordCount)
            frmUserAdmin->cboUserGroups->ItemIndex=nItemIndex;
         else
            frmUserAdmin->cboUserGroups->ItemIndex=frmUserAdmin->lstvwGroups->Items->Count;

         frmMain->stbStory->Panels->Items[2]->Text="The Browsing list is too long!";
	//     frmUserAdmin->lstvwGroups->EndUpdate();
         frmMain->stbStory->Panels->Items[1]->Text="User Group numbers: "+(AnsiString)dmdUserAdmin->daqGroups->RecordCount;
         return true;
      }
      else
      {
      frmUserAdmin->cboUserGroups->Items->Add(OrigStr(dmdUserAdmin->daqGroups->FieldByName("GroupID")->AsString));
      pListItem=frmUserAdmin->lstvwGroups->Items->Add();
      pListItem->Caption=OrigStr(dmdUserAdmin->daqGroups->FieldByName("GroupID")->AsString);
      pListItem->SubItems->Add(OrigStr(dmdUserAdmin->daqGroups->FieldByName("remark")->AsString));
      nType=dmdUserAdmin->daqGroups->FieldByName("GroupType")->AsInteger;
      switch (nType)
      {
         case 0:
              pListItem->SubItems->Add("Administrator");
              break;
         case 1:
              pListItem->SubItems->Add("Manager");
              break;
         case 2:
              pListItem->SubItems->Add("Operator");
              break;
         case 3:
              pListItem->SubItems->Add("User");
              break;
      }
      dmdUserAdmin->daqGroups->Next();
      j++;
      }
   }
 //  frmUserAdmin->lstvwGroups->EndUpdate();
   if (nItemIndex<=frmUserAdmin->lstvwGroups->Items->Count)
      frmUserAdmin->cboUserGroups->ItemIndex=nItemIndex;
   else
      frmUserAdmin->cboUserGroups->ItemIndex=frmUserAdmin->lstvwGroups->Items->Count;
   frmMain->stbStory->Panels->Items[1]->Text="User Group numbers: "+(AnsiString)frmUserAdmin->lstvwGroups->Items->Count;
   return true;
}
bool UsersAdd()
{
  AnsiString strSQLClause,strPassword;
  strPassword=frmUserAdmin->edtUserPassword->Text.Trim();
  EncodeString(strPassword);
  strSQLClause=(AnsiString)"insert lib_User (UserID,name,code,department,phone,remark,passwd)values('"
               +(AnsiString)SQLStr(frmUserAdmin->edtUserID->Text.Trim())
               +(AnsiString)"',"+(AnsiString)"'"
               +SQLStr(frmUserAdmin->edtUserName->Text.Trim())
               +(AnsiString)"',"+(AnsiString)"'"
               +SQLStr(frmUserAdmin->edtUserCode->Text.Trim())
               +(AnsiString)"',"+(AnsiString)"'"
               +SQLStr(frmUserAdmin->edtUserDep->Text.Trim())
               +(AnsiString)"',"+(AnsiString)"'"
               +SQLStr(frmUserAdmin->edtUserPhone->Text.Trim())
               +(AnsiString)"',"+(AnsiString)"'"
               +SQLStr(frmUserAdmin->edtUserRemark->Text.Trim())
               +(AnsiString)"',"+(AnsiString)"'"
               +SQLStr(strPassword)
               +(AnsiString)"')";
  dmdUserAdmin->daqUsers->SQL->Clear();
  dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
  try
  {
    dmdUserAdmin->daqUsers->ExecSQL();
    CreateLog("Add Users",frmUserAdmin->edtUserID->Text.Trim(),frmUserAdmin->edtUserName->Text.Trim(),"User Admin");
    dmdUserAdmin->daqUsers->SQL->Clear();
    if (frmUserAdmin->cboUserGroups->ItemIndex==0)
       strSQLClause=(AnsiString)"Select UserID,code,name,department,phone,remark from lib_User";
    else
       strSQLClause=(AnsiString)"Select UserID,code,name,department,phone,remark from lib_User Where UserID In(Select UserID from lib_GroupUser Where GroupID='"
                    +(AnsiString)SQLStr(frmUserAdmin->cboUserGroups->Text)+(AnsiString)"')";
    dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
    dmdUserAdmin->daqUsers->Open();
	frmMain->stbStory->Panels->Items[1]->Text="User numbers: "+(AnsiString)dmdUserAdmin->daqUsers->RecordCount;
    return true;
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    return false;
  }
}
bool GroupsAdd()
{
  AnsiString strSQLClause;
  strSQLClause=(AnsiString)"insert lib_Group (GroupID,remark,GroupType)values('"
               +(AnsiString)SQLStr(frmUserAdmin->edtGroupID->Text.Trim())
               +(AnsiString)"',"+(AnsiString)"'"
               +SQLStr(frmUserAdmin->edtGroupRemark->Text.Trim())
               +(AnsiString)"',"+
               +frmUserAdmin->cboGroupType->ItemIndex
               +(AnsiString)")";
  dmdUserAdmin->daqGroups->SQL->Clear();
  dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
  try
  {
    dmdUserAdmin->daqGroups->ExecSQL();
    CreateLog("Add User Groups",frmUserAdmin->edtGroupID->Text.Trim(),frmUserAdmin->edtGroupID->Text.Trim(),"User Admin");
    return true;
  }
  catch(...)
  {
	Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    return false;
  }
}
bool UsersDelete()
{
   AnsiString strSQLClause,strUserID,strUserName;
   TListItem *pListItem,*pNextItem;
   TItemStates ItemState;
   int nSelected,nSuper;
   ItemState<<isSelected;
   nSuper=0;
   pListItem=frmUserAdmin->lstvwUsers->Selected;
   nSelected=frmUserAdmin->lstvwUsers->SelCount;
   try
   {
     for (int i=0;i<nSelected;i++)
     {
       pNextItem=frmUserAdmin->lstvwUsers->GetNextItem(pListItem,sdBelow,ItemState);
       strUserID=pListItem->Caption;
       strUserName=pListItem->SubItems->Strings[0];
       dmdUserAdmin->daqUsers->SQL->Clear();
       strSQLClause=(AnsiString)"delete lib_User where UserID='"
                  +(AnsiString)SQLStr(strUserID)+(AnsiString)"'";
       dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
       strSQLClause=(AnsiString)"delete lib_GroupUser where UserID='"
                  +(AnsiString)SQLStr(strUserID)+(AnsiString)"'";
       dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
       strSQLClause=(AnsiString)"select UserID from lib_User where UserID='"
                    +(AnsiString)SQLStr(strUserID)+(AnsiString)"'";
       if (!CheckUnique(strSQLClause))
       {
         if (frmUserAdmin->Checkprivilege(strUserID))
         {
           dmdUserAdmin->daqUsers->ExecSQL();
           CreateLog("Delete Users",strUserID,strUserName,"User Admin");
           frmUserAdmin->lstvwUsers->Items->
               Delete(frmUserAdmin->lstvwUsers->Items->IndexOf(pListItem));
         }
         else
           nSuper++;
       }
       else
         frmUserAdmin->lstvwUsers->Items->
               Delete(frmUserAdmin->lstvwUsers->Items->IndexOf(pListItem));
       pListItem=pNextItem;
     }
     dmdUserAdmin->daqUsers->SQL->Clear();
     if (frmUserAdmin->cboUserGroups->ItemIndex==0)
         strSQLClause=(AnsiString)"Select UserID,code,name,department,phone,remark from lib_User";
     else
         strSQLClause=(AnsiString)"Select UserID,code,name,department,phone,remark from lib_User Where UserID In(Select UserID from lib_GroupUser Where GroupID='"
                      +(AnsiString)SQLStr(frmUserAdmin->cboUserGroups->Text)+(AnsiString)"')";
     dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
     dmdUserAdmin->daqUsers->Open();
     frmMain->stbStory->Panels->Items[1]->Text="User numbers: "+(AnsiString)dmdUserAdmin->daqUsers->RecordCount;
   }
   catch(...)
   {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return false;
   }

   if (nSuper<1)
      frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Some users have been deleted.";
   else
      frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"There are some users you have no privilege to delete.";
   return true;
}
bool GroupsDelete()
{
   AnsiString strSQLClause,strGroupID;
   TListItem *pListItem,*pNextItem;
   int nEditType,nSuper;
   TItemStates ItemState;
   int nSelected;
   ItemState<<isSelected;
   nSuper=0;
   pListItem=frmUserAdmin->lstvwGroups->Selected;
   nSelected=frmUserAdmin->lstvwGroups->SelCount;
   try
   {
     for (int i=0;i<nSelected;i++)
     {
       pNextItem=frmUserAdmin->lstvwGroups->GetNextItem(pListItem,sdBelow,ItemState);
       strGroupID=pListItem->Caption;
       dmdUserAdmin->daqGroups->SQL->Clear();
       strSQLClause=(AnsiString)"delete lib_Group where GroupID='"
                  +(AnsiString)SQLStr(strGroupID)+(AnsiString)"'";
       dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
       strSQLClause=(AnsiString)"delete lib_GroupUser where GroupID='"
                  +(AnsiString)SQLStr(strGroupID)+(AnsiString)"'";
       dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
       strSQLClause=(AnsiString)"select GroupType from lib_Group where GroupID='"
                    +SQLStr(strGroupID)+(AnsiString)"'";
       dmdUserAdmin->daqCheck->SQL->Clear();
       dmdUserAdmin->daqCheck->SQL->Add(strSQLClause);
       dmdUserAdmin->daqCheck->Open();
       if (dmdUserAdmin->daqCheck->RecordCount<1)
       {
         frmUserAdmin->lstvwGroups->Items->
                    Delete(frmUserAdmin->lstvwGroups->Items->IndexOf(pListItem));
         frmUserAdmin->cboUserGroups->Items->Delete(frmUserAdmin->cboUserGroups->Items->IndexOf(strGroupID));
       }
       else
       {
         nEditType=dmdUserAdmin->daqCheck->FieldByName("GroupType")->AsInteger;
         if ((nEditType>frmUserAdmin->nGroupType)||(frmUserAdmin->nGroupType<2))
         {
           dmdUserAdmin->daqGroups->ExecSQL();
           CreateLog("Delete User Groups",strGroupID,strGroupID,"User Admin");
           frmUserAdmin->lstvwGroups->Items->
                    Delete(frmUserAdmin->lstvwGroups->Items->IndexOf(pListItem));
           frmUserAdmin->cboUserGroups->Items->Delete(frmUserAdmin->cboUserGroups->Items->IndexOf(strGroupID));
         }
         else
           nSuper++;
       }
       pListItem=pNextItem;
     }
     dmdUserAdmin->daqGroups->SQL->Clear();
     strSQLClause=(AnsiString)"select GroupID,remark,GroupType from lib_Group";
     dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
     dmdUserAdmin->daqGroups->Open();
     frmMain->stbStory->Panels->Items[1]->Text="User numbers: "+(AnsiString)dmdUserAdmin->daqGroups->RecordCount;

   }
   catch(...)
   {
	 Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	 return false;
   }

   if (nSuper<1)
      frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"Several user groups have been deleted.";
   else
      frmMain->stbStory->Panels->Items[2]->Text=(AnsiString)"There are some groups you have no privilege to delete.";
   return true;
}
bool UserGroupsShow()
{
   AnsiString strSQLClause;
   strSQLClause=(AnsiString)"Select * from lib_GroupUser Where UserID='"
                   +(AnsiString)SQLStr(frmUserAdmin->edtUserID->Text)+(AnsiString)"'";
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
   frmUserAdmin->lstCurrentGroups->Items->Clear();
   while (!dmdUserAdmin->daqUsers->Eof)
   {
      frmUserAdmin->lstCurrentGroups->Items->Add(OrigStr(dmdUserAdmin->daqUsers->FieldValues["GroupID"]));
      dmdUserAdmin->daqUsers->Next();
   }
   return true;
}
bool GroupUsersShow()
{
   AnsiString strSQLClause;
   strSQLClause=(AnsiString)"Select * from lib_GroupUser Where GroupID='"
                   +(AnsiString)SQLStr(frmUserAdmin->edtGroupID->Text)+(AnsiString)"'";
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
   frmUserAdmin->lstCurrentUsers->Items->Clear();
   while (!dmdUserAdmin->daqUsers->Eof)
   {
      frmUserAdmin->lstCurrentUsers->Items->Add(OrigStr(dmdUserAdmin->daqUsers->FieldValues["UserID"]));
      dmdUserAdmin->daqUsers->Next();
   }
   return true;
}
bool UsersModify()
{
   AnsiString strSQLClause;
   AnsiString strPassword;
   strPassword=frmUserAdmin->edtUserPassword->Text.Trim();
   EncodeString(strPassword);
   dmdUserAdmin->daqUsers->SQL->Clear();
   strSQLClause=(AnsiString)"update lib_User set UserID="+
                (AnsiString)"'"+
                (AnsiString)SQLStr(frmUserAdmin->edtUserID->Text.Trim())+
                (AnsiString)"',name='"+
                (AnsiString)SQLStr(frmUserAdmin->edtUserName->Text.Trim())+
                (AnsiString)"',code='"+
                (AnsiString)SQLStr(frmUserAdmin->edtUserCode->Text.Trim())+
                (AnsiString)"',department='"+
                (AnsiString)SQLStr(frmUserAdmin->edtUserDep->Text.Trim())+
                (AnsiString)"',phone='"+
                (AnsiString)SQLStr(frmUserAdmin->edtUserPhone->Text.Trim())+
                (AnsiString)"',remark='"+
                (AnsiString)SQLStr(frmUserAdmin->edtUserRemark->Text.Trim())+
                (AnsiString)"',passwd='"+
                (AnsiString)SQLStr(strPassword)+
                (AnsiString)"'where UserID='"+
                (AnsiString)SQLStr(frmUserAdmin->strUserID)+(AnsiString)"'";
   dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
   try
   {
      dmdUserAdmin->daqUsers->Prepare();
      dmdUserAdmin->daqUsers->ExecSQL();
      CreateLog("Modify Users",frmUserAdmin->strUserID,frmUserAdmin->strUserName,"User Admin");
      return true;
   }
   catch(...)
   {
      Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      return false;
   }
}
bool GroupsModify()
{
   AnsiString strSQLClause;
   int GroupType;
   GroupType=frmUserAdmin->cboGroupType->ItemIndex;
   dmdUserAdmin->daqGroups->SQL->Clear();
   strSQLClause=(AnsiString)"update lib_Group set GroupID="+
                (AnsiString)"'"+
                (AnsiString)SQLStr(frmUserAdmin->edtGroupID->Text.Trim())+
                (AnsiString)"',remark='"+
                (AnsiString)SQLStr(frmUserAdmin->edtGroupRemark->Text.Trim())+
                (AnsiString)"',GroupType="+
                (AnsiString)GroupType+
                (AnsiString)"where GroupID='"+
                (AnsiString)SQLStr(frmUserAdmin->strGroupID)+(AnsiString)"'";
   dmdUserAdmin->daqGroups->SQL->Add(strSQLClause);
   try
   {
      dmdUserAdmin->daqGroups->Prepare();
      dmdUserAdmin->daqGroups->ExecSQL();
      CreateLog("Modify User Groups",frmUserAdmin->strGroupID,frmUserAdmin->strGroupID,"User Admin");
      return true;
   }
   catch(...)
   {
      Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
      return false;
   }
}
bool GroupUsersAdd()
{
  AnsiString strSQLClause;
  for (int i=0;i<frmUserAdmin->lstCurrentUsers->Items->Count;i++)
  {
     strSQLClause=(AnsiString)"Select GroupID,UserID from lib_GroupUser where GroupID='"
                  +(AnsiString)SQLStr(frmUserAdmin->edtGroupID->Text.Trim())+(AnsiString)"'and UserID='"
                  +(AnsiString)SQLStr(frmUserAdmin->lstCurrentUsers->Items->Strings[i])+(AnsiString)"'";
     if (CheckUnique(strSQLClause))
     {
       strSQLClause=(AnsiString)"insert lib_GroupUser(GroupID,UserID) values('"
                    +(AnsiString)SQLStr(frmUserAdmin->edtGroupID->Text.Trim())
                    +(AnsiString)"',"+(AnsiString)"'"
                    +SQLStr(frmUserAdmin->lstCurrentUsers->Items->Strings[i])
                    +(AnsiString)"')";
       dmdUserAdmin->daqUsers->SQL->Clear();
       dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
       try
       {
         dmdUserAdmin->daqUsers->ExecSQL();
       }
       catch(...)
       {
         Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
         return false;
       }
     }
   }
   return true;
}
bool UserGroupsAdd()
{
  AnsiString strSQLClause;
  for (int i=0;i<frmUserAdmin->lstCurrentGroups->Items->Count;i++)
  {
    strSQLClause=(AnsiString)"Select GroupID,UserID from lib_GroupUser where UserID='"
                  +(AnsiString)SQLStr(frmUserAdmin->edtUserID->Text.Trim())+(AnsiString)"'and GroupID='"
                  +(AnsiString)SQLStr(frmUserAdmin->lstCurrentGroups->Items->Strings[i])+(AnsiString)"'";
    if (CheckUnique(strSQLClause))
    {
      strSQLClause=(AnsiString)"insert lib_GroupUser(UserID,GroupID) values('"
                   +(AnsiString)SQLStr(frmUserAdmin->edtUserID->Text.Trim())
                   +(AnsiString)"',"+(AnsiString)"'"
                   +SQLStr(frmUserAdmin->lstCurrentGroups->Items->Strings[i])
                   +(AnsiString)"')";
      dmdUserAdmin->daqUsers->SQL->Clear();
      dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
      try
      {
        dmdUserAdmin->daqUsers->ExecSQL();
      }
      catch(...)
      {
        Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
        return false;
      }
    }
  }
  return true;
}
bool UserGroupsModify()
{
   AnsiString strSQLClause;
   dmdUserAdmin->daqUsers->SQL->Clear();
   strSQLClause=(AnsiString)"delete lib_GroupUser where UserID='"
                +(AnsiString)SQLStr(frmUserAdmin->strUserID.Trim())+(AnsiString)"'";
   dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
   try
   {
     dmdUserAdmin->daqUsers->Prepare();
     dmdUserAdmin->daqUsers->ExecSQL();
   }
   catch(...)
   {
     Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return false;
   }
   for (int i=0;i<frmUserAdmin->lstCurrentGroups->Items->Count;i++)
   {
      strSQLClause=(AnsiString)"Select GroupID,UserID from lib_GroupUser where UserID='"
                    +(AnsiString)SQLStr(frmUserAdmin->edtUserID->Text.Trim())+(AnsiString)"'and GroupID='"
                    +(AnsiString)SQLStr(frmUserAdmin->lstCurrentGroups->Items->Strings[i])+(AnsiString)"'";
      if (CheckUnique(strSQLClause))
      {
        strSQLClause=(AnsiString)"Insert lib_GroupUser (UserID,GroupID) values("+
                     (AnsiString)"'"+
                     (AnsiString)SQLStr(frmUserAdmin->edtUserID->Text.Trim())+
                     (AnsiString)"',"+
                     (AnsiString)"'"+
                     (AnsiString)SQLStr(frmUserAdmin->lstCurrentGroups->Items->Strings[i])+
                     (AnsiString)"')";
        dmdUserAdmin->daqUsers->SQL->Clear();
        dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
        try
        {
          dmdUserAdmin->daqUsers->Prepare();
          dmdUserAdmin->daqUsers->ExecSQL();
        }
        catch(...)
        {
          Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
          return false;
        }
      }
   }
   return true;
}
bool GroupUsersModify()
{
   AnsiString strSQLClause;
   dmdUserAdmin->daqUsers->SQL->Clear();
   strSQLClause=(AnsiString)"delete lib_GroupUser where GroupID='"
                +(AnsiString)SQLStr(frmUserAdmin->strGroupID.Trim())+(AnsiString)"'";
   dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
   try
   {
     dmdUserAdmin->daqUsers->Prepare();
     dmdUserAdmin->daqUsers->ExecSQL();
   }
   catch(...)
   {
     Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
     return false;
   }
   for (int i=0;i<frmUserAdmin->lstCurrentUsers->Items->Count;i++)
   {
      strSQLClause=(AnsiString)"Select GroupID,UserID from lib_GroupUser where GroupID='"
                    +(AnsiString)SQLStr(frmUserAdmin->edtGroupID->Text.Trim())+(AnsiString)"'and UserID='"
                    +(AnsiString)SQLStr(frmUserAdmin->lstCurrentUsers->Items->Strings[i])+(AnsiString)"'";
      if (CheckUnique(strSQLClause))
      {
        strSQLClause=(AnsiString)"Insert lib_GroupUser (GroupID,UserID) values("+
                     (AnsiString)"'"+
                     (AnsiString)SQLStr(frmUserAdmin->edtGroupID->Text.Trim())+
                     (AnsiString)"',"+
                     (AnsiString)"'"+
                     (AnsiString)SQLStr(frmUserAdmin->lstCurrentUsers->Items->Strings[i])+
                     (AnsiString)"')";
        dmdUserAdmin->daqUsers->SQL->Clear();
        dmdUserAdmin->daqUsers->SQL->Add(strSQLClause);
        try
        {
          dmdUserAdmin->daqUsers->Prepare();
          dmdUserAdmin->daqUsers->ExecSQL();
        }
        catch(...)
		{
		  Application->MessageBox(L"Database error!",L"TBS Library",MB_OK|MB_ICONINFORMATION);
          return false;
        }
      }
   }
   return true;
}
bool CreateLog(AnsiString Operation,AnsiString ObjID,AnsiString ObjStr,AnsiString Module)
{
  AnsiString strSQLClause,strFileID;
  strSQLClause =(AnsiString)"exec GetNewFileID";
  dmdUserAdmin->daqLogs->Close();
  dmdUserAdmin->daqLogs->SQL->Clear();
  dmdUserAdmin->daqLogs->SQL->Add(strSQLClause);
  try
  {
    dmdUserAdmin->daqLogs->Open();
  }
  catch(...)
  {
	Application->MessageBox(L"Failed update table lib_Log",L"TBS Library",MB_OK|MB_ICONINFORMATION);
    return false;
  }
  dmdUserAdmin->daqLogs->First();
  strFileID=dmdUserAdmin->daqLogs->FieldValues["FileID"];
  strSQLClause = (AnsiString)"Insert into lib_Log (LogID,OpID,OpTime,Operation,ObjID,ObjStr,Module) values('"
                  +(AnsiString)SQLStr(strFileID)
                  +(AnsiString)"','"
				  +SQLStr(dmdStory->strCurrentUserID)
                  +(AnsiString)"',GetDate(),'"
				  +SQLStr(Operation)
                  +(AnsiString)"','"
                  +SQLStr(ObjID)
                  +(AnsiString)"','"
                  +SQLStr(ObjStr)
                  +(AnsiString)"','"
                  +SQLStr(Module)
                  +(AnsiString)"')";
 dmdUserAdmin->daqLogs->SQL->Clear();
 dmdUserAdmin->daqLogs->SQL->Add(strSQLClause);
 try
 {
  dmdUserAdmin->daqLogs->ExecSQL();
  }
 catch(...)
 {
  Application->MessageBox(L"Failed update table lib_Log",L"TBS Library",MB_OK|MB_ICONINFORMATION);
  return false;
 }
 return true;
}
bool CheckUnique(AnsiString strSQLClause)
{
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
   if (dmdUserAdmin->daqCheck->RecordCount>0)
       return false;
   return true;
}
void ClearUserInput()
{
  frmUserAdmin->edtUserID->Clear();
  frmUserAdmin->edtUserName->Clear();
  frmUserAdmin->edtUserRemark->Clear();
  frmUserAdmin->edtUserDep->Clear();
  frmUserAdmin->edtUserCode->Clear();
  frmUserAdmin->edtUserPhone->Clear();
  frmUserAdmin->edtUserPassword->Clear();
  frmUserAdmin->edtUserConfirm->Clear();
  frmUserAdmin->lstCurrentGroups->Items->Clear();
}
void ClearGroupInput()
{
  frmUserAdmin->edtGroupID->Clear();
  frmUserAdmin->edtGroupRemark->Clear();
//frmUserAdmin->cboGroupType->ItemIndex=-1;
  frmUserAdmin->lstCurrentUsers->Items->Clear();
}
bool InitUsers()
{
   AnsiString strSQLClause;
   strSQLClause="select GroupID from lib_Group";
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
   frmUserAdmin->cboUserGroups->Items->Clear();
   frmUserAdmin->cboUserGroups->Items->Add("everyone");
   while (!dmdUserAdmin->daqGroups->Eof)
   {
      frmUserAdmin->cboUserGroups->Items->Add(OrigStr(dmdUserAdmin->daqGroups->FieldValues["GroupID"]));
      dmdUserAdmin->daqGroups->Next();
   }
   frmUserAdmin->cboUserGroups->ItemIndex=0;
   if (RefreshUsers())
     return true;
   return false;
}
