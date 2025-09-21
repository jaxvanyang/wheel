#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <wheel.h>

// Return false if failed, true otherwise.
bool make_tests() {
	pid_t pid = fork();
	switch (pid) {
	case -1:
		perror("failed to create child process");
		return false;
	case 0:
		execlp("make", "tests", NULL);
	}

	int status;
	if (waitpid(pid, &status, 0) == -1) {
		perror("wait failed");
		return false;
	}

	if (!WIFEXITED(status)) {
		error("expected child process exited");
	}

	return WEXITSTATUS(status) == 0;
}

char *find_test_by_pid(const Slist *tests, const pid_t *pids, pid_t pid) {
	for (usize i = 0; i < tests->length; ++i) {
		if (pids[i] == pid) {
			return slist_get(tests, i)->data;
		}
	}

	return NULL;
}

int main(int argc, char *const argv[]) {
	if (!make_tests()) {
		printf("make tests failed\n");
		return EXIT_FAILURE;
	}

	usize failed_cnt = 0, passed_cnt = 0;
	Slist *tests = slist_new();

	if (argc == 1) {
		DIR *dir = opendir("tests");

		if (dir == NULL) {
			perror("failed to open tests/");
			return EXIT_FAILURE;
		}

		while (true) {
			struct dirent *entry = readdir(dir);
			if (entry == NULL) {
				break;
			}

			if (entry->d_type == DT_REG && str_end_with(entry->d_name, ".c")) {
				str_remove_postfix(entry->d_name, ".c");
				slist_push(tests, str_from(entry->d_name));
			}
		}
	} else {
		for (int i = 1; i < argc; ++i) {
			slist_push(tests, str_from(argv[i]));
		}
	}

	pid_t *pids = malloc(sizeof(pid_t) * tests->length);

	for (usize i = 0; i < tests->length; ++i) {
		char *test = slist_get(tests, i)->data;
		printf("Testing %s...\n", test);

		pid_t pid = fork();

		if (pid == -1) {
			perror("failed to create child process");
			return EXIT_FAILURE;
		} else if (pid == 0) {
			Str *path = str_from("tests");
			path_join(path, test);
			printf("path: %s\n", path->data);
			if (execv(path->data, NULL) == -1) {
				perror("failed to run the test");
				return EXIT_FAILURE;
			}
		}

		pids[i] = pid;
	}

	for (usize i = 0; i < tests->length; ++i) {
		int status;
		pid_t pid = waitpid(-1, &status, WUNTRACED);
		if (pid == -1) {
			perror("wait failed");
			return EXIT_FAILURE;
		}

		char *test = find_test_by_pid(tests, pids, pid);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			++passed_cnt;
			printf("%s succeed\n", test);
		} else {
			++failed_cnt;
			printf("%s failed\n", test);
		}

		printf("passed/failed/total: %zu/%zu/%zu\n", passed_cnt, failed_cnt, tests->length);
	}

	FREE(pids);
	slist_free(tests);

	if (failed_cnt) {
		return EXIT_FAILURE;
	}
}
