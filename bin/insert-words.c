// insert-words - insert words into SQLite database
//
// Usage: insert-words <sqlite3.db>

#include <string.h>
#ifdef _WIN32
void main() {}
#else

#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <wheel.h>

#define BUF_SIZE 1024

void list_words(sqlite3 *db);
int print_words(void *_, int cols, char **row, char **col_names);
void insert_word(sqlite3 *db, const char *word, const char *meaning);
void delete_words(sqlite3 *db, const Slist *words);

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
			str_free(slist_delete(list, 0));
			delete_words(db, list);

			SLIST_FREE(list);
			continue;
		} else if (strcmp(slist_get(list, 0)->data, "list") == 0) {
			list_words(db);

			SLIST_FREE(list);
			continue;
		}

		Str *word = slist_delete(list, 0);
		char *meaning = str_join(list, " ");

		if (strlen(meaning) == 0) {
			insert_word(db, word->data, NULL);
		} else {
			insert_word(db, word->data, meaning);
		}

		FREE(meaning);
		str_free(word);

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
	int rc =
		sqlite3_exec(db, "select word, meaning from words", print_words, NULL, &err_msg);
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

void insert_word(sqlite3 *db, const char *word, const char *meaning) {
	assert(word != NULL);
	assert(strlen(word) != 0);

	int rc;
	sqlite3_stmt *stmt;
	const char *sql = "insert or replace into words (word, meaning) values (?, ?)";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		eprintln("ERROR: prepare failed: %s", sqlite3_errmsg(db));
	}

	sqlite3_bind_text(stmt, 1, word, -1, SQLITE_STATIC);
	if (meaning == NULL) {
		sqlite3_bind_null(stmt, 2);
	} else {
		sqlite3_bind_text(stmt, 2, meaning, -1, SQLITE_STATIC);
	}

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		eprintln("ERROR: insert failed: %s", sqlite3_errmsg(db));
	} else {
		eprintln("INFO: inserted: %d row affected", sqlite3_changes(db));
	}

	sqlite3_finalize(stmt);
}

void delete_words(sqlite3 *db, const Slist *words) {
	int rc;
	sqlite3_stmt *stmt;
	const char *sql = "delete from words where word = ?";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		eprintln("ERROR: prepare failed: %s", sqlite3_errmsg(db));
	}

	for (usize i = 0; i < words->len; ++i) {
		sqlite3_bind_text(stmt, 1, slist_get(words, i)->data, -1, SQLITE_STATIC);

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			eprintln("ERROR: delete failed: %s", sqlite3_errmsg(db));
		} else {
			eprintln("INFO: deleted: %d row affected", sqlite3_changes(db));
		}

		sqlite3_reset(stmt);
	}

	sqlite3_finalize(stmt);
}

#endif // _WIN32
