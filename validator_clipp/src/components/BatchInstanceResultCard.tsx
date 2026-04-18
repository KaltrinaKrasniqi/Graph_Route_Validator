import React from "react";
import { AlertTriangle, CheckCircle2, FileWarning, Files, XCircle } from "lucide-react";
import ValidationResultDetails from "@/components/ValidationResultDetails";
import { BatchValidationItem } from "@/types/validation";

const statusConfig: Record<
  BatchValidationItem["status"],
  { icon: React.ReactNode; text: string; bg: string; border: string; label: string }
> = {
  valid: {
    icon: <CheckCircle2 className="h-5 w-5" />,
    text: "text-success",
    bg: "bg-success/10",
    border: "border-success/30",
    label: "VALID",
  },
  invalid: {
    icon: <AlertTriangle className="h-5 w-5" />,
    text: "text-warning",
    bg: "bg-warning/10",
    border: "border-warning/30",
    label: "INVALID",
  },
  error: {
    icon: <XCircle className="h-5 w-5" />,
    text: "text-destructive",
    bg: "bg-destructive/10",
    border: "border-destructive/30",
    label: "ERROR",
  },
  missing_input: {
    icon: <FileWarning className="h-5 w-5" />,
    text: "text-warning",
    bg: "bg-warning/10",
    border: "border-warning/30",
    label: "MISSING INPUT",
  },
  missing_output: {
    icon: <FileWarning className="h-5 w-5" />,
    text: "text-warning",
    bg: "bg-warning/10",
    border: "border-warning/30",
    label: "MISSING OUTPUT",
  },
  duplicate_input: {
    icon: <Files className="h-5 w-5" />,
    text: "text-destructive",
    bg: "bg-destructive/10",
    border: "border-destructive/30",
    label: "DUPLICATE INPUT KEY",
  },
  duplicate_output: {
    icon: <Files className="h-5 w-5" />,
    text: "text-destructive",
    bg: "bg-destructive/10",
    border: "border-destructive/30",
    label: "DUPLICATE OUTPUT KEY",
  },
};

interface BatchInstanceResultCardProps {
  item: BatchValidationItem;
}

const BatchInstanceResultCard: React.FC<BatchInstanceResultCardProps> = ({ item }) => {
  const config = statusConfig[item.status];

  return (
    <article className="space-y-4 rounded-lg border bg-card p-5">
      <div className="flex flex-wrap items-start gap-4">
        <div className={`mt-0.5 ${config.text}`}>{config.icon}</div>
        <div className="min-w-0 flex-1">
          <div className="flex flex-wrap items-center gap-3">
            <h3 className="text-base font-semibold">{item.displayName}</h3>
            <span
              className={`inline-flex items-center rounded-full border px-3 py-1 text-[11px] font-bold tracking-wider ${config.bg} ${config.text} ${config.border}`}
            >
              {config.label}
            </span>
          </div>

          <p className="mt-2 text-sm text-muted-foreground">{item.message}</p>

          <div className="mt-3 flex flex-wrap gap-2">
            {item.inputFileName && (
              <span className="inline-flex items-center rounded bg-muted px-2 py-1 text-xs font-mono text-muted-foreground">
                input: {item.inputFileName}
              </span>
            )}
            {item.outputFileName && (
              <span className="inline-flex items-center rounded bg-muted px-2 py-1 text-xs font-mono text-muted-foreground">
                output: {item.outputFileName}
              </span>
            )}
          </div>
        </div>
      </div>

      {item.result && <ValidationResultDetails result={item.result} />}
    </article>
  );
};

export default BatchInstanceResultCard;
