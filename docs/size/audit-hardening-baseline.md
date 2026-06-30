# Audit Hardening Pre-Change Size Baseline

This records the pre-change audit-hardening size-report baseline for
`build/src/libmicro_opcua.a`, captured before production hardening changes.

T004a used the quickstart fallback command from
`specs/020-audit-hardening/quickstart.md` because this baseline document did not
exist yet.

## Command

```bash
./scripts/size-report.sh build/src/libmicro_opcua.a
```

T004a did not run `cmake --build build` because
`build/src/libmicro_opcua.a` already existed.

## Summed Baseline

| text | data | bss |
| ---: | ---: | --: |
| 152709 | 6224 | 0 |

## Raw Output

```text
Size report for: build/src/libmicro_opcua.a
   text	   data	    bss	    dec	    hex	filename
  11034	      0	      0	  11034	   2b1a	server.c.o (ex build/src/libmicro_opcua.a)
     32	      0	      0	     32	     20	status.c.o (ex build/src/libmicro_opcua.a)
   1952	      0	      0	   1952	    7a0	tcp_connection.c.o (ex build/src/libmicro_opcua.a)
   1568	      0	      0	   1568	    620	message_chunk.c.o (ex build/src/libmicro_opcua.a)
    309	      0	      0	    309	    135	sequence.c.o (ex build/src/libmicro_opcua.a)
    472	      0	      0	    472	    1d8	service_message.c.o (ex build/src/libmicro_opcua.a)
  40562	    816	      0	  41378	   a1a2	service_dispatch.c.o (ex build/src/libmicro_opcua.a)
   1022	      0	      0	   1022	    3fe	uasc.c.o (ex build/src/libmicro_opcua.a)
   1077	      0	      0	   1077	    435	secure_channel.c.o (ex build/src/libmicro_opcua.a)
   1372	      0	      0	   1372	    55c	session.c.o (ex build/src/libmicro_opcua.a)
    694	      0	      0	    694	    2b6	service_header.c.o (ex build/src/libmicro_opcua.a)
   3191	      0	      0	   3191	    c77	discovery.c.o (ex build/src/libmicro_opcua.a)
     32	      0	      0	     32	     20	alarms_conditions.c.o (ex build/src/libmicro_opcua.a)
    531	      0	      0	    531	    213	address_space.c.o (ex build/src/libmicro_opcua.a)
   6080	   5408	      0	  11488	   2ce0	base_nodes.c.o (ex build/src/libmicro_opcua.a)
   2270	      0	      0	   2270	    8de	node_id.c.o (ex build/src/libmicro_opcua.a)
    529	      0	      0	    529	    211	value_source.c.o (ex build/src/libmicro_opcua.a)
   2888	      0	      0	   2888	    b48	binary_reader.c.o (ex build/src/libmicro_opcua.a)
   2248	      0	      0	   2248	    8c8	binary_writer.c.o (ex build/src/libmicro_opcua.a)
    757	      0	      0	    757	    2f5	binary_string.c.o (ex build/src/libmicro_opcua.a)
   1730	      0	      0	   1730	    6c2	binary_nodeid.c.o (ex build/src/libmicro_opcua.a)
    722	      0	      0	    722	    2d2	binary_extension_object.c.o (ex build/src/libmicro_opcua.a)
   2644	      0	      0	   2644	    a54	binary_variant.c.o (ex build/src/libmicro_opcua.a)
    941	      0	      0	    941	    3ad	binary_datavalue.c.o (ex build/src/libmicro_opcua.a)
   1131	      0	      0	   1131	    46b	security_policy.c.o (ex build/src/libmicro_opcua.a)
     32	      0	      0	     32	     20	opcua_ids.c.o (ex build/src/libmicro_opcua.a)
   1943	      0	      0	   1943	    797	read.c.o (ex build/src/libmicro_opcua.a)
   4259	      0	      0	   4259	   10a3	browse.c.o (ex build/src/libmicro_opcua.a)
   7197	      0	      0	   7197	   1c1d	node_management.c.o (ex build/src/libmicro_opcua.a)
   1427	      0	      0	   1427	    593	query.c.o (ex build/src/libmicro_opcua.a)
   2629	      0	      0	   2629	    a45	binary_query.c.o (ex build/src/libmicro_opcua.a)
  20752	      0	      0	  20752	   5110	subscription.c.o (ex build/src/libmicro_opcua.a)
    749	      0	      0	    749	    2ed	pubsub.c.o (ex build/src/libmicro_opcua.a)
    323	      0	      0	    323	    143	uadp_encoder.c.o (ex build/src/libmicro_opcua.a)
    596	      0	      0	    596	    254	host_udp_adapter.c.o (ex build/src/libmicro_opcua.a)
    710	      0	      0	    710	    2c6	write.c.o (ex build/src/libmicro_opcua.a)
   3815	      0	      0	   3815	    ee7	history.c.o (ex build/src/libmicro_opcua.a)
   1219	      0	      0	   1219	    4c3	key_derivation.c.o (ex build/src/libmicro_opcua.a)
    492	      0	      0	    492	    1ec	certificate.c.o (ex build/src/libmicro_opcua.a)
   7051	      0	      0	   7051	   1b8b	asym_chunk.c.o (ex build/src/libmicro_opcua.a)
   4232	      0	      0	   4232	   1088	sym_chunk.c.o (ex build/src/libmicro_opcua.a)
    278	      0	      0	    278	    116	trustlist.c.o (ex build/src/libmicro_opcua.a)
  1636	      0	      0	   1636	    664	host_tcp_adapter.c.o (ex build/src/libmicro_opcua.a)
  7581	      0	      0	   7581	   1d9d	host_crypto_adapter.c.o (ex build/src/libmicro_opcua.a)
```

