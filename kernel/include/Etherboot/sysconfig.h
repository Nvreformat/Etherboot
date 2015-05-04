#ifndef SYSCONFIG_H
#define SYSCONFIG_H

// memory addresses and sizes
#define MEGABYTE		0x100000

#define MEM_SIZE		(256 * MEGABYTE)		// default size
#define GPU_MEM_SIZE		(64 * MEGABYTE)			// set in config.txt
#define ARM_MEM_SIZE		(MEM_SIZE - GPU_MEM_SIZE)	// normally overwritten

#define PAGE_SIZE		4096				// page size used by us

#define KERNEL_MAX_SIZE		(16 * MEGABYTE)			// all sizes must be a multiple of 16K
#define KERNEL_STACK_SIZE	0x20000
#define EXCEPTION_STACK_SIZE	0x8000
#define PAGE_TABLE1_SIZE	0x4000

#define MEM_KERNEL_START	0x8000
#define MEM_KERNEL_END		(MEM_KERNEL_START + KERNEL_MAX_SIZE)
#define MEM_KERNEL_STACK	(MEM_KERNEL_END + KERNEL_STACK_SIZE)		// expands down
#define MEM_ABORT_STACK		(MEM_KERNEL_STACK + EXCEPTION_STACK_SIZE)	// expands down
#define MEM_IRQ_STACK		(MEM_ABORT_STACK + EXCEPTION_STACK_SIZE)	// expands down
#define MEM_PAGE_TABLE1		MEM_IRQ_STACK				// must be 16K aligned

#define MEM_HEAP_START		0x4000000

#if RASPPI == 1
#define ARM_STRICT_ALIGNMENT
#define GPU_L2_CACHE_ENABLED
#endif

#define KERNEL_TIMERS		20

#endif
