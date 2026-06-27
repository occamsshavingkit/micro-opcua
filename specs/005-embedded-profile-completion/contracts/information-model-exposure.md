# Contract: Standard information-model (type-system) exposure

**Basis**: OPC-10000-3 (address space model, NodeClasses, standard references);
OPC-10000-5 (standard node set — ReferenceTypes, DataTypes, ObjectTypes, VariableTypes,
Server object); standard NodeIds per OPC-10000-6 Annex A. Exposed via Browse (§5.9.2) and
Read (§5.10). **Exact mandated node set is pinned in research.md.**
**Build gate**: `MICRO_OPCUA_BASE_NODES` (const static, flash only; nano/micro unaffected).

## Exposed nodes (delta over the current Nano/Micro base set)

1. **ReferenceType hierarchy** — the standard reference types the profile mandates, each with
   correct NodeClass=ReferenceType, BrowseName, IsAbstract, Symmetric, and InverseName, wired
   under References / HierarchicalReferences / NonHierarchicalReferences (OPC-10000-5 §11,
   OPC-10000-3 §7).
2. **DataType nodes** — the BaseDataType subtree mandated (built-in scalar types + any
   structured types exposed), NodeClass=DataType, HasSubtype-linked (OPC-10000-5 §12).
3. **ObjectType / VariableType nodes** — BaseObjectType, BaseVariableType, BaseDataVariableType,
   PropertyType, and any others mandated, HasSubtype-linked (OPC-10000-5 §6, §7).
4. **HasTypeDefinition references** — every exposed instance Object/Variable references its
   type node (OPC-10000-3 §7.7). Browsing an instance resolves to its type.
5. **Server object capability metadata** — fuller ServerCapabilities incl. OperationLimits
   (MaxNodesPerRead/Browse/etc.) reflecting actual server limits, and ServerProfileArray
   advertising the Embedded profile URI (OPC-10000-5 §6.3, §7).

## Behavior / validation (correctness gate)

- All added nodes are browseable (forward/inverse references resolve) and readable for their
  mandatory attributes (NodeId, NodeClass, BrowseName, DisplayName; type-specific attributes).
- Reading an unsupported attribute → `Bad_AttributeIdInvalid` (OPC-10000-4 §5.10.1); browsing
  an unknown node → `Bad_NodeIdUnknown`.
- Adding these nodes MUST NOT change nano/micro browse/read output (gated; profile-scoped).
- No heap, no mutable globals: nodes are `const`; resolution uses the existing
  `mu_resolve_node` fallback chain.

## Tests (test-first)

- Browse Root → Types → the mandated ReferenceType/DataType/ObjectType/VariableType nodes are
  reachable with correct attributes.
- An exposed instance's HasTypeDefinition resolves to the correct type node.
- Read Server.ServerCapabilities.OperationLimits and ServerProfileArray → expected values.
- nano/micro builds: golden browse/read vectors unchanged (regression).
