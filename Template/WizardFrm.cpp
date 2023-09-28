//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CommFunc.h"
#include "utGlobDm.h"
#include "TemplateForm.h"
#include "WizardFrm.h"
#include "utGlobDm.h"
#include "htmlformTemp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EditBtn"
//#pragma link "AdvGrid"
//#pragma link "AdvObj"
//#pragma link "BaseGrid"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvUtil"
#pragma resource "*.dfm"

#define FIELDSTORYID    "StoryID"
#define FIELDWRITETIME  "WriteTime"
#define FIELDMaterialID "MaterialID"

TfrmWizard *frmWizard;
extern int g_naFieldTypeSize[MAXFIELDTYPES];
//---------------------------------------------------------------------------
__fastcall TfrmWizard::TfrmWizard(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::FormShow(TObject *Sender)
{
	btnBack->Enabled = false;
	btnNext->Enabled = false;

	btnBack->Caption="< &Back";
	btnNext->Caption="&Next >";

	edtNodeName->Clear();
	chbInherit->Checked = false;
	chkType->Checked    = false;

	if (cboDirectory->Items->Count == 1)
	{
	  cboDirectory->ItemIndex = 0;
	  chbInherit->Enabled     = false;
	}
	else
	{
		if (cboDirectory->ItemIndex == 0)
		  chbInherit->Enabled = false;
		else
		  chbInherit->Enabled = true;
	}


    if (strgrdWizard->RowCount > 2)
    {
      strgrdWizard->RemoveRows(2,strgrdWizard->RowCount-2);
    }

	strgrdWizard->ClearRows(1,1);

   strgrdWizard->ClearComboString();
	strgrdWizard->Cells[0][0]="FieldName";
	strgrdWizard->Cells[1][0]="FieldType";
	strgrdWizard->Cells[2][0]="FieldSize";
  //  strgrdWizard->AddComboString("binary");
  //  strgrdWizard->AddComboString("bit");
  //  strgrdWizard->AddComboString("char");
	strgrdWizard->AddComboString("datetime");
  //  strgrdWizard->AddComboString("decimal");
  //  strgrdWizard->AddComboString("float");
	strgrdWizard->AddComboString("image");
	strgrdWizard->AddComboString("int");
  //  strgrdWizard->AddComboString("money");
  //  strgrdWizard->AddComboString("numeric");
  //  strgrdWizard->AddComboString("real");
  //  strgrdWizard->AddComboString("smalldatetime");
  //  strgrdWizard->AddComboString("smallint");
  //  strgrdWizard->AddComboString("smallmoney");
  //  strgrdWizard->AddComboString("sysname");
	strgrdWizard->AddComboString("text");
  //  strgrdWizard->AddComboString("timestamp");
  //  strgrdWizard->AddComboString("tinyint");
  //  strgrdWizard->AddComboString("varbinary");
	strgrdWizard->AddComboString("varchar");
	nbkWizard->PageIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::btnBackClick(TObject *Sender)
{
  switch (nbkWizard->PageIndex)
  {
	case 1:
		  btnBack->Enabled=false;
		  btnBack->Enabled=true;
		  nbkWizard->PageIndex=0;
		  break;
	case 2:
		  nbkWizard->PageIndex=1;
		  break;
	case 3:
			  if (chkType->Checked)
				nbkWizard->PageIndex=2;
			  else
				nbkWizard->PageIndex=1;

              btnBack->Caption="< &Back";
              btnNext->Caption="&Next >";
              break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::btnNextClick(TObject *Sender)
{
  AnsiString     strNodeID,strNodeName,strParentID;
  AnsiString     strFieldName,strFieldType,strFieldSize;
  AnsiString     strSQLClause,strBaseFieldsSQLClause,strExtFieldsSQLClause;
  int            i,nIndex,nRetVal,nCurrentYear;
  TTreeNode *    pTreeNode,* tnNodeJustAdded;
  AnsiString     strInputTemplate,strBrowseTemplate,strSearchTemplate,strAdvancedTemplate,strListTemplate;
  AnsiString     strTemplate,strWarning,strCurrentYear;
  AnsiString     strBaseFieldsIndexClause,strExtFieldsIndexClause;
  TBlobStream *  bsTemplateStream;
  TStringList *  slTemplate;
  TLocateOptions LocateOption;

  bsTemplateStream = NULL;
  slTemplate       = NULL;

  try
  {
	  switch (nbkWizard->PageIndex)
	  {
		case 0:
                  //清空表格
                  if (strgrdWizard->RowCount > 2)
                  {
                    strgrdWizard->RemoveRows(2,strgrdWizard->RowCount-2);
                  }

                  strgrdWizard->ClearRows(1,1);

					strgrdWizard->RowCount=2;
				  strgrdWizard->Rows[1]->Clear();

				  if (cboDirectory->ItemIndex > 0)
                  {
                      strParentID = (AnsiString)((char *)frmTemplate->trvwTemplate->Selected->Data);

                      nRetVal = frmTemplate->IsLeafNode(strParentID);

                      if (nRetVal == DATABASEERROR)
                      //不存在指定节点，数据库不一致
                          return;

                      if (nRetVal == LEAFNODE)
					  //当前节点为叶节点，不能创建子节点
                      {
						  Application->MessageBox(L"You can't create subnodes under a leaf node !",GLOBALMSGCAPTION,48);
                          return;
                      }

                      if (chbInherit->Checked)
                      {
                         strSQLClause = (AnsiString)"select * from "+
                                        strLIBEXTENDFIELDS+
                                        " where CategoryID = '"+
                                        SQLStr(strParentID)+
                                        (AnsiString)"'";

                         dmTemplate->qeGlobal->SQL->Clear();
                         dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                         dmTemplate->qeGlobal->Open();

                         dmTemplate->qeGlobal->First();
                         while (! dmTemplate->qeGlobal->Eof)
                         {
                              strFieldName = OrigStr(dmTemplate->qeGlobal->FieldValues["FieldName"]);
                              strFieldType = OrigStr(dmTemplate->qeGlobal->FieldValues["FieldType"]);
                              strFieldSize = dmTemplate->qeGlobal->FieldValues["FieldSize"];

							  strgrdWizard->Cells[0][strgrdWizard->RowCount-1] = strFieldName;
                              strgrdWizard->Cells[1][strgrdWizard->RowCount-1] = strFieldType;
                              strgrdWizard->Cells[2][strgrdWizard->RowCount-1] = strFieldSize;

                              strgrdWizard->RowCount++;

                              dmTemplate->qeGlobal->Next();
                         }
                      }
                  }

				btnBack->Enabled=true;
				nbkWizard->PageIndex=1;

			  break;
		case 1:

			  if (chkType->Checked)
              {
                   if ((nIndex = strgrdWizard->Cols[0]->IndexOf(FIELDDATEID)) < 0)
                   {
					  Application->MessageBox(L"You must create a field named 'Date' !",GLOBALMSGCAPTION,48);
                      return;
                   }
                   if (strgrdWizard->Cols[0]->Strings[nIndex] != FIELDDATEID)
				   {
					  Application->MessageBox(L"You must create a field named 'Date' !",GLOBALMSGCAPTION,48);
                      return;
                   }
                   if (strgrdWizard->Cols[1]->Strings[nIndex] != typeDATETIME)
                   {
					  Application->MessageBox(L"The type of field 'Date' must be datetime !",GLOBALMSGCAPTION,48);
                      return;
				   }

				   nbkWizard->PageIndex=2;
			  }
			  else
			  {
				nbkWizard->PageIndex=3;
				btnBack->Caption="< &Back";
				btnNext->Caption="&Finish";
			  }
			  break;
		case 2:
				  strInputTemplate = edtInput->Text.Trim();
				  if (strInputTemplate.IsEmpty())
				  {
						Application->MessageBox(L"You must specify an input template!",GLOBALMSGCAPTION,48);
						edtInput->SetFocus();
						return;
				  }

				  strBrowseTemplate = edtBrowse->Text.Trim();
                  if (strBrowseTemplate.IsEmpty())
                  {
                        Application->MessageBox(L"You must specify an browse template!",GLOBALMSGCAPTION,48);
                        edtBrowse->SetFocus();
		        	    return;
                  }

                  strSearchTemplate = edtSearch->Text.Trim();
                  if (strSearchTemplate.IsEmpty())
                  {
                        Application->MessageBox(L"You must specify an search template!",GLOBALMSGCAPTION,48);
                        edtSearch->SetFocus();
		        	    return;
                  }

                  strAdvancedTemplate = edtAdvance->Text.Trim();
                  if (strAdvancedTemplate.IsEmpty())
                  {
                        Application->MessageBox(L"You must specify an advanced template!",GLOBALMSGCAPTION,48);
                        edtAdvance->SetFocus();
		        	    return;
                  }

                  strListTemplate = edtList->Text.Trim();
                  if (strListTemplate.IsEmpty())
                  {
                        Application->MessageBox(L"You must specify an list template!",GLOBALMSGCAPTION,48);
                        edtList->SetFocus();
		        	    return;
                  }

                  //检查模板的有效性
                  if (! CheckHtmlValid())
                    return;

    			  nbkWizard->PageIndex=3;
                  btnBack->Caption="< &Back";
                  btnNext->Caption="&Finish";
                  break;
		case 3:
			  strSQLClause = "GetNewFileID";
			  dmTemplate->qeGlobal->SQL->Clear();
			  dmTemplate->qeGlobal->SQL->Add(strSQLClause);
			  dmTemplate->qeGlobal->Open();

			  dmTemplate->qeGlobal->First();
			  strNodeID = dmTemplate->qeGlobal->FieldValues["FileID"];

              //查询服务器上显示的年份
              strSQLClause = "select year = datepart(yy,getdate())";

              dmTemplate->qeGlobal->SQL->Clear();
              dmTemplate->qeGlobal->SQL->Add(strSQLClause);
              dmTemplate->qeGlobal->Open();

              dmTemplate->qeGlobal->First();
              nCurrentYear = dmTemplate->qeGlobal->FieldValues["year"];

			  strNodeName = edtNodeName->Text.Trim();

			  if (cboDirectory->ItemIndex == 0)
			  //指定根节点
			  {
				   strParentID = "";
			  }
			  else
			  {
				  pTreeNode = frmTemplate->trvwTemplate->Selected;

				  strParentID = (AnsiString)((char *)pTreeNode->Data);
			  }

			  //创建新节点
			  strSQLClause = "insert into "+
							 (AnsiString)strSTORYLIBTREE+
							 " values('"+
							 SQLStr(strNodeID)+
							 "','"+
							 SQLStr(strParentID)+
							 "','"+
							 SQLStr(strNodeName)+
							 "',"+
							 String((int)chkType->Checked)
							 +","+
							 String(0)+
							 ",NULL,NULL,NULL,NULL,NULL)";

			  dmTemplate->qeGlobal->SQL->Clear();
			  dmTemplate->qeGlobal->SQL->Add(strSQLClause);
			  dmTemplate->qeGlobal->ExecSQL();

              if (chkType->Checked)
              //指定新叶节点的五大界面模板
              {
                if (dmTemplate->tbCategory->Active)
                    dmTemplate->tbCategory->Close();

                dmTemplate->tbCategory->Open();

        		//确定Locate操作的参数
		        LocateOption << loCaseInsensitive;

           		if (! dmTemplate->tbCategory->Locate("NodeID",SQLStr(strNodeID),LocateOption))
                {
                    Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
                    return;
                }

                slTemplate = new TStringList;

                strInputTemplate    = edtInput->Text.Trim();
                strBrowseTemplate   = edtBrowse->Text.Trim();
                strSearchTemplate   = edtSearch->Text.Trim();
                strAdvancedTemplate = edtAdvance->Text.Trim();
                strListTemplate     = edtList->Text.Trim();

                for (i=0;i<5;i++)
                {
                    switch (i)
                    {
                        case 0:
                                strTemplate  = strInputTemplate;
                                strFieldName = "InputTem";
                                break;
                        case 1:
                                strTemplate  = strBrowseTemplate;
                                strFieldName = "ViewTem";
                                break;
                        case 2:
                                strTemplate  = strSearchTemplate;
                                strFieldName = "SearchTem";
                                break;
                        case 3:
                                strTemplate  = strAdvancedTemplate;
                                strFieldName = "AdvanceTem";
                                break;
                        case 4:
                                strTemplate  = strListTemplate;
                                strFieldName = "ListTem";
                    }

                    try
                    {
                        //打开界面模板
                        slTemplate->LoadFromFile(strTemplate);
                    }
                    catch(EInOutError * e)
                    {
                        switch (i)
                        {
                            case 0:
                                    strWarning = "Open Input Template Failed!";
                                    edtInput->SetFocus();
                                    break;
                            case 1:
                                    strWarning = "Open Browse Template Failed!";
                                    edtBrowse->SetFocus();
                                    break;
                            case 2:
                                    strWarning = "Open Search Template Failed!";
                                    edtSearch->SetFocus();
                                    break;
                            case 3:
                                    strWarning = "Open Advanced Template Failed!";
                                    edtAdvance->SetFocus();
                                    break;
                            case 4:
                                    strWarning = "Open List Template Failed!";
                                    edtList->SetFocus();
                        }

						Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
                        delete slTemplate;
						slTemplate = NULL;
                        return;
                    }

                    if (slTemplate->Count > 0)
                    //执行Text字段的“写”操作
                    {
                      dmTemplate->tbCategory->Edit();
                      bsTemplateStream = new TBlobStream((TBlobField*)dmTemplate->tbCategory->FieldByName(strFieldName),bmReadWrite);
                      bsTemplateStream->Seek(0,soFromBeginning);
                      bsTemplateStream->Truncate();
                      bsTemplateStream->Write(slTemplate->Text.c_str(),slTemplate->Text.Length());
                      dmTemplate->tbCategory->Post();
                      delete bsTemplateStream;
                      bsTemplateStream = NULL;
                    }
                }

                delete slTemplate;
                slTemplate = NULL;
              }

			  strExtFieldsSQLClause   = "";
              strExtFieldsIndexClause = "";
			  for (i=1;i<strgrdWizard->RowCount;i++)
			  {
				   strFieldName = strgrdWizard->Cells[0][i].Trim();
				   strFieldType = strgrdWizard->Cells[1][i].Trim();
				   strFieldSize = strgrdWizard->Cells[2][i].Trim();

				   if ((strFieldName.IsEmpty())||(strFieldType.IsEmpty()))
						continue;

				   if ((strFieldSize.IsEmpty())||(strFieldSize == "0"))
						strFieldSize = MatchField(strFieldType);

				   //创建扩展字段
				   strSQLClause = "insert into "+
								  (AnsiString)strLIBEXTENDFIELDS+
								  " values('"+
								  SQLStr(strNodeID)+
								  "','"+
								  SQLStr(strFieldName)+
								  "','"+
								  SQLStr(strFieldType)+
								  "',"+
								  strFieldSize+
								  ")";

				   dmTemplate->qeGlobal->SQL->Clear();
         		   dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                   dmTemplate->qeGlobal->ExecSQL();

				   //组织扩展字段部分的子查询
				   strExtFieldsSQLClause = strExtFieldsSQLClause+
										   strFieldName+
										   (AnsiString)" ";

				   if (strFieldType == (AnsiString)typeVARCHAR)
				   {
						strExtFieldsSQLClause = strExtFieldsSQLClause +
                    							strFieldType+
                           						(AnsiString)" ("+
												strFieldSize+
												(AnsiString)") NULL,";
				   }
				   else
				   {
                        strExtFieldsSQLClause = strExtFieldsSQLClause +
                                                strFieldType+
                                                (AnsiString)" NULL,";
				   }

                   if ( ((strFieldType == typeVARCHAR)||(strFieldType == typeTEXT))
                       && (strFieldName!="Duration")&&(strFieldName!="TimeCode") )
                   {
                        strExtFieldsIndexClause = strExtFieldsIndexClause+
                                                  "'"+
                                                  strFieldName+
                                                  "',";
                   }
			  }

              if (chkType->Checked)
              //为叶节点则需创建相关表
              {
                //组织基本字段部分的子查询
                strSQLClause = (AnsiString)"select * from "+
                               strLIBBASEFIELDS;

                dmTemplate->qeGlobal->SQL->Clear();
                dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                dmTemplate->qeGlobal->Open();

                strBaseFieldsSQLClause = "";
                dmTemplate->qeGlobal->First();
                while (! dmTemplate->qeGlobal->Eof)
                {
                  strFieldName = OrigStr(dmTemplate->qeGlobal->FieldValues["FieldName"]);

                  strBaseFieldsSQLClause = strBaseFieldsSQLClause+
                                           strFieldName+
                                           (AnsiString)" ";

                  strFieldType = OrigStr(dmTemplate->qeGlobal->FieldValues["FieldType"]);

                  if (strFieldType == (AnsiString)typeVARCHAR)
                  {
                      if (strFieldName == FIELDSTORYID)
                      {
                          strBaseFieldsSQLClause = strBaseFieldsSQLClause +
                                                   strFieldType+
                                                   (AnsiString)" ("+
                                                   String((int)dmTemplate->qeGlobal->FieldValues["FieldSize"])+
                                                   (AnsiString)") NOT NULL,";
                      }
                      else
                      {
                         strBaseFieldsSQLClause = strBaseFieldsSQLClause +
                         strFieldType+
                         (AnsiString)" ("+
                         String((int)dmTemplate->qeGlobal->FieldValues["FieldSize"])+
                         (AnsiString)") NULL,";
                      }
                  }
                  else
                  {
                      strBaseFieldsSQLClause = strBaseFieldsSQLClause +
                                               strFieldType+
                                               (AnsiString)" NULL,";
                  }

                  if ((strFieldType == typeVARCHAR)||(strFieldType == typeTEXT))
                  {
                       strBaseFieldsIndexClause = strBaseFieldsIndexClause+
                                                  "'"+
                                                  strFieldName+
                                                  "',";
                       if(strFieldName=="StoryID")
                           strBaseFieldsIndexClause = strBaseFieldsIndexClause+
                                                  "'Date',";
                  }

                  dmTemplate->qeGlobal->Next();
                }

/*
                //创建Date字段（该字段不出现在基本字段表中）
                strBaseFieldsSQLClause = strBaseFieldsSQLClause+
                                         "Date datetime NOT NULL,";
*/

                if ((! strBaseFieldsSQLClause.IsEmpty())||(! strExtFieldsSQLClause.IsEmpty()))
                {
                    //创建Old表
                    strSQLClause = (AnsiString)"CREATE TABLE lib_old"+
                                   strNodeID+
                                   "(";

                    if (! strBaseFieldsSQLClause.IsEmpty())
                      strSQLClause = strSQLClause + strBaseFieldsSQLClause;

                    if (! strExtFieldsSQLClause.IsEmpty())
                      strSQLClause = strSQLClause + strExtFieldsSQLClause;

                    //指定主码
                    strSQLClause = strSQLClause +
                                   "CONSTRAINT PK_lib_old"+
                                   strNodeID+
                                   "_1__10 "+
                                   "PRIMARY KEY CLUSTERED("+
                                   FIELDSTORYID+
                                   ")";

//                    strSQLClause.Delete(1strSQLClause.Length(),1);

                    //给结尾加上括号
                    strSQLClause = strSQLClause + ")";

                    //创建old表
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    //创建相关索引(WriteTime)
                    strSQLClause = "CREATE INDEX NewsIndex1 ON dbo.lib_old"+
                                   strNodeID+
                                   "("+
                                   FIELDWRITETIME+
                                   ")";

                    //执行SQL语句
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    //创建相关索引(MaterialID)
                    strSQLClause = "CREATE INDEX NewsIndex2 ON dbo.lib_old"+
                                   strNodeID+
                                   "("+
                                   FIELDMaterialID+
                                   ")";

                    //执行SQL语句
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    //创建相关索引(Date)
                    strSQLClause = "CREATE INDEX NewsIndex3 ON dbo.lib_old"+
                                   strNodeID+
                                   "("+
                                   FIELDDATEID+
                                   ")";

                    //执行SQL语句
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    //创建搜索引擎的有关索引
                    if (! frmTemplate->m_strDatabaseName.IsEmpty())
                    {
                        strSQLClause = strBaseFieldsIndexClause+strExtFieldsIndexClause;

                        if (! strSQLClause.IsEmpty())
                        {
                            //删除末尾的","
                            strSQLClause.Delete(strSQLClause.Length(),1);

                            strSQLClause = (AnsiString)"master..xpCreateTextIndex '"+
                                           frmTemplate->m_strCodeType+
                                           "','"+
                                           frmTemplate->m_strDatabaseName+
                                           ".dbo.lib_old"+
                                           strNodeID+
                                           "',"+
                                           strSQLClause;

                            //执行SQL语句
                            dmTemplate->qeGlobal->SQL->Clear();
                            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                            dmTemplate->qeGlobal->ExecSQL();
                        }
                    }

                    //创建1990-明年的数据表
                    for (i=1990;i<=nCurrentYear+1;i++)
                    {
                        strSQLClause = (AnsiString)"CREATE TABLE lib_"+
                                       String(i)+
                                       "_"+
                                       strNodeID+
                                       "(";

                        if (! strBaseFieldsSQLClause.IsEmpty())
                          strSQLClause = strSQLClause + strBaseFieldsSQLClause;

                        if (! strExtFieldsSQLClause.IsEmpty())
                          strSQLClause = strSQLClause + strExtFieldsSQLClause;

                        //指定主码
                        strSQLClause = strSQLClause +
                                       "CONSTRAINT INDEX_"+
                                       String(i)+
                                       "_"+
                                       strNodeID+
                                       " "+
                                       "PRIMARY KEY CLUSTERED("+
                                       FIELDSTORYID+
                                       ")";

                        //给结尾加上括号
                        strSQLClause = strSQLClause + ")";

                        //创建年份表
                        dmTemplate->qeGlobal->SQL->Clear();
                        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                        dmTemplate->qeGlobal->ExecSQL();

                        //创建相关索引(WriteTime)
                        strSQLClause = "CREATE INDEX NewsIndex1 ON dbo.lib_"+
                                       String(i)+
                                       "_"+
                                       strNodeID+
                                       "("+
                                       FIELDWRITETIME+
                                       ")";

                        //执行SQL语句
                        dmTemplate->qeGlobal->SQL->Clear();
                        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                        dmTemplate->qeGlobal->ExecSQL();

                        //创建相关索引(MaterialID)
                        strSQLClause = "CREATE INDEX NewsIndex2 ON dbo.lib_"+
                                       String(i)+
                                       "_"+
                                       strNodeID+
                                       "("+
                                       FIELDMaterialID+
                                       ")";

                        //执行SQL语句
                        dmTemplate->qeGlobal->SQL->Clear();
                        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                        dmTemplate->qeGlobal->ExecSQL();

                        //创建相关索引(Date)
                        strSQLClause = "CREATE INDEX NewsIndex3 ON dbo.lib_"+
                                       String(i)+
                                       "_"+
                                       strNodeID+
                                       "("+
                                       FIELDDATEID+
                                       ")";

                        //执行SQL语句
                        dmTemplate->qeGlobal->SQL->Clear();
                        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                        dmTemplate->qeGlobal->ExecSQL();

                        //创建搜索引擎的有关索引
                        if (! frmTemplate->m_strDatabaseName.IsEmpty())
                        {
                            strSQLClause = strBaseFieldsIndexClause+strExtFieldsIndexClause;

                            if (! strSQLClause.IsEmpty())
                            {
                                //删除末尾的","
                                strSQLClause.Delete(strSQLClause.Length(),1);

                                strSQLClause = (AnsiString)"master..xpCreateTextIndex '"+
                                               frmTemplate->m_strCodeType+
                                               "','"+
                                               frmTemplate->m_strDatabaseName+
                                               ".dbo.lib_"+
                                               String(i)+
                                               "_"+
                                               strNodeID+
                                               "',"+
                                               strSQLClause;

                                //执行SQL语句
                                dmTemplate->qeGlobal->SQL->Clear();
                                dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                                dmTemplate->qeGlobal->ExecSQL();
                            }
                        }
                    }

                    //创建New表
                    strSQLClause = (AnsiString)"CREATE TABLE lib_new"+
                                   strNodeID+
                                   "(";

                    if (! strBaseFieldsSQLClause.IsEmpty())
                      strSQLClause = strSQLClause + strBaseFieldsSQLClause;

                    if (! strExtFieldsSQLClause.IsEmpty())
                      strSQLClause = strSQLClause + strExtFieldsSQLClause;

                    //指定主码
                    strSQLClause = strSQLClause +
                                   "CONSTRAINT PK_lib_new"+
                                   strNodeID+
                                   "_1__10 "+
                                   "PRIMARY KEY CLUSTERED("+
                                   FIELDSTORYID+
                                   ")";

//                    strSQLClause.Delete(1strSQLClause.Length(),1);

                    //给结尾加上括号
                    strSQLClause = strSQLClause + ")";

                    //执行SQL语句
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    //创建相关索引(WriteTime)
                    strSQLClause = "CREATE INDEX NewsIndex1 ON dbo.lib_new"+
                                   strNodeID+
                                   "("+
                                   FIELDWRITETIME+
                                   ")";

                    //执行SQL语句
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    //创建相关索引(MaterialID)
                    strSQLClause = "CREATE INDEX NewsIndex2 ON dbo.lib_new"+
                                   strNodeID+
                                   "("+
                                   FIELDMaterialID+
                                   ")";

                    //执行SQL语句
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();

                    //创建相关索引(Date)
                    strSQLClause = "CREATE INDEX NewsIndex3 ON dbo.lib_new"+
                                   strNodeID+
                                   "("+
                                   FIELDDATEID+
                                   ")";

                    //执行SQL语句
                    dmTemplate->qeGlobal->SQL->Clear();
                    dmTemplate->qeGlobal->SQL->Add(strSQLClause);
                    dmTemplate->qeGlobal->ExecSQL();
                }

                //更新存储过程
                frmTemplate->UpdateStoryLibrary();
              }
              
			  btnBack->Caption="< &Back";
			  btnNext->Caption="&Next >";


			  if (cboDirectory->ItemIndex == 0)
			  //指定根节点
			  {
                  tnNodeJustAdded =
                      frmTemplate->trvwTemplate->Items->Add(NULL,edtNodeName->Text.Trim());
			  }
			  else
			  {
                  tnNodeJustAdded =
                      frmTemplate->trvwTemplate->Items->AddChild(pTreeNode,edtNodeName->Text.Trim());
			  }

			  tnNodeJustAdded->Data = new char[NODEIDSIZE];
			  lstrcpy((char *)tnNodeJustAdded->Data,strNodeID.c_str());
              if (chkType->Checked)
              //创建叶节点
              {
                  tnNodeJustAdded->ImageIndex    = 1;
                  tnNodeJustAdded->SelectedIndex = 1;
              }

			  frmTemplate->trvwTemplate->Selected = tnNodeJustAdded;

			  Close();

			  break;
        }
	  }
	  catch(...)
	  {
			Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);

            if (bsTemplateStream)
                delete bsTemplateStream;

            if (slTemplate)
                delete slTemplate;

			return;
	  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::strgrdWizardGetEditorType(TObject *Sender,
      int aCol, int aRow, TEditorType &aEditor)
{
  if ((1==aCol)&&(!strgrdWizard->Cells[0][aRow].Trim().IsEmpty()))
    aEditor=edComboList;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::strgrdWizardCanEditCell(TObject *Sender,
      int Arow, int Acol, bool &canedit)
{
  if (Acol!=0)
  {
    if (strgrdWizard->Cells[0][Arow].Trim().IsEmpty())
      canedit=false;
    else
    {
      if ((Acol==2)&&(!CheckType(strgrdWizard->Cells[1][Arow])))
        canedit=false;
    }
  }
}
//---------------------------------------------------------------------------
bool TfrmWizard::CheckType(AnsiString strType)
{
  if ((strType=="char")||(strType=="varchar")||(strType=="varbinary")
     ||(strType=="binary")||(strType=="decimal")||(strType=="numeric"))
     return true;
  else
     return false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::btnInputClick(TObject *Sender)
{
  if (frmTemplate->OpenDialog1->Execute())
     edtInput->Text=frmTemplate->OpenDialog1->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::btnBrowseClick(TObject *Sender)
{
  if (frmTemplate->OpenDialog1->Execute())
     edtBrowse->Text=frmTemplate->OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::btnSearchClick(TObject *Sender)
{
  if (frmTemplate->OpenDialog1->Execute())
     edtSearch->Text=frmTemplate->OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::btnAdvanceClick(TObject *Sender)
{
  if (frmTemplate->OpenDialog1->Execute())
     edtAdvance->Text=frmTemplate->OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::edtNodeNameChange(TObject *Sender)
{
  if (edtNodeName->Text.Trim().IsEmpty())
    btnNext->Enabled=false;
  else
    btnNext->Enabled=true;
}
//---------------------------------------------------------------------------

bool TfrmWizard::CheckFields(AnsiString strFieldName)
{
  int j;
  AnsiString strSQLClause;
  TLocateOptions LocateOption;

  j=0;
  for (int i=1;i<=strgrdWizard->RowCount;i++)
  {
	if (strFieldName.Trim()==strgrdWizard->Cells[0][i].Trim())
		j++;
  }
  if (j>1)
	return false;

  try
  {
	  //基本基本字段
	  strSQLClause = (AnsiString)"select * from "+
					 strLIBBASEFIELDS;

	  dmTemplate->qeGlobal->SQL->Clear();
      dmTemplate->qeGlobal->SQL->Add(strSQLClause);
      dmTemplate->qeGlobal->Open();

      //确定Locate操作的参数
      LocateOption << loCaseInsensitive;

      if (dmTemplate->qeGlobal->Locate("FieldName",SQLStr(strFieldName),LocateOption))
      //与基本字段名相同
      {
          return false;
      }
  }
  catch(...)
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::cboDirectoryDropDown(TObject *Sender)
{
/*
    RECT r1,r2;
    GetWindowRect(cboDirectory->Handle, &r1);
    GetWindowRect(Handle, &r2);
    frmDirectory->Left=r1.left;
    frmDirectory->Top=r1.bottom+1;
    frmDirectory->Width=200;
    frmDirectory->Height=r2.bottom-r1.bottom;
    frmDirectory->Show();
*/    
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::btnCancelClick(TObject *Sender)
{
    AnsiString strWarning;
    
    strWarning = (AnsiString)"Cancel creating current node ?";

	if (Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,MB_OKCANCEL+MB_ICONQUESTION)
                == IDOK)
	{
        Close();
    }
}
//---------------------------------------------------------------------------
int TfrmWizard::MatchField(AnsiString strFieldName)
{
     if (strFieldName == typeVARCHAR)
          return g_naFieldTypeSize[0];

     if (strFieldName == typeINT)
          return g_naFieldTypeSize[1];

     if (strFieldName == typeTEXT)
          return g_naFieldTypeSize[2];

     if (strFieldName == typeDATETIME)
          return g_naFieldTypeSize[3];

     if (strFieldName == typeIMAGE)
          return g_naFieldTypeSize[4];

     return 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::cboDirectoryChange(TObject *Sender)
{
	 if (cboDirectory->ItemIndex == 0)
     {
          chbInherit->Checked = false;
          chbInherit->Enabled = false;
     }
     else
          chbInherit->Enabled = true;
}
//---------------------------------------------------------------------------
bool TfrmWizard::CheckHtmlValid()
{
	ThtmlformTemp  * pHtmlFormTemp;
    TInputInfo * pInputInfo;
	TLocateOptions LocateOption;
    AnsiString strFieldName;
    AnsiString strSourceFieldName,strDestFieldName,strInvalidName,strWarning,strSQLClause;
	AnsiString strInputTemplate,strBrowseTemplate,strSearchTemplate,strAdvancedTemplate,strListTemplate;
	AnsiString strTemplate,strCategory;
    AnsiString strFlag;
    bool bIsValid,bHasExisted;
    int i,j,nTemplate;
    int nPos,nIndex;

    //获得五大模板
    strInputTemplate    = edtInput->Text.Trim();
    strBrowseTemplate   = edtBrowse->Text.Trim();
    strSearchTemplate   = edtSearch->Text.Trim();
    strAdvancedTemplate = edtAdvance->Text.Trim();
    strListTemplate     = edtList->Text.Trim();

	pHtmlFormTemp  = new ThtmlformTemp;
    pInputInfo = new TInputInfo;

    try
    {
        for (nTemplate=0;nTemplate<5;nTemplate++)
        {
            switch (nTemplate)
            {
                case 0:
                        strTemplate = strInputTemplate;
                        break;
                case 1:
                        strTemplate = strBrowseTemplate;
                        break;
                case 2:
                        strTemplate = strSearchTemplate;
                        break;
                case 3:
                        strTemplate = strAdvancedTemplate;
                        break;
                case 4:
                        strTemplate = strListTemplate;
            }

            //打开界面模板
            if (nTemplate == 0)
				pHtmlFormTemp->m_bInputTem = true;
            else
				pHtmlFormTemp->m_bInputTem = false;
			pHtmlFormTemp->ExtOpenTemplate(strTemplate);

            //获取控件名
			pHtmlFormTemp->GetControlName(pInputInfo);

			//清除界面模板
			pHtmlFormTemp->ClearTemplate();

			//检索基本字段
            strSQLClause = (AnsiString)"select FieldName from "+
                           strLIBBASEFIELDS;

            dmTemplate->qeGlobal->SQL->Clear();
            dmTemplate->qeGlobal->SQL->Add(strSQLClause);
            dmTemplate->qeGlobal->Open();

            //确定Locate操作的参数
            LocateOption << loCaseInsensitive;

            bIsValid = true;
            strInvalidName = "";
            for (i=0;i<pInputInfo->slInfoType->Count;i++)
            {
                strFieldName = pInputInfo->slInfoType->Strings[i];
                if (strFieldName.IsEmpty())
                    continue;
                nIndex = pInputInfo->slInfoType->IndexOf(strFieldName);
                if (nIndex != i)
                {
                    if (nTemplate == 0)
                        strWarning = " Input template is wrong.";

                    if (nTemplate == 1)
                        strWarning = " Browse template is wrong.";

                    if (nTemplate == 2)
                        strWarning = " Search template is wrong.";

                    if (nTemplate == 3)
                        strWarning = " Advanced search template is wrong.";

                    if (nTemplate == 4)
                        strWarning = " List template is wrong.";

					Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
					return false;
                }
                nPos = strFieldName.AnsiPos("_");
                if (nPos > 0)
                {
                   strSourceFieldName = strFieldName.SubString(0,nPos-1);
                   strFlag = strFieldName.SubString(nPos+1,strFieldName.Length()-nPos+1);
                }
                else
                   strSourceFieldName = strFieldName;
                nIndex = pInputInfo->slInfoType->IndexOf(strSourceFieldName + "_1");

                if ((strFlag != "1") && (strFlag != "2"))
                {
                  if (nIndex >= 0)
                  {
                      if (nTemplate == 0)
                          strWarning = " Input template is wrong.";

                      if (nTemplate == 1)
                          strWarning = " Browse template is wrong.";

                      if (nTemplate == 2)
                          strWarning = " Search template is wrong.";

                      if (nTemplate == 3)
                          strWarning = " Advanced search template is wrong.";

                      if (nTemplate == 4)
                          strWarning = " List template is wrong.";

					  Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
                      return false;
                  }

                  nIndex = pInputInfo->slInfoType->IndexOf(strSourceFieldName + "_2");
                  if (nIndex >= 0)
                  {
                      if (nTemplate == 0)
                          strWarning = " Input template is wrong.";

                      if (nTemplate == 1)
                          strWarning = " Browse template is wrong.";

                      if (nTemplate == 2)
                          strWarning = " Search template is wrong.";

                      if (nTemplate == 3)
                          strWarning = " Advanced search template is wrong.";

                      if (nTemplate == 4)
                          strWarning = " List template is wrong.";

					  Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
                      return false;
                  }
                }
                bHasExisted = false;

                if (nTemplate > 0)
                //除输入界面模板外，都须检查与基本字段的匹配情况
                {

                    if (dmTemplate->qeGlobal->Locate("FieldName",SQLStr(strSourceFieldName),LocateOption))
                    //与基本字段相匹配
                    {
                        bHasExisted = true;
                    }
                }
                else
                {
                    if (strSourceFieldName == FIELDMATERIALID)
                    //检查MaterialID字段的匹配情况
                    {
                        if (dmTemplate->qeGlobal->Locate("FieldName",SQLStr(strSourceFieldName),LocateOption))
                        //与基本字段相匹配
                        {
                            bHasExisted = true;
                        }
                    }

                    if (strSourceFieldName == FIELDSTORYID)
                    //检查MaterialID字段的匹配情况
                    {
                        if (dmTemplate->qeGlobal->Locate("FieldName",SQLStr(strSourceFieldName),LocateOption))
                        //与基本字段相匹配
						{
                             Application->MessageBox(L"Input Template can not include 'StoryID'",GLOBALMSGCAPTION,48);
                             return false;
                        }
                    }

                    if (strSourceFieldName == FIELDWRITETIME)
                    //检查MaterialID字段的匹配情况
                    {
                        if (dmTemplate->qeGlobal->Locate("FieldName",SQLStr(strSourceFieldName),LocateOption))
                        //与基本字段相匹配
                        {
							 Application->MessageBox(L"Input Template can not include 'WriteTime'",GLOBALMSGCAPTION,48);
                             return false;
                        }
                    }

                }

                if (! bHasExisted)
                {
                    //搜索已定义的扩展字段
                    for (j=1;j<strgrdWizard->RowCount;j++)
                    {
                         strDestFieldName = strgrdWizard->Cells[0][j].Trim();

                         if (strDestFieldName.IsEmpty())
                              continue;

                         if (strSourceFieldName == strDestFieldName)
                         //与已定义的扩展字段相匹配
                         {
                            bHasExisted = true;
                            break;
                         }
                    }
                }

                if (! bHasExisted)
                //找不到匹配的数据库字段
                {
                    bIsValid = false;
                    strInvalidName = strInvalidName+strSourceFieldName+"\n";
                }
            }

            if (! bIsValid)
            {
                //删除结尾的回车符
                strInvalidName.Delete(strInvalidName.Length(),1);

                switch (nTemplate)
                {
                    case 0:
                            strCategory = "Input Template";
                            edtInput->SetFocus();
                            break;
                    case 1:
                            strCategory = "Browse Template";
                            edtBrowse->SetFocus();
                            break;
                    case 2:
                            strCategory = "Search Template";
                            edtSearch->SetFocus();
                            break;
                    case 3:
                            strCategory = "Advanced Template";
                            edtAdvance->SetFocus();
                            break;
                    case 4:
                            strCategory = "List Template";
                            edtList->SetFocus();

                }

                strWarning = (AnsiString)"Below fields are not consistent with database fields\n"+
                             (AnsiString)"in current "+
                             strCategory+
                             " :\n\n"+
                             strInvalidName;

				Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);

				delete pHtmlFormTemp;
                delete pInputInfo;

                return false;
            }
		}

		delete pHtmlFormTemp;
        delete pInputInfo;

        return true;
    }
    catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		delete pHtmlFormTemp;
        delete pInputInfo;
		return false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::Button1Click(TObject *Sender)
{
  if (frmTemplate->OpenDialog1->Execute())
     edtList->Text = frmTemplate->OpenDialog1->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWizard::strgrdWizardClick(TObject *Sender)
{
 /*  if( strgrdWizard->Col == 1 )
	{
		TRect Recto     = strgrdWizard->CellRect(strgrdWizard->Col, strgrdWizard->Row);
		ComboBox1->Top  = strgrdWizard->Top;
		ComboBox1->Left = strgrdWizard->Left;
		ComboBox1->Top  = ComboBox1->Top + Recto.Top + strgrdWizard->GridLineWidth;
		ComboBox1->Left = ComboBox1->Left + Recto.Left + strgrdWizard->GridLineWidth + 1;
		ComboBox1->Height = (Recto.Bottom - Recto.Top) + 1;
		ComboBox1->Width  = Recto.Right - Recto.Left;
		ComboBox1->Visible = True;
	}
	else
		ComboBox1->Visible = False;
*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::strgrdWizardKeyPress(TObject *Sender, System::WideChar &Key)

{
/*  if (strgrdWizard->Col==2)
  {
    if ((!isdigit(Key))&&(Key!=VK_BACK))
      Key=NULL;
  }*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::strgrdWizardKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
  AnsiString strFieldName,strFieldType;
  strFieldName=strgrdWizard->Cells[0][strgrdWizard->RowCount-1];
  strFieldType=strgrdWizard->Cells[1][strgrdWizard->RowCount-1];
  if ((Key==VK_DOWN)&&(strgrdWizard->Row==strgrdWizard->RowCount-1)
	 &&(!strFieldName.Trim().IsEmpty())&&(!strFieldType.Trim().IsEmpty()))
  {
	strgrdWizard->RowCount++;
	strgrdWizard->Col = 0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::strgrdWizardCellValidate(TObject *Sender, int ACol, int ARow,
		  UnicodeString &Value, bool &Valid)
{
   int i,nFieldSize;

  if (ACol==2)
  {
	try
	{
	  i=Value.Trim().ToInt();
	  if (i>255)
	  {
		 Application->MessageBox(L"FieldSize must be between 1 and 255.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		 Value="255";
		 return;
	  }
	  if(i<=0)
	  {
		 Application->MessageBox(L"FieldSize must be between 1 and 255.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
		 Value="1";
		 return;
	  }
	}
	catch(...)
	{
	  Application->MessageBox(L"The value you input is not a valid integer,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  return;
	}
  }
  if (ACol==0)
  {
	if (!CheckFields(Value))
	{
	  Application->MessageBox(L"The FieldName is not unique,please input again.",L"TBS Library",MB_OK|MB_ICONINFORMATION);
	  Valid=false;
	  return;
	}
  }

  if (ACol == 1)
  //字段类型
  {
	nFieldSize = MatchField(Value);

	strgrdWizard->Cells[ACol+1][ARow] = String(nFieldSize);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmWizard::strgrdWizardGetEditMask(TObject *Sender, int ACol, int ARow,
          UnicodeString &Value)
{
  if (ACol==2)
	Value="000";
}
//---------------------------------------------------------------------------

