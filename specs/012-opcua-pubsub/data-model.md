# Data Model: PubSub

## Entities

1. **mu_pubsub_connection_t**
   - Fields: UDP port, optional destination address, enabled flag, PublisherId.
   - Purpose: Maps to the PubSubConnection in Part 14. Represents the network transport configuration.

2. **mu_pubsub_writer_group_t**
   - Fields: PublishingInterval, WriterGroupId, network message configuration.
   - Purpose: Triggers the publishing of messages at the given interval. Owns the timing.

3. **mu_pubsub_dataset_writer_t**
   - Fields: DataSetWriterId, caller-owned `mu_pubsub_field_t` array, field count.
   - Purpose: Encodes scalar Variant fields into a UADP Data Key Frame.

4. **mu_pubsub_field_t**
   - Fields: scalar `mu_variant_t` value.
   - Purpose: Represents one PublishedDataSet field for the scoped UADP/UDP Publisher.

## Scoped Wire Layout

- UADP NetworkMessage with UInt32 PublisherId.
- UADP PayloadHeader enabled with exactly one DataSetWriterId.
- UADP DataSet payload includes one UInt16 DataSetMessage size followed by one
  Data Key Frame DataSetMessage.
- DataSetFlags1 marks the DataSetMessage valid and uses Variant field encoding.

Normative references: OPC-10000-14 sections 7.2.4.4.2, 7.2.4.5.2,
7.2.4.5.4, and 7.2.4.5.5; Variant encoding uses OPC-10000-6 section 5.2.2.16.
