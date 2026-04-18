import React from "react";
import { BatchValidationSummary } from "@/types/validation";

interface BatchSummaryCardProps {
  summary: BatchValidationSummary;
}

const fields: { key: keyof BatchValidationSummary; label: string }[] = [
  { key: "totalInputFiles", label: "Input Files" },
  { key: "totalOutputFiles", label: "Output Files" },
  { key: "matchedPairs", label: "Matched Pairs" },
  { key: "validRuns", label: "Valid Runs" },
  { key: "invalidRuns", label: "Invalid Runs" },
  { key: "totalScoreAcrossValidInstances", label: "Total Valid Score" },
];

const BatchSummaryCard: React.FC<BatchSummaryCardProps> = ({ summary }) => {
  return (
    <div className="rounded-lg border bg-card p-6">
      <h2 className="mb-4 text-sm font-semibold uppercase tracking-wider text-muted-foreground">
        Batch Summary
      </h2>
      <div className="grid gap-3 sm:grid-cols-2 xl:grid-cols-3">
        {fields.map(({ key, label }) => (
          <div key={String(key)} className="rounded-md bg-muted/50 px-3 py-2.5">
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

export default BatchSummaryCard;
