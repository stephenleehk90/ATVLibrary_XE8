//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dir.h"
#include "shlobj.h"
#include "LogData.h"
#include "SourceTbFrm.h"
#include "LogFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSourceTbs *frmSourceTbs;
//---------------------------------------------------------------------------
__fastcall TfrmSourceTbs::TfrmSourceTbs(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSourceTbs::btnBrowseClick(TObject *Sender)
{
    AnsiString strPath,strFileName;
    struct ffblk SearchInfo;
    int nDone;

    strPath = browseForFolder(this->Handle);

    if (strPath.IsEmpty())
        return;

    edSourcePath->Text = strPath;

    cbbSourceTable->Enabled = true;
    cbbSourceTable->Items->Clear();

    //指定搜索目标
    strPath = strPath+"\\*.dbf";

    nDone = findfirst(strPath.c_str(),&SearchInfo,0);
    while (! nDone)
    {
        strFileName = SearchInfo.ff_name;

        //去掉末尾的.dbf
//        strFileName.Delete(strFileName.Length()-3,4);

        //添加源数据表名
        cbbSourceTable->Items->Add(strFileName);

        //继续搜索
        nDone = findnext(&SearchInfo);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSourceTbs::FormShow(TObject *Sender)
{
    edSourcePath->Clear();
    edSourcePath->ReadOnly   = true;
    cbbSourceTable->Items->Clear();
    cbbSourceTable->ItemIndex = -1;
    cbbSourceTable->Enabled = false;
}
//---------------------------------------------------------------------------
AnsiString TfrmSourceTbs::browseForFolder(HWND hParentWnd)
{
	BROWSEINFO BrowseInfo;
	TCHAR szFile[MAX_PATH];       //foldername缓冲区
	TCHAR szPath[MAX_PATH];       //filepath缓冲区
	LPMALLOC g_pMalloc;
	LPITEMIDLIST pidlBrowse;
    AnsiString strFolder;

    //清空当前路径
    strFolder = "";

    //获取IMALLOC指针
	SHGetMalloc(&g_pMalloc);

//	LPITEMIDLIST  pidl;
	LPSHELLFOLDER pDesktopFolder;
	char          szRootPath[MAX_PATH];
	OLECHAR       olePath[MAX_PATH];
//	ULONG         chEaten;
//	ULONG         dwAttributes;
//	HRESULT       hr;

    //初始化根目录
	BrowseInfo.pidlRoot = NULL;

	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		//IShellFolder::ParseDisplayName需要Unicode类型的文件名
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,szRootPath,-1,olePath,MAX_PATH);

		//把路径转换成ITEMIDLIST
/*		hr = pDesktopFolder->ParseDisplayName(NULL,NULL,olePath,
											  &chEaten,&pidl,&dwAttributes);

		if (! FAILED(hr))
		{
			BrowseInfo.pidlRoot = pidl;
		}
*/
	}
    else
        return strFolder;

	//初始化BROWSEINFO
	BrowseInfo.hwndOwner	  = hParentWnd;
	BrowseInfo.pszDisplayName = szFile;
	BrowseInfo.lpszTitle	  = "Browse for folder";
	BrowseInfo.ulFlags		  = BIF_RETURNONLYFSDIRS;
	BrowseInfo.lpfn			  = NULL;
	BrowseInfo.lParam		  = 0;
	BrowseInfo.iImage		  = 1;

    //打开目录对话框
	pidlBrowse = SHBrowseForFolder(&BrowseInfo);

    //返回值为NULL，表示没有选定目录
    if (pidlBrowse != NULL)
	{
        //把当前ITEM转换成系统路径
        if (SHGetPathFromIDList(pidlBrowse, szPath))
		{
			strFolder = szPath;
		}

        //释放SHBrowseForFolder.返回的item identifier指针
        g_pMalloc->Free(pidlBrowse);
	}

    return strFolder;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSourceTbs::btnOkClick(TObject *Sender)
{
    int i;
    
    if ((cbbSourceTable->Items->Count > 0)&&(cbbSourceTable->ItemIndex < 0))
    {
		Application->MessageBox(L"Please specify a source table!",GLOBALMSGCAPTION,48);
        ModalResult = mrNone;
    }

    for (i=0;i<frmLogs->lstvwFields->Items->Count;i++)
    {
        frmLogs->lstvwFields->Items->Item[i]->SubItems->Text = "";
        frmLogs->lstvwFields->Items->Item[i]->SubItems->Clear();
    }
}
//---------------------------------------------------------------------------

