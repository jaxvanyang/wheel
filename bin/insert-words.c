// insert-words - insert words into SQLite database
//
// Usage: insert-words <sqlite3.db>

#include <string.h>
#ifdef _WIN32
void main() {}
#else

#include <sqlite3.h>
#include <stdio.h>
#include <wheel.h>

#define BUF_SIZE 1024

void list_words(sqlite3 *db);
int print_words(void *_, int cols, char **row, char **col_names);

int main(int argc, const char *argv[]) {
	if (argc != 2) {
		error("expected 1 argument");
	}

	sqlite3 *db;
	char *err_msg = NULL;
	int rc;

	rc = sqlite3_open(argv[1], &db);
	if (rc != SQLITE_OK) {
		error("%s", sqlite3_errmsg(db));
	}

	eprintln("INFO: database %s connected", argv[1]);

	char sql[BUF_SIZE];
	Str *words = str_new();

	while (true) {
		eprint("> ");

		str_readline(words, stdin);

		Slist *list = str_split(words, .seps = "\t\f\v\r\n :");

		// words->len == 0 indicates EOF
		if (words->len == 0) {
			SLIST_FREE(list);
			break;
		}

		if (list->len == 0) {
			SLIST_FREE(list);
			continue;
		}

		if (strcmp(slist_get(list, 0)->data, "exit") == 0) {
			SLIST_FREE(list);
			break;
		}

		if (strcmp(slist_get(list, 0)->data, "delete") == 0) {
			for (usize i = 1; i < list->len; ++i) {
				snprintf(
					sql,
					sizeof(sql),
					"delete from words where word = '%s'",
					slist_get(list, i)->data
				);

				rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
				if (rc != SQLITE_OK) {
					eprintln("ERROR: %s", err_msg);
					sqlite3_free(err_msg);
				} else {
					eprintln("INFO: deleted: %d row affected", sqlite3_changes(db));
				}
			}

			SLIST_FREE(list);
			continue;
		} else if (strcmp(slist_get(list, 0)->data, "list") == 0) {
			list_words(db);

			SLIST_FREE(list);
			continue;
		}

		if (list->len == 1) {
			snprintf(
				sql,
				sizeof(sql),
				"insert or replace into words (word) values ('%s')",
				slist_get(list, 0)->data
			);
		} else {
			Str *word = slist_delete(list, 0);
			char *translation = str_join(list, " ");

			snprintf(
				sql,
				sizeof(sql),
				"insert or replace into words (word, translation) values ('%s', '%s')",
				word->data,
				translation
			);

			FREE(translation);
			str_free(word);
		}

		rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
		if (rc != SQLITE_OK) {
			eprintln("ERROR: %s", err_msg);
			sqlite3_free(err_msg);
		} else {
			eprintln("INFO: inserted: %d row affected", sqlite3_changes(db));
		}

		SLIST_FREE(list);
	}

	FREE(words);

	rc = sqlite3_close(db);
	if (rc != SQLITE_OK) {
		error("%s", sqlite3_errmsg(db));
	}
}

void list_words(sqlite3 *db) {
	char *err_msg = NULL;
	int rc = sqlite3_exec(
		db, "select word, translation from words", print_words, NULL, &err_msg
	);
	if (rc != SQLITE_OK) {
		eprintln("ERROR: %s", err_msg);
		sqlite3_free(err_msg);
	}
}

int print_words(void *_, int cols, char **row, char **col_names) {
	(void)cols;
	(void)col_names;

	if (row[1] == NULL) {
		printf("%s\n", row[0]);
	} else {
		printf("%s: %s\n", row[0], row[1]);
	}

	return SQLITE_OK;
}

#endif // _WIN32
