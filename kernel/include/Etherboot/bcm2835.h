//
// bcm2835.h
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef BCM2835_H
#define BCM2835_H

#include <Etherboot/sysconfig.h>

#if RASPPI == 1
#define ARM_IO_BASE		0x20000000
#else
#define ARM_IO_BASE		0x3F000000
#endif

#define GPU_IO_BASE		0x7E000000

#define GPU_CACHED_BASE		0x40000000
#define GPU_UNCACHED_BASE	0xC0000000

#if RASPPI == 1
	#ifdef GPU_L2_CACHE_ENABLED
		#define GPU_MEM_BASE	GPU_CACHED_BASE
	#else
		#define GPU_MEM_BASE	GPU_UNCACHED_BASE
	#endif
#else
	#define GPU_MEM_BASE	GPU_UNCACHED_BASE
#endif

//
// General Purpose I/O
//
#define ARM_GPIO_BASE		(ARM_IO_BASE + 0x200000)

#define ARM_GPIO_GPFSEL0	(ARM_GPIO_BASE + 0x00)
#define ARM_GPIO_GPFSEL1	(ARM_GPIO_BASE + 0x04)
#define ARM_GPIO_GPFSEL4	(ARM_GPIO_BASE + 0x10)
#define ARM_GPIO_GPSET0		(ARM_GPIO_BASE + 0x1C)
#define ARM_GPIO_GPCLR0		(ARM_GPIO_BASE + 0x28)
#define ARM_GPIO_GPLEV0		(ARM_GPIO_BASE + 0x34)
#define ARM_GPIO_GPEDS0		(ARM_GPIO_BASE + 0x40)
#define ARM_GPIO_GPREN0		(ARM_GPIO_BASE + 0x4C)
#define ARM_GPIO_GPFEN0		(ARM_GPIO_BASE + 0x58)
#define ARM_GPIO_GPHEN0		(ARM_GPIO_BASE + 0x64)
#define ARM_GPIO_GPLEN0		(ARM_GPIO_BASE + 0x70)
#define ARM_GPIO_GPAREN0	(ARM_GPIO_BASE + 0x7C)
#define ARM_GPIO_GPAFEN0	(ARM_GPIO_BASE + 0x88)
#define ARM_GPIO_GPPUD		(ARM_GPIO_BASE + 0x94)
#define ARM_GPIO_GPPUDCLK0	(ARM_GPIO_BASE + 0x98)

//
// UART0
//
#define ARM_UART0_BASE		(ARM_IO_BASE + 0x201000)

#define ARM_UART0_DR		(ARM_UART0_BASE + 0x00)
#define ARM_UART0_FR     	(ARM_UART0_BASE + 0x18)
#define ARM_UART0_IBRD   	(ARM_UART0_BASE + 0x24)
#define ARM_UART0_FBRD   	(ARM_UART0_BASE + 0x28)
#define ARM_UART0_LCRH   	(ARM_UART0_BASE + 0x2C)
#define ARM_UART0_CR     	(ARM_UART0_BASE + 0x30)
#define ARM_UART0_IFLS   	(ARM_UART0_BASE + 0x34)
#define ARM_UART0_IMSC   	(ARM_UART0_BASE + 0x38)
#define ARM_UART0_RIS    	(ARM_UART0_BASE + 0x3C)
#define ARM_UART0_MIS    	(ARM_UART0_BASE + 0x40)
#define ARM_UART0_ICR    	(ARM_UART0_BASE + 0x44)

//
// System Timers
//
#define ARM_SYSTIMER_BASE	(ARM_IO_BASE + 0x3000)

#define ARM_SYSTIMER_CS		(ARM_SYSTIMER_BASE + 0x00)
#define ARM_SYSTIMER_CLO	(ARM_SYSTIMER_BASE + 0x04)
#define ARM_SYSTIMER_CHI	(ARM_SYSTIMER_BASE + 0x08)
#define ARM_SYSTIMER_C0		(ARM_SYSTIMER_BASE + 0x0C)
#define ARM_SYSTIMER_C1		(ARM_SYSTIMER_BASE + 0x10)
#define ARM_SYSTIMER_C2		(ARM_SYSTIMER_BASE + 0x14)
#define ARM_SYSTIMER_C3		(ARM_SYSTIMER_BASE + 0x18)

