/* tests/unit/test_subscriptions_capacity.c
 *
 * Feature 005 US1 capacity checks for the Standard DataChange Subscription
 * 2017 Server Facet. These tests are active only when the Standard facet is
 * compiled in; the default micro/nano test build leaves them as no-ops.
 *
 * OPC-10000-4 5.13.2: Bad_TooManyMonitoredItems.
 * OPC-10000-4 5.14.2: Bad_TooManySubscriptions.
 * OPC-10000-4 5.14.5: Bad_TooManyPublishRequests.
 */
#include "unity.h"

#include "../../src/services/subscription.h"
#include "micro_opcua/status.h"

void setUp(void) {}
void tearDown(void) {}

#if MICRO_OPCUA_SUBSCRIPTIONS && MICRO_OPCUA_SUBSCRIPTIONS_STANDARD

static void assert_standard_capacity_configuration(void) {
    TEST_ASSERT_TRUE_MESSAGE(MU_MAX_SUBSCRIPTIONS >= 2u, "Standard facet requires at least 2 subscriptions");
    TEST_ASSERT_TRUE_MESSAGE(MU_MAX_MONITORED_ITEMS >= 100u, "Standard facet requires at least 100 monitored items");
    TEST_ASSERT_TRUE_MESSAGE(MU_MAX_PUBLISH_REQUESTS >= 5u,
                             "Standard facet requires at least 5 parked Publish requests");
    TEST_ASSERT_TRUE_MESSAGE(MU_MONITORED_QUEUE_DEPTH >= 2u,
                             "Standard facet requires monitored-item queue depth at least 2");
}

void test_standard_capacity_macros_meet_profile_minimums(void) {
    assert_standard_capacity_configuration();
}

void test_accepts_required_subscriptions_and_rejects_one_beyond_capacity(void) {
    mu_subscriptions_t subs;
    mu_subscriptions_init(&subs);
    assert_standard_capacity_configuration();

    for (opcua_uint32_t i = 0u; i < MU_MAX_SUBSCRIPTIONS; ++i) {
        mu_subscription_t *sub = NULL;
        TEST_ASSERT_EQUAL_HEX32(MU_STATUS_GOOD,
                                mu_subscription_create(&subs, 1u, 100u, 30u, 10u, 0u, 0u, true, 0u, &sub));
        TEST_ASSERT_NOT_NULL(sub);
    }

    mu_subscription_t *extra = NULL;
    TEST_ASSERT_EQUAL_HEX32(MU_STATUS_BAD_TOOMANYSUBSCRIPTIONS,
                            mu_subscription_create(&subs, 1u, 100u, 30u, 10u, 0u, 0u, true, 0u, &extra));
    TEST_ASSERT_NULL(extra);
}

void test_accepts_required_monitored_items_and_rejects_one_beyond_capacity(void) {
    mu_subscriptions_t subs;
    mu_subscriptions_init(&subs);
    assert_standard_capacity_configuration();

    for (opcua_uint32_t i = 0u; i < MU_MAX_MONITORED_ITEMS; ++i) {
        mu_monitored_item_t *item = NULL;
        TEST_ASSERT_EQUAL_HEX32(MU_STATUS_GOOD, mu_monitored_item_alloc(&subs, 1u, &item));
        TEST_ASSERT_NOT_NULL(item);
        TEST_ASSERT_NOT_EQUAL(0u, item->monitored_item_id);
    }

    mu_monitored_item_t *extra = NULL;
    TEST_ASSERT_EQUAL_HEX32(MU_STATUS_BAD_TOOMANYMONITOREDITEMS, mu_monitored_item_alloc(&subs, 1u, &extra));
    TEST_ASSERT_NULL(extra);
}

void test_accepts_required_parallel_publish_requests_and_rejects_one_beyond_capacity(void) {
    mu_subscriptions_t subs;
    mu_subscriptions_init(&subs);
    assert_standard_capacity_configuration();

    for (opcua_uint32_t i = 0u; i < MU_MAX_PUBLISH_REQUESTS; ++i) {
        mu_publish_request_t *request = NULL;
        TEST_ASSERT_EQUAL_HEX32(MU_STATUS_GOOD, mu_publish_request_enqueue(&subs, 1u, 100u + i, 200u + i, i, &request));
        TEST_ASSERT_NOT_NULL(request);
    }

    mu_publish_request_t *extra = NULL;
    TEST_ASSERT_EQUAL_HEX32(MU_STATUS_BAD_TOOMANYPUBLISHREQUESTS,
                            mu_publish_request_enqueue(&subs, 1u, 999u, 999u, 999u, &extra));
    TEST_ASSERT_NULL(extra);
}

#else

void test_standard_capacity_tests_require_standard_subscription_build(void) {
    TEST_PASS_MESSAGE("MICRO_OPCUA_SUBSCRIPTIONS_STANDARD is disabled in this build");
}

#endif

int main(void) {
    UNITY_BEGIN();
#if MICRO_OPCUA_SUBSCRIPTIONS && MICRO_OPCUA_SUBSCRIPTIONS_STANDARD
    RUN_TEST(test_standard_capacity_macros_meet_profile_minimums);
    RUN_TEST(test_accepts_required_subscriptions_and_rejects_one_beyond_capacity);
    RUN_TEST(test_accepts_required_monitored_items_and_rejects_one_beyond_capacity);
    RUN_TEST(test_accepts_required_parallel_publish_requests_and_rejects_one_beyond_capacity);
#else
    RUN_TEST(test_standard_capacity_tests_require_standard_subscription_build);
#endif
    return UNITY_END();
}
