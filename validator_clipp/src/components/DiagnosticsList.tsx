import React from "react";
import { Info, AlertTriangle, XCircle } from "lucide-react";
import { Diagnostic, DiagnosticSeverity } from "@/types/validation";

const severityConfig: Record<DiagnosticSeverity, { icon: React.ReactNode; bg: string; text: string; border: string }> = {
  info: {
    icon: <Info className="h-4 w-4" />,
    bg: "bg-info/10",
    text: "text-info",
    border: "border-info/30",
  },
  warning: {
    icon: <AlertTriangle className="h-4 w-4" />,
    bg: "bg-warning/10",
    text: "text-warning",
    border: "border-warning/30",
  },
  error: {
    icon: <XCircle className="h-4 w-4" />,
    bg: "bg-destructive/10",
    text: "text-destructive",
    border: "border-destructive/30",
  },
};

interface DiagnosticsListProps {
  diagnostics: Diagnostic[];
}

const DiagnosticsList: React.FC<DiagnosticsListProps> = ({ diagnostics }) => {
  if (diagnostics.length === 0) return null;

  return (
    <div className="rounded-lg border bg-card p-6">
      <h2 className="text-sm font-semibold uppercase tracking-wider text-muted-foreground mb-4">
        Diagnostics
      </h2>
      <div className="space-y-3">
        {diagnostics.map((d, i) => {
          const config = severityConfig[d.severity];
          return (
            <div key={i} className={`rounded-md border ${config.border} ${config.bg} px-4 py-3`}>
              <div className="flex items-start gap-3">
                <div className={`${config.text} mt-0.5 shrink-0`}>{config.icon}</div>
                <div className="min-w-0">
                  <div className="flex items-center gap-2 flex-wrap">
                    <span className="font-mono text-xs font-semibold">{d.code}</span>
                    <span className={`text-[10px] font-bold uppercase tracking-wider ${config.text}`}>
                      {d.severity}
                    </span>
                  </div>
                  <p className="mt-1 text-sm">{d.message}</p>
                  {d.context && (
                    <p className="mt-1 font-mono text-xs text-muted-foreground">
                      {d.context}
                    </p>
                  )}
                </div>
              </div>
            </div>
          );
        })}
      </div>
    </div>
  );
};

export default DiagnosticsList;
