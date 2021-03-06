/* Copyright (C) Boris Nikolaus, Germany, 1996-1997. All rights reserved. */
/* Copyright (C) Microsoft Corporation 1997-1998, All rights reserved. */

#ifndef __ASN1C_LIB_ASN1_H__
#define __ASN1C_LIB_ASN1_H__

#include <msasn1.h>

#ifdef __cplusplus
extern "C" {
#endif

// THE FOLLOWING IS FROM MS_CORE.H

/*
 * This file contains operating system specific defines:
 *
 * Dynamic link library support:
 * Define DllExport as declspec for exported functions and
 * DllImport as declspec for imported functions.
 *
 * Floating point encoding support:
 * For encoding floating point values either
 * - finite()+isinf()+copysign()+frexp() or
 * - finite()+fpclass()+FP_PINF+FP_NINF+frexp()
 * is needed. Define HAS_ISINF for the former case or HAS_FPCLASS for
 * the latter case.
 * Define HAS_IEEEFP_H for inclusion of <ieeefp.h> or HAS_FLOAT_H for
 * inclusion of <float.h> if required.
 *
 * Integer type support
 * [u]int{8,16,32}_t must specify an integral (unsigned iff u-prefixed)
 * type of the specified size (in bits).
 */

/* MS-Windows 95/MS-Windows NT */
#define THIRTYTWO_BITS  1
// #define HAS_SIXTYFOUR_BITS 1
#define HAS_FLOAT_H     1
#define HAS_FPCLASS     1
#define fpclass(_d)     _fpclass(_d)
#define finite(_d)      _finite(_d)
#define isnan(_d)       _isnan(_d)
#define FP_PINF         _FPCLASS_PINF
#define FP_NINF         _FPCLASS_NINF
// #define HAS_STRICMP     1
#define DBL_PINF        {0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x7f}
#define DBL_MINF        {0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xff}


extern ASN1_PUBLIC ASN1intx_t ASN1intx_0, ASN1intx_1, ASN1intx_2, ASN1intx_16, ASN1intx_256, ASN1intx_64K, ASN1intx_1G;
// extern ASN1_PUBLIC ASN1stringtable_t ASN1NumericStringTable;
extern ASN1_PUBLIC void ASN1API ASN1intx_addoctet(ASN1intx_t *, ASN1intx_t *, ASN1octet_t);
extern ASN1_PUBLIC void ASN1API ASN1intx_suboctet(ASN1intx_t *, ASN1intx_t *, ASN1octet_t);
extern ASN1_PUBLIC void ASN1API ASN1intx_muloctet(ASN1intx_t *, ASN1intx_t *, ASN1octet_t);
extern ASN1_PUBLIC void ASN1API ASN1intx_inc(ASN1intx_t *);
extern ASN1_PUBLIC void ASN1API ASN1intx_dec(ASN1intx_t *);
extern ASN1_PUBLIC void ASN1API ASN1intx_neg(ASN1intx_t *, ASN1intx_t *);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1intx_log2(ASN1intx_t *);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1intx_log256(ASN1intx_t *);
extern ASN1_PUBLIC ASN1int32_t ASN1API ASN1intx_cmp(ASN1intx_t *, ASN1intx_t *);
extern ASN1_PUBLIC void ASN1API ASN1intx_dup(ASN1intx_t *, ASN1intx_t *);
extern ASN1_PUBLIC void ASN1API ASN1intx_setuint32(ASN1intx_t *, ASN1uint32_t);
extern ASN1_PUBLIC void ASN1API ASN1intx_setint32(ASN1intx_t *, ASN1int32_t);
extern ASN1_PUBLIC int ASN1API ASN1intxisuint64(ASN1intx_t *);
extern ASN1_PUBLIC int ASN1API ASN1intxisint64(ASN1intx_t *);
extern ASN1_PUBLIC int ASN1API ASN1intxisuint32(ASN1intx_t *);
extern ASN1_PUBLIC int ASN1API ASN1intxisint32(ASN1intx_t *);
extern ASN1_PUBLIC int ASN1API ASN1intxisuint16(ASN1intx_t *);
extern ASN1_PUBLIC int ASN1API ASN1intxisint16(ASN1intx_t *);
extern ASN1_PUBLIC int ASN1API ASN1intxisuint8(ASN1intx_t *);
extern ASN1_PUBLIC int ASN1API ASN1intxisint8(ASN1intx_t *);
#ifdef HAS_SIXTYFOUR_BITS
extern ASN1_PUBLIC ASN1uint64_t ASN1API ASN1intx2uint64(ASN1intx_t *);
extern ASN1_PUBLIC ASN1int64_t ASN1API ASN1intx2int64(ASN1intx_t *);
#endif
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1intx2uint32(ASN1intx_t *);
extern ASN1_PUBLIC ASN1int32_t ASN1API ASN1intx2int32(ASN1intx_t *);
extern ASN1_PUBLIC ASN1uint16_t ASN1API ASN1intx2uint16(ASN1intx_t *);
extern ASN1_PUBLIC ASN1int16_t ASN1API ASN1intx2int16(ASN1intx_t *);
extern ASN1_PUBLIC ASN1uint8_t ASN1API ASN1intx2uint8(ASN1intx_t *);
extern ASN1_PUBLIC ASN1int8_t ASN1API ASN1intx2int8(ASN1intx_t *);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1intx_octets(ASN1intx_t *);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1uint32_log2(ASN1uint32_t);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1uint32_log256(ASN1uint32_t);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1uint32_octets(ASN1uint32_t);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1uint32_uoctets(ASN1uint32_t);
extern ASN1_PUBLIC ASN1uint32_t ASN1API ASN1int32_octets(ASN1int32_t);
extern ASN1_PUBLIC double ASN1API ASN1intx2double(ASN1intx_t *);
extern ASN1_PUBLIC double ASN1API ASN1real2double(ASN1real_t *);
extern ASN1_PUBLIC double ASN1API ASN1double_minf();
extern ASN1_PUBLIC double ASN1API ASN1double_pinf();
extern ASN1_PUBLIC int ASN1API ASN1double_isminf(double);
extern ASN1_PUBLIC int ASN1API ASN1double_ispinf(double);
extern ASN1_PUBLIC int ASN1API ASN1generalizedtime2string(char *, ASN1generalizedtime_t *);
extern ASN1_PUBLIC int ASN1API ASN1utctime2string(char *, ASN1utctime_t *);
extern ASN1_PUBLIC int ASN1API ASN1string2generalizedtime(ASN1generalizedtime_t *, char *);
extern ASN1_PUBLIC int ASN1API ASN1string2utctime(ASN1utctime_t *, char *);

/* ------ Memory Free APIs ------ */

extern ASN1_PUBLIC void ASN1API ASN1real_free(ASN1real_t *);
extern ASN1_PUBLIC void ASN1API ASN1bitstring_free(ASN1bitstring_t *);
extern ASN1_PUBLIC void ASN1API ASN1octetstring_free(ASN1octetstring_t *);
extern ASN1_PUBLIC void ASN1API ASN1objectidentifier_free(ASN1objectidentifier_t *);
extern ASN1_PUBLIC void ASN1API ASN1charstring_free(ASN1charstring_t *);
extern ASN1_PUBLIC void ASN1API ASN1char16string_free(ASN1char16string_t *);
extern ASN1_PUBLIC void ASN1API ASN1char32string_free(ASN1char32string_t *);
extern ASN1_PUBLIC void ASN1API ASN1ztcharstring_free(ASN1ztcharstring_t);
extern ASN1_PUBLIC void ASN1API ASN1ztchar16string_free(ASN1ztchar16string_t);
extern ASN1_PUBLIC void ASN1API ASN1ztchar32string_free(ASN1ztchar32string_t);
extern ASN1_PUBLIC void ASN1API ASN1external_free(ASN1external_t *);
extern ASN1_PUBLIC void ASN1API ASN1embeddedpdv_free(ASN1embeddedpdv_t *);
extern ASN1_PUBLIC void ASN1API ASN1characterstring_free(ASN1characterstring_t *);
extern ASN1_PUBLIC void ASN1API ASN1open_free(ASN1open_t *);

/* ------ Comparison APIs ------ */

extern ASN1_PUBLIC long ASN1API ASN1double_cmp(double, double);
extern ASN1_PUBLIC long ASN1API ASN1real_cmp(ASN1real_t *, ASN1real_t *);
extern ASN1_PUBLIC long ASN1API ASN1external_cmp(ASN1external_t *, ASN1external_t *);
extern ASN1_PUBLIC long ASN1API ASN1embeddedpdv_cmp(ASN1embeddedpdv_t *, ASN1embeddedpdv_t *);
extern ASN1_PUBLIC long ASN1API ASN1characterstring_cmp(ASN1characterstring_t *, ASN1characterstring_t *);
extern ASN1_PUBLIC long ASN1API ASN1sequenceoflengthpointer_cmp(ASN1uint32_t, void *, ASN1uint32_t, void *, ASN1uint32_t, int (*)(void *, void *));
extern ASN1_PUBLIC long ASN1API ASN1sequenceofsinglylinkedlist_cmp(void *, void *, ASN1uint32_t, int (*)(void *, void *));
extern ASN1_PUBLIC long ASN1API ASN1sequenceofdoublylinkedlist_cmp(void *, void *, ASN1uint32_t, int (*)(void *, void *));
extern ASN1_PUBLIC long ASN1API ASN1setoflengthpointer_cmp(ASN1uint32_t, void *, ASN1uint32_t, void *, ASN1uint32_t, int (*)(void *, void *));
extern ASN1_PUBLIC long ASN1API ASN1setofsinglylinkedlist_cmp(void *, void *, ASN1uint32_t, int (*)(void *, void *));
extern ASN1_PUBLIC long ASN1API ASN1setofdoublylinkedlist_cmp(void *, void *, ASN1uint32_t, int (*)(void *, void *));



#define ASN1BITSET(_val, _bitnr) \
    ((_val)[(_bitnr) >> 3] |= 0x80 >> ((_bitnr) & 7))
#define ASN1BITCLR(_val, _bitnr) \
    ((_val)[(_bitnr) >> 3] &= ~(0x80 >> ((_bitnr) & 7)))
#define ASN1BITTEST(_val, _bitnr) \
    ((_val)[(_bitnr) >> 3] & (0x80 >> ((_bitnr) & 7)))
    



#ifdef __cplusplus
}
#endif

#endif // __ASN1C_LIB_ASN1_H__