//
// Platform DMA Controller
//
#define ARM_DMA_BASE		(ARM_IO_BASE + 0x7000)

//
// Interrupt Controller
//
#define ARM_IC_BASE		(ARM_IO_BASE + 0xB000)

#define ARM_IC_IRQ_BASIC_PENDING  (ARM_IC_BASE + 0x200)
#define ARM_IC_IRQ_PENDING_1	  (ARM_IC_BASE + 0x204)
#define ARM_IC_IRQ_PENDING_2	  (ARM_IC_BASE + 0x208)
#define ARM_IC_FIQ_CONTROL	  (ARM_IC_BASE + 0x20C)
#define ARM_IC_ENABLE_IRQS_1	  (ARM_IC_BASE + 0x210)
#define ARM_IC_ENABLE_IRQS_2	  (ARM_IC_BASE + 0x214)
#define ARM_IC_ENABLE_BASIC_IRQS  (ARM_IC_BASE + 0x218)
#define ARM_IC_DISABLE_IRQS_1	  (ARM_IC_BASE + 0x21C)
#define ARM_IC_DISABLE_IRQS_2	  (ARM_IC_BASE + 0x220)
#define ARM_IC_DISABLE_BASIC_IRQS (ARM_IC_BASE + 0x224)

//
// ARM Timer
//
#define ARM_TIMER_BASE		(ARM_IO_BASE + 0xB000)

#define ARM_TIMER_LOAD		(ARM_TIMER_BASE + 0x400)
#define ARM_TIMER_VALUE		(ARM_TIMER_BASE + 0x404)
#define ARM_TIMER_CTRL		(ARM_TIMER_BASE + 0x408)
#define ARM_TIMER_IRQCLR	(ARM_TIMER_BASE + 0x40C)
#define ARM_TIMER_RAWIRQ	(ARM_TIMER_BASE + 0x410)
#define ARM_TIMER_MASKIRQ	(ARM_TIMER_BASE + 0x414)
#define ARM_TIMER_RELOAD	(ARM_TIMER_BASE + 0x418)
#define ARM_TIMER_PREDIV	(ARM_TIMER_BASE + 0x41C)
#define ARM_TIMER_CNTR		(ARM_TIMER_BASE + 0x420)

//
// Mailbox
//
#define MAILBOX_BASE		(ARM_IO_BASE + 0xB880)

#define MAILBOX_READ  		MAILBOX_BASE
#define MAILBOX_STATUS 		(MAILBOX_BASE + 0x18)
	#define MAILBOX_STATUS_EMPTY	0x40000000
	#define MAILBOX_STATUS_FULL	0x80000000
#define MAILBOX_WRITE		(MAILBOX_BASE + 0x20)

#define MAILBOX_CHANNEL_PM	0			// power management
#define MAILBOX_CHANNEL_FB 	1			// frame buffer
#define BCM_MAILBOX_PROP_OUT	8			// property tags (ARM to VC)

//
// Pulse Width Modulator
//
#define ARM_PWM_BASE		(ARM_IO_BASE + 0x20C000)

#define ARM_PWM_CTL		(ARM_PWM_BASE + 0x00)
#define ARM_PWM_STA		(ARM_PWM_BASE + 0x04)
#define ARM_PWM_DMAC		(ARM_PWM_BASE + 0x08)
#define ARM_PWM_RNG1		(ARM_PWM_BASE + 0x10)
#define ARM_PWM_DAT1		(ARM_PWM_BASE + 0x14)
#define ARM_PWM_FIF1		(ARM_PWM_BASE + 0x18)
#define ARM_PWM_RNG2		(ARM_PWM_BASE + 0x20)
#define ARM_PWM_DAT2		(ARM_PWM_BASE + 0x24)

//
// Clock Manager
//
#define ARM_CM_BASE		(ARM_IO_BASE + 0x101000)

