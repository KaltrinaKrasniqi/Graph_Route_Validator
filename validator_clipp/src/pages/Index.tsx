import React, { useState, useCallback } from "react";
import { Download, Play, FlaskConical, FileUp } from "lucide-react";
import { Button } from "@/components/ui/button";
import { ValidationResponse } from "@/types/validation";
import { mockResponses } from "@/data/mockResponses";
import { runValidator } from "@/services/validator";
import FileUploadCard from "@/components/FileUploadCard";
import ResultSummaryCard from "@/components/ResultSummaryCard";
import ValidationFlow from "@/components/ValidationFlow";
import DiagnosticsList from "@/components/DiagnosticsList";
import ParsedSummaryCard from "@/components/ParsedSummaryCard";
import ScoreBreakdownCard from "@/components/ScoreBreakdownCard";
import RawJsonPanel from "@/components/RawJsonPanel";

type MockKey = "VALID" | "INVALID" | "ERROR_INSTANCE";

const Index: React.FC = () => {
  const [instanceFile, setInstanceFile] = useState<File | null>(null);
  const [submissionFile, setSubmissionFile] = useState<File | null>(null);
  const [instanceText, setInstanceText] = useState<string>("");
  const [submissionText, setSubmissionText] = useState<string>("");
  const [result, setResult] = useState<ValidationResponse | null>(null);
  const [loading, setLoading] = useState(false);

  const readFile = (file: File): Promise<string> =>
    new Promise((resolve, reject) => {
      const reader = new FileReader();
      reader.onload = () => resolve(reader.result as string);
      reader.onerror = reject;
      reader.readAsText(file);
    });

  const handleInstanceSelect = useCallback(async (file: File) => {
    setInstanceFile(file);
    setInstanceText(await readFile(file));
  }, []);

  const handleSubmissionSelect = useCallback(async (file: File) => {
    setSubmissionFile(file);
    setSubmissionText(await readFile(file));
  }, []);

  const handleValidate = async () => {
    if (!instanceText || !submissionText) return;
    setLoading(true);
    try {
      const res = await runValidator(instanceText, submissionText);
      setResult(res);
    } finally {
      setLoading(false);
    }
  };

  const handleMock = (key: MockKey) => {
    setResult(mockResponses[key]);
  };

  const handleDownload = () => {
    if (!result) return;
    const blob = new Blob([JSON.stringify(result, null, 2)], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = "validation-report.json";
    a.click();
    URL.revokeObjectURL(url);
  };

  return (
    <div className="min-h-screen bg-background">
      {/* Header */}
      <header className="border-b bg-card">
        <div className="mx-auto max-w-4xl px-4 py-8 sm:px-6">
          <h1 className="text-2xl font-bold tracking-tight sm:text-3xl">
            Graph Route Validator
          </h1>
          <p className="mt-1.5 text-sm text-muted-foreground leading-relaxed max-w-2xl">
            Validate problem instances and submissions, inspect diagnostics, and review score breakdowns.
          </p>
        </div>
      </header>

      <main className="mx-auto max-w-4xl px-4 py-8 sm:px-6 space-y-6">
        {/* Upload Section */}
        <section>
          <div className="grid gap-4 sm:grid-cols-2">
            <FileUploadCard
              label="Input Instance"
              description="Upload the problem instance file (.txt)"
              file={instanceFile}
              onFileSelect={handleInstanceSelect}
              onFileRemove={() => { setInstanceFile(null); setInstanceText(""); }}
            />
            <FileUploadCard
              label="Submission"
              description="Upload the submission file (.txt)"
              file={submissionFile}
              onFileSelect={handleSubmissionSelect}
              onFileRemove={() => { setSubmissionFile(null); setSubmissionText(""); }}
            />
          </div>

          <div className="mt-4 flex flex-wrap items-center gap-3">
            <Button
              onClick={handleValidate}
              disabled={!instanceText || !submissionText || loading}
              size="lg"
              className="gap-2"
            >
              <Play className="h-4 w-4" />
              {loading ? "Validating…" : "Validate"}
            </Button>

            <div className="flex items-center gap-2">
              <span className="text-xs text-muted-foreground">Mock:</span>
              {(["VALID", "INVALID", "ERROR_INSTANCE"] as MockKey[]).map((key) => (
                <Button
                  key={key}
                  variant="outline"
                  size="sm"
                  onClick={() => handleMock(key)}
                  className="gap-1.5 text-xs"
                >
                  <FlaskConical className="h-3 w-3" />
                  {key.replace("_", " ")}
                </Button>
              ))}
            </div>
          </div>
        </section>

        {/* Empty State */}
        {!result && (
          <section className="rounded-lg border-2 border-dashed border-muted-foreground/20 bg-card px-6 py-16 text-center">
            <FileUp className="mx-auto h-10 w-10 text-muted-foreground/40" />
            <h2 className="mt-4 text-lg font-semibold">No validation yet</h2>
            <p className="mt-1 text-sm text-muted-foreground max-w-md mx-auto">
              Upload an input instance and submission file, then press <strong>Validate</strong> to review the report. Or try the <strong>Mock</strong> buttons to preview the interface.
            </p>
          </section>
        )}

        {/* Results */}
        {result && (
          <>
            <ResultSummaryCard result={result} />
            <ValidationFlow steps={result.steps} />
            <DiagnosticsList diagnostics={result.diagnostics} />
            {result.parsedSummary && <ParsedSummaryCard summary={result.parsedSummary} />}
            {result.scoreBreakdown && <ScoreBreakdownCard breakdown={result.scoreBreakdown} />}
            <RawJsonPanel data={result} />

            <div className="flex justify-end">
              <Button variant="outline" onClick={handleDownload} className="gap-2">
                <Download className="h-4 w-4" />
                Download Report
              </Button>
            </div>
          </>
        )}
      </main>
    </div>
  );
};

export default Index;
