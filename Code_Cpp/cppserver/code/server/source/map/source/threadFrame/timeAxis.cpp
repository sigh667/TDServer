//#include "stdafx.h"
//#include "threadPool/TimeAxis.h"
//#include "TimeManager.h"
//#include "ITimeEvent.h"
//
//
//
//VOID	STimeElement::CancelEvent()
//{
//	m_pEvent->Cancel();
//	SAFE_DELETE(m_pEvent);
//}
//
////////////////////////////////////////////////////////////////////////////
//CTimeAxis::CTimeAxis()
//{
//	m_pParent = NULL;
//}
//
//CTimeAxis::~CTimeAxis()
//{
//	Release();
//}
//
//
//VOID	CTimeAxis::Init( CSR_ScriptObj* pScriptObj )
//{
//	m_pParent = pScriptObj;
//}
//VOID	CTimeAxis::Release()
//{
//	while ( !m_Heap.empty() )
//	{
//		STimeElement* pTemp = m_Heap.top();
//		SAFE_DELETE(pTemp);
//		m_Heap.pop();
//	}
//}
//
//bool	CTimeAxis::Delete( int eType )
//{
//	size_t nSize = m_Heap.size();
//	for ( size_t i=0; i<nSize; ++i )
//	{
//		STimeElement* pElement = m_Heap.at(i);
//		if ( pElement && pElement->m_pEvent && pElement->m_pEvent->GetEventType()==eType )
//		{
//			pElement->CancelEvent();
//		}
//	}
//
//	return true;
//}
//
//STimeElement*	CTimeAxis::FindFirst( int eType )
//{
//	size_t nSize = m_Heap.size();
//	for ( size_t i=0; i<nSize; ++i )
//	{
//		STimeElement* pElement = m_Heap.at(i);
//		if ( pElement && pElement->m_pEvent && pElement->m_pEvent->GetEventType()==eType )
//		{
//			return pElement;
//		}
//	}
//
//	return NULL;
//}
//
//VOID			CTimeAxis::FindAll( int eType, TEList& teList )
//{
//	teList.clear();
//	size_t nSize = m_Heap.size();
//	for ( size_t i=0; i<nSize; ++i )
//	{
//		STimeElement* pElement = m_Heap.at(i);
//		if ( pElement && pElement->m_pEvent && pElement->m_pEvent->GetEventType()==eType )
//		{
//			teList.push_back(pElement);
//		}
//	}
//}
//
//
//BOOL	CTimeAxis::HeartBeat( const U_UINT64& uTime )
//{
//	if ( m_Heap.empty() )
//		return TRUE;
//	
//	STimeElement* pElement;
//	while ( (!m_Heap.empty()) && (pElement=m_Heap.top()) )
//	{
//		if ( (pElement->m_U_UINT64) <= uTime )
//		{			
//			m_Heap.pop();
//
//			if ( pElement->m_pEvent )
//			{
//				pElement->m_pEvent->_DoEvent( m_pParent );	
//			}					
//			SAFE_DELETE(pElement);
//		}
//		else
//		{
//			break;
//		}
//	}
//
//	return TRUE;
//}
//
//// 插入事件，nMilliseconds毫秒之后响应
//VOID	CTimeAxis::Insert( CTimeEvent* pEvent, UINT nMilliseconds )
//{
//	if ( pEvent==NULL )
//	{
//		MY_ASSERT( 0 && "CTimeAxis::Insert参数错误" );
//		return;
//	}
//
//	STimeElement* pElement = new STimeElement;
//	pElement->m_pEvent = pEvent;
//	pElement->m_U_UINT64 = gTimeManager.GetTime64();
//
//	pElement->m_U_UINT64 += nMilliseconds;
//
//	m_Heap.push(pElement);
//}
//// 插入事件，在时间为uANSITimen零Milliseconds毫秒之后响应
//VOID	CTimeAxis::Insert( CTimeEvent* pEvent, UINT uANSITime, UINT nMilliseconds )
//{
//	if ( pEvent==NULL || nMilliseconds>=1000 )
//	{
//		MY_ASSERT( 0 && "CTimeAxis::Insert参数错误" );
//		return;
//	}
//
//	STimeElement* pElement = new STimeElement;
//	pElement->m_pEvent = pEvent;
//	pElement->m_U_UINT64.m_UINT32[1] = uANSITime;
//	pElement->m_U_UINT64.m_UINT32[0] = nMilliseconds;
//
//	m_Heap.push(pElement);
//}