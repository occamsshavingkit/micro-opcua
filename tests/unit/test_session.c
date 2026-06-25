/* tests/unit/test_session.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

#include "../../src/services/session.h"

void test_session_create(void) {
    mu_session_t session;
    mu_session_init(&session);
    TEST_ASSERT_EQUAL(MU_SESSION_STATE_CLOSED, session.state);
    
    double revised_timeout;
    opcua_uint32_t session_id, auth_token;
    
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_session_create(&session, 5000.0, &revised_timeout, &session_id, &auth_token));
    TEST_ASSERT_EQUAL(MU_SESSION_STATE_CREATED, session.state);
    TEST_ASSERT_EQUAL(10000.0, revised_timeout); /* Bounded */
    TEST_ASSERT_EQUAL(1, session_id);
}

void test_session_activate_anonymous(void) {
    TEST_IGNORE_MESSAGE("Implement ActivateSession anonymous identity policy test");
}

void test_session_close(void) {
    TEST_IGNORE_MESSAGE("Implement CloseSession test");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_session_create);
    RUN_TEST(test_session_activate_anonymous);
    RUN_TEST(test_session_close);
    return UNITY_END();
}
