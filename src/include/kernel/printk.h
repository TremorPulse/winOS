#pragma once

int printk(int level, const char* __restrict, ...);
void panic(const char* msg);