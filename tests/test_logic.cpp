#include "game/logic.hpp"
#include <check.h>
#include <cstdint>

START_TEST(test_game_logic_1) {
    Snake::Game game = Snake::Game(300, 300);
    game.update_game(Snake::DIRECTION_UP);
    game.update_game(Snake::DIRECTION_RIGHT);
}
END_TEST

Suite *game_logic_test_suite(void) {
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("Game logic");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_game_logic_1);
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

    s = game_logic_test_suite();
    sr = srunner_create(s);

    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}