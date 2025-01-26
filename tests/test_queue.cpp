#include "utils/queue.hpp"
#include <check.h>
#include <cstdint>
#include <cstdlib>

START_TEST(test_queue_1) {
    Queue<uint32_t> queue;
    queue.enqueue(3);
    queue.enqueue(6);
    queue.enqueue(984);

    ck_assert_uint_eq(queue.dequeue()->value, 3);
    ck_assert_uint_eq(queue.dequeue()->value, 6);
    ck_assert_uint_eq(queue.dequeue()->value, 984);
}
END_TEST

Suite *queue_test_suite(void) {
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Queue");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_queue_1);
    suite_add_tcase(s, tc_core);

    /* Limits test case */
    // tc_limits = tcase_create("Limits");

    // tcase_add_test(tc_limits, test_money_create_zero);
    // suite_add_tcase(s, tc_limits);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = queue_test_suite();
    sr = srunner_create(s);

    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}