#ifndef DEFAULT_H
#define DEFAULT_H

//Default header, it gets included in every soruce file that is compiled

#include <stddef.h>
#include <stdint.h>
#include <Etherboot/itoa.h>

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef int		boolean;
#define FALSE		0
#define TRUE		1

#endif
