/* tests/unit/test_service_header.c */
#include "../../src/services/service_header.h"
#include "muc_opcua/muc_opcua.h"
#include "unity.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* RequestHeader (OPC 10000-4 7.32):
   authenticationToken(NodeId) timestamp(DateTime) requestHandle(UInt32)
   returnDiagnostics(UInt32) auditEntryId(String) timeoutHint(UInt32)
   additionalHeader(ExtensionObject). */
void test_request_header_decode(void) {
    opcua_byte_t buf[] = {
        0x00, 0x00,                         /* authenticationToken: TwoByte NodeId i=0 */
        0,    0,    0,    0,    0, 0, 0, 0, /* timestamp = 0 */
        7,    0,    0,    0,                /* requestHandle = 7 */
        0,    0,    0,    0,                /* returnDiagnostics = 0 */
        0xFF, 0xFF, 0xFF, 0xFF,             /* auditEntryId = null string */
        0xE8, 0x03, 0x00, 0x00,             /* timeoutHint = 1000 */
        0x00, 0x00, 0x00                    /* additionalHeader: null ExtensionObject (NodeId i=0 + enc 0x00) */
    };

    mu_binary_reader_t r;
    mu_binary_reader_init(&r, buf, sizeof(buf));

    mu_request_header_t h;
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_request_header_decode(&r, &h));
    TEST_ASSERT_EQUAL(7, h.request_handle);
    TEST_ASSERT_EQUAL(1000, h.timeout_hint);
    TEST_ASSERT_EQUAL(sizeof(buf), r.position); /* consumed the whole header */
}

/* ResponseHeader (OPC 10000-4 7.33):
   timestamp(DateTime) requestHandle(UInt32) serviceResult(StatusCode)
   serviceDiagnostics(DiagnosticInfo) stringTable(String[]) additionalHeader(ExtensionObject). */
void test_response_header_encode_bytes(void) {
    opcua_byte_t buf[64];
    mu_binary_writer_t w;
    mu_binary_writer_init(&w, buf, sizeof(buf));

    mu_response_header_t h;
    h.timestamp = 0;
    h.request_handle = 7;
    h.service_result = MU_STATUS_GOOD;

    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_response_header_encode(&w, &h));

    /* 8 (timestamp) + 4 (handle) + 4 (result) + 1 (empty DiagnosticInfo)
       + 4 (null String[] = -1) + 3 (null ExtensionObject: NodeId 0x00 0x00 + enc 0x00) = 24 */
    TEST_ASSERT_EQUAL(24, w.position);
    TEST_ASSERT_EQUAL(7, buf[8]);     /* requestHandle low byte */
    TEST_ASSERT_EQUAL(0, buf[12]);    /* serviceResult = Good */
    TEST_ASSERT_EQUAL(0x00, buf[16]); /* serviceDiagnostics: empty */
    TEST_ASSERT_EQUAL(0xFF, buf[17]); /* stringTable: null array length -1 */
    TEST_ASSERT_EQUAL(0x00, buf[21]); /* additionalHeader NodeId byte 0 */
    TEST_ASSERT_EQUAL(0x00, buf[22]); /* additionalHeader NodeId byte 1 */
    TEST_ASSERT_EQUAL(0x00, buf[23]); /* additionalHeader encoding 0x00 */
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_request_header_decode);
    RUN_TEST(test_response_header_encode_bytes);
    return UNITY_END();
}
