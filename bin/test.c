#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>
#define F_OK 0
#else
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
#include <string.h>
#include <wheel.h>

// Return false if failed, true otherwise.
bool make_tests() {
#ifdef _WIN32
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD exitCode;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Create the child process for "make tests"
	if (!CreateProcess(NULL, "make tests", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		fprintf(stderr, "CreateProcess failed (%lu)\n", GetLastError());
		return false;
	}

	// Wait until child process exits
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Get the exit code
	if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
		fprintf(stderr, "GetExitCodeProcess failed (%lu)\n", GetLastError());
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return false;
	}

	// Close process and thread handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return exitCode == 0;
#else
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
#endif
}

#ifndef _WIN32
char *find_test_by_pid(const Slist *tests, const pid_t *pids, pid_t pid) {
	for (usize i = 0; i < tests->length; ++i) {
		if (pids[i] == pid) {
			return slist_get(tests, i)->data;
		}
	}

	return NULL;
}
#endif

int main(int argc, char *const argv[]) {
	if (!make_tests()) {
		printf("make tests failed\n");
		return EXIT_FAILURE;
	}

	usize failed_cnt = 0, passed_cnt = 0;
	Slist *tests = slist_new();

	if (argc == 1) {
#ifdef _WIN32
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile("tests\\*.c", &findFileData);

		if (hFind == INVALID_HANDLE_VALUE) {
			fprintf(stderr, "failed to open tests/\n");
			return EXIT_FAILURE;
		}

		do {
			char *filename = findFileData.cFileName;
			if (str_end_with(filename, ".c")) {
				str_remove_postfix(filename, ".c");
				slist_push(tests, str_from(filename));
			}
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
#else
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

		closedir(dir);
#endif
	} else {
		for (int i = 1; i < argc; ++i) {
			slist_push(tests, str_from(argv[i]));
		}
	}

#ifdef _WIN32
	HANDLE *handles = malloc(sizeof(HANDLE) * tests->length);
	STARTUPINFO *si_array = malloc(sizeof(STARTUPINFO) * tests->length);
	PROCESS_INFORMATION *pi_array = malloc(sizeof(PROCESS_INFORMATION) * tests->length);

	for (usize i = 0; i < tests->length; ++i) {
		char *test = slist_get(tests, i)->data;
		printf("Testing %s...\n", test);

		Str *path = str_from("tests\\");
		str_push_str(path, test);
		str_push_str(path, ".exe");

		ZeroMemory(&si_array[i], sizeof(STARTUPINFO));
		si_array[i].cb = sizeof(STARTUPINFO);
		ZeroMemory(&pi_array[i], sizeof(PROCESS_INFORMATION));

		if (!CreateProcess(
					path->data, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si_array[i], &pi_array[i]
				)) {
			fprintf(stderr, "CreateProcess failed for %s (%lu)\n", test, GetLastError());
			str_free(path);
			return EXIT_FAILURE;
		}

		handles[i] = pi_array[i].hProcess;
		str_free(path);
	}

	for (usize i = 0; i < tests->length; ++i) {
		char *test = slist_get(tests, i)->data;

		// Wait for this specific process to complete
		DWORD result = WaitForSingleObject(handles[i], INFINITE);
		if (result == WAIT_FAILED) {
			fprintf(
				stderr, "WaitForSingleObject failed for %s (%lu)\n", test, GetLastError()
			);
			return EXIT_FAILURE;
		}

		DWORD exitCode;
		if (GetExitCodeProcess(handles[i], &exitCode) && exitCode == 0) {
			++passed_cnt;
			printf("%s succeed\n", test);
		} else {
			++failed_cnt;
			printf("%s failed\n", test);
		}

		// Close handles for completed process
		CloseHandle(pi_array[i].hProcess);
		CloseHandle(pi_array[i].hThread);

		printf(
			"passed/failed/total: %" USIZE_FMT "/%" USIZE_FMT "/%" USIZE_FMT "\n",
			passed_cnt,
			failed_cnt,
			tests->length
		);
	}

	FREE(handles);
	FREE(si_array);
	FREE(pi_array);
#else
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
			char *argv[] = {path->data, NULL};
			path_join(path, test);
			printf("path: %s\n", path->data);
			if (execv(path->data, argv) == -1) {
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
#endif

	slist_free(tests);

	if (failed_cnt) {
		return EXIT_FAILURE;
	}
}
