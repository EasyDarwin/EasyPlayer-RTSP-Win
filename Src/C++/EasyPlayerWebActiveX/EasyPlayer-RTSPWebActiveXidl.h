

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Feb 18 16:43:56 2019
 */
/* Compiler settings for EasyPlayerWebActiveX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __EasyPlayer2DRTSPWebActiveXidl_h__
#define __EasyPlayer2DRTSPWebActiveXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DEasyPlayerWebActiveX_FWD_DEFINED__
#define ___DEasyPlayerWebActiveX_FWD_DEFINED__
typedef interface _DEasyPlayerWebActiveX _DEasyPlayerWebActiveX;
#endif 	/* ___DEasyPlayerWebActiveX_FWD_DEFINED__ */


#ifndef ___DEasyPlayerWebActiveXEvents_FWD_DEFINED__
#define ___DEasyPlayerWebActiveXEvents_FWD_DEFINED__
typedef interface _DEasyPlayerWebActiveXEvents _DEasyPlayerWebActiveXEvents;
#endif 	/* ___DEasyPlayerWebActiveXEvents_FWD_DEFINED__ */


#ifndef __EasyPlayerWebActiveX_FWD_DEFINED__
#define __EasyPlayerWebActiveX_FWD_DEFINED__

#ifdef __cplusplus
typedef class EasyPlayerWebActiveX EasyPlayerWebActiveX;
#else
typedef struct EasyPlayerWebActiveX EasyPlayerWebActiveX;
#endif /* __cplusplus */

#endif 	/* __EasyPlayerWebActiveX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __EasyPlayerWebActiveXLib_LIBRARY_DEFINED__
#define __EasyPlayerWebActiveXLib_LIBRARY_DEFINED__

/* library EasyPlayerWebActiveXLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_EasyPlayerWebActiveXLib;

#ifndef ___DEasyPlayerWebActiveX_DISPINTERFACE_DEFINED__
#define ___DEasyPlayerWebActiveX_DISPINTERFACE_DEFINED__

/* dispinterface _DEasyPlayerWebActiveX */
/* [uuid] */ 


EXTERN_C const IID DIID__DEasyPlayerWebActiveX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FB5E35A6-2538-47C4-9B40-B0FB6A00DEA0")
    _DEasyPlayerWebActiveX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DEasyPlayerWebActiveXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DEasyPlayerWebActiveX * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DEasyPlayerWebActiveX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DEasyPlayerWebActiveX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DEasyPlayerWebActiveX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DEasyPlayerWebActiveX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DEasyPlayerWebActiveX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DEasyPlayerWebActiveX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DEasyPlayerWebActiveXVtbl;

    interface _DEasyPlayerWebActiveX
    {
        CONST_VTBL struct _DEasyPlayerWebActiveXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DEasyPlayerWebActiveX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DEasyPlayerWebActiveX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DEasyPlayerWebActiveX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DEasyPlayerWebActiveX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DEasyPlayerWebActiveX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DEasyPlayerWebActiveX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DEasyPlayerWebActiveX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DEasyPlayerWebActiveX_DISPINTERFACE_DEFINED__ */


#ifndef ___DEasyPlayerWebActiveXEvents_DISPINTERFACE_DEFINED__
#define ___DEasyPlayerWebActiveXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DEasyPlayerWebActiveXEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DEasyPlayerWebActiveXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("66887A6D-C75B-48B8-B5E1-BD722D875923")
    _DEasyPlayerWebActiveXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DEasyPlayerWebActiveXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DEasyPlayerWebActiveXEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DEasyPlayerWebActiveXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DEasyPlayerWebActiveXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DEasyPlayerWebActiveXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DEasyPlayerWebActiveXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DEasyPlayerWebActiveXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DEasyPlayerWebActiveXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DEasyPlayerWebActiveXEventsVtbl;

    interface _DEasyPlayerWebActiveXEvents
    {
        CONST_VTBL struct _DEasyPlayerWebActiveXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DEasyPlayerWebActiveXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DEasyPlayerWebActiveXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DEasyPlayerWebActiveXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DEasyPlayerWebActiveXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DEasyPlayerWebActiveXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DEasyPlayerWebActiveXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DEasyPlayerWebActiveXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DEasyPlayerWebActiveXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_EasyPlayerWebActiveX;

#ifdef __cplusplus

class DECLSPEC_UUID("1EE1C648-F4A9-42F9-9AA7-2C8E3AF7B7FD")
EasyPlayerWebActiveX;
#endif
#endif /* __EasyPlayerWebActiveXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