#define ARM_CM_GP0CTL		(ARM_CM_BASE + 0x70)
#define ARM_CM_GP0DIV		(ARM_CM_BASE + 0x74)

#define ARM_CM_PASSWD 		(0x5A << 24)

//
// USB Host Controller
//
#define ARM_USB_BASE		(ARM_IO_BASE + 0x980000)

#define ARM_USB_CORE_BASE	ARM_USB_BASE
#define ARM_USB_HOST_BASE	(ARM_USB_BASE + 0x400)
#define ARM_USB_POWER		(ARM_USB_BASE + 0xE00)

//
// External Mass Media Controller (SD Card)
//
#define ARM_EMMC_BASE		(ARM_IO_BASE + 0x300000)

//
// Power Manager (?)
//
#define ARM_PM_BASE		(ARM_IO_BASE + 0x100000)

#define ARM_PM_RSTC		(ARM_PM_BASE + 0x1C)
#define ARM_PM_WDOG		(ARM_PM_BASE + 0x24)

#define ARM_PM_PASSWD		(0x5A << 24)

// Interrupts

#define ARM_IRQS_PER_REG	32

#define ARM_IRQ1_BASE		0
#define ARM_IRQ2_BASE		(ARM_IRQ1_BASE + ARM_IRQS_PER_REG)
#define ARM_IRQBASIC_BASE	(ARM_IRQ2_BASE + ARM_IRQS_PER_REG)

#define ARM_IRQ_TIMER0		(ARM_IRQ1_BASE + 0)
#define ARM_IRQ_TIMER1		(ARM_IRQ1_BASE + 1)
#define ARM_IRQ_TIMER2		(ARM_IRQ1_BASE + 2)
#define ARM_IRQ_TIMER3		(ARM_IRQ1_BASE + 3)
#define ARM_IRQ_CODEC0		(ARM_IRQ1_BASE + 4)
#define ARM_IRQ_CODEC1		(ARM_IRQ1_BASE + 5)
#define ARM_IRQ_CODEC2		(ARM_IRQ1_BASE + 6)
#define ARM_IRQ_JPEG		(ARM_IRQ1_BASE + 7)
#define ARM_IRQ_ISP		(ARM_IRQ1_BASE + 8)
#define ARM_IRQ_USB		(ARM_IRQ1_BASE + 9)
#define ARM_IRQ_3D		(ARM_IRQ1_BASE + 10)
#define ARM_IRQ_TRANSPOSER	(ARM_IRQ1_BASE + 11)
#define ARM_IRQ_MULTICORESYNC0	(ARM_IRQ1_BASE + 12)
#define ARM_IRQ_MULTICORESYNC1	(ARM_IRQ1_BASE + 13)
#define ARM_IRQ_MULTICORESYNC2	(ARM_IRQ1_BASE + 14)
#define ARM_IRQ_MULTICORESYNC3	(ARM_IRQ1_BASE + 15)
#define ARM_IRQ_DMA0		(ARM_IRQ1_BASE + 16)
#define ARM_IRQ_DMA1		(ARM_IRQ1_BASE + 17)
#define ARM_IRQ_DMA2		(ARM_IRQ1_BASE + 18)
#define ARM_IRQ_DMA3		(ARM_IRQ1_BASE + 19)
#define ARM_IRQ_DMA4		(ARM_IRQ1_BASE + 20)
#define ARM_IRQ_DMA5		(ARM_IRQ1_BASE + 21)
#define ARM_IRQ_DMA6		(ARM_IRQ1_BASE + 22)
#define ARM_IRQ_DMA7		(ARM_IRQ1_BASE + 23)
#define ARM_IRQ_DMA8		(ARM_IRQ1_BASE + 24)
#define ARM_IRQ_DMA9		(ARM_IRQ1_BASE + 25)
#define ARM_IRQ_DMA10		(ARM_IRQ1_BASE + 26)
#define ARM_IRQ_DMA11		(ARM_IRQ1_BASE + 27)
#define ARM_IRQ_DMA12		(ARM_IRQ1_BASE + 28)
#define ARM_IRQ_AUX		(ARM_IRQ1_BASE + 29)
#define ARM_IRQ_ARM		(ARM_IRQ1_BASE + 30)
#define ARM_IRQ_VPUDMA		(ARM_IRQ1_BASE + 31)

