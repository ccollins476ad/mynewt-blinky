/**
 * Copyright (c) 2015 Runtime Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <console/console.h>
#include <hal/hal_system.h>
#include "os/os.h"

#include <stdint.h>
#include <unistd.h>

int             os_die_line;
const char     *os_die_module;

void __assert_func(const char *file, int line, const char *func, const char *e);

void
__assert_func(const char *file, int line, const char *func, const char *e)
{
    int sr;

    OS_ENTER_CRITICAL(sr);
    (void)sr;
    os_die_line = line;
    os_die_module = file;
    console_blocking_mode();
    console_printf("Assert %s; failed in %s:%d\n", e ? e : "", file, line);
    system_reset();
}

struct exception_frame {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
};

struct trap_frame {
    struct exception_frame *ef;
    uint32_t sp;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
};

void
os_default_irq(struct trap_frame *tf)
{
    console_blocking_mode();
    console_printf("Unhandled interrupt (%d), exception sp 0x%08x\n",
      SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk, (uint32_t)tf->ef);
    console_printf(" r0:0x%08x  r1:0x%08x  r2:0x%08x  r3:0x%08x\n",
      tf->ef->r0, tf->ef->r1, tf->ef->r2, tf->ef->r3);
    console_printf(" r4:0x%08x  r5:0x%08x  r6:0x%08x  r7:0x%08x\n",
      tf->r4, tf->r5, tf->r6, tf->r7);
    console_printf(" r8:0x%08x  r9:0x%08x r10:0x%08x r11:0x%08x\n",
      tf->r8, tf->r9, tf->r10, tf->r11);
    console_printf("r12:0x%08x  lr:0x%08x  pc:0x%08x psr:0x%08x\n",
      tf->ef->r12, tf->ef->lr, tf->ef->pc, tf->ef->psr);
    console_printf("ICSR:0x%08x HFSR:0x%08x CFSR:0x%08x\n",
      SCB->ICSR, SCB->HFSR, SCB->CFSR);
    console_printf("BFAR:0x%08x MMFAR:0x%08x\n", SCB->BFAR, SCB->MMFAR);
    system_reset();
}
