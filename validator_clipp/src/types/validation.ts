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
  alpha: number;
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
  alpha: number;
  lMax: number;
  wMax: number;
  coverage: number;
  efficiency: number;
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

export type BatchValidationItemStatus =
  | "valid"
  | "invalid"
  | "error"
  | "missing_input"
  | "missing_output"
  | "duplicate_input"
  | "duplicate_output";

export interface BatchValidationItem {
  key: string;
  displayName: string;
  inputFileName?: string;
  outputFileName?: string;
  status: BatchValidationItemStatus;
  message: string;
  result: ValidationResponse | null;
}

export interface BatchValidationSummary {
  totalInputFiles: number;
  totalOutputFiles: number;
  matchedPairs: number;
  validRuns: number;
  invalidRuns: number;
  totalScoreAcrossValidInstances: number;
}
