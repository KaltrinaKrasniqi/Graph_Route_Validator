import React from "react";
import { CheckCircle2, XCircle, AlertTriangle, Trophy } from "lucide-react";
import { ValidationResponse, ValidatorStatus } from "@/types/validation";

const statusConfig: Record<ValidatorStatus, { icon: React.ReactNode; bg: string; text: string; border: string; label: string }> = {
  VALID: {
    icon: <CheckCircle2 className="h-6 w-6" />,
    bg: "bg-success/10",
    text: "text-success",
    border: "border-success/30",
    label: "VALID",
  },
  INVALID: {
    icon: <AlertTriangle className="h-6 w-6" />,
    bg: "bg-warning/10",
    text: "text-warning",
    border: "border-warning/30",
    label: "INVALID",
  },
  ERROR_INSTANCE: {
    icon: <XCircle className="h-6 w-6" />,
    bg: "bg-destructive/10",
    text: "text-destructive",
    border: "border-destructive/30",
    label: "ERROR",
  },
  ERROR_SUBMISSION_FORMAT: {
    icon: <XCircle className="h-6 w-6" />,
    bg: "bg-destructive/10",
    text: "text-destructive",
    border: "border-destructive/30",
    label: "ERROR",
  },
};

interface ResultSummaryCardProps {
  result: ValidationResponse;
}

const ResultSummaryCard: React.FC<ResultSummaryCardProps> = ({ result }) => {
  const config = statusConfig[result.status];
  const meta = result.parsedSummary;

  return (
    <div className={`rounded-lg border ${config.border} ${config.bg} p-6`}>
      <div className="flex items-start gap-4">
        <div className={`${config.text} mt-0.5`}>{config.icon}</div>
        <div className="flex-1 min-w-0">
          <div className="flex items-center gap-3 flex-wrap">
            <span className={`inline-flex items-center rounded-full px-3 py-1 text-xs font-bold tracking-wider ${config.bg} ${config.text} border ${config.border}`}>
              {config.label}
            </span>
            {result.score > 0 && (
              <span className="inline-flex items-center gap-1.5 rounded-full bg-card border px-3 py-1 text-sm font-semibold">
                <Trophy className="h-3.5 w-3.5 text-warning" />
                {result.score}
              </span>
            )}
          </div>
          <p className="mt-2 text-sm leading-relaxed">{result.summary}</p>
          {meta && (
            <div className="mt-3 flex flex-wrap gap-2">
              {[
                { label: "Nodes", value: meta.nodes },
                { label: "Streets", value: meta.streets },
                { label: "Vehicles", value: meta.vehicles },
                { label: "Depot", value: meta.depot },
                { label: "Time limit", value: meta.timeLimit },
              ].map(({ label, value }) => (
                <span key={label} className="inline-flex items-center gap-1 rounded bg-card border px-2 py-0.5 text-xs font-mono text-muted-foreground">
                  {label}: <span className="font-semibold text-foreground">{value}</span>
                </span>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  );
};

export default ResultSummaryCard;
