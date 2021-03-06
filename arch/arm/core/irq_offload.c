/*
 * Copyright (c) 2015 Intel corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Software interrupts utility code - ARM implementation
 */

#include <kernel.h>
#include <irq_offload.h>

volatile irq_offload_routine_t offload_routine;
static void *offload_param;

/* Called by __svc */
void _irq_do_offload(void)
{
	offload_routine(offload_param);
}

void irq_offload(irq_offload_routine_t routine, void *parameter)
{
	int key;

	key = irq_lock();
	offload_routine = routine;
	offload_param = parameter;

	__asm__ volatile ("svc %[id]"
			  :
			  : [id] "i" (_SVC_CALL_IRQ_OFFLOAD)
			  : "memory");

	offload_routine = NULL;

	irq_unlock(key);
}
