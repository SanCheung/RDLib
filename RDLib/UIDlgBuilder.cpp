#include "StdAfx.h"

////
#include "TextUI.h"
//#include "SliderUI.h"
//#include "UIEdit.h"

//namespace DuiLib {

CDialogBuilder::CDialogBuilder() : m_pCallback(NULL), m_pstrtype(NULL)
{

}

CControlUI* CDialogBuilder::Create(STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback, 
                                   CPaintManagerUI* pManager, CControlUI* pParent)
{
    if( HIWORD(xml.m_lpstr) != NULL )
	{
        if( *(xml.m_lpstr) == _T('<') ) {
            if( !m_xml.Load(xml.m_lpstr) ) return NULL;
        }
        else {
            if( !m_xml.LoadFromFile(xml.m_lpstr) ) return NULL;
        }
    }
    else {
		HINSTANCE		hDll = CAppData::GetResourceDll();

        HRSRC hResource = ::FindResource( hDll, xml.m_lpstr, type);
        if( hResource == NULL ) 
			return NULL;

        HGLOBAL hGlobal = ::LoadResource( hDll, hResource);
        if( hGlobal == NULL ) {
            FreeResource(hResource);
            return NULL;
        }

        m_pCallback = pCallback;
        if( !m_xml.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(hDll, hResource) )) 
			return NULL;

        ::FreeResource(hResource);
        m_pstrtype = type;
    }

    return CreateFromXML(pCallback, pManager, pParent);
}

