# Conformance: Identity Token Policy

This project treats user identity handling as part of `ActivateSession`.
OPC-10000-4 §5.7.3 defines `ActivateSession`, and §5.7.3.2 places the
`userIdentityToken` in that request after the client signature, client software
certificates, and locale ids. The token accepted at activation must be one of
the user-token policies advertised for the selected endpoint.

## Default Policy

| Endpoint policy | Advertised user-token policy | `ActivateSession` behavior | Evidence scope |
|---|---|---|---|
| `SecurityPolicy#None` | `AnonymousIdentityToken` | Accepted for interoperability. | Default and profile-targeting flows keep anonymous-over-None usable. |
| `SecurityPolicy#None` | `UserNameIdentityToken` | Not advertised and rejected by default with `Bad_IdentityTokenRejected`. | OPC-10000-4 §5.7.3.3 defines `ActivateSession` service results; current unit and integration tests require plaintext username/password-over-None rejection. |
| Non-None secured policies | `UserNameIdentityToken` when `MICRO_OPCUA_USER_AUTH=ON` | Scoped implemented username/password activation path. | Evidence is limited to builds and tests that enable user authentication and a protecting security policy. |
| Secured policies with certificate support | Certificate/X509 user token where the security build and crypto adapter support it. | Scoped implemented activation path. | Evidence is limited to security-enabled builds with the required crypto adapter hooks. |

`SecurityPolicy#None` remains valid for anonymous interoperability and
non-production testing. It must not be used as an advertised or accepted default
path for plaintext username/password credentials. This keeps `GetEndpoints`
metadata and `ActivateSession` processing aligned: clients see anonymous tokens
on None endpoints, while username/password credentials require a secured endpoint
and the corresponding build/test evidence.
