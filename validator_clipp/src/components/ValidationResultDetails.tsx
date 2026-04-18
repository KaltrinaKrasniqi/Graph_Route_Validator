import React from "react";
import ResultSummaryCard from "@/components/ResultSummaryCard";
import ValidationFlow from "@/components/ValidationFlow";
import DiagnosticsList from "@/components/DiagnosticsList";
import ParsedSummaryCard from "@/components/ParsedSummaryCard";
import ScoreBreakdownCard from "@/components/ScoreBreakdownCard";
import RawJsonPanel from "@/components/RawJsonPanel";
import { ValidationResponse } from "@/types/validation";

interface ValidationResultDetailsProps {
  result: ValidationResponse;
}

const ValidationResultDetails: React.FC<ValidationResultDetailsProps> = ({ result }) => {
  return (
    <div className="space-y-4">
      <ResultSummaryCard result={result} />
      <ValidationFlow steps={result.steps} />
      <DiagnosticsList diagnostics={result.diagnostics} />
      {result.parsedSummary && <ParsedSummaryCard summary={result.parsedSummary} />}
      {result.scoreBreakdown && <ScoreBreakdownCard breakdown={result.scoreBreakdown} />}
      <RawJsonPanel data={result} />
    </div>
  );
};

export default ValidationResultDetails;