CControlUI* CDialogBuilder::CreateFromXML2(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
{
    m_pCallback = pCallback;
    CMarkupNode root = m_xml.GetRoot();
    if( !root.IsValid() ) return NULL;

    if( pManager ) {
        LPCTSTR pstrClass = NULL;
        int nAttributes = 0;
        LPCTSTR pstrName = NULL;
        LPCTSTR pstrValue = NULL;
        LPTSTR pstr = NULL;
        for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) {
            pstrClass = node.GetName();
            if( EQUSTR(pstrClass, _T("Image")) )
			{
                nAttributes = node.GetAttributeCount();
                LPCTSTR pImageName = NULL;
                LPCTSTR pImageResType = NULL;
                DWORD mask = 0;
                for( int i = 0; i < nAttributes; i++ ) 
				{
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);
                    if( EQUSTR(pstrName, _T("name")) )
					{
                        pImageName = pstrValue;
                    }
                    else if( EQUSTR(pstrName, _T("restype")) )
					{
                        pImageResType = pstrValue;
                    }
                    else if( EQUSTR(pstrName, _T("mask")) )
					{
                        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
                        mask = _tcstoul(pstrValue, &pstr, 16);
                    }
                }

                if( pImageName ) 
					pManager->AddImage(pImageName, pImageResType, mask);
            }
            else if( EQUSTR(pstrClass, _T("Font")) )
			{
                nAttributes = node.GetAttributeCount();
                LPCTSTR pFontName = NULL;
                int size = 12;
                bool bold = false;
                bool underline = false;
                bool italic = false;
                bool defaultfont = false;
                for( int i = 0; i < nAttributes; i++ )
				{
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);
                    if( EQUSTR(pstrName, _T("name")) )
					{
                        pFontName = pstrValue;
                    }
                    else if( EQUSTR(pstrName, _T("size")) )
					{
                        size = _tcstol(pstrValue, &pstr, 10);
                    }
                    else if( EQUSTR(pstrName, _T("bold")) ) 
					{
                        bold = EQUSTR_TRUE(pstrValue);
                    }
                    else if( EQUSTR(pstrName, _T("underline")) ) 
					{
                        underline = EQUSTR_TRUE(pstrValue);
                    }
                    else if( EQUSTR(pstrName, _T("italic")) )
					{
                        italic = EQUSTR_TRUE(pstrValue);
                    }
                    else if( EQUSTR(pstrName, _T("default")) )
					{
                        defaultfont = EQUSTR_TRUE(pstrValue);
                    }
                }
                if( pFontName )
				{
                    pManager->AddFont(pFontName, size, bold, underline, italic);
                    if( defaultfont ) 
						pManager->SetDefaultFont(pFontName, size, bold, underline, italic);
                }
            }
            else if( EQUSTR(pstrClass, _T("Default")) ) {
                nAttributes = node.GetAttributeCount();
                LPCTSTR pControlName = NULL;
                LPCTSTR pControlValue = NULL;
                for( int i = 0; i < nAttributes; i++ ) {
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);
                    if( EQUSTR(pstrName, _T("name")) ) {
                        pControlName = pstrValue;
                    }
                    else if( EQUSTR(pstrName, _T("value")) ) {
                        pControlValue = pstrValue;
                    }
                }
                if( pControlName ) {
                    pManager->AddDefaultAttributeList(pControlName, pControlValue);
                }
            }
        }

        pstrClass = root.GetName();
        if( EQUSTR(pstrClass, _T("Window")) )
		{
            if( pManager->GetPaintWindow() )
			{
                int nAttributes = root.GetAttributeCount();
                for( int i = 0; i < nAttributes; i++ )
				{
                    pstrName = root.GetAttributeName(i);
                    pstrValue = root.GetAttributeValue(i);
                    if( EQUSTR(pstrName, _T("size")) ) 
					{
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetInitSize(cx, cy);
                    } 
                    else if( EQUSTR(pstrName, _T("sizebox")) )
					{
						CDUIRect rc = CControlUI::FormatToRect( pstrValue );
                        pManager->SetSizeBox( rc );
                    }
                    else if( EQUSTR(pstrName, _T("caption")) )
					{
  						CDUIRect rc = CControlUI::FormatToRect( pstrValue );
                        pManager->SetCaptionRect( rc );
                    }
                    else if( EQUSTR(pstrName, _T("roundcorner")) ) {
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetRoundCorner(cx, cy);
                    } 
                    else if( EQUSTR(pstrName, _T("mininfo")) ) {
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetMinInfo(cx, cy);
                    }
                    else if( EQUSTR(pstrName, _T("maxinfo")) )
					{
                        LPTSTR pstr = NULL;
                        int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
                        int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
                        pManager->SetMaxInfo(cx, cy);
                    }
                    else if( EQUSTR(pstrName, _T("showdirty")) )
					{
                        pManager->SetShowUpdateRect(EQUSTR(pstrValue, _T("true")));
                    } 
                    else if( EQUSTR(pstrName, _T("alpha")) )
					{
                        pManager->SetTransparent(_ttoi(pstrValue));
                    } 
                    else if( EQUSTR(pstrName, _T("bktrans")) )
					{
                        pManager->SetBackgroundTransparent(EQUSTR(pstrValue, _T("true")));
                    } 
                    else if( EQUSTR(pstrName, _T("disabledfontcolor")) )
					{
                        DWORD clr = CControlUI::FormatToColor( pstrValue );
                        pManager->SetDefaultDisabledColor(clr);
                    } 
                    else if( EQUSTR(pstrName, _T("defaultfontcolor")) ) 
					{
                        DWORD cr = CControlUI::FormatToColor( pstrValue );
                        pManager->SetDefaultFontColor( cr );
                    }
                    else if( EQUSTR(pstrName, _T("linkfontcolor")) )
					{
                        DWORD cr = CControlUI::FormatToColor( pstrValue );
                        pManager->SetDefaultLinkFontColor( cr );
                    } 
                    else if( EQUSTR(pstrName, _T("linkhoverfontcolor")) )
					{
                        DWORD cr = CControlUI::FormatToColor( pstrValue );
                        pManager->SetDefaultLinkHoverFontColor( cr );
                    } 
                    else if( EQUSTR(pstrName, _T("selectedcolor")) )
					{
                        DWORD cr = CControlUI::FormatToColor( pstrValue );
                        pManager->SetDefaultSelectedBkColor( cr );
                    } 
					else if( EQUSTR(pstrName, _T("bordercolor")) )
					{
						DWORD cr = CControlUI::FormatToColor( pstrValue );
						pManager->SetBorderColor( cr );
					} 
                }
            }
        }
    }
    return _Parse(&root, pParent, pManager);
}