#define ARM_IRQ_HOSTPORT	(ARM_IRQ2_BASE + 0)
#define ARM_IRQ_VIDEOSCALER	(ARM_IRQ2_BASE + 1)
#define ARM_IRQ_CCP2TX		(ARM_IRQ2_BASE + 2)
#define ARM_IRQ_SDC		(ARM_IRQ2_BASE + 3)
#define ARM_IRQ_DSI0		(ARM_IRQ2_BASE + 4)
#define ARM_IRQ_AVE		(ARM_IRQ2_BASE + 5)
#define ARM_IRQ_CAM0		(ARM_IRQ2_BASE + 6)
#define ARM_IRQ_CAM1		(ARM_IRQ2_BASE + 7)
#define ARM_IRQ_HDMI0		(ARM_IRQ2_BASE + 8)
#define ARM_IRQ_HDMI1		(ARM_IRQ2_BASE + 9)
#define ARM_IRQ_PIXELVALVE1	(ARM_IRQ2_BASE + 10)
#define ARM_IRQ_I2CSPISLV	(ARM_IRQ2_BASE + 11)
#define ARM_IRQ_DSI1		(ARM_IRQ2_BASE + 12)
#define ARM_IRQ_PWA0		(ARM_IRQ2_BASE + 13)
#define ARM_IRQ_PWA1		(ARM_IRQ2_BASE + 14)
#define ARM_IRQ_CPR		(ARM_IRQ2_BASE + 15)
#define ARM_IRQ_SMI		(ARM_IRQ2_BASE + 16)
#define ARM_IRQ_GPIO0		(ARM_IRQ2_BASE + 17)
#define ARM_IRQ_GPIO1		(ARM_IRQ2_BASE + 18)
#define ARM_IRQ_GPIO2		(ARM_IRQ2_BASE + 19)
#define ARM_IRQ_GPIO3		(ARM_IRQ2_BASE + 20)
#define ARM_IRQ_I2C		(ARM_IRQ2_BASE + 21)
#define ARM_IRQ_SPI		(ARM_IRQ2_BASE + 22)
#define ARM_IRQ_I2SPCM		(ARM_IRQ2_BASE + 23)
#define ARM_IRQ_SDIO		(ARM_IRQ2_BASE + 24)
#define ARM_IRQ_UART		(ARM_IRQ2_BASE + 25)
#define ARM_IRQ_SLIMBUS		(ARM_IRQ2_BASE + 26)
#define ARM_IRQ_VEC		(ARM_IRQ2_BASE + 27)
#define ARM_IRQ_CPG		(ARM_IRQ2_BASE + 28)
#define ARM_IRQ_RNG		(ARM_IRQ2_BASE + 29)
#define ARM_IRQ_ARASANSDIO	(ARM_IRQ2_BASE + 30)
#define ARM_IRQ_AVSPMON		(ARM_IRQ2_BASE + 31)

#define ARM_IRQ_ARM_TIMER	(ARM_IRQBASIC_BASE + 0)
#define ARM_IRQ_ARM_MAILBOX	(ARM_IRQBASIC_BASE + 1)
#define ARM_IRQ_ARM_DOORBELL_0	(ARM_IRQBASIC_BASE + 2)
#define ARM_IRQ_ARM_DOORBELL_1	(ARM_IRQBASIC_BASE + 3)
#define ARM_IRQ_VPU0_HALTED	(ARM_IRQBASIC_BASE + 4)
#define ARM_IRQ_VPU1_HALTED	(ARM_IRQBASIC_BASE + 5)
#define ARM_IRQ_ILLEGAL_TYPE0	(ARM_IRQBASIC_BASE + 6)
#define ARM_IRQ_ILLEGAL_TYPE1	(ARM_IRQBASIC_BASE + 7)

#define IRQ_LINES		(ARM_IRQS_PER_REG * 2 + 8)

#endif
