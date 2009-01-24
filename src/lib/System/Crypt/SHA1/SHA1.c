/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh.                                                             *
*                                                                           *
* lulzJS is free software: you can redistribute it and/or modify            *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS is distributed in the hope that it will be useful.                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/
// Thanks to Sonia for this piece of OMG WTF IS THAT? :O

#include "SHA1.h"

JSBool exec (JSContext* cx) { return SHA1_initialize(cx); }

JSBool
SHA1_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "System", &jsParent);
    JS_GetProperty(cx, JSVAL_TO_OBJECT(jsParent), "Crypt", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &SHA1_class,
        SHA1_constructor, 0, NULL, SHA1_methods, NULL, SHA1_static_methods
    );

    if (object) {
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
SHA1_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    SHA1_ctx* data = JS_malloc(cx, sizeof(SHA1_ctx));
    JS_SetPrivate(cx, object, data);

    __SHA1_init(data);
    __SHA1_update(data, string, strlen(string));

    return JS_TRUE;
}

void
SHA1_finalize (JSContext* cx, JSObject* object)
{
    SHA1_ctx* data = JS_GetPrivate(cx, object);

    if (data) {
        JS_free(cx, data);
    }
}

#define __SHA1_rotate(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#define __SHA1_blk0(b32, i) ( \
        b32[i] = (__SHA1_rotate(b32[i], 24) & 0xFF00FF00) | \
        (__SHA1_rotate(b32[i], 8) & 0x00FF00FF))

#define __SHA1_blk(b32, i) ( \
        b32[i & 0x0F] = __SHA1_rotate(b32[(i + 13) & 0x0F] ^ \
        b32[(i + 8) & 0x0F] ^ \
        b32[(i + 2) & 0x0F] ^ \
        b32[i & 0x0F], 1))

#define __SHA1_R0(v,w,x,y,z,i,block) \
        z += ((w & (x ^ y) ) ^ y) + __SHA1_blk0(block->buffer32, i) + \
        0x5A827999 + __SHA1_rotate(v, 5); \
        w = __SHA1_rotate(w, 30);
        
#define __SHA1_R1(v,w,x,y,z,i,block) \
        z += ((w & (x ^ y)) ^ y) + __SHA1_blk(block->buffer32, i) + \
        0x5A827999 + __SHA1_rotate(v, 5); \
        w = __SHA1_rotate(w, 30);
        
#define __SHA1_R2(v,w,x,y,z,i,block) \
        z += (w ^ x ^ y) + __SHA1_blk(block->buffer32, i) + \
        0x6ED9EBA1 + __SHA1_rotate(v, 5); \
        w = __SHA1_rotate(w, 30);

#define __SHA1_R3(v,w,x,y,z,i,block) \
        z += (((w | x) & y)|(w & x)) + __SHA1_blk(block->buffer32, i) + \
        0x8F1BBCDC + __SHA1_rotate(v, 5); \
        w = __SHA1_rotate(w, 30);
        
#define __SHA1_R4(v,w,x,y,z,i,block) \
        z += (w ^ x ^ y) + __SHA1_blk(block->buffer32, i) + \
        0xCA62C1D6 + __SHA1_rotate(v, 5); \
        w = __SHA1_rotate(w, 30);

