import { ValidationResponse } from "@/types/validation";

export const mockValidResponse: ValidationResponse = {
  status: "VALID",
  summary: "Validation successful. Submission satisfies all hard constraints.",
  score: 736.8,
  parsedSummary: {
    nodes: 6,
    streets: 10,
    vehicles: 3,
    depot: 0,
    timeLimit: 300,
    wastePenalty: 1,
    mandatoryStreetCount: 3,
    optionalStreetCount: 4,
    connectorStreetCount: 3,
  },
  steps: [
    { name: "Parse instance", success: true, details: "Instance parsed successfully." },
    { name: "Validate instance", success: true, details: "All instance-level checks passed." },
    { name: "Parse submission", success: true, details: "Submission parsed successfully." },
    { name: "Validate submission format", success: true, details: "Submission format is valid." },
    { name: "Validate submission semantics", success: true, details: "All semantic checks passed." },
    { name: "Compute score", success: true, details: "Final score computed successfully." },
  ],
  diagnostics: [
    { severity: "info", code: "VALIDATION_OK", message: "Submission is valid and scorable.", context: "" },
  ],
  scoreBreakdown: {
    cleanedMandatoryCount: 3,
    cleanedOptionalCount: 2,
    distinctCleanedLength: 750,
    totalWaste: 13.2,
    wastePenalty: 1,
    finalScore: 736.8,
  },
};

export const mockInvalidResponse: ValidationResponse = {
  status: "INVALID",
  summary: "Submission invalid: vehicle 2 exceeds the time limit.",
  score: 0,
  parsedSummary: {
    nodes: 6,
    streets: 10,
    vehicles: 3,
    depot: 0,
    timeLimit: 300,
    wastePenalty: 1,
    mandatoryStreetCount: 3,
    optionalStreetCount: 4,
    connectorStreetCount: 3,
  },
  steps: [
    { name: "Parse instance", success: true, details: "Instance parsed successfully." },
    { name: "Validate instance", success: true, details: "All instance-level checks passed." },
    { name: "Parse submission", success: true, details: "Submission parsed successfully." },
    { name: "Validate submission format", success: true, details: "Submission format is valid." },
    { name: "Validate submission semantics", success: false, details: "Vehicle 2 total time = 315 exceeds limit T = 300." },
  ],
  diagnostics: [
    { severity: "error", code: "SUB_TIME_LIMIT", message: "Vehicle 2 exceeds the maximum allowed traversal time.", context: "vehicle=2" },
  ],
  scoreBreakdown: null,
};

export const mockErrorResponse: ValidationResponse = {
  status: "ERROR_INSTANCE",
  summary: "Instance error: duplicate street pair detected between nodes 3 and 5.",
  score: 0,
  parsedSummary: null,
  steps: [
    { name: "Parse instance", success: true, details: "Instance parsed successfully." },
    { name: "Validate instance", success: false, details: "Duplicate street pair detected for endpoints (3, 5)." },
  ],
  diagnostics: [
    { severity: "error", code: "INST_DUPLICATE_EDGE", message: "The instance contains more than one street for the same node pair.", context: "u=3,v=5" },
  ],
  scoreBreakdown: null,
};

export const mockResponses = {
  VALID: mockValidResponse,
  INVALID: mockInvalidResponse,
  ERROR_INSTANCE: mockErrorResponse,
};
