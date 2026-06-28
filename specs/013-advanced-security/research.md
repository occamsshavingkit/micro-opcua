# Phase 0: Outline & Research

## Resolved Clarifications

### 1. Cryptographic Backend
- **Decision**: The project utilizes multiple pluggable backends: OpenSSL for the host tests (`host_crypto_adapter.c`) and mbedTLS (`mbedtls_crypto_adapter.c`) / WolfSSL (`wolfssl_crypto_adapter.c`) for embedded targets.
- **Rationale**: The new algorithms (`Aes128-Sha256-RsaOaep`, `Aes256-Sha256-RsaPss`) must be implemented across these adapters, relying on their respective native support for RSA-OAEP and RSA-PSS.
- **Alternatives considered**: Writing bespoke crypto implementations (rejected due to size, security, and complexity risks).

### 2. Flash and RAM Impact
- **Decision**: Enabling RSA-OAEP and RSA-PSS in mbedTLS/WolfSSL will incur a modest Flash increase (estimated 5-10KB depending on compiler optimization). RAM impact will be limited to stack use during the operations, as the adapters are already designed to be heap-free.
- **Rationale**: The zero-heap embedded constraint (Constitution II) must be maintained.
- **Alternatives considered**: Pre-allocating large static pools (rejected as caller-provided buffers are the standard pattern in this codebase).

### 3. TrustList Enforcement
- **Decision**: The system will strictly enforce TrustList checking. If a TrustList is empty or not provided, the server will reject all `OpenSecureChannel` requests that use a security policy other than `None`.
- **Rationale**: Fail-secure approach. An unconfigured security feature shouldn't default to accepting all certificates, as it undermines the purpose of Application Authentication.
- **Alternatives considered**: Defaulting to "accept all" (rejected due to security risks).

## Additional Research Tasks
- Both `Aes128-Sha256-RsaOaep` and `Aes256-Sha256-RsaPss` require RSA operations with specific paddings.
- For Exact TrustList matching, we will compare the raw DER certificate payload provided by the client during `OpenSecureChannel` to the DER bytes in the provided TrustList.
