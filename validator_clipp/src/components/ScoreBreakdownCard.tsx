import React from "react";
import { ScoreBreakdown } from "@/types/validation";

interface ScoreBreakdownCardProps {
  breakdown: ScoreBreakdown;
}

const fields: { key: keyof ScoreBreakdown; label: string }[] = [
  { key: "cleanedMandatoryCount", label: "Cleaned Mandatory" },
  { key: "cleanedOptionalCount", label: "Cleaned Optional" },
  { key: "distinctCleanedLength", label: "Distinct Cleaned Length" },
  { key: "totalWaste", label: "Total Waste" },
  { key: "wastePenalty", label: "Waste Penalty" },
  { key: "finalScore", label: "Final Score" },
];

const ScoreBreakdownCard: React.FC<ScoreBreakdownCardProps> = ({ breakdown }) => {
  return (
    <div className="rounded-lg border bg-card p-6">
      <h2 className="text-sm font-semibold uppercase tracking-wider text-muted-foreground mb-4">
        Score Breakdown
      </h2>
      <div className="grid grid-cols-2 sm:grid-cols-3 gap-3">
        {fields.map(({ key, label }) => {
          const isFinal = key === "finalScore";
          return (
            <div
              key={key}
              className={`rounded-md px-3 py-2.5 ${isFinal ? "bg-primary/10 border border-primary/20 col-span-2 sm:col-span-1" : "bg-muted/50"}`}
            >
              <p className="text-[11px] font-medium uppercase tracking-wider text-muted-foreground">
                {label}
              </p>
              <p className={`mt-0.5 text-lg font-semibold font-mono ${isFinal ? "text-primary" : ""}`}>
                {breakdown[key]}
              </p>
            </div>
          );
        })}
      </div>
    </div>
  );
};

export default ScoreBreakdownCard;
