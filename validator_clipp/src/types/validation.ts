export type ValidatorStatus =
  | "ERROR_INSTANCE"
  | "ERROR_SUBMISSION_FORMAT"
  | "INVALID"
  | "VALID";

export type DiagnosticSeverity = "info" | "warning" | "error";

export interface ParsedSummary {
  nodes: number;
  streets: number;
  vehicles: number;
  depot: number;
  timeLimit: number;
  wastePenalty: number;
  mandatoryStreetCount: number;
  optionalStreetCount: number;
  connectorStreetCount: number;
}

export interface EvaluationStep {
  name: string;
  success: boolean;
  details: string;
}

export interface Diagnostic {
  severity: DiagnosticSeverity;
  code: string;
  message: string;
  context: string;
}

export interface ScoreBreakdown {
  cleanedMandatoryCount: number;
  cleanedOptionalCount: number;
  distinctCleanedLength: number;
  totalWaste: number;
  wastePenalty: number;
  finalScore: number;
}

export interface ValidationResponse {
  status: ValidatorStatus;
  summary: string;
  score: number;
  parsedSummary: ParsedSummary | null;
  steps: EvaluationStep[];
  diagnostics: Diagnostic[];
  scoreBreakdown: ScoreBreakdown | null;
}
