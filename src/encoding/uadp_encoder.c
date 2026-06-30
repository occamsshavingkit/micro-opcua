#include "micro_opcua/encoding.h"
#include "micro_opcua/pubsub.h"
#include "micro_opcua/status.h"

enum {
    MU_UADP_FLAGS_VERSION1 = 0x01,
    MU_UADP_FLAGS_PUBLISHER_ID = 0x10,
    MU_UADP_FLAGS_PAYLOAD_HEADER = 0x40,
    MU_UADP_FLAGS_EXTENDED_FLAGS1 = 0x80,
    MU_UADP_PUBLISHER_ID_UINT32 = 0x02,
    MU_UADP_DATASET_FLAGS_VALID_VARIANT = 0x01,
};

static opcua_boolean_t mu_pubsub_variant_is_supported_scalar(const mu_variant_t *value) {
    return value != NULL && !value->is_array;
}

static void mu_pubsub_patch_u16(opcua_byte_t *buffer, size_t offset, opcua_uint16_t value) {
    buffer[offset] = (opcua_byte_t)(value & 0xffu);
    buffer[offset + 1u] = (opcua_byte_t)((value >> 8u) & 0xffu);
}

/* Encode one scoped UADP NetworkMessage:
 * - UInt32 PublisherId
 * - one PayloadHeader entry with DataSetWriterId
 * - one Data Key Frame DataSetMessage
 * - Variant field encoding for caller-provided scalar fields
 *
 * Normative surface: OPC-10000-14 sections 7.2.4.4.2, 7.2.4.5.2,
 * 7.2.4.5.4, and 7.2.4.5.5. Variant values use OPC-10000-6 section
 * 5.2.2.16 through the shared binary Variant encoder.
 */
opcua_statuscode_t mu_encode_uadp_network_message(const mu_pubsub_connection_t *conn,
                                                  const mu_pubsub_writer_group_t *wg, opcua_byte_t *buffer,
                                                  size_t buffer_size, size_t *bytes_written) {
    const mu_pubsub_dataset_writer_t *writer = NULL;
    mu_binary_writer_t binary;
    size_t message_size_offset = 0;
    size_t message_start = 0;
    size_t message_size = 0;

    if (bytes_written) {
        *bytes_written = 0;
    }
    if (!conn || !wg || !buffer || !bytes_written) {
        return MU_STATUS_BAD_INVALIDARGUMENT;
    }

    writer = &wg->dataset_writer;
    if (writer->field_count > MU_PUBSUB_MAX_FIELDS) {
        return MU_STATUS_BAD_TOOMANYOPERATIONS;
    }
    if (writer->field_count > 0u && writer->fields == NULL) {
        return MU_STATUS_BAD_INVALIDARGUMENT;
    }
    for (size_t i = 0; i < writer->field_count; ++i) {
        if (!mu_pubsub_variant_is_supported_scalar(&writer->fields[i].value)) {
            return MU_STATUS_BAD_NOTSUPPORTED;
        }
    }

    mu_binary_writer_init(&binary, buffer, buffer_size);

    mu_binary_write_byte(&binary, (opcua_byte_t)(MU_UADP_FLAGS_VERSION1 | MU_UADP_FLAGS_PUBLISHER_ID |
                                                 MU_UADP_FLAGS_PAYLOAD_HEADER | MU_UADP_FLAGS_EXTENDED_FLAGS1));
    mu_binary_write_byte(&binary, MU_UADP_PUBLISHER_ID_UINT32);
    mu_binary_write_uint32(&binary, conn->publisher_id);

    mu_binary_write_byte(&binary, 1u); /* PayloadHeader Count */
    mu_binary_write_uint16(&binary, writer->data_set_writer_id);

    message_size_offset = binary.position;
    mu_binary_write_uint16(&binary, 0u); /* patched after DataSetMessage body */
    message_start = binary.position;

    mu_binary_write_byte(&binary, MU_UADP_DATASET_FLAGS_VALID_VARIANT);
    for (size_t i = 0; i < writer->field_count; ++i) {
        mu_binary_write_variant(&binary, &writer->fields[i].value);
    }

    if (binary.status != MU_STATUS_GOOD) {
        return binary.status;
    }
    message_size = binary.position - message_start;
    if (message_size > 0xffffu) {
        return MU_STATUS_BAD_ENCODINGLIMITSEXCEEDED;
    }

    mu_pubsub_patch_u16(buffer, message_size_offset, (opcua_uint16_t)message_size);
    *bytes_written = binary.position;
    return binary.status;
}
