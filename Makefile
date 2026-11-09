# Builds and runs the tests in tests/.  Each library module is
# compiled together with the test that exercises it.

CC      = gcc
CFLAGS  = -std=gnu11 -Wall -g

TESTS = tests/test_sets_array_sorted \
        tests/test_sets_array_unsorted \
        tests/test_table_strings \
        tests/test_table_generic \
        tests/test_list \
        tests/test_pqueue \
        tests/test_table_set

all: $(TESTS)

tests/test_sets_array_sorted: tests/test_sets_array.c sets-array/sorted.c sets-array/set.h
	$(CC) $(CFLAGS) -Isets-array -DIMPL_NAME='"sorted"' tests/test_sets_array.c sets-array/sorted.c -o $@

tests/test_sets_array_unsorted: tests/test_sets_array.c sets-array/unsorted.c sets-array/set.h
	$(CC) $(CFLAGS) -Isets-array -DIMPL_NAME='"unsorted"' tests/test_sets_array.c sets-array/unsorted.c -o $@

tests/test_table_strings: tests/test_table_strings.c hash-table-strings/table.c hash-table-strings/set.h
	$(CC) $(CFLAGS) -Ihash-table-strings tests/test_table_strings.c hash-table-strings/table.c -o $@

tests/test_table_generic: tests/test_table_generic.c hash-table-generic/table.c hash-table-generic/set.h
	$(CC) $(CFLAGS) -Ihash-table-generic tests/test_table_generic.c hash-table-generic/table.c -o $@

tests/test_list: tests/test_list.c linked-list/list.c linked-list/set.c linked-list/list.h linked-list/set.h
	$(CC) $(CFLAGS) -Ilinked-list tests/test_list.c linked-list/list.c linked-list/set.c -o $@

tests/test_pqueue: tests/test_pqueue.c priority-queue/pqueue.c priority-queue/pqueue.h
	$(CC) $(CFLAGS) -Ipriority-queue tests/test_pqueue.c priority-queue/pqueue.c -o $@

tests/test_table_set: tests/test_table_set.c hash-table-set/table.c hash-table-set/set.h
	$(CC) $(CFLAGS) -Ihash-table-set tests/test_table_set.c hash-table-set/table.c -o $@

test: all
	@for t in $(TESTS); do ./$$t || exit 1; done

clean:
	rm -f $(TESTS)

.PHONY: all test clean
