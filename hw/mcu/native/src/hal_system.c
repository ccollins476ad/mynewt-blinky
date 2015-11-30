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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "hal/hal_system.h"
#include "mcu/mcu_sim.h"

void
system_reset(void)
{
    while(1);
}

/*
 * When running projects within simulator, it's useful to be able to
 * pass operating info as parameters to simulator program.
 * This routine is meant to parse those.
 */
static void
usage(char *progname, int rc)
{
    const char msg[] =
      "Usage: %s [-f flash_file]\n"
      "     -f flash_file tells where binary flash file is located. It gets\n"
      "        created if it doesn't already exist.\n";

    write(2, msg, strlen(msg));
    exit(rc);
}

void
mcu_sim_parse_args(int argc, char **argv)
{
    int ch;
    char *progname = argv[0];

    while ((ch = getopt(argc, argv, "hf:")) != -1) {
        switch (ch) {
        case 'f':
            native_flash_file = optarg;
            break;
        case 'h':
            usage(progname, 0);
            break;
        default:
            usage(progname, -1);
            break;
        }
    }
}
