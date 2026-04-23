// test/unit/test_led_fsm.c
#include "unity.h"
#include "monitor_temperature.c"  // Include to test static functions

void setUp(void) {}
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
    led_state_t state = LED_STATE_GREEN;

    state = update_led_fsm(state, 400);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, state);
}

void test_green_state_enters_yellow_at_warning_threshold(void) {
    led_state_t state = LED_STATE_GREEN;

    state = update_led_fsm(state, 850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);
}

void test_green_state_enters_red_on_critical_low(void) {
    led_state_t state = LED_STATE_GREEN;

    state = update_led_fsm(state, 49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);
}

void test_green_state_enters_red_on_critical_high(void) {
    led_state_t state = LED_STATE_GREEN;

    state = update_led_fsm(state, 1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);
}

void test_yellow_state_hysteresis_until_warning_exit(void) {
    led_state_t state = LED_STATE_GREEN;

    /* Precondition: enter YELLOW through the warning threshold. */
    state = update_led_fsm(state, 850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);

    /* Stay in YELLOW within the warning band. */
    state = update_led_fsm(state, 900);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);

    /* Hysteresis: stay YELLOW at the exit boundary. */
    state = update_led_fsm(state, 830);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);

    /* Exit only once below the exit threshold. */
    state = update_led_fsm(state, 829);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, state);
}

void test_yellow_state_enters_red_on_critical_low(void) {
    led_state_t state = LED_STATE_GREEN;

    /* Precondition: enter YELLOW first. */
    state = update_led_fsm(state, 850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);

    state = update_led_fsm(state, 49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);
}

void test_yellow_state_enters_red_on_critical_high(void) {
    led_state_t state = LED_STATE_GREEN;

    /* Precondition: enter YELLOW first. */
    state = update_led_fsm(state, 850);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);

    state = update_led_fsm(state, 1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);
}

void test_red_state_hysteresis_for_critical_low_and_recovery_to_green(void) {
    led_state_t state = LED_STATE_GREEN;

    /* Precondition: enter RED through low critical. */
    state = update_led_fsm(state, 49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);

    /* Hysteresis: stay RED below the exit threshold. */
    state = update_led_fsm(state, 69);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);

    /* Recover to GREEN once the low-critical exit threshold is reached. */
    state = update_led_fsm(state, 70);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, state);
}

void test_red_state_recovers_to_yellow_when_temperature_reenters_warning_band(void) {
    led_state_t state = LED_STATE_GREEN;

    /* Precondition: enter RED first. */
    state = update_led_fsm(state, 49);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);

    state = update_led_fsm(state, 900);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);
}

void test_red_state_hysteresis_for_critical_high_and_recovery_to_yellow(void) {
    led_state_t state = LED_STATE_GREEN;

    /* Precondition: enter RED through high critical. */
    state = update_led_fsm(state, 1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);

    /* Hysteresis: stay RED at the exit boundary. */
    state = update_led_fsm(state, 1030);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);

    /* Recover to YELLOW once below the high-critical exit threshold. */
    state = update_led_fsm(state, 1029);
    TEST_ASSERT_EQUAL(LED_STATE_YELLOW, state);
}

void test_red_state_recovers_to_green_below_warning_band(void) {
    led_state_t state = LED_STATE_GREEN;

    /* Precondition: enter RED first. */
    state = update_led_fsm(state, 1050);
    TEST_ASSERT_EQUAL(LED_STATE_RED, state);

    state = update_led_fsm(state, 849);
    TEST_ASSERT_EQUAL(LED_STATE_GREEN, state);
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