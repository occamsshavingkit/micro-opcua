# Traceability: Fixtures

This document maps binary fixtures and sidecar files to specific OPC UA messages and data encodings.

## Fixture Structure
- `*.bin` files are raw byte fixtures.
- `*.py` or `*.sh` files are sidecar scripts to reproduce or validate the fixture.

| Fixture Path | Description | OPC UA Reference | Sidecar Script |
|--------------|-------------|------------------|----------------|
| `tests/fixtures/services/unsupported_services.md` | Matrix of unsupported services | Part 4, 7.38.2 | N/A |
| `tests/unit/test_message_chunk_errors.c` | Malformed chunk validation | Part 6, 6.7.2, 6.7.3 | N/A |
| `tests/unit/test_binary_string_errors.c` | Truncated and over-limit String | Part 6, 5.2.2.4 | N/A |
| `tests/unit/test_binary_array_errors.c` | Invalid array lengths | Part 6, 5.2.5 | N/A |
| `tests/unit/test_binary_nodeid_errors.c` | Invalid NodeId encoding mask | Part 6, 5.2.2.9 | N/A |
| `tests/unit/test_binary_extension_object_errors.c` | Invalid ExtensionObject encoding | Part 6, 5.2.2.15 | N/A |
| `tests/unit/test_service_state_errors.c` | Invalid state sequence errors | Part 4, 5.6.2.2, 5.7.2.1, 5.9.2.2, 5.11.2.2, 7.38.2 | N/A |
| `tests/unit/test_security_identity_errors.c` | Security and identity errors | Part 4, 5.6.2.2, 5.7.3.2, 7.38.2; Part 6, 6.7.4 | N/A |
| `tests/fuzz/fuzz_binary_reader.c` | Binary reader fuzz target | Part 6, 5.2.1 | N/A |
| `tests/fuzz/fuzz_message_chunk.c` | MessageChunk fuzz target | Part 6, 6.7.2, 6.7.3 | N/A |
| `tests/fuzz/fuzz_binary_types.c` | Binary types fuzz target | Part 6, 5.2.2.4, 5.2.2.9, 5.2.2.15, 5.2.5 | N/A |
| `tests/integration/test_single_client_limit.c` | Second-client rejection | Part 6, 7.1.5 | N/A |
