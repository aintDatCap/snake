#include "utils/list.hpp"
#include <cassert>
#include <check.h>
#include <cstdint>
#include <cstdlib>

START_TEST(test_list_1) {
    List<uint32_t> list;
    list.add_element(42);
    ck_assert_uint_eq(list.get_element_at(0)->value, 42);
    ck_assert(list.get_element_at(1) == nullptr);

    list.add_element(90);
    ck_assert_uint_eq(list.get_element_at(1)->value, 90);
    list.remove_element_at(0);
    ck_assert_uint_eq(list.get_element_at(0)->value, 90);
}
END_TEST

Suite *list_test_suite(void) {
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("List");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_list_1);
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

    s = list_test_suite();
    sr = srunner_create(s);

    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}