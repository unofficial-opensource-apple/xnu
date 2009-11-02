/*
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */
/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 */

#include <mach/vm_param.h>
#include <vm/vm_kern.h>
#include <vm/vm_map.h>
#include <i386/pmap.h>
#include <i386/io_map_entries.h>

extern vm_offset_t	virtual_avail;

/*
 * Allocate and map memory for devices that may need to be mapped before
 * Mach VM is running.
 */
vm_offset_t
io_map(phys_addr, size)
	vm_offset_t	phys_addr;
	vm_size_t	size;
{
	vm_offset_t	start;

	if (kernel_map == VM_MAP_NULL) {
	    /*
	     * VM is not initialized.  Grab memory.
	     */
	    start = virtual_avail;
	    virtual_avail += round_page(size);

	    (void) pmap_map_bd(start, phys_addr, phys_addr + round_page(size),
			       VM_PROT_READ|VM_PROT_WRITE);
	}
	else {
	    (void) kmem_alloc_pageable(kernel_map, &start, round_page(size));
	    (void) pmap_map(start, phys_addr, phys_addr + round_page(size),
			    VM_PROT_READ|VM_PROT_WRITE);
	}
	return (start);
}

/* just wrap this since io_map handles it */

vm_offset_t io_map_spec(vm_offset_t phys_addr, vm_size_t size)
{
  return (io_map(phys_addr, size));
}
