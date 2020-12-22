/*
* Author: Ryan Cox
* 
* Copyright (C) 2020, Brigham Young University
* 
* In case you want to use this really boring code that just calls execve, here you go:
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
* to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*
* Compile with: gcc -fPIC -shared -o create_uvn{,.c}
*
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <slurm/spank.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <fcntl.h>

#define SCRIPT_PATH "/usr/local/sbin/create_uvn"  /* should make this a parameter */

SPANK_PLUGIN(create_uvn, 1);

int slurm_spank_init (spank_t sp, int argc, char **argv) {
	pid_t child;
	int status;
	uid_t uid;
	u_int32_t step_id;
	spank_err_t rc;

	/* only work on the remote side */
	if(!spank_remote(sp))
		return 0;

	/* only work on the "remote" (meaning slurmstepd) context */
	if(spank_context() != S_CTX_REMOTE)
		return 0;

	rc = spank_get_item (sp, S_JOB_STEPID, &step_id);

	/* fork and execve a script then wait for its completion */
	if(fork()) {
		waitpid(-1, &status, 0);
	} else {
		char uid_str[22];
		rc = spank_get_item (sp, S_JOB_UID, &uid);
		snprintf(uid_str, 36, "SLURM_JOB_UID=%d", uid);

		slurm_info("create_uvn: slurm_spank_init: running for uid=%d", uid);
		
		char *exec_argv[] = { SCRIPT_PATH, NULL };
		char *exec_env[] = { uid_str, NULL };

		/* close fds or problems will occur */
		close(2);
		close(1);
		close(0);
		open("/dev/null", O_RDONLY); /* stdin */

		execve(SCRIPT_PATH, exec_argv, exec_env);

		/* execve failed. uh oh */
		slurm_error("create_uvn: slurm_spank_init: execve failed: %m");
		return -1;
	}

	slurm_info("create_uvn: slurm_spank_init: done");
	return 0;
}
