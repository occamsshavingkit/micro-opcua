/* src/encoding/binary_nodeid.c */
#include "micro_opcua/encoding.h"

opcua_statuscode_t mu_binary_read_nodeid(mu_binary_reader_t *reader, mu_nodeid_t *value) {
    opcua_byte_t encoding_mask;
    opcua_statuscode_t status = mu_binary_read_byte(reader, &encoding_mask);
    if (status != MU_STATUS_GOOD)
        return status;

    opcua_byte_t format = encoding_mask & 0x0F;

    switch (format) {
    case 0x00: /* TwoByte */
        value->identifier_type = MU_NODEID_NUMERIC;
        value->namespace_index = 0;
        {
            opcua_byte_t id;
            status = mu_binary_read_byte(reader, &id);
            if (status != MU_STATUS_GOOD)
                return status;
            value->identifier.numeric = id;
        }
        break;

    case 0x01: /* FourByte */
        value->identifier_type = MU_NODEID_NUMERIC;
        {
            opcua_byte_t ns;
            status = mu_binary_read_byte(reader, &ns);
            if (status != MU_STATUS_GOOD)
                return status;
            value->namespace_index = ns;

            opcua_uint16_t id;
            status = mu_binary_read_uint16(reader, &id);
            if (status != MU_STATUS_GOOD)
                return status;
            value->identifier.numeric = id;
        }
        break;

    case 0x02: /* Numeric */
        value->identifier_type = MU_NODEID_NUMERIC;
        status = mu_binary_read_uint16(reader, &value->namespace_index);
        if (status != MU_STATUS_GOOD)
            return status;
        status = mu_binary_read_uint32(reader, &value->identifier.numeric);
        if (status != MU_STATUS_GOOD)
            return status;
        break;

    case 0x03: /* String */
        value->identifier_type = MU_NODEID_STRING;
        status = mu_binary_read_uint16(reader, &value->namespace_index);
        if (status != MU_STATUS_GOOD)
            return status;
        status = mu_binary_read_string(reader, &value->identifier.string);
        if (status != MU_STATUS_GOOD)
            return status;
        break;

    default:
        return MU_STATUS_BAD_DECODINGERROR;
    }

    return MU_STATUS_GOOD;
}

opcua_statuscode_t mu_binary_write_nodeid(mu_binary_writer_t *writer, const mu_nodeid_t *value) {
    if (!value)
        return MU_STATUS_BAD_ENCODINGERROR;

    opcua_statuscode_t status;

    switch (value->identifier_type) {
    case MU_NODEID_NUMERIC:
        /* Use TwoByte if possible */
        if (value->namespace_index == 0 && value->identifier.numeric <= 255) {
            status = mu_binary_write_byte(writer, 0x00); /* TwoByte mask */
            if (status != MU_STATUS_GOOD)
                return status;
            return mu_binary_write_byte(writer, (opcua_byte_t)value->identifier.numeric);
        }
        /* Use FourByte if possible */
        if (value->namespace_index <= 255 && value->identifier.numeric <= 65535) {
            status = mu_binary_write_byte(writer, 0x01); /* FourByte mask */
            if (status != MU_STATUS_GOOD)
                return status;
            status = mu_binary_write_byte(writer, (opcua_byte_t)value->namespace_index);
            if (status != MU_STATUS_GOOD)
                return status;
            return mu_binary_write_uint16(writer, (opcua_uint16_t)value->identifier.numeric);
        }
        /* Use full Numeric */
        status = mu_binary_write_byte(writer, 0x02); /* Numeric mask */
        if (status != MU_STATUS_GOOD)
            return status;
        status = mu_binary_write_uint16(writer, value->namespace_index);
        if (status != MU_STATUS_GOOD)
            return status;
        return mu_binary_write_uint32(writer, value->identifier.numeric);

    case MU_NODEID_STRING:
        status = mu_binary_write_byte(writer, 0x03); /* String mask */
        if (status != MU_STATUS_GOOD)
            return status;
        status = mu_binary_write_uint16(writer, value->namespace_index);
        if (status != MU_STATUS_GOOD)
            return status;
        return mu_binary_write_string(writer, &value->identifier.string);

    default:
        return MU_STATUS_BAD_ENCODINGERROR;
    }
}