CMarkup* CDialogBuilder::GetMarkup()
{
    return &m_xml;
}

void CDialogBuilder::GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const
{
    return m_xml.GetLastErrorMessage(pstrMessage, cchMax);
}

void CDialogBuilder::GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const
{
    return m_xml.GetLastErrorLocation(pstrSource, cchMax);
}

CControlUI* CDialogBuilder::_Parse(CMarkupNode* pRoot, CControlUI* pParent, CPaintManagerUI* pManager)
{
    IContainerUI*	pContainer = NULL;
    CControlUI*		pReturn = NULL;
    for( CMarkupNode node = pRoot->GetChild() ; node.IsValid(); node = node.GetSibling() ) 
	{
        LPCTSTR pstrClass = node.GetName();
        if( EQUSTR(pstrClass, _T("Image")) || 
			EQUSTR(pstrClass, _T("Font"))  || 
			EQUSTR(pstrClass, _T("Default")) )
		{
			continue;
		}

		if( EQUSTR(pstrClass, _T("Script")) )
		{
			LPCTSTR pstrValue = node.GetValue();
			pManager->m_strScript = pstrValue;
			continue;
		}

        CControlUI* pControl = NULL;
        if( EQUSTR(pstrClass, _T("Include")) ) 
		{
            if( !node.HasAttributes() ) continue;

            int count = 1;
            LPTSTR pstr = NULL;
            TCHAR szValue[500] = { 0 };
            SIZE_T cchLen = lengthof(szValue) - 1;
            if ( node.GetAttributeValue(_T("count"), szValue, cchLen) )
                count = _tcstol(szValue, &pstr, 10);
            cchLen = lengthof(szValue) - 1;
            if ( !node.GetAttributeValue(_T("source"), szValue, cchLen) ) continue;
            for ( int i = 0; i < count; i++ )
			{
				// 使用资源dll，从资源中读取
                CDialogBuilder		builder;
                if( m_pstrtype != NULL ) 
				{
                    WORD id = (WORD)_tcstol(szValue, &pstr, 10); 
                    pControl = builder.Create((UINT)id, m_pstrtype, m_pCallback, pManager, pParent);
                }
                else
				{
                    pControl = builder.Create((LPCTSTR)szValue, (UINT)0, m_pCallback, pManager, pParent);
                }
            }
            continue;
        }
		//树控件XML解析
		//else if( _tcscmp(pstrClass, _T("TreeNode")) ) {
		//	CTreeNodeUI* pParentNode	= static_cast<CTreeNodeUI*>(pParent->GetInterface(_T("TreeNode")));
		//	CTreeNodeUI* pNode			= new CTreeNodeUI();
		//	if(pParentNode){
		//		if(!pParentNode->Add(pNode)){
		//			delete pNode;
		//			continue;
		//		}
		//	}

		//	// 若有控件默认配置先初始化默认属性
		//	if( pManager ) {
		//		pNode->SetManager(pManager, NULL, false);
		//		LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
		//		if( pDefaultAttributes ) {
		//			pNode->ApplyAttributeList(pDefaultAttributes);
		//		}
		//	}

		//	// 解析所有属性并覆盖默认属性
		//	if( node.HasAttributes() ) {
		//		TCHAR szValue[500] = { 0 };
		//		SIZE_T cchLen = lengthof(szValue) - 1;
		//		// Set ordinary attributes
		//		int nAttributes = node.GetAttributeCount();
		//		for( int i = 0; i < nAttributes; i++ ) {
		//			pNode->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
		//		}
		//	}

		//	//检索子节点及附加控件
		//	if(node.HasChildren()){
		//		CControlUI* pSubControl = _Parse(&node,pNode,pManager);
		//		if(pSubControl && _tcscmp(pSubControl->GetClass(),_T("TreeNodeUI")) != 0)
		//		{
		//			// 					pSubControl->SetFixedWidth(30);
		//			// 					CHorizontalLayoutUI* pHorz = pNode->GetTreeNodeHoriznotal();
		//			// 					pHorz->Add(new CEditUI());
		//			// 					continue;
		//		}
		//	}

		//	if(!pParentNode){
		//		CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(pParent->GetInterface(_T("TreeView")));
		//		ASSERT(pTreeView);
		//		if( pTreeView == NULL ) return NULL;
		//		if( !pTreeView->Add(pNode) ) {
		//			delete pNode;
		//			continue;
		//		}
		//	}
		//	continue;
		//}
        else {
            SIZE_T cchLen = _tcslen(pstrClass);
            switch( cchLen ) {
            case 4:
                if( EQUSTR(pstrClass, _T("Edit")) )			pControl = new CEditUI;
				else if( EQUSTR(pstrClass, _T("List")) )	pControl = new CListUI;
                else if( EQUSTR(pstrClass, _T("Text")) )	pControl = new CTextUI;
				//else if( EQUSTR(pstrClass, _T("Tree")) )	pControl = new CTreeUI;
				else if( EQUSTR(pstrClass, _T("HBox")) )	pControl = new CHorizontalLayoutUI;
				else if( EQUSTR(pstrClass, _T("VBox")) )	pControl = new CVerticalLayoutUI;
				else if( EQUSTR(pstrClass, _T("TBox")) )	pControl = new CTabLayoutUI;
                break;
            case 5:
                if( EQUSTR(pstrClass, _T("Combo")) )		pControl = new CComboUI;
                else if( EQUSTR(pstrClass, _T("Label")) )	pControl = new CLabelUI;
                break;
            case 6:
	            if( EQUSTR(pstrClass, _T("Button")) )		pControl = new CButtonUI;
				else if( EQUSTR(pstrClass, _T("Option")) )	pControl = new COptionUI;
    //            else if( EQUSTR(pstrClass, _T("Slider")) )	pControl = new CSliderUI;
				else if( EQUSTR(pstrClass, _T("Center")) )	pControl = new CCenterUI;
				//else if( EQUSTR(pstrClass, _T("Switch")) )	pControl = new CSwitchUI;
		
                break;
            case 7:
                if( EQUSTR(pstrClass, _T("Control")) )		pControl = new CControlUI;
				//else if( EQUSTR(pstrClass, _T("ActiveX")) )	pControl = new CActiveXUI;
				//else if( EQUSTR(pstrClass, _T("Outlook")) )	pControl = new COutlookUI;		
                break;
            case 8:
                if( EQUSTR(pstrClass, _T("Progress")) )			pControl = new CProgressUI;
   //             else if(  EQUSTR(pstrClass, _T("RichEdit")) )	pControl = new CRichEditUI;
				else if(EQUSTR(pstrClass, _T("CheckBox")) )		pControl = new CCheckBoxUI;	
			//	else if( EQUSTR(pstrClass, _T("ComboBox")) )	pControl = new CComboBoxUI;	
			//	else if( EQUSTR(pstrClass, _T("DateTime")) )	pControl = new CDateTimeUI;
			//	//else if( EQUSTR(pstrClass, _T("TreeView")) )			pControl = new CTreeViewUI;
   //             break;
   //         case 9:
   //             if( EQUSTR(pstrClass, _T("Container")) )		pControl = new CContainerUI;
   //             else if( EQUSTR(pstrClass, _T("TabLayout")) )	pControl = new CTabLayoutUI;
   //             else if( EQUSTR(pstrClass, _T("ScrollBar")) )	pControl = new CScrollBarUI; 
			//	else if(EQUSTR(pstrClass, _T("ColorItem")) )	pControl = new CColorItemUI; 
             break;
          case 10:
               if( EQUSTR(pstrClass, _T("ListHeader")) )		pControl = new CListHeaderUI;
			//	else if( EQUSTR(pstrClass, _T("TileLayout")) )	pControl = new CTileLayoutUI;
			//	else if(EQUSTR(pstrClass, _T("WebBrowser")))	pControl = new CWebBrowserUI;
			//	else if(EQUSTR(pstrClass, _T("ImageLabel")))	pControl = new CImageLabelUI;
              break;
			//case 11:
			//	if( EQUSTR(pstrClass, _T("OutlookItem")) )		pControl = new COutlookItemUI;
			//	else if(EQUSTR(pstrClass, _T("ColorPicker")) )	pControl = new CColorPickerUI; 
			//	break;
			//case 13:
			//	if( EQUSTR(pstrClass, _T("SeparatorLine")) )	pControl = new CSeparatorLineUI;
			//	break;
         case 14:
             if( EQUSTR(pstrClass, _T("VerticalLayout")) )			pControl = new CVerticalLayoutUI;
             else if( EQUSTR(pstrClass, _T("ListHeaderItem")) )		pControl = new CListHeaderItemUI;
			//	//else if( EQUSTR(pstrClass, _T("DateTimePicker")) )		pControl = new CDateTimePickerUI;
             break;
   //         case 15:
   //             if( EQUSTR(pstrClass, _T("ListTextElement")) )			pControl = new CListTextElementUI;
			//	else if( EQUSTR(pstrClass, _T("DropableControl")) )		pControl = new CDropableControlUI;				
   //             break;
            case 16:
                if( EQUSTR(pstrClass, _T("HorizontalLayout")) )			pControl = new CHorizontalLayoutUI;
                else if( EQUSTR(pstrClass, _T("ListLabelElement")) )	pControl = new CListLabelElementUI;
				else if( EQUSTR(pstrClass, _T("ListHeaderShadow")) )	pControl = new CListHeaderShadowUI;
                break;
			//case 19:
			//	if (EQUSTR(pstrClass, _T("HorizontalImageList")))		pControl = new CHorizontalImageListUI;
			//	break;
			//case 20:
			//	if( EQUSTR(pstrClass, _T("ListContainerElement")) )		pControl = new CListContainerElementUI;
   //             break;
            }

            // User-supplied control factory
            if( pControl == NULL )
			{
                CStdPtrArray* pPlugins = CAppData::GetPlugins();
                LPCREATECONTROL lpCreateControl = NULL;
                for( int i = 0; i < pPlugins->GetSize(); ++i )
				{
                    lpCreateControl = (LPCREATECONTROL)pPlugins->GetAt(i);
                    if( lpCreateControl != NULL ) 
					{
                        pControl = lpCreateControl(pstrClass);
                        if( pControl != NULL ) break;
                    }
                }
            }

            if( pControl == NULL && m_pCallback != NULL )
			{
                pControl = m_pCallback->CreateControl(pstrClass);
            }
        }

        ASSERT(pControl);
        if( pControl == NULL ) continue;

        // Add children
        if( node.HasChildren() ) {
            _Parse(&node, pControl, pManager);
        }
        // Attach to parent
        // 因为某些属性和父窗口相关，比如selected，必须先Add到父窗口
        if( pParent != NULL )
		{
            if( pContainer == NULL ) 
				pContainer = static_cast<IContainerUI*>(pParent->GetInterface(_T("IContainer")));
            ASSERT(pContainer);
            if( pContainer == NULL ) return NULL;
            if( !pContainer->Add(pControl) )
			{
                delete pControl;
                continue;
            }
        }
        // Init default attributes
        if( pManager )
		{
            pControl->SetManager(pManager, NULL, false);
            LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
            if( pDefaultAttributes )
			{
                pControl->ApplyAttributeList(pDefaultAttributes);
            }
        }
        // Process attributes
        if( node.HasAttributes() )
		{
            TCHAR szValue[500] = { 0 };
            SIZE_T cchLen = lengthof(szValue) - 1;
            // Set ordinary attributes
            int nAttributes = node.GetAttributeCount();
            for( int i = 0; i < nAttributes; i++ )
			{
                pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
            }
        }
        if( pManager ) {
            pControl->SetManager(NULL, NULL, false);
        }
        // Return first item
        if( pReturn == NULL ) pReturn = pControl;
    }
    return pReturn;
}

