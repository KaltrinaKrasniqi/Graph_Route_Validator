import React, { useState } from "react";
import { FileUp, Play } from "lucide-react";
import BatchInstanceResultCard from "@/components/BatchInstanceResultCard";
import BatchSummaryCard from "@/components/BatchSummaryCard";
import FolderUploadCard from "@/components/FolderUploadCard";
import { Button } from "@/components/ui/button";
import { ScrollArea } from "@/components/ui/scroll-area";
import { runValidator } from "@/services/validator";
import { BatchValidationItem, BatchValidationSummary, ValidationResponse } from "@/types/validation";

type IndexedFile = {
  key: string;
  file: File;
};

const readFile = (file: File): Promise<string> =>
  new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = () => resolve(reader.result as string);
    reader.onerror = () => reject(new Error(`Failed to read file "${file.name}".`));
    reader.readAsText(file);
  });

const stripKnownAffixes = (value: string) => {
  let current = value;
  let changed = true;

  while (changed) {
    changed = false;

    const next = current
      .replace(/^(instance|input|submission|solution|output|sol|out|sub)[-_ ]*/i, "")
      .replace(/[-_ ]*(instance|input|submission|solution|output|sol|out|sub)$/i, "")
      .trim();

    if (next !== current) {
      current = next;
      changed = true;
    }
  }

  return current;
};

const getMatchKey = (fileName: string) => {
  const withoutExtension = fileName.replace(/\.[^/.]+$/, "").trim().toLowerCase();
  const normalized = stripKnownAffixes(withoutExtension).replace(/[-_ ]+/g, "");

  return normalized || withoutExtension.replace(/[-_ ]+/g, "");
};

const indexFiles = (files: File[]) => {
  const buckets = new Map<string, IndexedFile[]>();

  files.forEach((file) => {
    const key = getMatchKey(file.name);
    const current = buckets.get(key);
    if (current) {
      current.push({ key, file });
    } else {
      buckets.set(key, [{ key, file }]);
    }
  });

  return buckets;
};

const sortItems = (items: BatchValidationItem[]) =>
  [...items].sort((left, right) =>
    left.displayName.localeCompare(right.displayName, undefined, {
      numeric: true,
      sensitivity: "base",
    })
  );

const getRunStatus = (result: ValidationResponse): BatchValidationItem["status"] =>
  result.status === "VALID" ? "valid" : "invalid";

