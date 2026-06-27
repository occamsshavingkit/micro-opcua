# micro-opcua profile build presets.
#
# Thin wrappers over CMake that produce a minimal_server example binary configured
# for each OPC UA server-profile tier. The profiles differ in the feature set that
# is compiled in (see the MICRO_OPCUA_* CMake options).
#
#   make nano      Nano Embedded Device 2017 Server Profile — SecurityPolicy None,
#                  the full Nano service surface (Discovery, Session, Read, the View
#                  Service Set) + the standard Base Information node set, subscriptions
#                  OFF. Complete.
#   make micro     Micro profile configuration — Nano + the data-change subscription
#                  engine (MICRO_OPCUA_SUBSCRIPTIONS=ON: the Embedded Data Change
#                  Subscription Server Facet). Distinct from nano. (Concurrent ≥2-session
#                  support is the remaining Micro item.)
#   make embedded  Embedded profile configuration — Micro + SecurityPolicy
#                  Basic256Sha256 ON (sign/encrypt). The Standard DataChange Subscription
#                  facet and full type-system exposure are still pending.
#   make all-profiles   build nano, micro and embedded
#   make test           configure with tests and run the full ctest suite
#   make clean          remove the profile build directories
#
# Example binary lands at build/<profile>/examples/minimal_server.

CMAKE ?= cmake
BUILD  ?= build
COMMON := -DMICRO_OPCUA_BUILD_EXAMPLES=ON -DMICRO_OPCUA_OPTIMIZE_SIZE=ON

.PHONY: help nano micro embedded all-profiles test clean

help:
	@echo "micro-opcua profile builds:"
	@echo "  make nano      Nano 2017 profile (None, subscriptions OFF)  -> $(BUILD)/nano"
	@echo "  make micro     Micro profile (None + subscriptions ON)      -> $(BUILD)/micro"
	@echo "  make embedded  Embedded config (Basic256Sha256 + subs ON)   -> $(BUILD)/embedded"
	@echo "  make test      build with tests and run ctest"
	@echo "  make clean     remove profile build directories"

nano:
	@echo ">> NANO profile: SecurityPolicy None, full Nano service surface, subscriptions OFF"
	$(CMAKE) -S . -B $(BUILD)/nano $(COMMON) -DMICRO_OPCUA_SECURITY=OFF -DMICRO_OPCUA_SUBSCRIPTIONS=OFF
	$(CMAKE) --build $(BUILD)/nano

micro:
	@echo ">> MICRO profile: Nano + data-change subscriptions (MICRO_OPCUA_SUBSCRIPTIONS=ON)"
	$(CMAKE) -S . -B $(BUILD)/micro $(COMMON) -DMICRO_OPCUA_SECURITY=OFF -DMICRO_OPCUA_SUBSCRIPTIONS=ON
	$(CMAKE) --build $(BUILD)/micro

embedded:
	@echo ">> EMBEDDED profile: Basic256Sha256 ON + subscriptions ON (NOTE: full type system pending)"
	$(CMAKE) -S . -B $(BUILD)/embedded $(COMMON) -DMICRO_OPCUA_SECURITY=ON -DMICRO_OPCUA_SUBSCRIPTIONS=ON
	$(CMAKE) --build $(BUILD)/embedded

all-profiles: nano micro embedded

test:
	$(CMAKE) -S . -B $(BUILD)/test -DMICRO_OPCUA_BUILD_TESTS=ON
	$(CMAKE) --build $(BUILD)/test
	cd $(BUILD)/test && ctest --output-on-failure

clean:
	rm -rf $(BUILD)/nano $(BUILD)/micro $(BUILD)/embedded $(BUILD)/test
