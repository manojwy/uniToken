#include "fts3_unicode.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
** The following two macros - READ_UTF8 and WRITE_UTF8 - have been copied
** from the sqlite3 source file utf.c. If this file is compiled as part
** of the amalgamation, they are not required.
*/
static const unsigned char sqlite3Utf8Trans1[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x00, 0x00,
};

#define READ_UTF8(zIn, zTerm, c)                           \
  c = *(zIn++);                                            \
  if( c>=0xc0 ){                                           \
    c = sqlite3Utf8Trans1[c-0xc0];                         \
    while( zIn!=zTerm && (*zIn & 0xc0)==0x80 ){            \
      c = (c<<6) + (0x3f & *(zIn++));                      \
    }                                                      \
    if( c<0x80                                             \
        || (c&0xFFFFF800)==0xD800                          \
        || (c&0xFFFFFFFE)==0xFFFE ){  c = 0xFFFD; }        \
  }

#define WRITE_UTF8(zOut, c) {                          \
  if( c<0x00080 ){                                     \
    *zOut++ = (u8)(c&0xFF);                            \
  }                                                    \
  else if( c<0x00800 ){                                \
    *zOut++ = 0xC0 + (u8)((c>>6)&0x1F);                \
    *zOut++ = 0x80 + (u8)(c & 0x3F);                   \
  }                                                    \
  else if( c<0x10000 ){                                \
    *zOut++ = 0xE0 + (u8)((c>>12)&0x0F);               \
    *zOut++ = 0x80 + (u8)((c>>6) & 0x3F);              \
    *zOut++ = 0x80 + (u8)(c & 0x3F);                   \
  }else{                                               \
    *zOut++ = 0xF0 + (u8)((c>>18) & 0x07);             \
    *zOut++ = 0x80 + (u8)((c>>12) & 0x3F);             \
    *zOut++ = 0x80 + (u8)((c>>6) & 0x3F);              \
    *zOut++ = 0x80 + (u8)(c & 0x3F);                   \
  }                                                    \
}

/*
** Return true if, for the purposes of tokenization, codepoint iCode is
** considered a token character (not a separator).
*/
static int unicodeIsAlnum(
//                          unicode_tokenizer *p,
                          int iCode){
  assert( (sqlite3FtsUnicodeIsalnum(iCode) & 0xFFFFFFFE)==0 );
    return sqlite3FtsUnicodeIsalnum(iCode) ^ 0;
}


int unicodeTokens(char* utf){
    
    char str[1000];
    strcpy(str, utf);
    
    char token[1000] = {0};
    char tkn[1000] = {0};
    
//    printf("STR: %s\n", str);
    int iCode;
    char *zOut;
    const unsigned char *z = (const unsigned char *)&str[0];
    
    
    while(z) {
    
        memset(token, '\0', 1000);
    const unsigned char *zStart = z;
    const unsigned char *zEnd;
    const unsigned char *zTerm = (const unsigned char *)&str[strlen(str)];
    
    /* Scan past any delimiter characters before the start of the next token.
     ** Return SQLITE_DONE early if this takes us all the way to the end of
     ** the input.  */
    while( z<zTerm ){
        READ_UTF8(z, zTerm, iCode);
        if( unicodeIsAlnum(iCode) ) break;
        zStart = z;
    }
    if( zStart>=zTerm ) return SQLITE_DONE;
    
    zOut = &token[0];
    do {
        int iOut;
        /* Write the folded case of the last character read to the output */
        zEnd = z;
        iOut = sqlite3FtsUnicodeFold(iCode, 1);
        if( iOut ){
            WRITE_UTF8(zOut, iOut);
        }
        
//        printf("zOut: %s, z: %s\n", zOut, z);
        
        /* If the cursor is not at EOF, read the next character */
        if( z>=zTerm ) break;
        READ_UTF8(z, zTerm, iCode);
        
        //printf(": [%s]\n", token);
    }while( unicodeIsAlnum(iCode)
           || sqlite3FtsUnicodeIsdiacritic(iCode)
           );
        printf("TOKEN: [%s]\n", token);
    }
      return SQLITE_OK;
}