CControlUI* CDialogBuilder::CreateFromXML(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
{
	m_pCallback = pCallback;
	CMarkupNode root = m_xml.GetRoot();
	if( !root.IsValid() ) return NULL;

	if( pManager ) {
		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;
		for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) {
			pstrClass = node.GetName();
			if( EQUSTR(pstrClass, _T("Image")) )
			{
				_ParseImage( &node, pManager );
			}
			else if( EQUSTR(pstrClass, _T("Font")) )
			{
				_ParseFont( &node, pManager );
			}
			else if( EQUSTR(pstrClass, _T("Default")) ) 
			{
				_ParseDefault( &node, pManager );
			}
			//else if( EQUSTR(pstrClass, _T("Script")) ) 
			//{
			//	_ParseScript( &node, pManager );
			//}
		}

		pstrClass = root.GetName();
		if( EQUSTR(pstrClass, _T("Window")) )
		{
			if( pManager->GetPaintWindow() )
			{
				int nAttributes = root.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ )
				{
					pstrName = root.GetAttributeName(i);
					pstrValue = root.GetAttributeValue(i);
					if( EQUSTR(pstrName, _T("size")) ) 
					{
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetInitSize(cx, cy);
					} 
					else if( EQUSTR(pstrName, _T("sizebox")) )
					{
						CDUIRect rc = CControlUI::FormatToRect( pstrValue );
						pManager->SetSizeBox( rc );
					}
					else if( EQUSTR(pstrName, _T("caption")) )
					{
						CDUIRect rc = CControlUI::FormatToRect( pstrValue );
						pManager->SetCaptionRect( rc );
					}
					else if( EQUSTR(pstrName, _T("roundcorner")) ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetRoundCorner(cx, cy);
					} 
					else if( EQUSTR(pstrName, _T("mininfo")) ) {
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetMinInfo(cx, cy);
					}
					else if( EQUSTR(pstrName, _T("maxinfo")) )
					{
						LPTSTR pstr = NULL;
						int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
						int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
						pManager->SetMaxInfo(cx, cy);
					}
					else if( EQUSTR(pstrName, _T("showdirty")) )
					{
						pManager->SetShowUpdateRect(EQUSTR(pstrValue, _T("true")));
					} 
					else if( EQUSTR(pstrName, _T("alpha")) )
					{
						pManager->SetTransparent(_ttoi(pstrValue));
					} 
					else if( EQUSTR(pstrName, _T("bktrans")) )
					{
						pManager->SetBackgroundTransparent(EQUSTR(pstrValue, _T("true")));
					} 
					else if( EQUSTR(pstrName, _T("disabledfontcolor")) )
					{
						DWORD clr = CControlUI::FormatToColor( pstrValue );
						pManager->SetDefaultDisabledColor(clr);
					} 
					else if( EQUSTR(pstrName, _T("defaultfontcolor")) ) 
					{
						DWORD cr = CControlUI::FormatToColor( pstrValue );
						pManager->SetDefaultFontColor( cr );
					}
					else if( EQUSTR(pstrName, _T("linkfontcolor")) )
					{
						DWORD cr = CControlUI::FormatToColor( pstrValue );
						pManager->SetDefaultLinkFontColor( cr );
					} 
					else if( EQUSTR(pstrName, _T("linkhoverfontcolor")) )
					{
						DWORD cr = CControlUI::FormatToColor( pstrValue );
						pManager->SetDefaultLinkHoverFontColor( cr );
					} 
					else if( EQUSTR(pstrName, _T("selectedcolor")) )
					{
						DWORD cr = CControlUI::FormatToColor( pstrValue );
						pManager->SetDefaultSelectedBkColor( cr );
					} 
					else if( EQUSTR(pstrName, _T("bordercolor")) )
					{
						DWORD cr = CControlUI::FormatToColor( pstrValue );
						pManager->SetBorderColor( cr );
					} 
				}
			}
		}
	}

	return _Parse(&root, pParent, pManager);
}


