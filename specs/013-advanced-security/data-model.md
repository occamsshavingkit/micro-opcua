# Data Model: Advanced Security Policies

## Entities

### `mu_security_policy_t` (Enum/Identifier)
Extend existing security policies with new IDs:
- `MU_SECURITY_POLICY_AES128_SHA256_RSAOAEP`
- `MU_SECURITY_POLICY_AES256_SHA256_RSAPSS`

### `mu_trust_list_t`
A struct encapsulating the list of trusted certificates.
- **Fields**:
  - `certificates`: Array of pointers to certificate byte arrays (DER encoded).
  - `lengths`: Array of lengths corresponding to the certificates.
  - `count`: Number of certificates in the list.

### `mu_crypto_adapter_t`
The crypto adapter interface will need to be extended to support RSA-OAEP and RSA-PSS, or its existing asymmetric functions will be parameterized by the security policy.
- `rsa_oaep_decrypt` / `rsa_oaep_encrypt`
- `rsa_pss_sign` / `rsa_pss_verify`

## State Transitions
During `OpenSecureChannel`:
- If the policy is `Aes128-Sha256-RsaOaep` or `Aes256-Sha256-RsaPss`, the server parses the client's certificate.
- The server checks if the client certificate exactly matches any certificate in the configured `mu_trust_list_t`.
- If matched, proceeds with secure channel establishment.
- If not matched, returns `Bad_SecurityChecksFailed` or `Bad_CertificateUntrusted`.
