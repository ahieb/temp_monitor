// test/unit/test_led_fsm.c
#include "unity.h"
#include "monitor_temperature.c"  // Include to test static functions

void setUp(void) {
    /* Reset global state before each test */
    g_led_state = LED_STATE_GREEN;
}

void tearDown(void) {}

// Initial state tests
void test_initial_state_green_normal_temp(void) {
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, get_initial_led_state(500));
}

void test_initial_state_yellow_warning_high(void) {
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, get_initial_led_state(900));
}

void test_initial_state_red_critical_low(void) {
    TEST_ASSERT_EQUAL(LED_STATE_RED, get_initial_led_state(30));
}

void test_initial_state_red_critical_high(void) {
    TEST_ASSERT_EQUAL(LED_STATE_RED, get_initial_led_state(1100));
}

void test_green_state_stays_green_in_safe_range(void) {
    g_led_state = LED_STATE_GREEN;

    update_led_fsm(400);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, g_led_state);
}

void test_green_state_enters_yellow_at_warning_threshold(void) {
    g_led_state = LED_STATE_GREEN;

    update_led_fsm(850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);
}

void test_green_state_enters_red_on_critical_low(void) {
    g_led_state = LED_STATE_GREEN;

    update_led_fsm(49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);
}

void test_green_state_enters_red_on_critical_high(void) {
    g_led_state = LED_STATE_GREEN;

    update_led_fsm(1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);
}

void test_yellow_state_hysteresis_until_warning_exit(void) {
    g_led_state = LED_STATE_GREEN;

    /* Precondition: enter YELLOW through the warning threshold. */
    update_led_fsm(850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);

    /* Stay in YELLOW within the warning band. */
    update_led_fsm(900);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);

    /* Hysteresis: stay YELLOW at the exit boundary. */
    update_led_fsm(830);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);

    /* Exit only once below the exit threshold. */
    update_led_fsm(829);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, g_led_state);
}

void test_yellow_state_enters_red_on_critical_low(void) {
    g_led_state = LED_STATE_GREEN;

    /* Precondition: enter YELLOW first. */
    update_led_fsm(850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);

    update_led_fsm(49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);
}

void test_yellow_state_enters_red_on_critical_high(void) {
    g_led_state = LED_STATE_GREEN;

    /* Precondition: enter YELLOW first. */
    update_led_fsm(850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);

    update_led_fsm(1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);
}

void test_red_state_hysteresis_for_critical_low_and_recovery_to_green(void) {
    g_led_state = LED_STATE_GREEN;

    /* Precondition: enter RED through low critical. */
    update_led_fsm(49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);

    /* Hysteresis: stay RED below the exit threshold. */
    update_led_fsm(69);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);

    /* Recover to GREEN once the low-critical exit threshold is reached. */
    update_led_fsm(70);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, g_led_state);
}

void test_red_state_recovers_to_yellow_when_temperature_reenters_warning_band(void) {
    g_led_state = LED_STATE_GREEN;

    /* Precondition: enter RED first. */
    update_led_fsm(49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);

    update_led_fsm(900);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);
}

void test_red_state_hysteresis_for_critical_high_and_recovery_to_yellow(void) {
    g_led_state = LED_STATE_GREEN;

    /* Precondition: enter RED through high critical. */
    update_led_fsm(1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);

    /* Hysteresis: stay RED at the exit boundary. */
    update_led_fsm(1030);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);

    /* Recover to YELLOW once below the high-critical exit threshold. */
    update_led_fsm(1029);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, g_led_state);
}

void test_red_state_recovers_to_green_below_warning_band(void) {
    g_led_state = LED_STATE_GREEN;

    /* Precondition: enter RED first. */
    update_led_fsm(1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, g_led_state);

    update_led_fsm(849);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, g_led_state);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initial_state_green_normal_temp);
    RUN_TEST(test_initial_state_yellow_warning_high);
    RUN_TEST(test_initial_state_red_critical_low);
    RUN_TEST(test_initial_state_red_critical_high);

    RUN_TEST(test_green_state_stays_green_in_safe_range);
    RUN_TEST(test_green_state_enters_yellow_at_warning_threshold);
    RUN_TEST(test_green_state_enters_red_on_critical_low);
    RUN_TEST(test_green_state_enters_red_on_critical_high);

    RUN_TEST(test_yellow_state_hysteresis_until_warning_exit);
    RUN_TEST(test_yellow_state_enters_red_on_critical_low);
    RUN_TEST(test_yellow_state_enters_red_on_critical_high);

    RUN_TEST(test_red_state_hysteresis_for_critical_low_and_recovery_to_green);
    RUN_TEST(test_red_state_recovers_to_yellow_when_temperature_reenters_warning_band);
    RUN_TEST(test_red_state_hysteresis_for_critical_high_and_recovery_to_yellow);
    RUN_TEST(test_red_state_recovers_to_green_below_warning_band);

    return UNITY_END();
}
