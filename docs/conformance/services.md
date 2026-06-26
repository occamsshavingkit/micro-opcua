# Conformance: Services Support Matrix

Status of each OPC UA Service in micro-opcua. `Implemented` means handled with a
spec-correct response and covered by tests; `Unsupported` returns
`Bad_ServiceUnsupported`. Single client / channel / session.

| Service | OPC 10000-4 § | Status | Notes |
|---------|---------------|--------|-------|
| FindServers | 5.5.2 | Implemented | Returns this server's ApplicationDescription |
| GetEndpoints | 5.5.3 | Implemented | None + Basic256Sha256 (Sign / SignAndEncrypt) endpoints |
| OpenSecureChannel | 5.6.2 | Implemented | None and Basic256Sha256 (asymmetric handshake) |
| CloseSecureChannel | 5.6.3 | Implemented | |
| CreateSession | 5.7.2 | Implemented | ServerSignature on secured channels; ServerEndpoints |
| ActivateSession | 5.7.3 | Implemented | Anonymous identity only (else `Bad_IdentityTokenInvalid`) |
| CloseSession | 5.7.4 | Implemented | |
| Read | 5.10.2 | Implemented | Attribute Read incl. the Base Information nodes |
| Browse | 5.9.2 | Implemented | HierarchicalReferences + includeSubtypes |
| BrowseNext | 5.9.3 | Implemented | No continuation points issued -> `Bad_ContinuationPointInvalid` |
| TranslateBrowsePathsToNodeIds | 5.9.4 | Implemented | RelativePath walk over the address space; `Bad_NoMatch` |
| RegisterNodes | 5.9.5 | Implemented | Identity mapping (NodeIds copied back) |
| UnregisterNodes | 5.9.6 | Implemented | No-op, returns Good |
| Write | 5.10.3 | Unsupported | Not in Nano |
| Call | 5.11.2 | Unsupported | Method Service Set (not in Nano) |
| CreateSubscription / MonitoredItems / Publish | 5.13 | Unsupported | Subscriptions are the Micro profile, not Nano |
| HistoryRead / HistoryUpdate | 5.10 | Unsupported | Not in Nano |
| AddNodes / DeleteNodes / AddReferences / DeleteReferences | 5.7 | Unsupported | NodeManagement (not in Nano) |
| QueryFirst / QueryNext | 5.9.x | Unsupported | Query (not in Nano) |

The View Service Set (Browse, BrowseNext, TranslateBrowsePaths, RegisterNodes,
UnregisterNodes) is the set of Core Server Facet "View" conformance units required
by Nano; all are now implemented (`tests/integration/test_view_services.c`).
