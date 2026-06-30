# Traceability: OPC UA PubSub (012-opcua-pubsub)

## Target Profile
Scoped PubSub UADP/UDP Publisher (OPC-10000-14), profile-targeting only.

## Implemented Scope

- UInt32 `PublisherId`.
- UADP PayloadHeader with one `DataSetWriterId`.
- One sized Data Key Frame `DataSetMessage`.
- Scalar fields encoded as OPC UA Binary Variants.
- UDP unicast, multicast, or broadcast destination selected by
  `mu_pubsub_connection_t.address`; `NULL` keeps the host broadcast default.

## Explicitly Out of Scope

- UADP Subscriber.
- PubSub security.
- MQTT, AMQP, and JSON mappings.
- Dynamic PublishedDataSet management.
- Arrays, multiple DataSetWriters per WriterGroup, metadata NetworkMessages,
  discovery NetworkMessages, delta frames, and chunked NetworkMessages.

## Mappings

| Implementation File | OPC-10000 Section | Description |
|---------------------|-------------------|-------------|
| `include/micro_opcua/pubsub.h` | OPC-10000-14 §7.2, §7.3 | Public scoped publisher configuration API; caller-owned field array contract |
| `src/encoding/uadp_encoder.c` | OPC-10000-14 §7.2.4.4.2, §7.2.4.5.2, §7.2.4.5.4, §7.2.4.5.5; OPC-10000-6 §5.2.2.16 | UADP NetworkMessage, PayloadHeader, Data Key Frame, and Variant field encoding |
| `src/core/pubsub.c` | OPC-10000-14 §5.4.6.2.2, §7.3.2.1 | Cooperative Publisher timing and UDP send dispatch |
| `src/core/server.c` | OPC-10000-14 §5.4.6.2.2, §7.3.2.1 | `mu_server_poll()` drives PubSub without requiring an active TCP Session |
| `src/platform/host_udp_adapter.c` | OPC-10000-14 §7.3.2.1 | Host UDP send adapter for UADP datagrams |
| `examples/pubsub_server/main.c` | OPC-10000-14 §7.3.2.1 | Host example for configured scalar UADP/UDP publishing |

| Test File | OPC-10000 Section | Description |
|-----------|-------------------|-------------|
| `tests/unit/test_uadp_encoding.c` | OPC-10000-14 §7.2.4.4.2, §7.2.4.5.2, §7.2.4.5.4, §7.2.4.5.5; OPC-10000-6 §5.2.2.16 | Byte-level UADP encoder tests for flags, PublisherId, DataSetWriterId, DataSetMessage size, Variant fields, and fixed field capacity |
| `tests/unit/test_pubsub.c` | OPC-10000-14 §5.4.6.2.2, §7.3.2.1 | Publisher timing including 32-bit tick wrap, configured destination address, and connectionless `mu_server_poll()` behavior |
