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

    //ָ������Ŀ��
    strPath = strPath+"\\*.dbf";

    nDone = findfirst(strPath.c_str(),&SearchInfo,0);
    while (! nDone)
    {
        strFileName = SearchInfo.ff_name;

        //ȥ��ĩβ��.dbf
//        strFileName.Delete(strFileName.Length()-3,4);

        //���Դ���ݱ���
        cbbSourceTable->Items->Add(strFileName);

        //��������
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
	TCHAR szFile[MAX_PATH];       //foldername������
	TCHAR szPath[MAX_PATH];       //filepath������
	LPMALLOC g_pMalloc;
	LPITEMIDLIST pidlBrowse;
    AnsiString strFolder;

    //��յ�ǰ·��
    strFolder = "";

    //��ȡIMALLOCָ��
	SHGetMalloc(&g_pMalloc);

//	LPITEMIDLIST  pidl;
	LPSHELLFOLDER pDesktopFolder;
	char          szRootPath[MAX_PATH];
	OLECHAR       olePath[MAX_PATH];
//	ULONG         chEaten;
//	ULONG         dwAttributes;
//	HRESULT       hr;

    //��ʼ����Ŀ¼
	BrowseInfo.pidlRoot = NULL;

	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		//IShellFolder::ParseDisplayName��ҪUnicode���͵��ļ���
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,szRootPath,-1,olePath,MAX_PATH);

		//��·��ת����ITEMIDLIST
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

	//��ʼ��BROWSEINFO
	BrowseInfo.hwndOwner	  = hParentWnd;
	BrowseInfo.pszDisplayName = szFile;
	BrowseInfo.lpszTitle	  = "Browse for folder";
	BrowseInfo.ulFlags		  = BIF_RETURNONLYFSDIRS;
	BrowseInfo.lpfn			  = NULL;
	BrowseInfo.lParam		  = 0;
	BrowseInfo.iImage		  = 1;

    //��Ŀ¼�Ի���
	pidlBrowse = SHBrowseForFolder(&BrowseInfo);

    //����ֵΪNULL����ʾû��ѡ��Ŀ¼
    if (pidlBrowse != NULL)
	{
        //�ѵ�ǰITEMת����ϵͳ·��
        if (SHGetPathFromIDList(pidlBrowse, szPath))
		{
			strFolder = szPath;
		}

        //�ͷ�SHBrowseForFolder.���ص�item identifierָ��
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

