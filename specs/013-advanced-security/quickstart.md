# Quickstart: Advanced Security

## Configuring the Server with TrustList and New Policies

To start a server utilizing `Aes128-Sha256-RsaOaep` or `Aes256-Sha256-RsaPss`, and enforcing client certificate validation:

```c
#include "micro_opcua/server.h"
#include "micro_opcua/security.h"

// 1. Define your trusted client certificates (DER encoded)
static const uint8_t trusted_client_cert[] = { /* ... */ };
static const uint8_t* cert_ptrs[] = { trusted_client_cert };
static const size_t cert_lens[] = { sizeof(trusted_client_cert) };

// 2. Initialize the TrustList
mu_trust_list_t trust_list = {
    .certificates = cert_ptrs,
    .lengths = cert_lens,
    .count = 1
};

// 3. Configure the server
mu_server_config_t config = {
    .crypto_adapter = &my_mbedtls_adapter,
    .trust_list = &trust_list,
    .supported_policies = MU_SECURITY_POLICY_AES128_SHA256_RSAOAEP | MU_SECURITY_POLICY_AES256_SHA256_RSAPSS,
    // ... other config ...
};

mu_server_t server;
mu_server_init(&server, &config);
```

When a client connects, the server will now validate the client's certificate against `trust_list` and use the specified advanced cryptography algorithms.
