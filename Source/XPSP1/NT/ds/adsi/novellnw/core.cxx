//---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995
//
//  File:  core.cxx
//
//  Contents:
//
//  History:   11-1-95     krishnag    Created.
//
//----------------------------------------------------------------------------
#include "NWCOMPAT.hxx"
#pragma hdrstop

HRESULT
CCoreADsObject::InitializeCoreObject(
        BSTR Parent,
        BSTR Name,
        BSTR ClassName,
        BSTR Schema,
        REFCLSID rclsid,
        DWORD dwObjectState
        )
{
    HRESULT hr = S_OK;
    ADsAssert(Parent);
    ADsAssert(Name);
    ADsAssert(ClassName);


    if (  ( _tcsicmp( ClassName, PRINTJOB_CLASS_NAME ) == 0 )
       || ( _tcsicmp( ClassName, SESSION_CLASS_NAME ) == 0 )
       || ( _tcsicmp( ClassName, RESOURCE_CLASS_NAME ) == 0 )
       )
    {
        //
        // This three classes are not really DS objects so they don't
        // really have a parent. Hence, we set the parent string to empty
        // string.
        //
        hr = ADsAllocString( TEXT(""), &_ADsPath);

        BAIL_ON_FAILURE(hr);

        hr = ADsAllocString( TEXT(""), &_Parent);
    }
    else
    {
        hr = BuildADsPath(
                 Parent,
                 Name,
                 &_ADsPath
                 );

        BAIL_ON_FAILURE(hr);

        hr = ADsAllocString( Parent, &_Parent);
    }

    BAIL_ON_FAILURE(hr);

    hr = BuildADsGuid(
            rclsid,
            &_ADsGuid
        );
    BAIL_ON_FAILURE(hr);

    BAIL_ON_FAILURE(hr);

    hr = ADsAllocString( Name, &_Name);
    BAIL_ON_FAILURE(hr);

    hr = ADsAllocString( ClassName, &_ADsClass);
    BAIL_ON_FAILURE(hr);

    hr = BuildSchemaPath(
        Parent,
        Name,
        Schema,
        &_Schema
        );
    BAIL_ON_FAILURE(hr);

    _dwObjectState = dwObjectState;

error:
    RRETURN(hr);
}

CCoreADsObject::CCoreADsObject():
                        _Name(NULL),
                        _ADsPath(NULL),
                        _Parent(NULL),
                        _ADsClass(NULL),
                        _Schema(NULL),
                        _ADsGuid(NULL),
                        _dwObjectState(0)
{
}


CCoreADsObject::~CCoreADsObject()
{
    if (_Name) {
        ADsFreeString(_Name);
    }

    if (_ADsPath) {
        ADsFreeString(_ADsPath);
    }

    if (_Parent) {
        ADsFreeString(_Parent);
    }

    if (_ADsClass) {
        ADsFreeString(_ADsClass);
    }

    if (_Schema) {
        ADsFreeString(_Schema);
    }

    if (_ADsGuid) {
        ADsFreeString(_ADsGuid);
    }
}


HRESULT
CCoreADsObject::get_CoreName(BSTR * retval)
{
    HRESULT hr;

    if (FAILED(hr = ValidateOutParameter(retval))){
        NW_RRETURN_EXP_IF_ERR(hr);
    }

    hr = ADsAllocString(_Name, retval);
    NW_RRETURN_EXP_IF_ERR(hr);
}


HRESULT
CCoreADsObject::get_CoreADsPath(BSTR * retval)
{
    HRESULT hr;

    if (FAILED(hr = ValidateOutParameter(retval))){
        NW_RRETURN_EXP_IF_ERR(hr);
    }

    hr = ADsAllocString(_ADsPath, retval);
    NW_RRETURN_EXP_IF_ERR(hr);
}


HRESULT
CCoreADsObject::get_CoreADsClass(BSTR * retval)
{
    HRESULT hr;

    if (FAILED(hr = ValidateOutParameter(retval))){
        NW_RRETURN_EXP_IF_ERR(hr);
    }

    hr = ADsAllocString(_ADsClass, retval);
    NW_RRETURN_EXP_IF_ERR(hr);
}


HRESULT
CCoreADsObject::get_CoreParent(BSTR * retval)
{

    HRESULT hr;

   if (FAILED(hr = ValidateOutParameter(retval))){
        NW_RRETURN_EXP_IF_ERR(hr);
    }

    hr = ADsAllocString(_Parent, retval);
    NW_RRETURN_EXP_IF_ERR(hr);
}

HRESULT
CCoreADsObject::get_CoreSchema(BSTR * retval)
{

    HRESULT hr;

    if (FAILED(hr = ValidateOutParameter(retval))){
        NW_RRETURN_EXP_IF_ERR(hr);
    }

    if ( _Schema == NULL || *_Schema == 0 )
        NW_RRETURN_EXP_IF_ERR(E_ADS_PROPERTY_NOT_SUPPORTED);

    hr = ADsAllocString(_Schema, retval);
    NW_RRETURN_EXP_IF_ERR(hr);
}

HRESULT
CCoreADsObject::get_CoreGUID(BSTR * retval)
{
    HRESULT hr;

    if (FAILED(hr = ValidateOutParameter(retval))){
        NW_RRETURN_EXP_IF_ERR(hr);
    }

    hr = ADsAllocString(_ADsGuid, retval);
    NW_RRETURN_EXP_IF_ERR(hr);
}

STDMETHODIMP
CCoreADsObject::GetInfo(THIS_ BOOL fExplicit, DWORD dwPropertyID)
{
    NW_RRETURN_EXP_IF_ERR(E_NOTIMPL);
}






















