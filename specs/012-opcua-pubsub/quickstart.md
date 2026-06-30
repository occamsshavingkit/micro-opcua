# Quickstart: PubSub UADP

```c
// 1. Configure the server for PubSub
mu_server_config_t config = mu_server_config_default();
config.pubsub.enabled = true;
config.pubsub.port = 4840;
config.pubsub.publisher_id = 1234;
config.pubsub.address = "239.0.0.1"; /* NULL defaults to broadcast */

// 2. Add a writer group
static mu_pubsub_field_t fields[] = {
    {.value = {MU_TYPE_DOUBLE, {.d = 23.5}}},
};
mu_pubsub_writer_group_t wg = {
    .publishing_interval_ms = 1000,
    .writer_group_id = 1,
    .dataset_writer = {
        .data_set_writer_id = 1,
        .fields = fields,
        .field_count = 1,
    },
};
mu_server_add_writer_group(server, &wg);

// 3. Tick the server
// The poll loop will broadcast UADP/UDP packets based on the interval without
// requiring an active TCP client Session.
while (true) {
    mu_server_poll(server); 
}
```

This quickstart uses the scoped publisher implemented by this feature:
one DataSetWriter per WriterGroup, caller-owned scalar Variant fields, UADP
PayloadHeader enabled, no Subscriber, no PubSub security, and no MQTT/AMQP/JSON
mapping.
