# ASTRAEON Data Map

## Data Stores

| Store | Purpose | Primary Producers | Primary Consumers |
| --- | --- | --- | --- |
| Event Store | Source-of-truth event history. | Runtime, Commander, Probe, Business, AI. | Time Machine, portals, AI, audits. |
| Telemetry Store | Metrics and health time series. | Runtime, Probe, Infrastructure, Home. | Dashboards, diagnostics, alerts. |
| Knowledge DB | Datasheets, protocols, lessons, prompts, repairs. | Engineers, AI, ingestion tools. | AI, Commander, Vision. |
| Business DB | Sales, POS, OCR, profit/loss, daily summaries. | Business Engine, OCR, POS. | Dashboards, AI assistant. |
| Inventory DB | Stock, suppliers, purchases, food cost. | POS, purchase, inventory workflows. | Business dashboard, AI. |
| Accounting DB | Receipts, cashflow, cost, profit/loss. | OCR, accounting workflows. | Executive portal, reports. |
| Media DB | FX3, DJI, streaming, image/video evidence. | Media pipelines. | Portals, AI, reports. |
| Hardware Registry | Boards, probes, pins, wiring, fixtures. | Hardware Lab, Commander. | Validation Engine, Vision. |
| Device Registry | Devices, capabilities, firmware, ownership. | Runtime, Probe, Infrastructure. | Commander, Security. |
| Mission Store | Long-running goals, plans, approvals, status. | Commander, AI, Automation. | Operators, AI, reports. |
| Time Machine Store | Event snapshots and replay indexes. | Event Store, telemetry snapshots. | Debugging, audits, Digital Twin. |
| AI Memory | Plans, diagnoses, operator preferences, lessons. | AI, Knowledge, handoff reports. | AI agents, semantic search. |
| Universe Graph | Links domains, services, APIs, events, and data. | Registries, docs, service maps. | Vision, AI, Governance. |

## Data Rules

- Data must identify source, timestamp, and ownership.
- Hardware validation data must include session id and evidence references.
- Business data must be auditable and reversible where practical.
- Secret data must never enter logs, events, or public reports.
- Retention rules must be explicit per store.
