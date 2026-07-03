# ASTRAEON Business Capability

| Capability | Status | Dependencies | Next Milestone |
| --- | --- | --- | --- |
| POS | Planned | Business DB, inventory, dashboard | POS data model. |
| Accounting | Planned | receipt OCR, cashflow, reports | Accounting schema. |
| Inventory | Planned | POS, supplier, purchase | Inventory registry. |
| Supplier | Planned | purchase records, inventory | Supplier profile model. |
| CRM | Concept | customer records, communication | CRM boundary design. |
| Cashflow | Planned | sales, expenses, accounting | Cashflow dashboard. |
| Profit | Planned | POS, cost, accounting | P/L report. |
| Receipt OCR | Planned | OCR, media evidence | OCR evidence pipeline. |
| Slip OCR | Planned | OCR, payment evidence | Slip validation workflow. |
| LINE MAN | Planned | sales/import integration | LINE MAN GP data model. |
| Delivery | Concept | POS, communication | Delivery workflow map. |
| Analytics | Planned | Business DB, AI | Daily summary and insights. |

## Business Rules

- Business events use `business.*`.
- OCR outputs preserve raw evidence.
- Accounting changes are auditable.
- AI business assistant cannot mutate records without policy approval.
