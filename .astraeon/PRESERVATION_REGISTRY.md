# ASTRAEON Preservation Registry

## Rule

Everything already implemented is a protected asset.

Preserve -> Verify -> Enhance -> Integrate.

No existing ASTRAEON-X system may be rewritten without proven benefit,
compatibility plan, migration path, rollback plan, and Architect approval.

## Protected Systems

| System | Owner | Repository | Layer | Dependencies | Maturity | Future Evolution |
| --- | --- | --- | --- | --- | --- | --- |
| Runtime | ASTRAEON-X | RogueMaster | Kernel | SDK, event bus, logger | L3 Software | Kernel compatibility layer and runtime API stability. |
| Session Engine | ASTRAEON-X | RogueMaster | Kernel | Runtime, event bus, persistence | L3 Software | Mission/session correlation across domains. |
| Event Bus | ASTRAEON-X | RogueMaster | Kernel | Runtime, dispatcher, handlers | L3 Software | Cross-domain event gateway and replay. |
| Logger | ASTRAEON-X | RogueMaster | Data Platform | Runtime, storage | L3 Software | Structured cross-domain logging. |
| Persistence | ASTRAEON-X | RogueMaster | Data Platform | Storage, events, logger | L3 Software | Event store and Time Machine integration. |
| Validation | ASTRAEON-X | RogueMaster | Laboratory | Runtime tests, AEP, CI | L3 Software | Recipe-driven hardware validation. |
| SDK | ASTRAEON-X | RogueMaster | Developer | Runtime, public API, adapters | L3 Software | Embedded SDK v1 stability and plugin SDK. |
| GPIO | ASTRAEON-X | RogueMaster | Hardware | SDK, device HAL, controller | L3 Software | Hardware validation after known-good fixture. |
| UART | ASTRAEON-X | RogueMaster | Hardware | SDK, serial adapter, controller | L3 Software | TX/RX hardware validation and Probe endpoint. |
| SPI | ASTRAEON-Hardware | Future | Hardware | SDK, Probe, validation recipes | L0 Concept | Probe SPI tool and validation recipes. |
| SAFE PATCH | Governance | RogueMaster | Governance | Git, contract, CI | L3 Software | Cross-repository safety policy. |
| Engineering Contract | Governance | RogueMaster | Governance | AEO, Architect approvals | L3 Software | Policy engine input. |
| AEP | ASTRAEON-X | RogueMaster | Laboratory | Runtime tests, public API, architecture audit | L3 Software | Shared verification profile registry. |
| CI | ASTRAEON-X | RogueMaster | Laboratory | AEP, fbt, runtime tests | L3 Software | Multi-repository validation orchestration. |
| Release Pipeline | Developer | RogueMaster | Developer | Git, CI, auto release script | L3 Software | Release manager and remote sync evidence. |
| Executive Portal | Commander | Existing portal stack | Commander | Streamlit, FastAPI, data services | L1 Architecture | Preserve URL and evolve into portal registry. |
| Engineering Portal | Commander | Existing portal stack | Commander | CI, Git, AEO, telemetry | L1 Architecture | Engineering dashboard for releases and risks. |
| Operations Portal | Commander | Existing portal stack | Commander | Business, communication, tasks | L1 Architecture | Daily operations workflow. |
| Current Streamlit Apps | Commander | Existing app stack | Commander | Python, service APIs | L1 Architecture | Register pages and data dependencies. |
| FastAPI Services | Infrastructure | Node01 services | Infrastructure | Docker, Redis, data stores | L1 Architecture | Service registry and health model. |
| Node01 Services | Infrastructure | Node01 | Infrastructure | Docker, MQTT, Grafana, storage | L1 Architecture | Managed service inventory. |
| Grafana | Infrastructure | Node01 | Infrastructure | Prometheus, telemetry | L1 Architecture | Unified telemetry dashboards. |
| Prometheus | Infrastructure | Node01 | Infrastructure | exporters, service metrics | L1 Architecture | Metrics source for health and alerting. |
| MQTT | Infrastructure | Node01 | Communication | broker, devices, events | L1 Architecture | Event notification and device bridge. |
| Home Assistant | Home | Home stack | Home | MQTT, ESP32, sensors | L1 Architecture | Home domain integration. |
| Slip OCR | Business | Business stack | Business | OCR, storage, AI | L1 Architecture | Receipt and accounting pipeline. |
| Receipt OCR | Business | Business stack | Business | OCR, business DB | L1 Architecture | Evidence-preserving expense flow. |
| POS | Business | Business stack | Business | inventory, accounting, dashboard | L1 Architecture | Daily sales and reporting. |
| Accounting | Business | Business stack | Business | receipt OCR, POS, cashflow | L1 Architecture | Auditable accounting records. |
| Cashflow | Business | Business stack | Business | accounting, sales, purchase | L1 Architecture | Executive cashflow dashboard. |
| Inventory | Business | Business stack | Business | POS, purchase, supplier | L1 Architecture | Stock and food-cost control. |
| Media Pipeline | Media | Media stack | Media | FX3, DJI, storage | L0 Concept | Event-linked media evidence. |
| Weather Layer | Cloud | Future service | Cloud | weather API, location | L0 Concept | Context for automation and reporting. |
| Environment Layer | Home | Home stack | Home | sensors, Home Assistant | L0 Concept | Environmental telemetry. |
| Location Layer | Cloud | Future service | Cloud | maps, devices, privacy policy | L0 Concept | Location-aware missions. |
| DJI Integration | Media | Future service | Media | DJI device, media pipeline | L0 Concept | Drone media and robotics evidence. |
| FX3 Pipeline | Media | Future service | Media | FX3 camera, media DB | L0 Concept | Capture workflow and AI editing. |
