#ifndef TDB_BREAKPOINT_H_
#define TDB_BREAKPOINT_H_

#define TDB_BREAKPOINT_LABEL 	0x1
#define TDB_BREAKPOINT_ADDRESS 	0x2

struct tdb_breakpoint
{
	int address;
	char *label;

	int type;

};

#endif