# Audit Hardening Pre-Change Stack-Usage Baseline

This records the pre-change audit-hardening stack-usage baseline captured before
production hardening changes. This is evidence for the starting point only, not a
post-change comparison.

T004c used the existing stack-usage build flow because this baseline document did
not yet contain a stack command.

## Commands

```bash
cmake --build build/audit-embedded-stack
bash scripts/check_stack_usage.sh --build-dir build/audit-embedded-stack --threshold 10240
```

## Summary

| estimate | threshold | result |
| ---: | ---: | :--- |
| 3040 bytes | 10240 bytes | PASS |

**Method**: `handle_data_chunk_secure + max(asymmetric unwrap/wrap phase, OpenSecureChannel dispatch/KDF phase)`

## Frames

```text
src/core/server.c:handle_data_chunk_secure        0 (not emitted)
src/security/asym_chunk.c:mu_asym_chunk_unwrap      304
src/security/asym_chunk.c:mu_asym_chunk_wrap     2912
src/security/asym_chunk.c:max_helper            128
src/core/service_dispatch.c:mu_service_dispatch      176
src/core/service_dispatch.c:handle_open_secure_channel      256
src/services/secure_channel.c:mu_secure_channel_open       48
src/security/sym_chunk.c:mu_sym_keys_derive      272
src/security/key_derivation.c:mu_p_sha256       288
```

## Phases

```text
asymmetric unwrap/wrap                         3040
OpenSecureChannel dispatch/KDF                  992
```

## Raw Stack-Usage Output

```text
Secured OPN stack estimate: 3040 bytes
Threshold: 10240 bytes
Method: handle_data_chunk_secure + max(asymmetric unwrap/wrap phase, OpenSecureChannel dispatch/KDF phase)
Frames:
  src/core/server.c:handle_data_chunk_secure        0 (not emitted)
  src/security/asym_chunk.c:mu_asym_chunk_unwrap      304
  src/security/asym_chunk.c:mu_asym_chunk_wrap     2912
  src/security/asym_chunk.c:max_helper            128
  src/core/service_dispatch.c:mu_service_dispatch      176
  src/core/service_dispatch.c:handle_open_secure_channel      256
  src/services/secure_channel.c:mu_secure_channel_open       48
  src/security/sym_chunk.c:mu_sym_keys_derive      272
  src/security/key_derivation.c:mu_p_sha256       288
Phases:
  asymmetric unwrap/wrap                         3040
  OpenSecureChannel dispatch/KDF                  992
PASS: secured OPN stack estimate is within threshold
```
