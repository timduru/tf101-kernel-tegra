/*
 * include/linux/nvmap.h
 *
 * structure declarations for nvmem and nvmap user-space ioctls
 *
 * Copyright (c) 2009-2012, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/atomic.h>
//#include <mach/nvmap.h>

#include <linux/ioctl.h>
#include <linux/file.h>
#include <linux/rbtree.h>

#if !defined(__KERNEL__)
#define __user
#endif

#ifndef _LINUX_NVMAP_H
#define _LINUX_NVMAP_H

#define NVMAP_HEAP_SYSMEM  (1ul<<31)
#define NVMAP_HEAP_IOVMM   (1ul<<30)

/* common carveout heaps */
#define NVMAP_HEAP_CARVEOUT_IRAM    (1ul<<29)
#define NVMAP_HEAP_CARVEOUT_VPR     (1ul<<28)
#define NVMAP_HEAP_CARVEOUT_GENERIC (1ul<<0)

#define NVMAP_HEAP_CARVEOUT_MASK    (NVMAP_HEAP_IOVMM - 1)

/* allocation flags */
#define NVMAP_HANDLE_UNCACHEABLE     (0x0ul << 0)
#define NVMAP_HANDLE_WRITE_COMBINE   (0x1ul << 0)
#define NVMAP_HANDLE_INNER_CACHEABLE (0x2ul << 0)
#define NVMAP_HANDLE_CACHEABLE       (0x3ul << 0)
#define NVMAP_HANDLE_CACHE_FLAG      (0x3ul << 0)

#define NVMAP_HANDLE_SECURE          (0x1ul << 2)

#if defined(__KERNEL__)

#if defined(CONFIG_TEGRA_NVMAP)
struct nvmap_handle;
struct nvmap_client;
struct nvmap_device;

#define nvmap_ref_to_id(_ref)		((unsigned long)(_ref)->handle)

#define nvmap_ref_to_handle(_ref) (*(struct nvmap_handle **)(_ref))
/* Convert User space handle to Kernel. */
#define nvmap_convert_handle_u2k(h) (h)

/* handle_ref objects are client-local references to an nvmap_handle;
 * they are distinct objects so that handles can be unpinned and
 * unreferenced the correct number of times when a client abnormally
 * terminates */
struct nvmap_handle_ref {
	struct nvmap_handle *handle;
	struct rb_node	node;
	atomic_t	dupes;	/* number of times to free on file close */
	atomic_t	pin;	/* number of times to unpin on free */
};

#elif defined(CONFIG_ION_TEGRA)
/* For Ion Mem Manager support through nvmap_* API's. */
#include "../../../../../drivers/gpu/ion/ion_priv.h"
#define nvmap_client ion_client
#define nvmap_device ion_device
#define nvmap_handle ion_handle
#define nvmap_handle_ref ion_handle
#define nvmap_ref_to_handle(_ref) (struct ion_handle *)_ref
/* Convert User space handle to Kernel. */
#define nvmap_convert_handle_u2k(h) ({ \
	if ((u32)h >= TASK_SIZE) { \
		pr_err("Invalid user space handle."); \
		BUG(); \
	} \
	(*((u32 *)h)); })

#endif /* CONFIG_ION_TEGRA */

#define nvmap_id_to_handle(_id) ((struct nvmap_handle *)(_id))

struct nvmap_pinarray_elem {
	__u32 patch_mem;
	__u32 patch_offset;
	__u32 pin_mem;
	__u32 pin_offset;
	__u32 reloc_shift;
};

struct nvmap_client *nvmap_create_client(struct nvmap_device *dev,
					 const char *name);

struct nvmap_handle_ref *nvmap_alloc(struct nvmap_client *client, size_t size,
				     size_t align, unsigned int flags);

/* struct nvmap_handle_ref *nvmap_alloc(struct nvmap_client *client, size_t size,
				     size_t align, unsigned int flags,
				     unsigned int heap_mask); */ // Compilation fix

void nvmap_free(struct nvmap_client *client, struct nvmap_handle_ref *r);

void *nvmap_mmap(struct nvmap_handle_ref *r);

void nvmap_munmap(struct nvmap_handle_ref *r, void *addr);

struct nvmap_client *nvmap_client_get_file(int fd);

struct nvmap_client *nvmap_client_get(struct nvmap_client *client);

void nvmap_client_put(struct nvmap_client *c);

phys_addr_t nvmap_pin(struct nvmap_client *c, struct nvmap_handle_ref *r);

phys_addr_t nvmap_handle_address(struct nvmap_client *c, unsigned long id);

void nvmap_unpin(struct nvmap_client *client, struct nvmap_handle_ref *r);

int nvmap_pin_array(struct nvmap_client *client, struct nvmap_handle *gather,
		    const struct nvmap_pinarray_elem *arr, int nr,
		    struct nvmap_handle **unique);

void nvmap_unpin_handles(struct nvmap_client *client,
			 struct nvmap_handle **h, int nr);

int nvmap_patch_word(struct nvmap_client *client,
		     struct nvmap_handle *patch,
		     u32 patch_offset, u32 patch_value);

struct nvmap_handle_ref *nvmap_duplicate_handle_id(struct nvmap_client *client,
						   unsigned long id);

struct nvmap_platform_carveout {
	const char *name;
	unsigned int usage_mask;
	phys_addr_t base;
	size_t size;
	size_t buddy_size;
};

struct nvmap_platform_data {
	const struct nvmap_platform_carveout *carveouts;
	unsigned int nr_carveouts;
};

/**********************************************************************************/

struct nvmap_handle_ref *nvmap_alloc_iovm(struct nvmap_client *client,
	size_t size, size_t align, unsigned int flags, unsigned int iova_start);

struct nvmap_handle_ref *nvmap_create_handle(struct nvmap_client *client,
					     size_t size);

struct nvmap_handle *nvmap_get_handle_id(struct nvmap_client *client,
					 unsigned long id);

int nvmap_alloc_handle_id(struct nvmap_client *client,
			  unsigned long id, unsigned int heap_mask,
			  size_t align, unsigned int flags);

void _nvmap_handle_free(struct nvmap_handle *h);

static inline void nvmap_handle_put(struct nvmap_handle *h)
{
	int cnt = atomic_dec_return(&h->ref);

	if (WARN_ON(cnt < 0)) {
		pr_err("%s: %s put to negative references\n",
			__func__, current->comm);
	} else if (cnt == 0)
		_nvmap_handle_free(h);
}

void nvmap_free_handle_id(struct nvmap_client *c, unsigned long id);

int nvmap_pin_ids(struct nvmap_client *client,
		  unsigned int nr, const unsigned long *ids);

void nvmap_unpin_ids(struct nvmap_client *priv,
		     unsigned int nr, const unsigned long *ids);

/**********************************************************************************/

extern struct nvmap_device *nvmap_dev;

#endif /* __KERNEL__ */

#endif /* _LINUX_NVMAP_H */
