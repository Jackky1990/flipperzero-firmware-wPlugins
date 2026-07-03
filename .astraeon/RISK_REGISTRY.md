# ASTRAEON Risk Registry

| Risk Area | Risk | Impact | Mitigation | Status |
| --- | --- | --- | --- | --- |
| Business | OCR/accounting errors corrupt reports. | Incorrect operational decisions. | Preserve raw evidence, audit changes, human approval. | Open |
| Hardware | Unsafe wiring damages devices. | Hardware damage. | Observe before control, validation board, pin allowlists. | Open |
| Infrastructure | Node01 service drift. | Portal/service instability. | Service registry, Docker manifests, health checks. | Open |
| Cloud | External API outage or policy change. | AI/weather/maps/backup disruption. | Local fallback, retry policy, clear cloud boundary. | Open |
| AI | Ungrounded recommendations. | Bad engineering or business decisions. | Evidence citations, policy gates, human approval. | Open |
| Power | Device power instability. | Failed validation or device damage. | Known-good supplies, telemetry, lab checklist. | Open |
| Data | Loss or inconsistency across stores. | Broken audits and reports. | backups, event source of truth, Time Machine. | Open |
| Security | Secrets leaked through logs or prompts. | Account compromise. | secrets policy, audit, redaction, RBAC. | Open |
| Compliance | Business/accounting records incomplete. | Legal/accounting risk. | audit trail, retention, approval workflow. | Open |
| Scope | Universe docs mixed into firmware work. | Technical debt and confusion. | Keep docs in `.astraeon`; separate future repositories. | Open |

## Risk Rules

- Risks must have owner, mitigation, and status before production work.
- Hardware risks block execution until fixture and recovery are defined.
