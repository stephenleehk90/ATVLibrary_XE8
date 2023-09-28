//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <comctrls.hpp>
#include <mask.hpp>
#include <stdio.h>
#include "ImageEn.hpp"
#include <stdlib.h>
#include "htmlform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define MAXSIZE 1024
#define HORIZSPACE 10
#define VERTISPACE 10
#define MAXCOLORCOUNT 16
#define DEFAULTROWCOUNT 10
#define DEFAULTCOLCOUNT 10
#define DEFAULTSIZEPERCOL 5
#define DEFAULTSIZEPERROW 16

AnsiString strDefaultColor[MAXCOLORCOUNT] =
           {
                "Black","White","Red","Green",
                "Blue","Yellow","Gray","Maroon",
                "Olive","Navy","Purple","Lime",
                "Aqua","Teal","Fuchsia","Silver"     
           };

TColor     clDefaultColor[MAXCOLORCOUNT] =
           {
                clBlack,clWhite,clRed,clGreen,
                clBlue,clYellow,clGray,clMaroon,
                clOlive,clNavy,clPurple,clLime,
                clAqua,clTeal,clFuchsia,clSilver
           };
//---------------------------------------------------------------------------
THtmlForm::THtmlForm()
{
    m_pControlInfoHeader = NULL;
	m_pCurrentPointer    = NULL;
    m_pGlobalFont = new TFont;
    m_FontStyles.Clear();

    //显示控件的初始位置
    m_nInitX = 10;
    m_nInitY = 10;

    //当前控件所在的行和列
    m_nRow = 0;
    m_nColumn = 0;

    //当前控件的左上角
    m_nCurrentWidth = m_nInitX;
    m_nCurrentHeight = m_nInitY;

    //当前行上所有控件的最大高度
    m_nLineMaxHeight = 0;

    //所有行中的最大宽度
    m_nLineMaxWidth = 0;

    //当前行的对齐方式
    m_nAlignMode = alLEFT;

    //标识是否另起一行
    m_bNewLinePerControl = false;

    m_bDatetime = false;
    m_bEdit = false;
    m_bTimeLength = false;
    m_bTimeCode = false;


    //当前控件类型
    m_nType = ctrlLABEL;

    m_slTemplate = new TStringList;    
}
//---------------------------------------------------------------------------
THtmlForm::~THtmlForm(void)
{
	ControlInfo * pParentNode,* pNode;
    while (m_pControlInfoHeader)
	//如果控件信息链表不空，则释放所有节点
	{
		pParentNode = m_pControlInfoHeader;
        if (pParentNode->pNextControl)
    		pNode = pParentNode->pNextControl;
        else
        //有且仅有一个节点
        {
            delete m_pControlInfoHeader;
            m_pControlInfoHeader = NULL;

            return;
        }

		while (pNode->pNextControl)
        {
            pParentNode = pNode;
            pNode       = pNode->pNextControl;
        }

		delete pNode;
		pParentNode->pNextControl = NULL;
	}

    delete m_pGlobalFont;

    delete m_slTemplate;
}
//---------------------------------------------------------------------------
TColor THtmlForm::MapColor(AnsiString strColor)
{
    int i;

    for (i=0;i<MAXCOLORCOUNT;i++)
        if (strColor == strDefaultColor[i])
            return clDefaultColor[i];

    return clBlack;
}
//---------------------------------------------------------------------------
void THtmlForm::Analyse(AnsiString& strCaption)
{
	int nPos,nRows,nCols,nTextSize,nMaxLength;
    ControlInfo * pControlInfo;
    AnsiString strText,strButtonCaption,strColor,strInitialText,strControlName,strSubCaption;
    bool bIsSelectedItem = false;
    AnsiString strType,strRValue,strGValue,strBValue;
    int nRValue,nGValue,nBValue;
    TDateTimePicker* pDateTimePicker;
    TMaskEdit* pMaskEdit;
    TDateTime time;
    TEdit* pEdit;

    SIZE FontExtent;    

	//如为空串则返回
    strCaption = strCaption.Trim();

	if (strCaption.IsEmpty())
		return;

    strControlName = "";

	while (! strCaption.IsEmpty())
	{
		if (MatchString(strCaption,"<FONT"))
		//字体设置
        {
    		if (MatchString(strCaption,"COLOR"))
	    	//字体颜色
		    {
			    if (MatchString(strCaption,"="))
	    			if (MatchString(strCaption,"\""))
					{
						nPos = strCaption.Pos("\"");

						if (nPos > 0)
    					//提取颜色值代码
	    				{
		    				strColor = strCaption.SubString(1,nPos-1).Trim();

							if (MatchString(strColor,"#"))
    						//用数字表示
	    					{
                                strRValue = "0x"+strColor.SubString(1,2);
                                strGValue = "0x"+strColor.SubString(3,2);
                                strBValue = "0x"+strColor.SubString(5,2);
                                nRValue   = StrToInt(strRValue);
                                nGValue   = StrToInt(strGValue);
                                nBValue   = StrToInt(strBValue);

		    					m_pGlobalFont->Color = (TColor)RGB(nRValue,nGValue,nBValue);
			    			}
							else
							{
								m_pGlobalFont->Color = (TColor)MapColor(strColor);
    						}
	    				}
					}
		    }

			//滤掉颜色定义部分
			FilterString(strCaption);

            continue;
        }

/*      if (MatchString(strCaption,"<INPUT"))
        {
            nPos = strCaption.Pos("SRC");

            if (nPos > 0)
            {
                strType = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                if (MatchString(strType,"SRC"))
                  if (MatchString(strType,"="))
                    if (MatchString(strType,"\""))
                    {
                        nPos = strType.Pos("\"");
                        if (nPos > 0)
                            strControlName = strType.SubString(1,nPos-1);
                    }

                    //创建Image
                    if (m_bNewLinePerControl)
                    {
                        if (m_nColumn > 0)
                        {
                            m_nRow++;
                            m_nCurrentHeight += m_nLineMaxHeight;
                            m_nCurrentHeight += VERTISPACE;
                            if (m_nLineMaxWidth < m_nCurrentWidth)
                                m_nLineMaxWidth = m_nCurrentWidth;
                            m_nCurrentWidth = m_nInitX;
                            m_nLineMaxHeight = 0;
                            m_nColumn = 0;
                        }
                    }

                    TImage * pImage;
                    pImage = new TImage(m_pParentWindow);
                    pImage->Visible       = false;
                    pImage->Parent        = m_pParentWindow;
                    pImage->Top           = m_nCurrentHeight;
                    pImage->Left          = m_nCurrentWidth;

                    //创建控件信息节点
                    pControlInfo                 = new ControlInfo;
                    pControlInfo->pControl       = pImage;
                    pControlInfo->strClassName   = "TImage";
                    pControlInfo->strControlName = strControlName;
                    pControlInfo->ptLeftTop.x    = pImage->Left;
                    pControlInfo->ptLeftTop.y    = pImage->Top;
                    pControlInfo->pNextControl   = NULL;

                    //加入控件信息节点链表
                    if (! m_pControlInfoHeader)
                    //链表为空
                    {
                        m_pControlInfoHeader = pControlInfo;

                        m_pCurrentPointer    = pControlInfo;
                    }
                    else
                    {
                        m_pCurrentPointer->pNextControl = pControlInfo;

                        m_pCurrentPointer = m_pCurrentPointer->pNextControl;
                    }

                    m_nColumn++;

                    m_nCurrentWidth += pImage->Width;

                    m_nCurrentWidth += HORIZSPACE;

                    if (pImage->Height > m_nLineMaxHeight)
                        m_nLineMaxHeight = pImage->Height;
            }

            //过滤
            FilterString(strCaption);

            continue;
        }

*/		if (MatchString(strCaption,"<INPUT"))
		{
			if (MatchString(strCaption,"TYPE"))
				if (MatchString(strCaption,"="))
				{
					if (! MatchString(strCaption,"\""))
					{
                      if (!m_bInputTem)
                      {
						if (MatchString(strCaption,"reset"))
						//为一Reset Button
						{
//                            nButtonType = 0;

							if (MatchString(strCaption,"VALUE"))
							//指定了Button的Caption
							{
								if (! MatchString(strCaption,"="))
								//无法解释，使用默认值
									strButtonCaption = "&Reset";
								else
								{
									if (! MatchString(strCaption,"\""))
									//无法解释，使用默认值
										strButtonCaption = "&Reset";
									else
									{
										nPos = strCaption.Pos("\"");
										if (nPos > 0)
										//滤掉后引号
											strButtonCaption = strCaption.SubString(1,nPos-1).Trim();
									}
								}
							}
							else
								strButtonCaption = "&Reset";
						}
						else
						{
							if (MatchString(strCaption,"submit"))
							//为一Submit Button
							{
//                                nButtonType = 1;

								if (MatchString(strCaption,"VALUE"))
								//指定了Button的Caption
								{
									if (! MatchString(strCaption,"="))
									//无法解释，使用默认值
										strButtonCaption = "&Submit";
									else
									{
										if (! MatchString(strCaption,"\""))
										//无法解释，使用默认值
											strButtonCaption = "&Submit";
										else
										{
											nPos = strCaption.Pos("\"");
											if (nPos > 0)
											//滤掉后引号
												strButtonCaption = strCaption.SubString(1,nPos-1).Trim();
										}
									}
								}
								else
									strButtonCaption = "&Submit";
							}
							else
							{
								//过滤
								FilterString(strCaption);

								continue;
							}
						}


                        if (m_bNewLinePerControl)
                        {
                            if (m_nColumn > 0)
                            {
                                m_nRow++;
                                m_nCurrentHeight += m_nLineMaxHeight;
                                m_nCurrentHeight += VERTISPACE;
                                if (m_nLineMaxWidth < m_nCurrentWidth)
                                    m_nLineMaxWidth = m_nCurrentWidth;
                                m_nCurrentWidth = m_nInitX;
                                m_nLineMaxHeight = 0;
                                m_nColumn = 0;
                            }
                        }

						//创建Button
                        if (m_bNewLinePerControl)
                        {
                            if (m_nColumn > 0)
                            {
                                m_nRow++;
                                m_nCurrentHeight += m_nLineMaxHeight;
                                m_nCurrentHeight += VERTISPACE;
                                if (m_nLineMaxWidth < m_nCurrentWidth)
                                    m_nLineMaxWidth = m_nCurrentWidth;
                                m_nCurrentWidth = m_nInitX;
                                m_nLineMaxHeight = 0;
                                m_nColumn = 0;
                            }
                        }

						TButton * pButton;
						pButton = new TButton(m_pParentWindow);
                        pButton->Visible     = false;
						pButton->Parent		 = m_pParentWindow;
						pButton->Caption	 = strButtonCaption;
						m_pGlobalFont->Style = m_FontStyles;
						pButton->Font        = m_pGlobalFont;
						pButton->Top         = m_nCurrentHeight;
						pButton->Left        = m_nCurrentWidth;

/*                        if (nButtonType == 0)
                            pButton->OnClick = FOnReset;
                        else
                            pButton->OnClick = FOnSubmit;

*/						//创建控件信息节点
						pControlInfo               = new ControlInfo;
						pControlInfo->pControl     = pButton;
						pControlInfo->strClassName = "TButton";
						pControlInfo->ptLeftTop.x  = pButton->Left;
						pControlInfo->ptLeftTop.y  = pButton->Top;
						pControlInfo->pNextControl = NULL;

						//加入控件信息节点链表
						if (! m_pControlInfoHeader)
						//链表为空
						{
							m_pControlInfoHeader = pControlInfo;

							m_pCurrentPointer    = pControlInfo;
						}
						else
						{
							m_pCurrentPointer->pNextControl = pControlInfo;

							m_pCurrentPointer = m_pCurrentPointer->pNextControl;
						}

						m_nColumn++;

						m_nCurrentWidth += pButton->Width;

						m_nCurrentWidth += HORIZSPACE;

						if (pButton->Height > m_nLineMaxHeight)
							m_nLineMaxHeight = pButton->Height;

					  }	//过滤
					  FilterString(strCaption);

					  continue;
					}
					if (MatchString(strCaption,"text"))
					//为一Edit
					{

                        //提取控件名字
                        strControlName = PickupName(strCaption);
                        if (m_bTimeLength)
                        {
                          strInitialText = "";
                          nPos = strCaption.Pos("VALUE");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"VALUE");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          strInitialText = strText.SubString(1,nPos-1);
                                  }
                          }

                          nTextSize = 0;
                          nPos = strCaption.Pos("SIZE");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"SIZE");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          nTextSize = StrToInt(strText.SubString(1,nPos-1));
                                  }
                          }

                          nMaxLength = 0;
                          nPos = strCaption.Pos("MAXLENGTH");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"MAXLENGTH");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          nMaxLength = StrToInt(strText.SubString(1,nPos-1));
                                  }
                          }

                          //创建Edit
                          if (m_bNewLinePerControl)
                          {
                              if (m_nColumn > 0)
                              {
                                  m_nRow++;
                                  m_nCurrentHeight += m_nLineMaxHeight;
                                  m_nCurrentHeight += VERTISPACE;
                                  if (m_nLineMaxWidth < m_nCurrentWidth)
                                      m_nLineMaxWidth = m_nCurrentWidth;
                                  m_nCurrentWidth = m_nInitX;
                                  m_nLineMaxHeight = 0;
                                  m_nColumn = 0;
                              }
                          }

                          pMaskEdit = new TMaskEdit(m_pParentWindow);
                          pMaskEdit->Visible       = false;
                          pMaskEdit->Parent        = m_pParentWindow;
                          m_pGlobalFont->Style = m_FontStyles;
                          pMaskEdit->Font          = m_pGlobalFont;
                          pMaskEdit->Top           = m_nCurrentHeight;
                          pMaskEdit->Left          = m_nCurrentWidth;
                          pMaskEdit->MaxLength     = nMaxLength;
                          pMaskEdit->EditMask = (AnsiString)"99:99:99";                          
                          if (nTextSize > 0)
                          {
                              FontExtent   = getTextExtent();
                              pMaskEdit->ClientWidth = nTextSize*FontExtent.cx;
                          }

                        }
                        if (m_bTimeCode)
                        {
                          strInitialText = "";
                          nPos = strCaption.Pos("VALUE");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"VALUE");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          strInitialText = strText.SubString(1,nPos-1);
                                  }
                          }

                          nTextSize = 0;
                          nPos = strCaption.Pos("SIZE");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"SIZE");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          nTextSize = StrToInt(strText.SubString(1,nPos-1));
                                  }
                          }

                          nMaxLength = 0;
                          nPos = strCaption.Pos("MAXLENGTH");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"MAXLENGTH");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          nMaxLength = StrToInt(strText.SubString(1,nPos-1));
                                  }
                          }

                          //创建Edit
                          if (m_bNewLinePerControl)
                          {
                              if (m_nColumn > 0)
                              {
                                  m_nRow++;
                                  m_nCurrentHeight += m_nLineMaxHeight;
                                  m_nCurrentHeight += VERTISPACE;
                                  if (m_nLineMaxWidth < m_nCurrentWidth)
                                      m_nLineMaxWidth = m_nCurrentWidth;
                                  m_nCurrentWidth = m_nInitX;
                                  m_nLineMaxHeight = 0;
                                  m_nColumn = 0;
                              }
                          }

                          pMaskEdit = new TMaskEdit(m_pParentWindow);
                          pMaskEdit->Visible       = false;
                          pMaskEdit->Parent        = m_pParentWindow;
                          m_pGlobalFont->Style = m_FontStyles;
                          pMaskEdit->Font          = m_pGlobalFont;
                          pMaskEdit->Top           = m_nCurrentHeight;
                          pMaskEdit->Left          = m_nCurrentWidth;
                          pMaskEdit->MaxLength     = nMaxLength;
                          pMaskEdit->EditMask = (AnsiString)"99:99:99:99";
                          if (nTextSize > 0)
                          {
                              FontExtent   = getTextExtent();
                              pMaskEdit->ClientWidth = nTextSize*FontExtent.cx;
                          }

                        }
                        if (m_bDatetime)
                        {
                          pDateTimePicker= new TDateTimePicker(m_pParentWindow);

                          pDateTimePicker->DateFormat=dfLong;
                          pDateTimePicker->Date=time.CurrentDate();
                          pDateTimePicker->Visible       = false;
                          pDateTimePicker->Parent        = m_pParentWindow;
                          m_pGlobalFont->Style = m_FontStyles;
                          pDateTimePicker->Font          = m_pGlobalFont;
                          pDateTimePicker->Top           = m_nCurrentHeight;
                          pDateTimePicker->Left          = m_nCurrentWidth;
                          NMDATETIMECHANGE dtc;
                          SYSTEMTIME now;
                          GetSystemTime(&now);
                          dtc.nmhdr.hwndFrom = pDateTimePicker->Handle;
                          dtc.nmhdr.idFrom = NULL;
                          dtc.nmhdr.code = DTN_DATETIMECHANGE;
                          dtc.dwFlags = GDT_VALID;
                          dtc.st = now;

                          pDateTimePicker->Perform(WM_NOTIFY, NULL, (int)&dtc);

                          if (m_bNewLinePerControl)
                          {
                              if (m_nColumn > 0)
                              {
                                  m_nRow++;
                                  m_nCurrentHeight += m_nLineMaxHeight;
                                  m_nCurrentHeight += VERTISPACE;
                                  if (m_nLineMaxWidth < m_nCurrentWidth)
                                      m_nLineMaxWidth = m_nCurrentWidth;
                                  m_nCurrentWidth = m_nInitX;
                                  m_nLineMaxHeight = 0;
                                  m_nColumn = 0;
                              }
                          }
                        }
                        if (m_bEdit)
                        {
                          strInitialText = "";
                          nPos = strCaption.Pos("VALUE");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"VALUE");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          strInitialText = strText.SubString(1,nPos-1);
                                  }
                          }

                          nTextSize = 0;
                          nPos = strCaption.Pos("SIZE");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"SIZE");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          nTextSize = StrToInt(strText.SubString(1,nPos-1));
                                  }
                          }

                          nMaxLength = 0;
                          nPos = strCaption.Pos("MAXLENGTH");
                          if (nPos > 0)
                          {
                              strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                              MatchString(strText,"MAXLENGTH");
                              if (MatchString(strText,"="))
                                  if (MatchString(strText,"\""))
                                  {
                                      nPos = strText.Pos("\"");
                                      //提取初始值
                                      if (nPos > 0)
                                          nMaxLength = StrToInt(strText.SubString(1,nPos-1));
                                  }
                          }

                          //创建Edit
                          if (m_bNewLinePerControl)
                          {
                              if (m_nColumn > 0)
                              {
                                  m_nRow++;
                                  m_nCurrentHeight += m_nLineMaxHeight;
                                  m_nCurrentHeight += VERTISPACE;
                                  if (m_nLineMaxWidth < m_nCurrentWidth)
                                      m_nLineMaxWidth = m_nCurrentWidth;
                                  m_nCurrentWidth = m_nInitX;
                                  m_nLineMaxHeight = 0;
                                  m_nColumn = 0;
                              }
                          }

                          pEdit = new TEdit(m_pParentWindow);
                          pEdit->Visible       = false;
                          pEdit->Parent        = m_pParentWindow;
                          m_pGlobalFont->Style = m_FontStyles;
                          pEdit->Font          = m_pGlobalFont;
                          pEdit->Top           = m_nCurrentHeight;
                          pEdit->Left          = m_nCurrentWidth;
                          pEdit->MaxLength     = nMaxLength;
                          if (strControlName == "MaterialID")
                            pEdit->CharCase = ecUpperCase;
                          else
                            pEdit->CharCase = ecNormal;
                          if (nTextSize > 0)
                          {
                              FontExtent   = getTextExtent();
                              pEdit->ClientWidth = nTextSize*FontExtent.cx;
                          }
                        }

                        if (m_bDatetime)
						{
                        //创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->pControl       = pDateTimePicker;
						pControlInfo->strClassName   = "TDateTimePicker";
						pControlInfo->strControlName = strControlName;
						pControlInfo->ptLeftTop.x    = pDateTimePicker->Left;
						pControlInfo->ptLeftTop.y    = pDateTimePicker->Top;
						pControlInfo->pNextControl   = NULL;
                        }
                        if (m_bTimeLength)
						{
                        //创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->pControl       = pMaskEdit;
						pControlInfo->strClassName   = "TMaskEdit";
						pControlInfo->strControlName = strControlName;
						pControlInfo->ptLeftTop.x    = pMaskEdit->Left;
						pControlInfo->ptLeftTop.y    = pMaskEdit->Top;
						pControlInfo->pNextControl   = NULL;
                        }
                        if (m_bTimeCode)
						{
                        //创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->pControl       = pMaskEdit;
						pControlInfo->strClassName   = "TMaskEdit";
						pControlInfo->strControlName = strControlName;
						pControlInfo->ptLeftTop.x    = pMaskEdit->Left;
						pControlInfo->ptLeftTop.y    = pMaskEdit->Top;
						pControlInfo->pNextControl   = NULL;
                        }

                        if (m_bEdit)
                        {
                        //创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->pControl       = pEdit;
						pControlInfo->strClassName   = "TEdit";
						pControlInfo->strControlName = strControlName;
						pControlInfo->ptLeftTop.x    = pEdit->Left;
						pControlInfo->ptLeftTop.y    = pEdit->Top;
						pControlInfo->pNextControl   = NULL;
                        }
						//加入控件信息节点链表
						if (! m_pControlInfoHeader)
						//链表为空
						{
							m_pControlInfoHeader = pControlInfo;

							m_pCurrentPointer    = pControlInfo;
						}
						else
						{
							m_pCurrentPointer->pNextControl = pControlInfo;

							m_pCurrentPointer = m_pCurrentPointer->pNextControl;
						}

						m_nColumn++;

                        if (m_bDatetime)
                            m_nCurrentWidth += pDateTimePicker->Width;
                        if (m_bTimeLength)
                            m_nCurrentWidth += pMaskEdit->Width;
                        if (m_bTimeCode)
                            m_nCurrentWidth += pMaskEdit->Width;
                        if (m_bEdit)
     						m_nCurrentWidth += pEdit->Width;

						m_nCurrentWidth += HORIZSPACE;

                        if (m_bDatetime)
                        {
                          if (pDateTimePicker->Height > m_nLineMaxHeight)
                              m_nLineMaxHeight = pDateTimePicker->Height;
                        }
                        if (m_bTimeLength)
                        {
                          if (pMaskEdit->Height > m_nLineMaxHeight)
                              m_nLineMaxHeight = pMaskEdit->Height;
                        }
                        if (m_bTimeCode)
                        {
                          if (pMaskEdit->Height > m_nLineMaxHeight)
                              m_nLineMaxHeight = pMaskEdit->Height;
                        }
                        if (m_bEdit)
                        {
                          if (pEdit->Height > m_nLineMaxHeight)
                              m_nLineMaxHeight = pEdit->Height;
                        }
                        if (m_bTimeCode)
                        {
                          TButton * pButton;
                          m_bTimeCode = false;
                          pButton = new TButton(m_pParentWindow);
                          pButton->Visible     = false;
                          pButton->Parent		 = m_pParentWindow;
                          pButton->Caption	 = (AnsiString)"...";
//                          m_pGlobalFont->Style = m_FontStyles;
//                		  pButton->Font        = m_pGlobalFont;
                          pButton->Font->Size = 8;
                          pButton->Top         = m_nCurrentHeight;
                          pButton->Left        = m_nCurrentWidth;
                          pButton->Width       = 25;

                          pButton->OnClick = FOnSubmit;

                          //创建控件信息节点
                          pControlInfo               = new ControlInfo;
                          pControlInfo->pControl     = pButton;
                          pControlInfo->strClassName = "TButton";
            			  pControlInfo->strControlName = strControlName+" ";                          
                          pControlInfo->ptLeftTop.x  = pButton->Left;
                          pControlInfo->ptLeftTop.y  = pButton->Top;
                          pControlInfo->pNextControl = NULL;

                          //加入控件信息节点链表
                          if (! m_pControlInfoHeader)
                          //链表为空
                          {
                              m_pControlInfoHeader = pControlInfo;

                              m_pCurrentPointer    = pControlInfo;
                          }
                          else
                          {
                              m_pCurrentPointer->pNextControl = pControlInfo;

                              m_pCurrentPointer = m_pCurrentPointer->pNextControl;
                          }

                          m_nColumn++;

                          m_nCurrentWidth += pButton->Width;

                          m_nCurrentWidth += HORIZSPACE;

                          if (pButton->Height > m_nLineMaxHeight)
                              m_nLineMaxHeight = pButton->Height;
                        }
						//过滤
						FilterString(strCaption);
                        m_bDatetime=false;
                        m_bTimeLength=false;
                        m_bTimeCode=false;
                        m_bEdit=false;
						continue;
					}


					if (MatchString(strCaption,"radio"))
					//为一RadioButton
					{
						m_nType = ctrlRADIOBUTTON;

                        //提取控件名字
                        strControlName = PickupName(strCaption);
                        
						//过滤
                        //不设Caption的Radio将不予创建
						FilterString(strCaption);

						continue;
					}

					if (MatchString(strCaption,"checkbox"))
					//为一CheckBox
					{
						m_nType = ctrlCHECKBOX;

                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//过滤
                        //不设Caption的CheckBox将不予创建
						FilterString(strCaption);

                        continue;
					}
				}

			if (MatchString(strCaption,"SRC"))
            {
                nPos = strCaption.Pos("TYPE");

                if (nPos > 0)
                {
                    strType = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                    if (MatchString(strType,"TYPE"))
                      if (MatchString(strType,"="))
                        if (MatchString(strType,"\""))
                        {
                            nPos = strType.Pos("\"");
                            if (nPos > 0)
                                strType = strType.SubString(1,nPos-1);
                        }

                    if (strType == "image")
                    //表示Image字段
                    {
                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//创建Image
                        if (m_bNewLinePerControl)
                        {
                            if (m_nColumn > 0)
                            {
                                m_nRow++;
                                m_nCurrentHeight += m_nLineMaxHeight;
                                m_nCurrentHeight += VERTISPACE;
                                if (m_nLineMaxWidth < m_nCurrentWidth)
                                    m_nLineMaxWidth = m_nCurrentWidth;
                                m_nCurrentWidth = m_nInitX;
                                m_nLineMaxHeight = 0;
                                m_nColumn = 0;
                            }
                        }

                        TPanel * pPanel;
						pPanel = new TPanel(m_pParentWindow);
                        pPanel->Visible       = false;
						pPanel->Parent        = m_pParentWindow;
						pPanel->Top           = m_nCurrentHeight;
						pPanel->Left          = m_nCurrentWidth;
                        pPanel->Width         = IMAGEWIDTH;
                        pPanel->Height        = IMAGEHEIGHT;
                        pPanel->BevelInner    = bvNone;
                        pPanel->BevelOuter    = bvLowered;

						pControlInfo                 = new ControlInfo;
						pControlInfo->pControl       = pPanel;
						pControlInfo->strClassName   = "TPanel";
						pControlInfo->strControlName = "";
						pControlInfo->ptLeftTop.x    = pPanel->Left;
						pControlInfo->ptLeftTop.y    = pPanel->Top;
						pControlInfo->pNextControl   = NULL;

						//加入控件信息节点链表
						if (! m_pControlInfoHeader)
						//链表为空
						{
							m_pControlInfoHeader = pControlInfo;

							m_pCurrentPointer    = pControlInfo;
						}
						else
						{
							m_pCurrentPointer->pNextControl = pControlInfo;

							m_pCurrentPointer = m_pCurrentPointer->pNextControl;
						}


						TImageEn * pImage;
						pImage = new TImageEn(m_pParentWindow);
//                        pImage->Visible       = false;
						pImage->Parent        = pPanel;
                        pImage->Align         = alClient;
                        pImage->Background    = clBtnFace;
                        pImage->NColors       = nc24bit;
                        pImage->AutoUndo      = false;
                        pImage->Cursor        = crArrow;
                        pImage->DragCursor    = crArrow;
/*						pImage->Top           = m_nCurrentHeight;
						pImage->Left          = m_nCurrentWidth;
                        pImage->Width         = 100;
                        pImage->Height        = 100;
                        pImage->Stretch       = true;
*/                        pImage->OnDblClick     = FOnReset;

						//创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->pControl       = pImage;
						pControlInfo->strClassName   = "TImageEn";
						pControlInfo->strControlName = strControlName;
//						pControlInfo->ptLeftTop.x    = pImage->Left;
//						pControlInfo->ptLeftTop.y    = pImage->Top;
						pControlInfo->pNextControl   = NULL;

						//加入控件信息节点链表
						if (! m_pControlInfoHeader)
						//链表为空
						{
							m_pControlInfoHeader = pControlInfo;

							m_pCurrentPointer    = pControlInfo;
						}
						else
						{
							m_pCurrentPointer->pNextControl = pControlInfo;

							m_pCurrentPointer = m_pCurrentPointer->pNextControl;
						}

						m_nColumn++;

						m_nCurrentWidth += pPanel->Width;

						m_nCurrentWidth += HORIZSPACE;

//						if (pImage->Height > m_nLineMaxHeight)
//							m_nLineMaxHeight = pImage->Height;
                    }
                }
            }

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<TEXTAREA"))
		//为一Memo
		{
            nRows = 0;
            nCols = 0;

            //提取控件名字
            strControlName = PickupName(strCaption);

            nPos = strCaption.Pos("ROWS");
            if (nPos > 0)
            //提取行数            
            {
                strSubCaption = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                if (MatchString(strSubCaption,"ROWS"))
                    if (MatchString(strSubCaption,"="))
                        if (MatchString(strSubCaption,"\""))
                        {
                        	nPos = strSubCaption.Pos("\"");
                    	    if (nPos > 0)
                            {
                                strText    = strSubCaption.SubString(1,nPos-1);
                                nRows = StrToInt(strText);
                            }
                        }
            }

            nPos = strCaption.Pos("COLS");
            if (nPos > 0)
            //提取列数
            {
                strSubCaption = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                if (MatchString(strSubCaption,"COLS"))
                    if (MatchString(strSubCaption,"="))
                        if (MatchString(strSubCaption,"\""))
                        {
                        	nPos = strSubCaption.Pos("\"");
                    	    if (nPos > 0)
                            {
                                strText = strSubCaption.SubString(1,nPos-1);
                                nCols   = StrToInt(strText);
                            }
                        }
            }

            //设置为缺省值
            if (nRows == 0)
                nRows = DEFAULTROWCOUNT;

            if (nCols == 0)
                nCols = DEFAULTCOLCOUNT;

			//创建Memo
            if (m_bNewLinePerControl)
            {
                if (m_nColumn > 0)
                {
                    m_nRow++;
                    m_nCurrentHeight += m_nLineMaxHeight;
                    m_nCurrentHeight += VERTISPACE;
                    if (m_nLineMaxWidth < m_nCurrentWidth)
                        m_nLineMaxWidth = m_nCurrentWidth;
                    m_nCurrentWidth = m_nInitX;
                    m_nLineMaxHeight = 0;
                    m_nColumn = 0;
                }
            }

			TMemo * pMemo;
			pMemo = new TMemo(m_pParentWindow);
            FontExtent = getTextExtent();
            pMemo->Visible       = false;
			pMemo->Parent        = m_pParentWindow;
			pMemo->Top           = m_nCurrentHeight;
			pMemo->Left          = m_nCurrentWidth;
			pMemo->Width         = nCols*FontExtent.cx;
			pMemo->Height        = (nRows+1)*FontExtent.cy;
            pMemo->WantReturns   = true;            
            m_pGlobalFont->Style = m_FontStyles;
			pMemo->Font          = m_pGlobalFont;
			pMemo->ScrollBars    = ssVertical;

			//创建控件信息节点
			pControlInfo                 = new ControlInfo;
			pControlInfo->pControl       = pMemo;
			pControlInfo->strClassName   = "TMemo";
            pControlInfo->strControlName = strControlName;
			pControlInfo->ptLeftTop.x    = pMemo->Left;
			pControlInfo->ptLeftTop.y    = pMemo->Top;
			pControlInfo->pNextControl   = NULL;

			//加入控件信息节点链表
			if (! m_pControlInfoHeader)
			//链表为空
			{
				m_pControlInfoHeader = pControlInfo;

				m_pCurrentPointer    = pControlInfo;
			}
			else
			{
				m_pCurrentPointer->pNextControl = pControlInfo;

				m_pCurrentPointer = m_pCurrentPointer->pNextControl;
			}

			m_nColumn++;

			m_nCurrentWidth += pMemo->Width;

			m_nCurrentWidth += HORIZSPACE;

			if (pMemo->Height > m_nLineMaxHeight)
				m_nLineMaxHeight = pMemo->Height;

			FilterString(strCaption);

            m_nType = ctrlMEMO;

			continue;
		}

		if (MatchString(strCaption,"<SELECT"))
		//为一ComboBox
		{
            //提取控件名字
            strControlName = PickupName(strCaption);

			//创建ComboBox
            if (m_bNewLinePerControl)
            {
                if (m_nColumn > 0)
                {
                    m_nRow++;
                    m_nCurrentHeight += m_nLineMaxHeight;
                    m_nCurrentHeight += VERTISPACE;
                    if (m_nLineMaxWidth < m_nCurrentWidth)
                        m_nLineMaxWidth = m_nCurrentWidth;
                    m_nCurrentWidth = m_nInitX;
                    m_nLineMaxHeight = 0;
                    m_nColumn = 0;
                }
            }
            
			TComboBox * pComboBox;
			pComboBox = new TComboBox(m_pParentWindow);
            pComboBox->Visible    = false;
			pComboBox->Parent     = m_pParentWindow;
			pComboBox->Style      = Stdctrls::csDropDownList;
            m_pGlobalFont->Style  = m_FontStyles;
			pComboBox->Font       = m_pGlobalFont;
			pComboBox->Top        = m_nCurrentHeight;
			pComboBox->Left       = m_nCurrentWidth;

			//创建控件信息节点
			pControlInfo                 = new ControlInfo;
			pControlInfo->pControl       = pComboBox;
			pControlInfo->strClassName   = "TComboBox";
            pControlInfo->strControlName = strControlName;
			pControlInfo->ptLeftTop.x    = pComboBox->Left;
			pControlInfo->ptLeftTop.y    = pComboBox->Top;
			pControlInfo->pNextControl   = NULL;

			//加入控件信息节点链表
			if (! m_pControlInfoHeader)
			//链表为空
			{
				m_pControlInfoHeader = pControlInfo;

				m_pCurrentPointer    = pControlInfo;
			}
			else
			{
				m_pCurrentPointer->pNextControl = pControlInfo;

				m_pCurrentPointer = m_pCurrentPointer->pNextControl;
			}

			m_nColumn++;

			m_nCurrentWidth += pComboBox->Width;

			m_nCurrentWidth += HORIZSPACE;

			if (pComboBox->Height > m_nLineMaxHeight)
				m_nLineMaxHeight = pComboBox->Height;

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<OPTION"))
		//为一ComboBox的选项
		{
			m_nType = ctrlCOMBOBOXITEM;

			if (MatchString(strCaption,"SELECTED"))
			//表明此项为缺省选项
				bIsSelectedItem = true;
			else
				bIsSelectedItem = false;

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<BIG>"))
		//字体放大
		{
			m_pGlobalFont->Size++;

			continue;
		}

		if (MatchString(strCaption,"</BIG>"))
		//字体放大
		{
			m_pGlobalFont->Size--;

			continue;
		}

		if (MatchString(strCaption,"</SMALL>"))
		//字体缩小
		{
			m_pGlobalFont->Size++;

			continue;
		}

		if (MatchString(strCaption,"<B>"))
		//粗体
		{
            m_FontStyles << fsBold;

			continue;
		}

		if (MatchString(strCaption,"</B>"))
		//去掉粗体
		{
            m_FontStyles >> fsBold;

			continue;
		}

		if (MatchString(strCaption,"<I>"))
		//斜体
		{
            m_FontStyles << fsItalic;

			continue;
		}

		if (MatchString(strCaption,"</I>"))
		//去掉斜体
		{
            m_FontStyles >> fsItalic;

			continue;
		}

		if (MatchString(strCaption,"<U>"))
		//加下划线
		{
            m_FontStyles << fsUnderline;

			continue;
		}

		if (MatchString(strCaption,"</U>"))
		//去掉下划线
		{
            m_FontStyles >> fsUnderline;

			continue;
		}

		if (MatchString(strCaption,"<STRIKE>"))
		//加删除线
		{
            m_FontStyles << fsStrikeOut;

			continue;
		}

		if (MatchString(strCaption,"</STRIKE>"))
		//去掉删除线
		{
            m_FontStyles >> fsStrikeOut;

			continue;
		}

		if (MatchString(strCaption,"</FONT>"))
		//恢复字体的缺省颜色
        {
            m_pGlobalFont->Color = clWindowText;
        }

		if (MatchString(strCaption,"&nbsp;"))
        {
            FontExtent   = getTextExtent();

            m_nCurrentWidth += FontExtent.cx;

            continue;
        }

		if ((MatchString(strCaption,"<H"))||(MatchString(strCaption,"</H")))
        //自动换行
        {
            m_nRow++;
            m_nCurrentHeight += m_nLineMaxHeight;
            m_nCurrentHeight += VERTISPACE;
            if (m_nLineMaxWidth < m_nCurrentWidth)
                m_nLineMaxWidth = m_nCurrentWidth;
            m_nCurrentWidth = m_nInitX;
            m_nLineMaxHeight = 0;
            m_nColumn = 0;

			return;
        }

		if (MatchString(strCaption,"<LI"))
        //自动换行
        {
            m_nRow++;
            m_nCurrentHeight += m_nLineMaxHeight;
            m_nCurrentHeight += VERTISPACE;
            if (m_nLineMaxWidth < m_nCurrentWidth)
                m_nLineMaxWidth = m_nCurrentWidth;
            m_nCurrentWidth = m_nInitX;
            m_nLineMaxHeight = 0;
            m_nColumn = 0;

			return;
        }

		if (MatchString(strCaption,"<OL"))
        {
            //自动换行
            m_nRow++;
            m_nCurrentHeight += m_nLineMaxHeight;
            m_nCurrentHeight += VERTISPACE;
            if (m_nLineMaxWidth < m_nCurrentWidth)
                m_nLineMaxWidth = m_nCurrentWidth;
            m_nCurrentWidth = m_nInitX;
            m_nLineMaxHeight = 0;
            m_nColumn = 0;

			return;
        }

		if (MatchString(strCaption,"<PRE>"))
        //每个控件自动另起一行
        {
            m_bNewLinePerControl = true;

			continue;
        }

		if (MatchString(strCaption,"</PRE>"))
        //取消自动换行
        {
            m_bNewLinePerControl = false;

			continue;
        }

		if (MatchString(strCaption,"<P"))
        //新的一行
		{
            if (m_pControlInfoHeader)
			//控件队列不空，则自动换行
            {
                m_nRow++;
                m_nCurrentHeight += m_nLineMaxHeight;
                m_nCurrentHeight += VERTISPACE;
                if (m_nLineMaxWidth < m_nCurrentWidth)
                    m_nLineMaxWidth = m_nCurrentWidth;
                m_nCurrentWidth = m_nInitX;
                m_nLineMaxHeight = 0;
                m_nColumn = 0;
            }

    		if (MatchString(strCaption,">"))
                continue;

        	if (MatchString(strCaption,"ALIGN"))
			//设置对齐方式
			{
				if (MatchString(strCaption,"="))
				{
					if (MatchString(strCaption,"Left"))
					//左对齐
						m_nAlignMode = alLEFT;

					if (MatchString(strCaption,"Center"))
					//中心对齐
						m_nAlignMode = alCENTER;

					if (MatchString(strCaption,"Right"))
					//右对齐
						m_nAlignMode = alRIGHT;

					m_nAlignMode = alLEFT;
				}
			}

			//滤掉对齐格式定义部分
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"</PRE>"))
        //每个控件自动另起一行
        {
            m_bNewLinePerControl = false;

			continue;
        }

		//忽略下述描述符
		if (MatchString(strCaption,"<TT>"))
			continue;

		if (MatchString(strCaption,"</TT>"))
			continue;

		if (MatchString(strCaption,"</TEXTAREA>"))
        {
            m_nType = ctrlLABEL;

            continue;
        }

		if (MatchString(strCaption,"</SELECT>"))
        {
            if (m_nType == ctrlCOMBOBOXITEM)
                m_nType = ctrlLABEL;
                 
            continue;
        }

		//忽略其他控制符
		if (MatchString(strCaption,"<"))
		{
			FilterString(strCaption);

			continue;
		}

		//否则即为文本，提取并作相应处理
        switch (m_nType)
        {
            case ctrlRADIOBUTTON :
                                   //提取Caption
                                    strText = PickupCaption(strCaption);

                                    //创建RadioButton
                                    if (m_bNewLinePerControl)
                                    {
                                        if (m_nColumn > 0)
                                        {
                                            m_nRow++;
                                            m_nCurrentHeight += m_nLineMaxHeight;
                                            m_nCurrentHeight += VERTISPACE;
                                            if (m_nLineMaxWidth < m_nCurrentWidth)
                                                m_nLineMaxWidth = m_nCurrentWidth;
                                            m_nCurrentWidth = m_nInitX;
                                            m_nLineMaxHeight = 0;
                                            m_nColumn = 0;
                                        }
                                    }

                                    TRadioButton * pRadioButton;
                                    pRadioButton = new TRadioButton(m_pParentWindow);
                                    pRadioButton->Visible  = false;
                                    pRadioButton->Caption  = strText;
                                    pRadioButton->Parent   = m_pParentWindow;
                                    m_pGlobalFont->Style   = m_FontStyles;
                                    pRadioButton->Font     = m_pGlobalFont;
                                    pRadioButton->Top      = m_nCurrentHeight;
                                    pRadioButton->Left     = m_nCurrentWidth;

                                    //创建控件信息节点
                                    pControlInfo                 = new ControlInfo;
                                    pControlInfo->pControl       = pRadioButton;
                                    pControlInfo->strClassName   = "TRadioButton";
            						pControlInfo->strControlName = strControlName;
                                    pControlInfo->ptLeftTop.x    = pRadioButton->Left;
                                    pControlInfo->ptLeftTop.y    = pRadioButton->Top;
                                    pControlInfo->pNextControl   = NULL;

									//加入控件信息节点链表
									if (! m_pControlInfoHeader)
									//链表为空
									{
										m_pControlInfoHeader = pControlInfo;

										m_pCurrentPointer    = pControlInfo;
									}
									else
									{
										m_pCurrentPointer->pNextControl = pControlInfo;

										m_pCurrentPointer = m_pCurrentPointer->pNextControl;
									}

									break;
			case ctrlLABEL :
								//提取文本内容
                                strText = PickupCaption(strCaption);

								//创建文本
                                if (m_bNewLinePerControl)
                                {
                                    if (m_nColumn > 0)
                                    {
                                        m_nRow++;
                                        m_nCurrentHeight += m_nLineMaxHeight;
                                        m_nCurrentHeight += VERTISPACE;
                                        if (m_nLineMaxWidth < m_nCurrentWidth)
                                            m_nLineMaxWidth = m_nCurrentWidth;
                                        m_nCurrentWidth = m_nInitX;
                                        m_nLineMaxHeight = 0;
                                        m_nColumn = 0;
                                    }
                                }

								TLabel * pLabel;
								pLabel = new TLabel(m_pParentWindow);
                                pLabel->Visible      = false;
								pLabel->Parent       = m_pParentWindow;
								pLabel->Top          = m_nCurrentHeight;
								pLabel->Left         = m_nCurrentWidth;
                                m_pGlobalFont->Style = m_FontStyles;
								pLabel->Font         = m_pGlobalFont;
								pLabel->Caption      = strText;
                                pLabel->Width        = strText.Length()*getTextExtent().cx;
								pLabel->AutoSize     = false;

								//创建控件信息节点
								pControlInfo               = new ControlInfo;
								pControlInfo->pControl     = pLabel;
								pControlInfo->strClassName = "TLabel";
								pControlInfo->ptLeftTop.x  = pLabel->Left;
								pControlInfo->ptLeftTop.y  = pLabel->Top;
								pControlInfo->pNextControl = NULL;

								//加入控件信息节点链表
								if (! m_pControlInfoHeader)
								//链表为空
								{
									m_pControlInfoHeader = pControlInfo;

									m_pCurrentPointer    = pControlInfo;
								}
								else
								{
									m_pCurrentPointer->pNextControl = pControlInfo;

									m_pCurrentPointer = m_pCurrentPointer->pNextControl;
								}

								break;
			case ctrlCHECKBOX :
								//提取Caption
                                strText = PickupCaption(strCaption);

								//创建CheckBox
                                if (m_bNewLinePerControl)
                                {
                                    if (m_nColumn > 0)
                                    {
                                        m_nRow++;
                                        m_nCurrentHeight += m_nLineMaxHeight;
                                        m_nCurrentHeight += VERTISPACE;
                                        if (m_nLineMaxWidth < m_nCurrentWidth)
                                            m_nLineMaxWidth = m_nCurrentWidth;
                                        m_nCurrentWidth = m_nInitX;
                                        m_nLineMaxHeight = 0;
                                        m_nColumn = 0;
                                    }
                                }

								TCheckBox * pCheckBox;
								pCheckBox = new TCheckBox(m_pParentWindow);
                                pCheckBox->Visible   = false;
								pCheckBox->Caption   = strText;
								pCheckBox->Parent    = m_pParentWindow;
                                m_pGlobalFont->Style = m_FontStyles;
								pCheckBox->Font		 = m_pGlobalFont;
								pCheckBox->Top       = m_nCurrentHeight;
								pCheckBox->Left      = m_nCurrentWidth;

								//创建控件信息节点
								pControlInfo                 = new ControlInfo;
								pControlInfo->pControl       = pCheckBox;
								pControlInfo->strClassName   = "TCheckBox";
        						pControlInfo->strControlName = strControlName;
								pControlInfo->ptLeftTop.x    = pCheckBox->Left;
								pControlInfo->ptLeftTop.y    = pCheckBox->Top;
								pControlInfo->pNextControl   = NULL;

								//加入控件信息节点链表
								if (! m_pControlInfoHeader)
								//链表为空
								{
									m_pControlInfoHeader = pControlInfo;

									m_pCurrentPointer    = pControlInfo;
								}
								else
								{
									m_pCurrentPointer->pNextControl = pControlInfo;

									m_pCurrentPointer = m_pCurrentPointer->pNextControl;
								}

								break;
			case ctrlCOMBOBOXITEM :
			                    //提取文本内容
                                strText = PickupCaption(strCaption);

								if (! m_pCurrentPointer)
								//上一个控件不是ComboBox，忽略之
									continue;

								if (m_pCurrentPointer->strClassName == "TComboBox")
								{
									((TComboBox *)m_pCurrentPointer->pControl)->Items->Add(strText);

									if (bIsSelectedItem)
									{
										((TComboBox *)m_pCurrentPointer->pControl)->ItemIndex =
												((TComboBox *)m_pCurrentPointer->pControl)->Items->Count-1;
									}
								}

								break;
            case ctrlMEMO:
			                    //提取文本内容
                                strText = PickupCaption(strCaption);

								if (! m_pCurrentPointer)
								//上一个控件不是Memo，忽略之
									continue;

								if (m_pCurrentPointer->strClassName == "TMemo")
								{
									((TMemo *)m_pCurrentPointer->pControl)->Lines->Add(strText);
								}

								break;
        }

		if ((m_nType != ctrlCOMBOBOXITEM)&&(m_nType != ctrlMEMO))
		{
			m_nColumn++;

			m_nCurrentWidth += pControlInfo->pControl->Width;

			m_nCurrentWidth += HORIZSPACE;

			if (pControlInfo->pControl->Height > m_nLineMaxHeight)
				m_nLineMaxHeight = pControlInfo->pControl->Height;

            //恢复缺省的文本类型
            m_nType = ctrlLABEL;
		}

        continue;
	}
}
//---------------------------------------------------------------------------
void THtmlForm::AnalyseName(AnsiString& strCaption)
{
    ControlInfo * pControlInfo;
    AnsiString strText,strButtonCaption,strColor,strInitialText,strControlName,strSubCaption;
    AnsiString strType;
//    bool bIsSelectedItem = false;
    int nPos;

	//如为空串则返回
    strCaption = strCaption.Trim();

	if (strCaption.IsEmpty())
		return;

    strControlName = "";

	while (! strCaption.IsEmpty())
	{
		if (MatchString(strCaption,"<FONT"))
		//字体设置
        {
			//滤掉颜色定义部分
			FilterString(strCaption);

            continue;
        }

		if (MatchString(strCaption,"<INPUT"))
		{
			if (MatchString(strCaption,"TYPE"))
				if (MatchString(strCaption,"="))
				{
					if (! MatchString(strCaption,"\""))
					{
						//过滤
						FilterString(strCaption);

						continue;
					}

					if (MatchString(strCaption,"text"))
					//为一Edit
					{
                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->strControlName = strControlName;
                        pControlInfo->pNextControl   = NULL;

						//加入控件信息节点链表
						if (! m_pControlInfoHeader)
						//链表为空
						{
							m_pControlInfoHeader = pControlInfo;

							m_pCurrentPointer    = pControlInfo;
						}
						else
						{
							m_pCurrentPointer->pNextControl = pControlInfo;

							m_pCurrentPointer = m_pCurrentPointer->pNextControl;
						}

						//过滤
						FilterString(strCaption);

						continue;
					}

					if (MatchString(strCaption,"radio"))
					//为一RadioButton
					{
						m_nType = ctrlRADIOBUTTON;

                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//过滤
                        //不设Caption的Radio将不予创建
						FilterString(strCaption);

						continue;
					}

					if (MatchString(strCaption,"checkbox"))
					//为一CheckBox
					{
						m_nType = ctrlCHECKBOX;

                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//过滤
                        //不设Caption的CheckBox将不予创建
						FilterString(strCaption);

                        continue;
					}
				}

			if (MatchString(strCaption,"SRC"))
            {
                nPos = strCaption.Pos("TYPE");

                if (nPos > 0)
                {
                    strType = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                    if (MatchString(strType,"TYPE"))
                      if (MatchString(strType,"="))
                        if (MatchString(strType,"\""))
                        {
                            nPos = strType.Pos("\"");
                            if (nPos > 0)
                                strType = strType.SubString(1,nPos-1);
                        }

                    if (strType == "image")
                    //表示Image字段
                    {
                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->strControlName = strControlName;
                        pControlInfo->pNextControl   = NULL;
                    }
                }
            }

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<TEXTAREA"))
		//为一Memo
		{
            //提取控件名字
            strControlName = PickupName(strCaption);

			//创建控件信息节点
			pControlInfo                 = new ControlInfo;
            pControlInfo->strControlName = strControlName;
			pControlInfo->pNextControl   = NULL;

			//加入控件信息节点链表
			if (! m_pControlInfoHeader)
			//链表为空
			{
				m_pControlInfoHeader = pControlInfo;

				m_pCurrentPointer    = pControlInfo;
			}
			else
			{
				m_pCurrentPointer->pNextControl = pControlInfo;

				m_pCurrentPointer = m_pCurrentPointer->pNextControl;
			}

            m_nType = ctrlMEMO;

			continue;
		}

		if (MatchString(strCaption,"<SELECT"))
		//为一ComboBox
		{
            //提取控件名字
            strControlName = PickupName(strCaption);

			//创建控件信息节点
			pControlInfo                 = new ControlInfo;
            pControlInfo->strControlName = strControlName;
			pControlInfo->pNextControl   = NULL;

			//加入控件信息节点链表
			if (! m_pControlInfoHeader)
			//链表为空
			{
				m_pControlInfoHeader = pControlInfo;

				m_pCurrentPointer    = pControlInfo;
			}
			else
			{
				m_pCurrentPointer->pNextControl = pControlInfo;

				m_pCurrentPointer = m_pCurrentPointer->pNextControl;
			}

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<OPTION"))
		//为一ComboBox的选项
		{
			m_nType = ctrlCOMBOBOXITEM;

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<BIG>"))
		//字体放大
		{
			continue;
		}

		if (MatchString(strCaption,"<SMALL>"))
		//字体缩小
		{
			continue;
		}

		if (MatchString(strCaption,"<B>"))
		//粗体
		{
			continue;
		}

		if (MatchString(strCaption,"<I>"))
		//斜体
		{
			continue;
		}

		if (MatchString(strCaption,"<U>"))
		//加下划线
		{
			continue;
		}

		if (MatchString(strCaption,"<STRIKE>"))
		//加删除线
		{
			continue;
		}

		if (MatchString(strCaption,"&nbsp;"))
        {
            continue;
        }

		if ((MatchString(strCaption,"<H"))||(MatchString(strCaption,"</H")))
        //自动换行
        {
			return;
        }

		if (MatchString(strCaption,"<LI"))
        //自动换行
        {
			return;
        }

		if (MatchString(strCaption,"<OL"))
        {
			return;
        }

		if (MatchString(strCaption,"<PRE>"))
        //每个控件自动另起一行
        {
			continue;
        }

		if (MatchString(strCaption,"<P"))
        //新的一行
		{
			//滤掉对齐格式定义部分
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"</PRE>"))
        //每个控件自动另起一行
        {
			continue;
        }

		//忽略下述描述符
		if (MatchString(strCaption,"<TT>"))
			continue;

		if (MatchString(strCaption,"</TT>"))
			continue;
		if (MatchString(strCaption,"</BIG>"))
			continue;
		if (MatchString(strCaption,"</SMALL>"))
			continue;
		if (MatchString(strCaption,"</B>"))
			continue;
		if (MatchString(strCaption,"</I>"))
			continue;
		if (MatchString(strCaption,"</U>"))
			continue;
		if (MatchString(strCaption,"</STRIKE>"))
			continue;
		if (MatchString(strCaption,"</FONT>"))
			continue;
		if (MatchString(strCaption,"</TEXTAREA>"))
        {
            m_nType = ctrlLABEL;

            continue;
        }
		if (MatchString(strCaption,"</SELECT>"))
        {
            if (m_nType == ctrlCOMBOBOXITEM)
                m_nType = ctrlLABEL;

            continue;
        }

		//忽略其他控制符
		if (MatchString(strCaption,"<"))
		{
			FilterString(strCaption);

			continue;
		}

		//否则即为文本，提取并作相应处理
        switch (m_nType)
        {
            case ctrlRADIOBUTTON :
                                    //提取文本内容
                                    strText = PickupCaption(strCaption);

                                    //创建控件信息节点
                                    pControlInfo                 = new ControlInfo;
            						pControlInfo->strControlName = strControlName;
                                    pControlInfo->pNextControl   = NULL;

									//加入控件信息节点链表
									if (! m_pControlInfoHeader)
									//链表为空
									{
										m_pControlInfoHeader = pControlInfo;

										m_pCurrentPointer    = pControlInfo;
									}
									else
									{
										m_pCurrentPointer->pNextControl = pControlInfo;

										m_pCurrentPointer = m_pCurrentPointer->pNextControl;
									}

									break;
			case ctrlLABEL :
								//提取文本内容
                                strText = PickupCaption(strCaption);

								break;
			case ctrlCHECKBOX :
								//提取Caption
                                strText = PickupCaption(strCaption);

								//创建控件信息节点
								pControlInfo                 = new ControlInfo;
        						pControlInfo->strControlName = strControlName;
								pControlInfo->pNextControl   = NULL;

								//加入控件信息节点链表
								if (! m_pControlInfoHeader)
								//链表为空
								{
									m_pControlInfoHeader = pControlInfo;

									m_pCurrentPointer    = pControlInfo;
								}
								else
								{
									m_pCurrentPointer->pNextControl = pControlInfo;

									m_pCurrentPointer = m_pCurrentPointer->pNextControl;
								}

								break;
			case ctrlCOMBOBOXITEM :
			                    //提取文本内容
                                strText = PickupCaption(strCaption);

								break;
            case ctrlMEMO:
			                    //提取文本内容
                                strText = PickupCaption(strCaption);

								break;
        }

		if ((m_nType != ctrlCOMBOBOXITEM)&&(m_nType != ctrlMEMO))
		{
            //恢复缺省的文本类型
            m_nType = ctrlLABEL;
		}

        continue;
	}
}
//---------------------------------------------------------------------------
void THtmlForm::ExtAnalyseName(AnsiString& strCaption)
{
    ControlInfo * pControlInfo;
    AnsiString strText,strButtonCaption,strColor,strInitialText,strControlName,strSubCaption;
    AnsiString strType;
//    bool bIsSelectedItem = false;
    int nPos;

	//如为空串则返回
    strCaption = strCaption.Trim();

	if (strCaption.IsEmpty())
		return;

    strControlName = "";

	while (! strCaption.IsEmpty())
	{
		if (MatchString(strCaption,"<FONT"))
		//字体设置
        {
			//滤掉颜色定义部分
			FilterString(strCaption);

            continue;
        }

        if (MatchString(strCaption,"<IMG"))
        {
            if (MatchString(strCaption,"SRC"))
              if (MatchString(strCaption,"="))
                if (MatchString(strCaption,"\""))
                {
                    nPos = strCaption.Pos("\"");
                    //提取初始值
                    if (nPos > 0)
                        strControlName = strCaption.SubString(1,nPos-1);

                    //创建控件信息节点
                    pControlInfo                 = new ControlInfo;
                    pControlInfo->strControlName = strControlName;
                    pControlInfo->pNextControl   = NULL;

                    //加入控件信息节点链表
                    if (! m_pControlInfoHeader)
                    //链表为空
                    {
                        m_pControlInfoHeader = pControlInfo;

                        m_pCurrentPointer    = pControlInfo;
                    }
                    else
                    {
                        m_pCurrentPointer->pNextControl = pControlInfo;

                        m_pCurrentPointer = m_pCurrentPointer->pNextControl;
                    }
                }

            //过滤
            FilterString(strCaption);

            continue;
        }

		if (MatchString(strCaption,"<INPUT"))
		{
			if (MatchString(strCaption,"TYPE"))
				if (MatchString(strCaption,"="))
				{
					if (! MatchString(strCaption,"\""))
					{
						//过滤
						FilterString(strCaption);

						continue;
					}

					if (MatchString(strCaption,"text"))
					//为一Edit
					{
                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->strControlName = strControlName;
                        pControlInfo->pNextControl   = NULL;

						//加入控件信息节点链表
						if (! m_pControlInfoHeader)
						//链表为空
						{
							m_pControlInfoHeader = pControlInfo;

							m_pCurrentPointer    = pControlInfo;
						}
						else
						{
							m_pCurrentPointer->pNextControl = pControlInfo;

							m_pCurrentPointer = m_pCurrentPointer->pNextControl;
						}

						//过滤
						FilterString(strCaption);

						continue;
					}

					if (MatchString(strCaption,"radio"))
					//为一RadioButton
					{
						m_nType = ctrlRADIOBUTTON;

                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//过滤
                        //不设Caption的Radio将不予创建
						FilterString(strCaption);

						continue;
					}

					if (MatchString(strCaption,"checkbox"))
					//为一CheckBox
					{
						m_nType = ctrlCHECKBOX;

                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//过滤
                        //不设Caption的CheckBox将不予创建
						FilterString(strCaption);

                        continue;
					}
				}

			if (MatchString(strCaption,"SRC"))
            {
                nPos = strCaption.Pos("TYPE");

                if (nPos > 0)
                {
                    strType = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
                    if (MatchString(strType,"TYPE"))
                      if (MatchString(strType,"="))
                        if (MatchString(strType,"\""))
                        {
                            nPos = strType.Pos("\"");
                            if (nPos > 0)
                                strType = strType.SubString(1,nPos-1);
                        }

                    if (strType == "image")
                    //表示Image字段
                    {
                        //提取控件名字
                        strControlName = PickupName(strCaption);

						//创建控件信息节点
						pControlInfo                 = new ControlInfo;
						pControlInfo->strControlName = strControlName;
                        pControlInfo->pNextControl   = NULL;
                    }
                }
            }

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<TEXTAREA"))
		//为一Memo
		{
            //提取控件名字
            strControlName = PickupName(strCaption);

			//创建控件信息节点
			pControlInfo                 = new ControlInfo;
            pControlInfo->strControlName = strControlName;
			pControlInfo->pNextControl   = NULL;

			//加入控件信息节点链表
			if (! m_pControlInfoHeader)
			//链表为空
			{
				m_pControlInfoHeader = pControlInfo;

				m_pCurrentPointer    = pControlInfo;
			}
			else
			{
				m_pCurrentPointer->pNextControl = pControlInfo;

				m_pCurrentPointer = m_pCurrentPointer->pNextControl;
			}

            m_nType = ctrlMEMO;

			continue;
		}

		if (MatchString(strCaption,"<SELECT"))
		//为一ComboBox
		{
            //提取控件名字
            strControlName = PickupName(strCaption);

			//创建控件信息节点
			pControlInfo                 = new ControlInfo;
            pControlInfo->strControlName = strControlName;
			pControlInfo->pNextControl   = NULL;

			//加入控件信息节点链表
			if (! m_pControlInfoHeader)
			//链表为空
			{
				m_pControlInfoHeader = pControlInfo;

				m_pCurrentPointer    = pControlInfo;
			}
			else
			{
				m_pCurrentPointer->pNextControl = pControlInfo;

				m_pCurrentPointer = m_pCurrentPointer->pNextControl;
			}

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<OPTION"))
		//为一ComboBox的选项
		{
			m_nType = ctrlCOMBOBOXITEM;

			//过滤
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"<BIG>"))
		//字体放大
		{
			continue;
		}

		if (MatchString(strCaption,"<SMALL>"))
		//字体缩小
		{
			continue;
		}

		if (MatchString(strCaption,"<B>"))
		//粗体
		{
			continue;
		}

		if (MatchString(strCaption,"<I>"))
		//斜体
		{
			continue;
		}

		if (MatchString(strCaption,"<U>"))
		//加下划线
		{
			continue;
		}

		if (MatchString(strCaption,"<STRIKE>"))
		//加删除线
		{
			continue;
		}

		if (MatchString(strCaption,"&nbsp;"))
        {
            continue;
        }

		if ((MatchString(strCaption,"<H"))||(MatchString(strCaption,"</H")))
        //自动换行
        {
			return;
        }

		if (MatchString(strCaption,"<LI"))
        //自动换行
        {
			return;
        }

		if (MatchString(strCaption,"<OL"))
        {
			return;
        }

		if (MatchString(strCaption,"<PRE>"))
        //每个控件自动另起一行
        {
			continue;
        }

		if (MatchString(strCaption,"<P"))
        //新的一行
		{
			//滤掉对齐格式定义部分
			FilterString(strCaption);

			continue;
		}

		if (MatchString(strCaption,"</PRE>"))
        //每个控件自动另起一行
        {
			continue;
        }

		//忽略下述描述符
		if (MatchString(strCaption,"<TT>"))
			continue;

		if (MatchString(strCaption,"</TT>"))
			continue;
		if (MatchString(strCaption,"</BIG>"))
			continue;
		if (MatchString(strCaption,"</SMALL>"))
			continue;
		if (MatchString(strCaption,"</B>"))
			continue;
		if (MatchString(strCaption,"</I>"))
			continue;
		if (MatchString(strCaption,"</U>"))
			continue;
		if (MatchString(strCaption,"</STRIKE>"))
			continue;
		if (MatchString(strCaption,"</FONT>"))
			continue;
		if (MatchString(strCaption,"</TEXTAREA>"))
        {
            m_nType = ctrlLABEL;

            continue;
        }
		if (MatchString(strCaption,"</SELECT>"))
        {
            if (m_nType == ctrlCOMBOBOXITEM)
                m_nType = ctrlLABEL;

            continue;
        }

		//忽略其他控制符
		if (MatchString(strCaption,"<"))
		{
			FilterString(strCaption);

			continue;
		}

		//否则即为文本，提取并作相应处理
        switch (m_nType)
        {
            case ctrlRADIOBUTTON :
                                    //提取文本内容
                                    strText = PickupCaption(strCaption);

                                    //创建控件信息节点
                                    pControlInfo                 = new ControlInfo;
            						pControlInfo->strControlName = strControlName;
                                    pControlInfo->pNextControl   = NULL;

									//加入控件信息节点链表
									if (! m_pControlInfoHeader)
									//链表为空
									{
										m_pControlInfoHeader = pControlInfo;

										m_pCurrentPointer    = pControlInfo;
									}
									else
									{
										m_pCurrentPointer->pNextControl = pControlInfo;

										m_pCurrentPointer = m_pCurrentPointer->pNextControl;
									}

									break;
			case ctrlLABEL :
								//提取文本内容
                                strText = PickupCaption(strCaption);

								break;
			case ctrlCHECKBOX :
								//提取Caption
                                strText = PickupCaption(strCaption);

								//创建控件信息节点
								pControlInfo                 = new ControlInfo;
        						pControlInfo->strControlName = strControlName;
								pControlInfo->pNextControl   = NULL;

								//加入控件信息节点链表
								if (! m_pControlInfoHeader)
								//链表为空
								{
									m_pControlInfoHeader = pControlInfo;

									m_pCurrentPointer    = pControlInfo;
								}
								else
								{
									m_pCurrentPointer->pNextControl = pControlInfo;

									m_pCurrentPointer = m_pCurrentPointer->pNextControl;
								}

								break;
			case ctrlCOMBOBOXITEM :
			                    //提取文本内容
                                strText = PickupCaption(strCaption);

								break;
            case ctrlMEMO:
			                    //提取文本内容
                                strText = PickupCaption(strCaption);

								break;
        }

		if ((m_nType != ctrlCOMBOBOXITEM)&&(m_nType != ctrlMEMO))
		{
            //恢复缺省的文本类型
            m_nType = ctrlLABEL;
		}

        continue;
	}
}
//---------------------------------------------------------------------------
void THtmlForm::InitGlobalFont(void)
{
    m_FontStyles.Clear();
    m_pGlobalFont->Charset = ANSI_CHARSET;
    m_pGlobalFont->Color   = clWindowText;
    m_pGlobalFont->Height  = -16;
    m_pGlobalFont->Name    = "Courier";
    m_pGlobalFont->Size    = 11;
    m_pGlobalFont->Pitch   = fpDefault;
    m_pGlobalFont->Style   = m_FontStyles;
}
//---------------------------------------------------------------------------
void THtmlForm::FilterString(AnsiString& strCaption)
//过滤
{
	int nPos;

	nPos = strCaption.Pos(">");
	if (nPos > 0)
    {
		strCaption = strCaption.Delete(1,nPos).Trim();
    }
}
//---------------------------------------------------------------------------
bool THtmlForm::OpenTemplate(TScrollingWinControl * AParent,AnsiString Template)
{
    FILE * pStream;
    char szLine[MAXSIZE];
    AnsiString strLine,strCaption,strColor;
    AnsiString strRValue,strGValue,strBValue;
    int nRValue,nGValue,nBValue;
    int nState = stHTMLHEAD,nPos;

    if (AParent == NULL)
    {
//    	Application->MessageBox("No Parent Window Specified!",GLOBALMSGCAPTION,MB_OK);
        return false;
    }

    AParent->VertScrollBar->Position = 0;
    ((TScrollBox* )AParent)->AutoScroll = true;
    
    m_pParentWindow = AParent;
    if ((pStream = fopen(Template.c_str(),"rt")) == NULL)
    {
//    	Application->MessageBox("File Open Error!",GLOBALMSGCAPTION,MB_OK);
        return false;
    }

    m_slTemplate->Clear();

    while (fgets(szLine,MAXSIZE,pStream))
    {
        strLine = (AnsiString)szLine;

        //删除结尾的回车符
        strLine.Delete(strLine.Length(),1);

        m_slTemplate->Add(strLine);

        strCaption = strLine;

        switch (nState)
        {
            case stHTMLHEAD :
                                if (MatchString(strCaption,"</HEAD>"))
                                {
                                  nState = stHTMLBEGIN;
                                  continue;
                                }

                                break;
            case stHTMLBEGIN :
                                if (MatchString(strCaption,"<BODY"))
                                {
                                    if (MatchString(strCaption,"BGCOLOR"))
                                        if (MatchString(strCaption,"="))
                                            if (MatchString(strCaption,"\""))
                                                if (MatchString(strCaption,"#"))
                                                //用数字表示颜色值
                                                {
                                                    nPos = strCaption.Pos("\"");
                                                    if (nPos > 0)
                                                    {
                                                        strColor = strCaption.SubString(1,nPos-1);
                                                        strRValue = "0x"+strColor.SubString(1,2);
                                                        strGValue = "0x"+strColor.SubString(3,2);
                                                        strBValue = "0x"+strColor.SubString(5,2);
                                                        nRValue   = StrToInt(strRValue);
                                                        nGValue   = StrToInt(strGValue);
                                                        nBValue   = StrToInt(strBValue);
                                                        ((TScrollBox *)m_pParentWindow)->Color = (TColor)RGB(nRValue,nGValue,nBValue);
                                                    }
                                                }
                                    continue;
                                }

                                if (MatchString(strCaption,"<FORM "))
                                {

                                    nState = stFORMBEGIN;

                                    continue;
                                }

                                break;
            case stFORMBEGIN :
                                if (MatchString(strCaption,"</FORM"))
                                {
                                    nState = stFORMEND;

                                    continue;
                                }

                                break;
            case stFORMEND :
                                if (MatchString(strCaption,"</HTML"))
                                {
                                    nState = stHTMLEND;

                                    continue;
                                }

                                break;
        }

        if (nState == stHTMLHEAD)
            continue;

        Analyse(strCaption);

		if (nState == stHTMLEND)
        //HTML文件结束
			break;
    }

    fclose(pStream);

    ShowControls();
    AParent->VertScrollBar->Range = AParent->VertScrollBar->Range+10;
    return true;
}
//---------------------------------------------------------------------------
/*
bool THtmlForm::OpenTemplate(AnsiString Template)
{
    FILE * pStream;
    char szLine[MAXSIZE];
    AnsiString strLine,strCaption,strColor;
    int nState = stHTMLHEAD,nPos;

    m_pParentWindow = NULL;

    if ((pStream = fopen(Template.c_str(),"rt")) == NULL)
    {
        return false;
    }

    m_slTemplate->Clear();

    while (fgets(szLine,MAXSIZE,pStream))
    {
        strLine = (AnsiString)szLine;

        //删除结尾的回车符
        strLine.Delete(strLine.Length(),1);

        m_slTemplate->Add(strLine);

        strCaption = strLine;

        switch (nState)
        {
			case stHTMLHEAD :
								if (MatchString(strCaption,"<FORM "))
								{
									nState = stFORMBEGIN;

                                    continue;
								}

								break;
			case stFORMBEGIN :
								if (MatchString(strCaption,"</FORM"))
								{
									nState = stFORMEND;
								}

								break;
		}

		if (nState == stFORMBEGIN)
		{
            AnalyseName(strCaption);
		}

		if (nState == stFORMEND)
        //Form描述段结束
			break;
    }

    fclose(pStream);

    return true;
}
*/
//---------------------------------------------------------------------------
bool THtmlForm::ExtOpenTemplate(AnsiString Template)
{
    FILE * pStream;
    char szLine[MAXSIZE];
    AnsiString strLine,strCaption;
//    int nState = stHTMLHEAD;

    if ((pStream = fopen(Template.c_str(),"rt")) == NULL)
        return false;

    m_slTemplate->Clear();

    while (fgets(szLine,MAXSIZE,pStream))
    {
        strLine = (AnsiString)szLine;

        //删除结尾的回车符
        strLine.Delete(strLine.Length(),1);

        m_slTemplate->Add(strLine);

        strCaption = strLine;

        if (MatchString(strCaption,"<HTML>"))
        {
//            nState = stHTMLBEGIN;

            continue;
        }

        if (MatchString(strCaption,"</HTML>"))
        {
//            nState = stHTMLEND;
            break;
        }

        ExtAnalyseName(strCaption);
    }

    fclose(pStream);

    return true;
}
//---------------------------------------------------------------------------
bool THtmlForm::OpenTemplate(TScrollingWinControl * AParent,
                             AnsiString strDBName,AnsiString strTableName,
                             AnsiString strNodeField,AnsiString strTemplateField,
                             AnsiString strNodeID)
{
    AnsiString strBuffer,strLine,strCaption,strColor;
    AnsiString strRValue,strGValue,strBValue;
    int nRValue,nGValue,nBValue;
    int nState = stHTMLHEAD,nPos;
    TLocateOptions LocateOption;
    TBlobStream * bsTemplateStream = NULL;
    char * pBuffer = NULL;

    if (AParent == NULL)
    {
        return false;
    }

    AParent->VertScrollBar->Position = 0;
    ((TScrollBox* )AParent)->AutoScroll = true;
    m_pParentWindow = AParent;

    tbTemplate = NULL;

    try
    {
      tbTemplate = new TTable(m_pParentWindow);
      tbTemplate->DatabaseName = strDBName;
      tbTemplate->TableName    = strTableName;
      tbTemplate->Open();

      //确定Locate操作的参数
      LocateOption << loCaseInsensitive;

      if (! tbTemplate->Locate(strNodeField,strNodeID,LocateOption))
      //没有找到指定记录
      {
          delete tbTemplate;
          tbTemplate = NULL;
          return false;
      }

      bsTemplateStream = new TBlobStream((TBlobField*)tbTemplate->FieldByName(strTemplateField),bmRead);
      pBuffer = new char[bsTemplateStream->Size+1];
      bsTemplateStream->Read(pBuffer,bsTemplateStream->Size);
      pBuffer[bsTemplateStream->Size] = '\0';
      strBuffer = (AnsiString)pBuffer;
      delete pBuffer;
      pBuffer = NULL;
      delete bsTemplateStream;
      bsTemplateStream = NULL;

      m_slTemplate->Clear();

      while (! strBuffer.IsEmpty())
      {
          //提取一行
          nPos = strBuffer.Pos("\r\n");
          if (nPos > 0)
          {
            strLine = strBuffer.SubString(1,nPos-1);
            strBuffer.Delete(1,nPos+1);
          }
          else
          {
            strLine = strBuffer;
            strBuffer.Delete(1,strBuffer.Length());
          }

          m_slTemplate->Add(strLine);

          strCaption = strLine;

          switch (nState)
          {
              case stHTMLHEAD :
                                  if (MatchString(strCaption,"</HEAD>"))
                                  {
                                    nState = stHTMLBEGIN;
                                    continue;
                                  }

                                  break;
              case stHTMLBEGIN :
                                  if (MatchString(strCaption,"<BODY"))
                                  {
                                      if (MatchString(strCaption,"BGCOLOR"))
                                          if (MatchString(strCaption,"="))
                                              if (MatchString(strCaption,"\""))
                                                  if (MatchString(strCaption,"#"))
                                                  //用数字表示颜色值
                                                  {
                                                      nPos = strCaption.Pos("\"");
                                                      if (nPos > 0)
                                                      {
                                                          strColor = strCaption.SubString(1,nPos-1);
                                                          strRValue = "0x"+strColor.SubString(1,2);
                                                          strGValue = "0x"+strColor.SubString(3,2);
                                                          strBValue = "0x"+strColor.SubString(5,2);
                                                          nRValue   = StrToInt(strRValue);
                                                          nGValue   = StrToInt(strGValue);
                                                          nBValue   = StrToInt(strBValue);
                                                          ((TScrollBox *)m_pParentWindow)->Color = (TColor)RGB(nRValue,nGValue,nBValue);
                                                      }
                                                  }
                                      continue;
                                  }

                                  if (MatchString(strCaption,"<FORM "))
                                  {

                                      nState = stFORMBEGIN;

                                      continue;
                                  }

                                  break;
              case stFORMBEGIN :
                                  if (MatchString(strCaption,"</FORM"))
                                  {
                                      nState = stFORMEND;

                                      continue;
                                  }

                                  break;
              case stFORMEND :
                                  if (MatchString(strCaption,"</HTML"))
                                  {
                                      nState = stHTMLEND;

                                      continue;
                                  }

                                  break;
          }

          if (nState == stHTMLHEAD)
              continue;

          Analyse(strCaption);

          if (nState == stHTMLEND)
          //HTML文件结束
              break;
      }
      ShowControls();
      AParent->VertScrollBar->Range = AParent->VertScrollBar->Range+10;
      return true;
    }
    catch(...)
    {
        if (tbTemplate)
            delete tbTemplate;

        if (pBuffer)
            delete pBuffer;

        if (bsTemplateStream)
            delete bsTemplateStream;

        return false;
    }
}
//---------------------------------------------------------------------------
bool THtmlForm::SaveTemplate(AnsiString TemplatePath)
{
    try
    {
        m_slTemplate->SaveToFile(TemplatePath);
    }
    catch(...)
    {
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void THtmlForm::ShowControls()
{
	ControlInfo * pNode;

    if (m_pControlInfoHeader)
	//如果控件信息链表不空，则逐个显示所有控件
	{
		pNode = m_pControlInfoHeader;

        if (pNode->pControl != NULL)
            pNode->pControl->Visible = true;

		while (pNode->pNextControl)
        {
            pNode = pNode->pNextControl;

            if (pNode->pControl != NULL)
                pNode->pControl->Visible = true;
        }
	}
}
//---------------------------------------------------------------------------
void THtmlForm::CloseTemplate()
{
	ControlInfo * pParentNode,* pNode;

    while (m_pControlInfoHeader)
	//如果控件信息链表不空，则释放所有节点
	{
		pParentNode = m_pControlInfoHeader;
        if (pParentNode->pNextControl)
    		pNode = pParentNode->pNextControl;
        else
        //有且仅有一个节点
        {
            m_pControlInfoHeader->pControl->Parent = NULL;
            delete m_pControlInfoHeader;
            m_pControlInfoHeader = NULL;

            break;
        }

		while (pNode->pNextControl)
        {
            pParentNode = pNode;
            pNode       = pNode->pNextControl;
        }

        pNode->pControl->Parent = NULL;
		delete pNode;
		pParentNode->pNextControl = NULL;
	}

    m_pControlInfoHeader = NULL;
	m_pCurrentPointer    = NULL;

    //初始化字体
    InitGlobalFont();

    //初始化字体风格
    m_FontStyles.Clear();

    //初始化当前控件所在的行和列
    m_nRow = 0;
    m_nColumn = 0;

    //初始化当前控件的左上角
    m_nCurrentWidth  = m_nInitX;
    m_nCurrentHeight = m_nInitY;

    //初始化当前行上所有控件的最大高度
    m_nLineMaxHeight = 0;

    //初始化所有行中的最大宽度
    m_nLineMaxWidth = 0;

    //初始化当前行的对齐方式
    m_nAlignMode = alLEFT;

    //初始化标识是否另起一行
    m_bNewLinePerControl = false;

    //初始化当前控件类型
    m_nType = ctrlLABEL;

    m_slTemplate->Clear();

    if (m_pParentWindow)
        ((TScrollBox *)m_pParentWindow)->Color = clBtnFace;
}
//---------------------------------------------------------------------------
void THtmlForm::ClearTemplate()
{
	ControlInfo * pParentNode,* pNode;

    while (m_pControlInfoHeader)
	//如果控件信息链表不空，则释放所有节点
	{
		pParentNode = m_pControlInfoHeader;
        if (pParentNode->pNextControl)
    		pNode = pParentNode->pNextControl;
        else
        //有且仅有一个节点
        {
            delete m_pControlInfoHeader;
            m_pControlInfoHeader = NULL;

            break;
        }

		while (pNode->pNextControl)
        {
            pParentNode = pNode;
            pNode       = pNode->pNextControl;
        }

		delete pNode;
		pParentNode->pNextControl = NULL;
	}

    m_pControlInfoHeader = NULL;
	m_pCurrentPointer    = NULL;

    //初始化字体
    InitGlobalFont();

    //初始化字体风格
    m_FontStyles.Clear();

    //初始化当前控件所在的行和列
    m_nRow = 0;
    m_nColumn = 0;

    //初始化当前控件的左上角
    m_nCurrentWidth  = m_nInitX;
    m_nCurrentHeight = m_nInitY;

    //初始化当前行上所有控件的最大高度
    m_nLineMaxHeight = 0;

    //初始化所有行中的最大宽度
    m_nLineMaxWidth = 0;

    //初始化当前行的对齐方式
    m_nAlignMode = alLEFT;

    //初始化标识是否另起一行
    m_bNewLinePerControl = false;

    //初始化当前控件类型
    m_nType = ctrlLABEL;

    m_slTemplate->Clear();
}
//---------------------------------------------------------------------------
bool THtmlForm::MatchString(AnsiString& strTarget,AnsiString strMatch)
//确定字符串strTarget首部是否为字符串strMatch
//是，则从strTarget中删除strMatch，返回TRUE
//否，返回FALSE
{
    AnsiString strTitle;
    int nMatchCount = strMatch.Length();

    int nCount = min(strTarget.Length(),nMatchCount);

    strTitle = strTarget.SubString(0,nCount);

    if (strTitle.UpperCase() == strMatch.UpperCase())
    //不区分大小写
    {
        strTarget.Delete(1,nCount);
        strTarget = strTarget.Trim();
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
AnsiString THtmlForm::PickupCaption(AnsiString& strCaption)
//提取Caption
{
    int nPos1,nPos2,nPos;
    AnsiString strText;

    //定位控制符
    nPos1 = strCaption.Pos("<");
    nPos2 = strCaption.Pos("&");

    if (nPos1 > 0)
    {
        if (nPos2 > 0)
            nPos = min(nPos1,nPos2);
        else
            nPos = nPos1;
    }
    else
    {
        if (nPos2 > 0)
            nPos = nPos2;
        else
            nPos = 0;
    }

    if (nPos > 0)
        strText = strCaption.SubString(1,nPos-1);
    else
        strText = strCaption;

    MatchString(strCaption,strText);

    return strText;
}
//---------------------------------------------------------------------------
SIZE THtmlForm::getTextExtent()
{
    SIZE FontExtent;
    TLabel * pLabel;
    TCanvas * pTempCanvas;
    AnsiString strTest("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    pLabel = new TLabel(m_pParentWindow);
    pTempCanvas = pLabel->Canvas;
    pLabel->Visible = false;
    pLabel->Parent = m_pParentWindow;
    pTempCanvas->Font = m_pGlobalFont;

    FontExtent = pTempCanvas->TextExtent(strTest);

    FontExtent.cx = FontExtent.cx/strTest.Length()+1;

    pLabel->Parent = NULL;

    delete pLabel;

    return FontExtent;
}
//---------------------------------------------------------------------------
AnsiString THtmlForm::PickupName(AnsiString strCaption)
{
    int nPos;
    AnsiString strText,strName;

    strName = "";
    nPos = strCaption.Pos("NAME");
    if (nPos > 0)
    {
        strText = strCaption.SubString(nPos,strCaption.Length()-nPos+1);
        MatchString(strText,"NAME");
        if (MatchString(strText,"="))
            if (MatchString(strText,"\""))
            {
                nPos = strText.Pos("\"");
                //提取初始值
                if (nPos > 0)
                    strName = strText.SubString(1,nPos-1);
            }
    }
/*    nPos = strName.Pos("__");
    if (nPos > 0)
    {
        strKeywords = strName.SubString(nPos+3,strName.Length()-nPos);
        strKeyField = strName.SubString(0,nPos-1);
        m_bKeywords=true;
    }
*/    nPos =strName.Pos("_");
    if (nPos >0)
    {
       strText=strName.SubString(nPos+1,strName.Length()-nPos);
       if (strText=="t")
       {
         m_bDatetime = true;
         m_bEdit = false;
       }
       if (strText=="l")
       {
         m_bTimeLength = true;
         m_bEdit = false;
       }
       if (strText=="tc")
       {
         m_bTimeCode = true;
         m_bEdit = false;
       }
       if (!m_bTimeLength && !m_bTimeCode && !m_bDatetime)
           m_bEdit = true;
       if ((strText=="0")||(strText=="1")||(strText=="2"))
         m_bEdit = true;
    }
    else
       m_bEdit = true;

    //滤掉可能出现的下划线
    nPos = strName.Pos("_");
    if (nPos > 0)
    {
        if (m_bInputTem)
            strName = strName.SubString(1,nPos-1);
    }

    return strName;
}

//---------------------------------------------------------------------------
AnsiString THtmlForm::SQLStr(AnsiString strText)
{
   unsigned int nLength = strText.Length(),i;
   AnsiString str1=strText;
   unsigned char *p=(unsigned char*)strText.c_str();
   unsigned int flag=0, Count = 1;

   for (i=0;i<nLength;i++,p++)
    {
    	if (flag==0)
        {
        	if ( ((*p)>0x80)&((*p)<0xff) )
            {
            flag=1;
            }
            else if (*p == '\'')
    	    {
        	str1.Insert("'",i+Count);
            Count++;
        	}
        }
        else
        {
        	flag=0;
        }
    }
    str1+=".";
    return str1;
}
//---------------------------------------------------------------------------
void THtmlForm::GetControlText(TInputInfo * NewValues)
{
	ControlInfo * pNode;
    TMemo * pMemo;
    AnsiString strText;
    char * pText;
    bool bChecked;

	pNode = m_pControlInfoHeader;

	//检索控件信息链表
    while (pNode)
	{
//        if (NewValues->lInfo->Count > 1)
//            strData = (AnsiString)(char *)NewValues->lInfo->Items[1];

//       NewValues->slInfoType->Add(pNode->strClassName);
        NewValues->slInfoType->Add(pNode->strControlName);

        strText = "";
        if (pNode->strClassName == "TEdit")
        {
            strText = ((TEdit *)pNode->pControl)->Text;
            if (strText.IsEmpty())
                pText = NULL;
            else
            {
                pText   = new char[strText.Length()+1];
                strcpy(pText,strText.c_str());
            }

            NewValues->lInfo->Add(pText);

            pNode = pNode->pNextControl;

            continue;
        }
        if (pNode->strClassName == "TMaskEdit")
        {
            strText = ((TMaskEdit *)pNode->pControl)->Text;
            if ((strText.IsEmpty())||(strText=="  :  :  :  ")||(strText=="  :  :  "))
                pText = NULL;
            else
            {
                pText   = new char[strText.Length()+1];
                strcpy(pText,strText.c_str());
            }

            NewValues->lInfo->Add(pText);

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TDateTimePicker")
        {
            strText = (AnsiString)((int)((TDateTimePicker *)pNode->pControl)->Date);
            if (strText.IsEmpty())
                pText = NULL;
            else
            {
                pText   = new char[strText.Length()+1];
                strcpy(pText,strText.c_str());
            }

            NewValues->lInfo->Add(pText);

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TMemo")
        {
            pMemo = (TMemo *)pNode->pControl;
/*            for (i=0;i<pMemo->Lines->Count;i++)
                strText = strText + pMemo->Lines->Strings[i] + "\n";

            //删除结尾的回车符
            strText.Delete(strText.Length(),1);

            if (strText.IsEmpty())
                pText = NULL;
            else
            {
                pText = new char[strText.Length()+1];
                strcpy(pText,strText.c_str());
            }
*/
            strText = pMemo->Text;
            if (strText.IsEmpty())
                pText = NULL;
            else
            {
                pText = new char[strText.Length()+1];
                strcpy(pText,strText.c_str());
            }
            NewValues->lInfo->Add(pText);
                        
            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TRadioButton")
        {
            pText = new char[2];

            bChecked = ((TRadioButton *)pNode->pControl)->Checked;

            if (bChecked)
                strcpy(pText,"1");
            else
                strcpy(pText,"0");

            NewValues->lInfo->Add(pText);

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TCheckBox")
        {
            pText = new char[2];

            bChecked = ((TCheckBox *)pNode->pControl)->Checked;

            if (bChecked)
                strcpy(pText,"1");
            else
                strcpy(pText,"0");

            NewValues->lInfo->Add(pText);

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TComboBox")
        {
            TComboBox * pComboBox = (TComboBox *)(pNode->pControl);

            if (pComboBox->ItemIndex >= 0)
            {
                strText = pComboBox->Items->Strings[pComboBox->ItemIndex].c_str();

                pText   = new char[strText.Length()+1];

                strcpy(pText,strText.c_str());
            }
            else
                pText = NULL;

            NewValues->lInfo->Add(pText);

            pNode = pNode->pNextControl;

            continue;
        }

        NewValues->lInfo->Add(NULL);

        pNode = pNode->pNextControl;

        continue;
	}
}

//---------------------------------------------------------------------------
void THtmlForm::GetControlName(TInputInfo * NewValues)
{
	ControlInfo * pNode;

    //清空控件名列表
    NewValues->slInfoType->Clear();

	pNode = m_pControlInfoHeader;

	//检索控件信息链表
    while (pNode)
	{
        NewValues->slInfoType->Add(pNode->strControlName);

        pNode = pNode->pNextControl;

        continue;
	}
}
//---------------------------------------------------------------------------
void THtmlForm::SetControlText(TInputInfo * NewValues)
{
	ControlInfo * pNode;
    TMemo * pMemo;
    TComboBox * pComboBox;
    AnsiString strText,strLine;
    bool bChecked;
    int nCount = 0,nPos;

	pNode = m_pControlInfoHeader;

	//检索控件信息链表
    while (pNode)
	{
        if (nCount >= NewValues->lInfo->Count)
            break;

        if (pNode->strClassName == "TEdit")
        {
            strText = (AnsiString)((char *)NewValues->lInfo->Items[nCount]);

            ((TEdit *)pNode->pControl)->Text = strText;

            pNode = pNode->pNextControl;

            nCount++;

            continue;
        }

        if (pNode->strClassName == "TMemo")
        {
            pMemo = (TMemo *)pNode->pControl;

            pMemo->Clear();

            strText = (AnsiString)((char *)NewValues->lInfo->Items[nCount]);

            while (! strText.IsEmpty())
            {
                nPos = strText.Pos("\n");

                if (nPos > 0)
                {
                    strLine = strText.SubString(1,nPos-1);
                    strText.Delete(1,nPos);
                }
                else
                {
                    strLine = strText;
                    strText.Delete(1,strText.Length());
                }

                pMemo->Lines->Add(strLine);
            }

            pNode = pNode->pNextControl;

            nCount++;

            continue;
        }

        if (pNode->strClassName == "TRadioButton")
        {
            bChecked = *((bool *)NewValues->lInfo->Items[nCount]);

            ((TRadioButton *)pNode->pControl)->Checked = bChecked;

            pNode = pNode->pNextControl;

            nCount++;

            continue;
        }

        if (pNode->strClassName == "TCheckBox")
        {
            bChecked = *((bool *)NewValues->lInfo->Items[nCount]);

            ((TCheckBox *)pNode->pControl)->Checked = bChecked;

            pNode = pNode->pNextControl;

            nCount++;

            continue;
        }

        if (pNode->strClassName == "TComboBox")
        {
            strText = (AnsiString)((char *)NewValues->lInfo->Items[nCount]);

            pComboBox = (TComboBox *)(pNode->pControl);

            pComboBox->ItemIndex = pComboBox->Items->IndexOf(strText);

            pNode = pNode->pNextControl;

            nCount++;

            continue;
        }

        pNode = pNode->pNextControl;

        nCount++;

        continue;
	}
}
//---------------------------------------------------------------------------
AnsiString THtmlForm::GetTimeCodeName()
{
    AnsiString strFieldName;
	ControlInfo * pNode;
	pNode = m_pControlInfoHeader;
    strFieldName = (AnsiString)"";
    while (pNode)
	{
       if ((pNode->strClassName == "TButton")&&(((TButton*)(pNode->pControl))->Focused()))
       {
            strFieldName = pNode->strControlName.Trim();
            return strFieldName;
       }
       else
       {
        	pNode = pNode->pNextControl;
            continue;
       }
    }
    return strFieldName;

}
//---------------------------------------------------------------------------
AnsiString THtmlForm::GetImageName()
{
    AnsiString strFieldName;
	ControlInfo * pNode;
	pNode = m_pControlInfoHeader;
    strFieldName = (AnsiString)"";
    while (pNode)
	{
       if ((pNode->strClassName == "TImageEn")&&(pNode->pControl->ControlState.Contains(csLButtonDown)))
       {
            strFieldName = pNode->strControlName;
            return strFieldName;
       }
       else
       {
        	pNode = pNode->pNextControl;
            continue;
       }
    }
    return strFieldName;

}
//---------------------------------------------------------------------------
bool THtmlForm::GetImage(TStream* pStream,AnsiString strFieldName)
{
/*	ControlInfo * pNode;
	pNode = m_pControlInfoHeader;
    while (pNode)
	{
       if ((pNode->strClassName == "TImageEn")&&(pNode->strControlName == strFieldName))
       {
             try{
             ((TImageEn* )pNode->pControl)->SaveToStreamJpeg(pStream,100);
             }
             catch(...)
             {
             }
             return true;
       }
       else
       {
        	pNode = pNode->pNextControl;
            continue;
       }
    }
*/    return true;

}
//---------------------------------------------------------------------------
bool THtmlForm::SetTimeCode(AnsiString strTimeCode,AnsiString strFieldName)
{
	ControlInfo * pNode;
    int i;

    i=0;
	pNode = m_pControlInfoHeader;
    while (pNode)
	{
       if ((pNode->strClassName == "TMaskEdit")&&(pNode->strControlName == strFieldName))
       {
            SetSingleControlText(i,strTimeCode.c_str());
            return true;
       }
       else
       {
        	pNode = pNode->pNextControl;
            i++;
            continue;
       }
    }
    return false;

}
//---------------------------------------------------------------------------
bool THtmlForm::SetImage(TStream* pStream,AnsiString strFieldName)
{
	ControlInfo * pNode;
	pNode = m_pControlInfoHeader;
    while (pNode)
	{
       if ((pNode->strClassName == "TImageEn")&&(pNode->strControlName == strFieldName))
       {
            pStream->Position = 0;
            try{
                ((TImageEn *)pNode->pControl)->LoadFromStreamJpeg(pStream);
                ((TImageEn *)pNode->pControl)->Resample(IMAGEWIDTH,IMAGEHEIGHT,rfNone);
            }
            catch(...)
            {
            }
            return true;
       }
       else
       {
        	pNode = pNode->pNextControl;
            continue;
       }
    }
    return false;
}

//---------------------------------------------------------------------------
bool THtmlForm::SetSingleControlText(int nIndex,char * pNewValue)
{
	ControlInfo * pNode;
    TMemo * pMemo;
    TComboBox * pComboBox;
    AnsiString strText,strLine;
    bool bChecked;
    int nCount = 0;

	pNode = m_pControlInfoHeader;

	//检索控件信息链表
    while (pNode)
	{
        if (nCount == nIndex)
        {
            if (pNode->strClassName == "TEdit")
            {
                strText = (AnsiString)pNewValue;

				((TEdit *)pNode->pControl)->Text = strText;

				return true;
			}

            if (pNode->strClassName == "TMaskEdit")
            {
                strText = (AnsiString)pNewValue;

				((TMaskEdit *)pNode->pControl)->Text = strText;

				return true;
			}

            if (pNode->strClassName == "TDateTimePicker")
            {
                strText = (AnsiString)pNewValue;

				((TDateTimePicker *)pNode->pControl)->Date =(TDateTime)strText.ToInt();

				return true;
			}

			if (pNode->strClassName == "TMemo")
			{
				pMemo = (TMemo *)pNode->pControl;
                pMemo->Clear();

				strText = (AnsiString)pNewValue;
                pMemo->Text = strText;

/*	 			while (! strText.IsEmpty())
				{
					nPos = strText.Pos("\n");

					if (nPos > 0)
					{
						strLine = strText.SubString(1,nPos-1);
						strText.Delete(1,nPos);
					}
					else
					{
						strLine = strText;
						strText.Delete(1,strText.Length());
					}
                    pMemo->Lines->Add(strLine);

	    		}
*/
				return true;
			}

			if (pNode->strClassName == "TRadioButton")
			{
				if (pNewValue[0] == '0')
					bChecked = false;
				else
					bChecked = true;

				((TRadioButton *)pNode->pControl)->Checked = bChecked;

				pNode = pNode->pNextControl;

				return true;
			}

			if (pNode->strClassName == "TCheckBox")
			{
				if (pNewValue[0] == '0')
					bChecked = false;
				else
					bChecked = true;

				((TCheckBox *)pNode->pControl)->Checked = bChecked;

				pNode = pNode->pNextControl;

				return true;
			}

			if (pNode->strClassName == "TComboBox")
			{
				strText = (AnsiString)pNewValue;

				pComboBox = (TComboBox *)(pNode->pControl);

				pComboBox->ItemIndex = pComboBox->Items->IndexOf(strText);

				pNode = pNode->pNextControl;

				return true;
			}
		}

		pNode = pNode->pNextControl;

		nCount++;

		continue;
	}

	return false;
}

//---------------------------------------------------------------------------
void THtmlForm::ClearValues(TInputInfo * NewValues)
{
	ControlInfo * pNode;
    int nCount = 0;
    char * pText;

	pNode = m_pControlInfoHeader;

	//检索控件信息链表
    while (pNode)
	{
        if (NewValues->lInfo->Count <= nCount)
        //值链表长度小于控件信息链表长度，则创建新的值单元
            NewValues->lInfo->Add(NULL);
        else
        {
            pText = (char *)NewValues->lInfo->Items[nCount];
            if (pText)
            //若值指针不空，则清空之
            {
                delete pText;
                pText = NULL;
            }

            pNode = pNode->pNextControl;

            nCount++;

            continue;
	    }
    }
}
//---------------------------------------------------------------------------
void THtmlForm::ClearControlText()
{
	ControlInfo * pNode;
    TMemo       * pMemo;
    TComboBox   * pComboBox;
    TDateTime   time;

	pNode = m_pControlInfoHeader;

	//检索控件信息链表
    while (pNode)
	{
        if (pNode->strClassName == "TEdit")
        {
            ((TEdit *)pNode->pControl)->Text = "";

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TDateTimePicker")
        {
            ((TDateTimePicker *)pNode->pControl)->Date=time.CurrentDate();

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TImageEn")
        {
            ((TImageEn *)pNode->pControl)->Clear();

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TMaskEdit")
        {
            ((TMaskEdit *)pNode->pControl)->Text = "";

            pNode = pNode->pNextControl;

            continue;
        }


        if (pNode->strClassName == "TMemo")
        {
            pMemo = (TMemo *)pNode->pControl;

			pMemo->Lines->Clear();

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TRadioButton")
        {
            ((TRadioButton *)pNode->pControl)->Checked = false;

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TCheckBox")
        {
            ((TCheckBox *)pNode->pControl)->Checked = false;

            pNode = pNode->pNextControl;

            continue;
        }

        if (pNode->strClassName == "TComboBox")
        {
            pComboBox = (TComboBox *)(pNode->pControl);

            pComboBox->ItemIndex = -1;

            pNode = pNode->pNextControl;

            continue;
        }
        pNode = pNode->pNextControl;
	}
}

//---------------------------------------------------------------------------
void THtmlForm::ClearControlText(TInputInfo* pInfo)
{
	ControlInfo * pNode;
    TMemo       * pMemo;
    TComboBox   * pComboBox;
    int i;

	pNode = m_pControlInfoHeader;
    i=0;

	//检索控件信息链表
    while (pNode)
	{
        if (pNode->strClassName == "TEdit")
        {
            if (pInfo->slInfoType->Strings[i]=="MaterialID")
            {
               ((TEdit *)pNode->pControl)->SetFocus();
               ((TEdit *)pNode->pControl)->SelectAll();
            }
            else
               ((TEdit *)pNode->pControl)->Text = "";

            pNode = pNode->pNextControl;

            i++;
            continue;
        }

        if (pNode->strClassName == "TImageEn")
        {
            ((TImageEn *)pNode->pControl)->Clear();

            pNode = pNode->pNextControl;

            i++;
            continue;
        }

        if (pNode->strClassName == "TMaskEdit")
        {
            ((TMaskEdit *)pNode->pControl)->Text = "";

            pNode = pNode->pNextControl;

            i++;
            continue;
        }


        if (pNode->strClassName == "TMemo")
        {
            pMemo = (TMemo *)pNode->pControl;

			pMemo->Lines->Clear();

            pNode = pNode->pNextControl;

            i++;
            continue;
        }

        if (pNode->strClassName == "TRadioButton")
        {
            ((TRadioButton *)pNode->pControl)->Checked = false;

            pNode = pNode->pNextControl;

            i++;
            continue;
        }

        if (pNode->strClassName == "TCheckBox")
        {
            ((TCheckBox *)pNode->pControl)->Checked = false;

            pNode = pNode->pNextControl;

            i++;
            continue;
        }

        if (pNode->strClassName == "TComboBox")
        {
            pComboBox = (TComboBox *)(pNode->pControl);

            pComboBox->ItemIndex = -1;

            pNode = pNode->pNextControl;

            i++;
            continue;
        }
        i++;
        pNode = pNode->pNextControl;
	}
}
//---------------------------------------------------------------------------
TInputInfo::TInputInfo()
{
    slInfoType = new TStringList;
    lInfo      = new TList;
}
//---------------------------------------------------------------------------
TInputInfo::~TInputInfo()
{
    char * pText;

    for (int i=0;i<lInfo->Count;i++)
    {
        pText = (char *)lInfo->Items[i];

        if (pText != NULL)
            delete pText;
    }
    
    delete slInfoType;
    delete lInfo;
}
//---------------------------------------------------------------------------

