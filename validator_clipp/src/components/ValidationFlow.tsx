import React from "react";
import { CheckCircle2, XCircle } from "lucide-react";
import { EvaluationStep } from "@/types/validation";

interface ValidationFlowProps {
  steps: EvaluationStep[];
}

const ValidationFlow: React.FC<ValidationFlowProps> = ({ steps }) => {
  return (
    <div className="rounded-lg border bg-card p-6">
      <h2 className="text-sm font-semibold uppercase tracking-wider text-muted-foreground mb-4">
        Evaluation Flow
      </h2>
      <div className="space-y-0">
        {steps.map((step, i) => (
          <div key={i} className="flex gap-4">
            {/* Timeline line */}
            <div className="flex flex-col items-center">
              <div className={`rounded-full p-0.5 ${step.success ? "text-success" : "text-destructive"}`}>
                {step.success ? (
                  <CheckCircle2 className="h-5 w-5" />
                ) : (
                  <XCircle className="h-5 w-5" />
                )}
              </div>
              {i < steps.length - 1 && (
                <div className="w-px flex-1 bg-border my-1" />
              )}
            </div>
            {/* Content */}
            <div className="pb-5">
              <p className="text-sm font-medium leading-none">{step.name}</p>
              <p className="mt-1 text-xs text-muted-foreground">{step.details}</p>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};

export default ValidationFlow;
