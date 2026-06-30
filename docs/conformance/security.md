# Conformance: Security

**WARNING**: This server may expose `SecurityPolicy#None` endpoints for
anonymous interoperability and non-production testing.

- **GetEndpoints policy metadata**: OPC-10000-4 §5.5.4.2 defines the
  `GetEndpoints` response parameters, including each returned endpoint's security
  policy information. When this project advertises
  `http://opcfoundation.org/UA/SecurityPolicy#None`, that is documented endpoint
  policy metadata for interoperability, not a claim that the endpoint provides
  message security.
- **Non-production use only**: Endpoints using `SecurityPolicy#None` provide no
  message security and should only be used for anonymous access in trusted,
  isolated networks or for testing.
- **Credential handling**: This documentation does not endorse plaintext
  username/password credentials over `SecurityPolicy#None`; the default behavior
  must not advertise or accept those credentials over `None`.
- **Crypto-pluggability**: The architecture is designed to allow plugging in a
  cryptographic backend (e.g., mbedTLS, BearSSL) for secured endpoint policies.