void __SHA1_transform(uint32_t state[5], uint8_t buffer[64])
{
    uint32_t a, b, c, d, e;

    typedef union buf_32 {
        uint8_t  buffer8[64];
        uint32_t buffer32[16];
    } buf_32;

    buf_32 *block = (buf_32 *) buffer;

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    __SHA1_R0(a,b,c,d,e, 0, block);   __SHA1_R0(e,a,b,c,d, 1, block);
    __SHA1_R0(d,e,a,b,c, 2, block);   __SHA1_R0(c,d,e,a,b, 3, block);
    __SHA1_R0(b,c,d,e,a, 4, block);   __SHA1_R0(a,b,c,d,e, 5, block);
    __SHA1_R0(e,a,b,c,d, 6, block);   __SHA1_R0(d,e,a,b,c, 7, block);
    __SHA1_R0(c,d,e,a,b, 8, block);   __SHA1_R0(b,c,d,e,a, 9, block);
    __SHA1_R0(a,b,c,d,e,10, block);   __SHA1_R0(e,a,b,c,d,11, block);
    __SHA1_R0(d,e,a,b,c,12, block);   __SHA1_R0(c,d,e,a,b,13, block);
    __SHA1_R0(b,c,d,e,a,14, block);   __SHA1_R0(a,b,c,d,e,15, block);
    __SHA1_R1(e,a,b,c,d,16, block);   __SHA1_R1(d,e,a,b,c,17, block);
    __SHA1_R1(c,d,e,a,b,18, block);   __SHA1_R1(b,c,d,e,a,19, block);

    __SHA1_R2(a,b,c,d,e,20, block);   __SHA1_R2(e,a,b,c,d,21, block);
    __SHA1_R2(d,e,a,b,c,22, block);   __SHA1_R2(c,d,e,a,b,23, block);
    __SHA1_R2(b,c,d,e,a,24, block);   __SHA1_R2(a,b,c,d,e,25, block);
    __SHA1_R2(e,a,b,c,d,26, block);   __SHA1_R2(d,e,a,b,c,27, block);
    __SHA1_R2(c,d,e,a,b,28, block);   __SHA1_R2(b,c,d,e,a,29, block);
    __SHA1_R2(a,b,c,d,e,30, block);   __SHA1_R2(e,a,b,c,d,31, block);
    __SHA1_R2(d,e,a,b,c,32, block);   __SHA1_R2(c,d,e,a,b,33, block);
    __SHA1_R2(b,c,d,e,a,34, block);   __SHA1_R2(a,b,c,d,e,35, block);
    __SHA1_R2(e,a,b,c,d,36, block);   __SHA1_R2(d,e,a,b,c,37, block);
    __SHA1_R2(c,d,e,a,b,38, block);   __SHA1_R2(b,c,d,e,a,39, block);

    __SHA1_R3(a,b,c,d,e,40, block);   __SHA1_R3(e,a,b,c,d,41, block);
    __SHA1_R3(d,e,a,b,c,42, block);   __SHA1_R3(c,d,e,a,b,43, block);
    __SHA1_R3(b,c,d,e,a,44, block);   __SHA1_R3(a,b,c,d,e,45, block);
    __SHA1_R3(e,a,b,c,d,46, block);   __SHA1_R3(d,e,a,b,c,47, block);
    __SHA1_R3(c,d,e,a,b,48, block);   __SHA1_R3(b,c,d,e,a,49, block);
    __SHA1_R3(a,b,c,d,e,50, block);   __SHA1_R3(e,a,b,c,d,51, block);
    __SHA1_R3(d,e,a,b,c,52, block);   __SHA1_R3(c,d,e,a,b,53, block);
    __SHA1_R3(b,c,d,e,a,54, block);   __SHA1_R3(a,b,c,d,e,55, block);
    __SHA1_R3(e,a,b,c,d,56, block);   __SHA1_R3(d,e,a,b,c,57, block);
    __SHA1_R3(c,d,e,a,b,58, block);   __SHA1_R3(b,c,d,e,a,59, block);

    __SHA1_R4(a,b,c,d,e,60, block);   __SHA1_R4(e,a,b,c,d,61, block);
    __SHA1_R4(d,e,a,b,c,62, block);   __SHA1_R4(c,d,e,a,b,63, block);
    __SHA1_R4(b,c,d,e,a,64, block);   __SHA1_R4(a,b,c,d,e,65, block);
    __SHA1_R4(e,a,b,c,d,66, block);   __SHA1_R4(d,e,a,b,c,67, block);
    __SHA1_R4(c,d,e,a,b,68, block);   __SHA1_R4(b,c,d,e,a,69, block);
    __SHA1_R4(a,b,c,d,e,70, block);   __SHA1_R4(e,a,b,c,d,71, block);
    __SHA1_R4(d,e,a,b,c,72, block);   __SHA1_R4(c,d,e,a,b,73, block);
    __SHA1_R4(b,c,d,e,a,74, block);   __SHA1_R4(a,b,c,d,e,75, block);
    __SHA1_R4(e,a,b,c,d,76, block);   __SHA1_R4(d,e,a,b,c,77, block);
    __SHA1_R4(c,d,e,a,b,78, block);   __SHA1_R4(b,c,d,e,a,79, block);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

void __SHA1_init(SHA1_ctx *ctx)
{
    ctx->count[0] = 0;
    ctx->count[1] = 0;

    ctx->computed = 0;

    ctx->H[0] = 0x67452301;
    ctx->H[1] = 0xEFCDAB89;
    ctx->H[2] = 0x98BADCFE;
    ctx->H[3] = 0x10325476;
    ctx->H[4] = 0xC3D2E1F0;    
}

void __SHA1_update(SHA1_ctx *ctx, uint8_t* data, const unsigned int len)
{
    unsigned int i, x;
    
    x = (ctx->count[0] >> 3) & 63;
    if ((ctx->count[0] += len << 3) < (len << 3)) ctx->count[1]++;
    ctx->count[1] += (len >> 29);

    if ((x + len) > 63) {
        memcpy(ctx->buffer + x, data, (i = 64 - x));
        __SHA1_transform(ctx->H, ctx->buffer);
        for (; i + 63 < len; i += 64) {
            __SHA1_transform(ctx->H, &data[i]);
        }
        x = 0;
    } else {
        i = 0;
    }
    
    memcpy(ctx->buffer + x, data + i, len - i);
}

void __SHA1_final(SHA1_ctx* ctx)
{
    uint32_t i;
    uint8_t fncount[8], nullch, padch;

    nullch = '\0';
    padch  = '\200';
    ctx->computed = 1;
    
    for (i = 0; i < 4; i++)
        fncount[i] = (uint8_t) ((ctx->count[1] >>
        ((3 - (i & 3)) * 8)) & 0xFF);

    for (; i < 8; i++)
        fncount[i] = (uint8_t) ((ctx->count[0] >>
        ((3 - (i & 3)) * 8)) & 0xFF);
    
    __SHA1_update(ctx, &padch, 1);
    while ((ctx->count[0] & 504) != 448) {
        __SHA1_update(ctx, &nullch, 1);
    }

    __SHA1_update(ctx, fncount, 8);
}

JSBool
SHA1_toString (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string   = JS_malloc(cx, 41*sizeof(char));
    SHA1_ctx* data = JS_GetPrivate(cx, object);

    __SHA1_toString(data, string);
    
    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, 40));
    return JS_TRUE;
}

void __SHA1_toString(SHA1_ctx *ctx, char out[41])
{
    if (!ctx->computed)
        __SHA1_final(ctx);
    
    sprintf(out,"%X%X%X%X%X",
        (unsigned int) ctx->H[0],
        (unsigned int) ctx->H[1],
        (unsigned int) ctx->H[2],
        (unsigned int) ctx->H[3],
        (unsigned int) ctx->H[4]);
}
