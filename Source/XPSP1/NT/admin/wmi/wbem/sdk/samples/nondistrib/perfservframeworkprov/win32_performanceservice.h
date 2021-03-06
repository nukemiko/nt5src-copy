// Copyright (c) 1997-2001 Microsoft Corporation, All Rights Reserved
//
// Win32_PerformanceService.H -- WMI provider class definition
//
// Generated by Microsoft WMI Code Generation Engine
//
// Description: 
//
//=================================================================

// Property set identification
//============================

#define PROVIDER_NAME_WIN32_PERFORMANCESERVICE "Win32_PerformanceService"

// Property name externs -- defined in Win32_PerformanceService.cpp
//=================================================

extern const char* pFirstCounter ;
extern const char* pLastCounter ;
extern const char* pLibrary ;

class CWin32_PerformanceService : public Provider 
{
	public:
		// Constructor/destructor
		//=======================

		CWin32_PerformanceService(const CHString& chsClassName, LPCSTR lpszNameSpace);
		virtual ~CWin32_PerformanceService();

	protected:
		// Reading Functions
		//============================
		virtual HRESULT EnumerateInstances(MethodContext*  pMethodContext, long lFlags = 0L);
		virtual HRESULT GetObject(CInstance* pInstance, long lFlags = 0L);
		virtual HRESULT ExecQuery(MethodContext *pMethodContext, CFrameworkQuery& Query, long lFlags = 0L);

		// Writing Functions
		//============================
		virtual HRESULT PutInstance(const CInstance& Instance, long lFlags = 0L);
		virtual HRESULT DeleteInstance(const CInstance& Instance, long lFlags = 0L);

		// Other Functions
		virtual HRESULT ExecMethod( const CInstance& Instance,
						const BSTR bstrMethodName,
						CInstance *pInParams,
						CInstance *pOutParams,
						long lFlags = 0L );

		// TO DO: Declare any additional functions and accessor
		// functions for private data used by this class
		//===========================================================

	private:
		// All data members for CWin32_PerformanceService should be included here.   
		// Each private member should have a protected accessor

		// Helper Functions
		//=================

		BOOL ConstructInstance(HKEY hServices, LPCTSTR szServiceName, CInstance* pInstance);


} ;