void CDialogBuilder::_ParseImage( CMarkupNode* node, CPaintManagerUI *pManager )
{
	if( node == nullptr )
		return;

	LPCTSTR pstrClass = node->GetName();
	if( !EQUSTR(pstrClass, _T("Image")) )
		return;

	int nAttributes = node->GetAttributeCount();
	LPCTSTR pImageName = NULL;
	LPCTSTR pImageResType = NULL;

	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPTSTR pstr = NULL;

	DWORD mask = 0;
	for( int i = 0; i < nAttributes; i++ ) 
	{
		pstrName = node->GetAttributeName(i);
		pstrValue = node->GetAttributeValue(i);
		if( EQUSTR(pstrName, _T("name")) )
		{
			pImageName = pstrValue;
		}
		else if( EQUSTR(pstrName, _T("restype")) )
		{
			pImageResType = pstrValue;
		}
		else if( EQUSTR(pstrName, _T("mask")) )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			mask = _tcstoul(pstrValue, &pstr, 16);
		}
	}

	if( pImageName ) 
		pManager->AddImage(pImageName, pImageResType, mask);
}

void CDialogBuilder::_ParseFont( CMarkupNode* node, CPaintManagerUI *pManager )
{
	if( node == nullptr )
		return;

	LPCTSTR pstrClass = node->GetName();
	if( !EQUSTR(pstrClass, _T("Font")) )
		return;

	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPTSTR pstr = NULL;

	int nAttributes = node->GetAttributeCount();
	LPCTSTR pFontName = NULL;
	int size = 12;
	bool bold = false;
	bool underline = false;
	bool italic = false;
	bool defaultfont = false;
	for( int i = 0; i < nAttributes; i++ )
	{
		pstrName = node->GetAttributeName(i);
		pstrValue = node->GetAttributeValue(i);
		if( EQUSTR(pstrName, _T("name")) )
		{
			pFontName = pstrValue;
		}
		else if( EQUSTR(pstrName, _T("size")) )
		{
			size = _tcstol(pstrValue, &pstr, 10);
		}
		else if( EQUSTR(pstrName, _T("bold")) ) 
		{
			bold = EQUSTR_TRUE(pstrValue);
		}
		else if( EQUSTR(pstrName, _T("underline")) ) 
		{
			underline = EQUSTR_TRUE(pstrValue);
		}
		else if( EQUSTR(pstrName, _T("italic")) )
		{
			italic = EQUSTR_TRUE(pstrValue);
		}
		else if( EQUSTR(pstrName, _T("default")) )
		{
			defaultfont = EQUSTR_TRUE(pstrValue);
		}
	}
	if( pFontName )
	{
		pManager->AddFont(pFontName, size, bold, underline, italic);
		if( defaultfont ) 
			pManager->SetDefaultFont(pFontName, size, bold, underline, italic);
	}
}

