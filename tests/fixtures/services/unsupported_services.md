# Unsupported Services Fixture

This fixture defines the expected rejection codes for unsupported services and transports.

| Input | Expected | Description | OPC UA Reference |
|-------|----------|-------------|------------------|
| Write Request | `Bad_ServiceUnsupported` | Standard rejection for unsupported service | OPC-10000-4 5.11.4 |
| Call Request | `Bad_ServiceUnsupported` | Standard rejection for unsupported service | OPC-10000-4 5.11.2 |
| HistoryRead Request | `Bad_ServiceUnsupported` | Standard rejection for unsupported service | OPC-10000-4 5.11.3 |
| TransferSubscriptions Request | `Bad_ServiceUnsupported` | Above the Embedded DataChange facet (Standard tier) | OPC-10000-4 5.14.7 |
| XML Payload | `Bad_ServiceUnsupported` or Connection Closed | Not supported by transport | OPC-10000-6 5.3 |
| JSON Payload | `Bad_ServiceUnsupported` or Connection Closed | Not supported by transport | OPC-10000-6 5.4 |
| HTTPS Connection | TCP Rejection / Closed | Not supported by minimal server | OPC-10000-6 7.4 |
| WebSockets Connection | TCP Rejection / Closed | Not supported by minimal server | OPC-10000-6 7.5 |
