import React from "react";
import { ParsedSummary } from "@/types/validation";

interface ParsedSummaryCardProps {
  summary: ParsedSummary;
}

const fields: { key: keyof ParsedSummary; label: string }[] = [
  { key: "nodes", label: "Nodes" },
  { key: "streets", label: "Streets" },
  { key: "vehicles", label: "Vehicles" },
  { key: "depot", label: "Depot" },
  { key: "timeLimit", label: "Time Limit" },
  { key: "wastePenalty", label: "Waste Penalty" },
  { key: "mandatoryStreetCount", label: "Mandatory Streets" },
  { key: "optionalStreetCount", label: "Optional Streets" },
  { key: "connectorStreetCount", label: "Connector Streets" },
];

const ParsedSummaryCard: React.FC<ParsedSummaryCardProps> = ({ summary }) => {
  return (
    <div className="rounded-lg border bg-card p-6">
      <h2 className="text-sm font-semibold uppercase tracking-wider text-muted-foreground mb-4">
        Parsed Instance Summary
      </h2>
      <div className="grid grid-cols-2 sm:grid-cols-3 gap-3">
        {fields.map(({ key, label }) => (
          <div key={key} className="rounded-md bg-muted/50 px-3 py-2.5">
            <p className="text-[11px] font-medium uppercase tracking-wider text-muted-foreground">
              {label}
            </p>
            <p className="mt-0.5 text-lg font-semibold font-mono">{summary[key]}</p>
          </div>
        ))}
      </div>
    </div>
  );
};

export default ParsedSummaryCard;