const BatchValidationView: React.FC = () => {
  const [inputFiles, setInputFiles] = useState<File[]>([]);
  const [outputFiles, setOutputFiles] = useState<File[]>([]);
  const [results, setResults] = useState<BatchValidationItem[]>([]);
  const [summary, setSummary] = useState<BatchValidationSummary | null>(null);
  const [loading, setLoading] = useState(false);

  const handleValidate = async () => {
    setLoading(true);

    const nextSummary: BatchValidationSummary = {
      totalInputFiles: inputFiles.length,
      totalOutputFiles: outputFiles.length,
      matchedPairs: 0,
      validRuns: 0,
      invalidRuns: 0,
      totalScoreAcrossValidInstances: 0,
    };

    const nextResults: BatchValidationItem[] = [];
    const inputBuckets = indexFiles(inputFiles);
    const outputBuckets = indexFiles(outputFiles);

    inputBuckets.forEach((entries, key) => {
      if (entries.length > 1) {
        entries.forEach(({ file }) => {
          nextResults.push({
            key: `duplicate-input-${file.webkitRelativePath || file.name}`,
            displayName: key,
            inputFileName: file.name,
            status: "duplicate_input",
            message: "Multiple input files share the same match key in the selected folder.",
            result: null,
          });
        });
      }
    });

    outputBuckets.forEach((entries, key) => {
      if (entries.length > 1) {
        entries.forEach(({ file }) => {
          nextResults.push({
            key: `duplicate-output-${file.webkitRelativePath || file.name}`,
            displayName: key,
            outputFileName: file.name,
            status: "duplicate_output",
            message: "Multiple output files share the same match key in the selected folder.",
            result: null,
          });
        });
      }
    });

    const uniqueInputKeys = [...inputBuckets.entries()]
      .filter(([, entries]) => entries.length === 1)
      .map(([key]) => key);
    const uniqueOutputKeys = [...outputBuckets.entries()]
      .filter(([, entries]) => entries.length === 1)
      .map(([key]) => key);

    const uniqueOutputKeySet = new Set(uniqueOutputKeys);
    const uniqueInputKeySet = new Set(uniqueInputKeys);
    const matchedKeys = uniqueInputKeys.filter((key) => uniqueOutputKeySet.has(key));

    nextSummary.matchedPairs = matchedKeys.length;

    uniqueInputKeys
      .filter((key) => !uniqueOutputKeySet.has(key))
      .forEach((key) => {
        const file = inputBuckets.get(key)?.[0]?.file;
        if (!file) return;

        nextResults.push({
          key: `missing-output-${file.webkitRelativePath || file.name}`,
          displayName: key,
          inputFileName: file.name,
          status: "missing_output",
          message: "No matching output file was found for this input file.",
          result: null,
        });
      });

    uniqueOutputKeys
      .filter((key) => !uniqueInputKeySet.has(key))
      .forEach((key) => {
        const file = outputBuckets.get(key)?.[0]?.file;
        if (!file) return;

        nextResults.push({
          key: `missing-input-${file.webkitRelativePath || file.name}`,
          displayName: key,
          outputFileName: file.name,
          status: "missing_input",
          message: "No matching input file was found for this output file.",
          result: null,
        });
      });

    for (const key of matchedKeys) {
      const inputFile = inputBuckets.get(key)?.[0]?.file;
      const outputFile = outputBuckets.get(key)?.[0]?.file;

      if (!inputFile || !outputFile) {
        continue;
      }

      try {
        const [instanceText, submissionText] = await Promise.all([
          readFile(inputFile),
          readFile(outputFile),
        ]);

        const result = await runValidator(instanceText, submissionText);
        const status = getRunStatus(result);

        nextResults.push({
          key: `matched-${key}`,
          displayName: key,
          inputFileName: inputFile.name,
          outputFileName: outputFile.name,
          status,
          message:
            status === "valid"
              ? "Validation completed successfully for this matched pair."
              : "Validation completed with issues for this matched pair.",
          result,
        });

        if (status === "valid") {
          nextSummary.validRuns += 1;
          nextSummary.totalScoreAcrossValidInstances += result.score;
        } else {
          nextSummary.invalidRuns += 1;
        }
      } catch (error) {
        nextResults.push({
          key: `error-${key}`,
          displayName: key,
          inputFileName: inputFile.name,
          outputFileName: outputFile.name,
          status: "error",
          message: error instanceof Error ? error.message : "Unexpected validator or parsing error.",
          result: null,
        });
        nextSummary.invalidRuns += 1;
      }
    }

    setResults(sortItems(nextResults));
    setSummary(nextSummary);
    setLoading(false);
  };

  return (
    <div className="space-y-6">
      <section>
        <div className="grid gap-4 sm:grid-cols-2">
          <FolderUploadCard
            label="Input Folder"
            description="Choose the folder containing the instance files."
            files={inputFiles}
            onFilesSelect={setInputFiles}
            onFilesRemove={() => setInputFiles([])}
          />
          <FolderUploadCard
            label="Output Folder"
            description="Choose the folder containing the corresponding solution files."
            files={outputFiles}
            onFilesSelect={setOutputFiles}
            onFilesRemove={() => setOutputFiles([])}
          />
        </div>

        <div className="mt-4 flex flex-wrap items-center gap-3">
          <Button
            onClick={handleValidate}
            disabled={(inputFiles.length === 0 && outputFiles.length === 0) || loading}
            size="lg"
            className="gap-2"
          >
            <Play className="h-4 w-4" />
            {loading ? "Running batch..." : "Run Batch Validation"}
          </Button>
          <p className="text-sm text-muted-foreground">
            Files are matched by their core name, ignoring extensions and common
            prefixes or suffixes like instance, solution, input, and output.
          </p>
        </div>

        <p className="mt-3 rounded-md border bg-muted/40 px-3 py-2 text-xs text-muted-foreground">
          Example: <span className="font-mono">instance1.txt</span> matches
          {" "}
          <span className="font-mono">instance1_solution.txt</span> or
          {" "}
          <span className="font-mono">solution1.txt</span>.
        </p>
      </section>

      {!summary && (
        <section className="rounded-lg border-2 border-dashed border-muted-foreground/20 bg-card px-6 py-16 text-center">
          <FileUp className="mx-auto h-10 w-10 text-muted-foreground/40" />
          <h2 className="mt-4 text-lg font-semibold">No batch validation yet</h2>
          <p className="mx-auto mt-1 max-w-2xl text-sm text-muted-foreground">
            Select an input folder and an output folder, then run batch validation to process
            every matched pair while keeping unmatched or failed cases in the results list.
          </p>
        </section>
      )}

      {summary && (
        <>
          <BatchSummaryCard summary={summary} />

          <section className="space-y-4">
            <div>
              <h2 className="text-sm font-semibold uppercase tracking-wider text-muted-foreground">
                Per-Instance Results
              </h2>
              <p className="mt-1 text-sm text-muted-foreground">
                Each pair is processed independently. Unmatched files and per-instance failures stay
                visible here instead of crashing the batch.
              </p>
            </div>

            <ScrollArea className="h-[70vh] rounded-lg border bg-muted/20 p-4">
              <div className="space-y-4 pr-3">
                {results.map((item) => (
                  <BatchInstanceResultCard key={item.key} item={item} />
                ))}
              </div>
            </ScrollArea>
          </section>
        </>
      )}
    </div>
  );
};

export default BatchValidationView;
