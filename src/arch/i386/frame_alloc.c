#include <stdint.h>
#include <kernel/paging.h>
#include <kernel/frame_alloc.h>

#define FREE 0
#define USED 1

uint32_t* frame_map;
uint32_t npages;
uint32_t startframe;
uint32_t endkernel;

#define PREFRAME_COUNT 20
static pageframe_t pre_frames[PREFRAME_COUNT];

static pageframe_t kalloc_frame_int()
{
    uint32_t i = 0;
    while (frame_map[i] != FREE) {
        i++;
        if (i == npages) {
            pageframe_t error_frame = {0}; // Not present
            return error_frame;
        }
    }
    frame_map[i] = USED;

    pageframe_t frame = {0};
    frame.present = 1;
    frame.rw = 1;
    frame.user = 0;
    frame.frame_addr = (startframe + (i * 0x1000)) >> 12;
    return frame;
}

pageframe_t kalloc_frame()
{
    static uint8_t allocate = 1;
    static uint8_t pframe = 0;
    pageframe_t ret;

    if (pframe == PREFRAME_COUNT) {
        allocate = 1;
    }

    if (allocate == 1) {
        for (int i = 0; i < PREFRAME_COUNT; i++) {
            pre_frames[i] = kalloc_frame_int();
        }
        pframe = 0;
        allocate = 0;
    }
    ret = pre_frames[pframe];
    pframe++;
    return ret;
}

void kfree_frame(pageframe_t a)
{
    // Extract the physical address from the struct
    uint32_t addr = a.frame_addr << 12;
    uint32_t index = (addr - startframe) / 0x1000;
    if (index < npages) {
        frame_map[index] = FREE;
    }
}