/////////////////////////////////////////////////////////////////////////////
//
//  Core Detours Functionality (detours.h of detours.lib)
//
//  Microsoft Research Detours Package, Version 4.0.1
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//

#pragma once
#ifndef _DETOURS_STRSAFECOMPAT_H_
#define _DETOURS_STRSAFECOMPAT_H_

#include "detours.h"
#include <stdio.h>
#include <limits.h>

#if _MSC_VER < 1400
#define _Must_inspect_result_
#endif

_Must_inspect_result_
static inline HRESULT StringCchLengthA(
    _In_reads_or_z_(cchMax) LPCSTR psz,
    _In_
    _In_range_(1, STRSAFE_MAX_CCH) size_t cchMax,
    _Out_opt_
    _Deref_out_range_(<, cchMax)
    _Deref_out_range_(<=, _String_length_(psz))
    _Out_ size_t* pcch)
{
    HRESULT hr = S_OK;
    size_t cchMaxPrev = cchMax;

    if (cchMax > 2147483647) {
        return ERROR_INVALID_PARAMETER;
    }

    while (cchMax && (*psz != '\0')) {
        psz++;
        cchMax--;
    }

    if (cchMax == 0) {
        // the string is longer than cchMax
        hr = ERROR_INVALID_PARAMETER;
    }

    if (SUCCEEDED(hr) && pcch) {
        *pcch = cchMaxPrev - cchMax;
    }

    return hr;
}

_Must_inspect_result_
static inline HRESULT StringCchCopyA(
    _Out_writes_(cchDest) _Always_(_Post_z_) LPSTR pszDest,
    _In_ size_t cchDest,
    _In_ LPCSTR pszSrc)
{
    HRESULT hr = S_OK;

    if (cchDest == 0) {
        // can not null terminate a zero-byte dest buffer
        hr = ERROR_INVALID_PARAMETER;
    }
    else {
        while (cchDest && (*pszSrc != '\0')) {
            *pszDest++ = *pszSrc++;
            cchDest--;
        }

        if (cchDest == 0) {
            // we are going to truncate pszDest
            pszDest--;
            hr = ERROR_INVALID_PARAMETER;
        }

        *pszDest= '\0';
    }

    return hr;
}

_Must_inspect_result_
static inline HRESULT StringCchCatA(
    _Out_writes_(cchDest) _Always_(_Post_z_) LPSTR pszDest,
    _In_ size_t cchDest,
    _In_ LPCSTR pszSrc)
{
    HRESULT hr;
    size_t cchDestCurrent;

    if (cchDest > 2147483647){
        return ERROR_INVALID_PARAMETER;
    }

    hr = StringCchLengthA(pszDest, cchDest, &cchDestCurrent);

    if (SUCCEEDED(hr) && cchDestCurrent < cchDest) {
        hr = StringCchCopyA(pszDest + cchDestCurrent,
                            cchDest - cchDestCurrent,
                            pszSrc);
    }

    return hr;
}

static inline HRESULT StringCchPrintfA(
  LPSTR pszDest,
  size_t cchDest,
  LPCSTR pszFormat,
  ...
)
{
    va_list va_args;
    va_start(va_args, pszFormat);
    int res = _vsnprintf(pszDest, cchDest, pszFormat, va_args);
    va_end(va_args);
    if (res < 0) {
        return ERROR_INVALID_PARAMETER;
    }
    return S_OK;
}

_Must_inspect_result_
static inline HRESULT StringCchLengthW(
    _In_reads_or_z_(cchMax) LPCWSTR psz,
    _In_
    _In_range_(1, STRSAFE_MAX_CCH) size_t cchMax,
    _Out_opt_
    _Deref_out_range_(<, cchMax)
    _Deref_out_range_(<=, _String_length_(psz))
    _Out_ size_t* pcch)
{
    HRESULT hr = S_OK;
    size_t cchMaxPrev = cchMax;

    if (cchMax > 2147483647) {
        return ERROR_INVALID_PARAMETER;
    }

    while (cchMax && (*psz != '\0')) {
        psz++;
        cchMax--;
    }

    if (cchMax == 0) {
        // the string is longer than cchMax
        hr = ERROR_INVALID_PARAMETER;
    }

    if (SUCCEEDED(hr) && pcch) {
        *pcch = cchMaxPrev - cchMax;
    }

    return hr;
}

_Must_inspect_result_
static inline HRESULT StringCchCopyW(
    _Out_writes_(cchDest) _Always_(_Post_z_) LPWSTR pszDest,
    _In_ size_t cchDest,
    _In_ LPCWSTR pszSrc)
{
    HRESULT hr = S_OK;

    if (cchDest == 0) {
        // can not null terminate a zero-byte dest buffer
        hr = ERROR_INVALID_PARAMETER;
    }
    else {
        while (cchDest && (*pszSrc != '\0')) {
            *pszDest++ = *pszSrc++;
            cchDest--;
        }

        if (cchDest == 0) {
            // we are going to truncate pszDest
            pszDest--;
            hr = ERROR_INVALID_PARAMETER;
        }

        *pszDest= '\0';
    }

    return hr;
}

_Must_inspect_result_
static inline HRESULT StringCchCatW(
    _Out_writes_(cchDest) _Always_(_Post_z_) LPWSTR pszDest,
    _In_ size_t cchDest,
    _In_ LPCWSTR pszSrc)
{
    HRESULT hr;
    size_t cchDestCurrent;

    if (cchDest > 2147483647){
        return ERROR_INVALID_PARAMETER;
    }

    hr = StringCchLengthW(pszDest, cchDest, &cchDestCurrent);

    if (SUCCEEDED(hr) && cchDestCurrent < cchDest) {
        hr = StringCchCopyW(pszDest + cchDestCurrent,
                            cchDest - cchDestCurrent,
                            pszSrc);
    }

    return hr;
}

static inline HRESULT StringCchPrintfW(
  LPWSTR pszDest,
  size_t cchDest,
  LPCWSTR pszFormat,
  ...
)
{
    va_list va_args;
    va_start(va_args, pszFormat);
    int res = _vsnwprintf(pszDest, cchDest, pszFormat, va_args);
    va_end(va_args);
    if (res < 0) {
        return ERROR_INVALID_PARAMETER;
    }
    return S_OK;
}


_Must_inspect_result_
static inline
HRESULT
DWordAdd(
    _In_ ULONG ulAugend,
    _In_ ULONG ulAddend,
    _Out_ _Deref_out_range_(==, ulAugend + ulAddend) ULONG* pulResult)
{
    HRESULT hr;

    if ((ulAugend + ulAddend) >= ulAugend)
    {
        *pulResult = (ulAugend + ulAddend);
        hr = S_OK;
    }
    else
    {
        *pulResult = 0xffffffff;
        hr = (HRESULT)0x80070216L;
    }

    return hr;
}

_Must_inspect_result_
static inline
HRESULT
DWordMult(
    _In_ ULONG ulMultiplicand,
    _In_ ULONG ulMultiplier,
    _Out_ _Deref_out_range_(==, ulMultiplicand * ulMultiplier) ULONG* pulResult)
{
    if (ulMultiplicand > UINT_MAX / ulMultiplier)
    {
        *pulResult = 0xffffffff;
        return (HRESULT)0x80070216L;
    }
    else
    {
        *pulResult = ulMultiplicand * ulMultiplier;
        return S_OK;
    }
}
#endif // _DETOURS_STRSAFECOMPAT_H
//
////////////////////////////////////////////////////////////////  End of File.