void CDialogBuilder::_ParseDefault( CMarkupNode* node, CPaintManagerUI *pManager )
{
	if( node == nullptr )
		return;

	LPCTSTR pstrClass = node->GetName();
	if( !EQUSTR(pstrClass, _T("Default")) )
		return;

	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPTSTR pstr = NULL;

	int		nAttributes = node->GetAttributeCount();
	LPCTSTR pControlName = NULL;
	LPCTSTR pControlValue = NULL;
	for( int i = 0; i < nAttributes; i++ ) {
		pstrName = node->GetAttributeName(i);
		pstrValue = node->GetAttributeValue(i);
		if( EQUSTR(pstrName, _T("name")) ) {
			pControlName = pstrValue;
		}
		else if( EQUSTR(pstrName, _T("value")) ) {
			pControlValue = pstrValue;
		}
	}
	if( pControlName ) {
		pManager->AddDefaultAttributeList(pControlName, pControlValue);
	}
}

//void CDialogBuilder::_ParseScript( CMarkupNode* node, CPaintManagerUI *pManager )
//{
//	if( node == nullptr )
//		return;
//
//	LPCTSTR pstrClass = node->GetName();
//	if( !EQUSTR(pstrClass, _T("Script")) )
//		return;
//
//	LPCTSTR pstrValue = node->GetValue();
//	pManager->m_strScript = pstrValue;
//}

//} // namespace DuiLib
