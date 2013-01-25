#ifndef TDB_BREAKPOINT_H_
#define TDB_BREAKPOINT_H_

#define TDB_BREAKPOINT_LABEL 	0x1
#define TDB_BREAKPOINT_ADDRESS 	0x2

typedef struct tdb_breakpoint_s
{
	int address;
	char *label;

	int type;

} tdb_breakpoint_t;

#endif
