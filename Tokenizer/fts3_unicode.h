#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_NOMEM 2
#define SQLITE_DONE 3


#define UNUSED_PARAMETER(x) (void)(x)


typedef unsigned char u8;         /* 1-byte (or larger) unsigned integer */
typedef short int i16;            /* 2-byte (or larger) signed integer */
typedef unsigned int u32;         /* 4-byte unsigned integer */




int sqlite3FtsUnicodeIsalnum(int c);
int sqlite3FtsUnicodeIsdiacritic(int c);
int sqlite3FtsUnicodeFold(int c, int bRemoveDiacritic);

