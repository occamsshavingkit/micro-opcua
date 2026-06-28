# Feature Specification: Advanced Security Policies

**Feature Branch**: `013-advanced-security`  
**Created**: 2026-06-28  
**Status**: Draft  
**Input**: User description: "Implement Aes128-Sha256-RsaOaep and Aes256-Sha256-RsaPss. Add basic TrustList management for rejecting unknown client certificates."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Secure Channel with Aes128-Sha256-RsaOaep (Priority: P1)

A client application connects to the embedded server requesting the `Aes128-Sha256-RsaOaep` security policy. The server and client successfully establish the secure channel and communicate using the appropriate cryptographic algorithms for encryption and signature.

**Why this priority**: Required to support standard IoT clients that enforce the `Aes128-Sha256-RsaOaep` security policy.

**Independent Test**: Can be tested by configuring an external OPC UA client (e.g., UaExpert) to connect using only this security policy and verifying telemetry reads work.

**Acceptance Scenarios**:

1. **Given** a server configured to accept `Aes128-Sha256-RsaOaep`, **When** the client initiates `OpenSecureChannel` with this policy, **Then** the server replies with a valid response encrypted and signed using `Aes128-Sha256-RsaOaep`.

---

### User Story 2 - Secure Channel with Aes256-Sha256-RsaPss (Priority: P1)

A client application connects to the embedded server requesting the `Aes256-Sha256-RsaPss` security policy. The server correctly establishes the secure channel.

**Why this priority**: Represents the highest tier of standardized OPC UA security profiles commonly mandated in critical infrastructure.

**Independent Test**: Can be tested by configuring an external OPC UA client to connect using only `Aes256-Sha256-RsaPss`.

**Acceptance Scenarios**:

1. **Given** a server configured to accept `Aes256-Sha256-RsaPss`, **When** the client initiates `OpenSecureChannel` with this policy, **Then** the server replies with a valid response encrypted and signed using `Aes256-Sha256-RsaPss`.

---

### User Story 3 - Client Certificate TrustList Management (Priority: P2)

The server administrator provisions a list of trusted client certificates (TrustList) onto the device. When a new client attempts to connect, the server validates the client's certificate against the TrustList. If the certificate is unknown, the connection is rejected.

**Why this priority**: Provides the foundation for application-level authentication, ensuring only authorized clients can open a secure channel.

**Independent Test**: Can be tested by attempting to connect with two different certificates—one in the TrustList, one not—and verifying the latter is rejected.

**Acceptance Scenarios**:

1. **Given** a server with a configured TrustList, **When** an unknown client sends an `OpenSecureChannel` request, **Then** the server rejects the request with `Bad_SecurityChecksFailed`.
2. **Given** a server with a configured TrustList, **When** a trusted client sends an `OpenSecureChannel` request, **Then** the server accepts the connection.

### Edge Cases

- What happens when a client sends a request with an unsupported security policy URI? (Must return `Bad_SecurityPolicyRejected`)
- How does the system handle certificate validation when the TrustList is empty?
- What happens if the TrustList exceeds the maximum allocated size in the zero-heap environment?

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST implement the `Aes128-Sha256-RsaOaep` security policy algorithms for symmetric and asymmetric cryptography.
- **FR-002**: System MUST implement the `Aes256-Sha256-RsaPss` security policy algorithms.
- **FR-003**: System MUST provide an API (`mu_server_config` or similar) to provide a static TrustList (array of certificates).
- **FR-004**: System MUST validate client certificates against the TrustList during the `OpenSecureChannel` handshake.
- **FR-005**: System MUST reject untrusted certificates with an appropriate status code (e.g., `Bad_SecurityChecksFailed`).
- **FR-006**: System MUST allow configuring whether TrustList enforcement is strict or if it allows any certificate. [NEEDS CLARIFICATION: Should the server default to accepting all certificates if no TrustList is provided, or should it strictly require a TrustList for Secure connection?]

### OPC UA Normative Scope *(mandatory for protocol features)*

- **OPC-001**: Target OPC UA role/profile assumption is Micro Embedded Device Server Profile.
- **OPC-002**: Implemented security policies are `http://opcfoundation.org/UA/SecurityPolicy#Aes128_Sha256_RsaOaep` and `http://opcfoundation.org/UA/SecurityPolicy#Aes256_Sha256_RsaPss` (OPC 10000-7).
- **OPC-003**: Unsupported security policies return `Bad_SecurityPolicyRejected` (OPC 10000-4, section 5.5).
- **OPC-004**: Wire encoding/transport requirements cite OPC-10000-6 for UASC Security Headers and Asymmetric/Symmetric message chunk layouts.
- **OPC-005**: TrustList functionality corresponds to basic Application Authentication (OPC 10000-4, section 5.6.2).

### Scope Boundaries *(mandatory)*

- **In Scope**: Cryptographic implementations for the new security policies, exact matching of client certificates against a statically provided list of trusted certificates (TrustList).
- **Out of Scope**: Certificate revocation lists (CRL), complex certificate chain validation (CA roots), dynamic push/pull of TrustLists via OPC UA methods (Global Discovery Server integration).
- **Compatibility Claim**: `Aes128-Sha256-RsaOaep` and `Aes256-Sha256-RsaPss` facets.
- **Application Headroom Goal**: The cryptographic primitives must be designed to avoid heap allocation, executing securely in fixed buffers to preserve zero-heap architecture.

### Key Entities *(include if feature involves data)*

- **SecurityPolicy**: Defines the specific algorithms to use (symmetric encryption, asymmetric encryption, hashing, key derivation).
- **TrustList**: A collection of approved client X.509 certificates.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Clients can successfully connect and exchange data using the `Aes128-Sha256-RsaOaep` security policy in integration tests.
- **SC-002**: Clients can successfully connect and exchange data using the `Aes256-Sha256-RsaPss` security policy in integration tests.
- **SC-003**: 100% of connection attempts from untrusted client certificates are rejected during `OpenSecureChannel` when TrustList is enforced.
- **SC-004**: Implementation of new cryptographic policies adds 0 bytes of dynamic heap allocation.

## Assumptions

- Standard platform cryptographic adapters (mbedTLS, WolfSSL) natively support the required algorithms (RSA-OAEP, RSA-PSS).
- TrustList matching will initially be done by exact byte-for-byte comparison of the DER encoded certificate.
