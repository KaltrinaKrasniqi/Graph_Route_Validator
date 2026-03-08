import React, { useRef } from "react";
import { Upload, X, FileText } from "lucide-react";
import { Button } from "@/components/ui/button";

interface FileUploadCardProps {
  label: string;
  description: string;
  file: File | null;
  onFileSelect: (file: File) => void;
  onFileRemove: () => void;
}

const FileUploadCard: React.FC<FileUploadCardProps> = ({
  label,
  description,
  file,
  onFileSelect,
  onFileRemove,
}) => {
  const inputRef = useRef<HTMLInputElement>(null);

  const handleDrop = (e: React.DragEvent) => {
    e.preventDefault();
    const f = e.dataTransfer.files[0];
    if (f) onFileSelect(f);
  };

  return (
    <div className="rounded-lg border bg-card p-6 transition-shadow hover:shadow-md">
      <h3 className="text-sm font-semibold uppercase tracking-wider text-muted-foreground mb-1">
        {label}
      </h3>
      <p className="text-xs text-muted-foreground mb-4">{description}</p>

      <input
        ref={inputRef}
        type="file"
        accept=".txt"
        className="hidden"
        onChange={(e) => {
          const f = e.target.files?.[0];
          if (f) onFileSelect(f);
        }}
      />

      {file ? (
        <div className="flex items-center gap-3 rounded-md border border-primary/20 bg-primary/5 px-4 py-3">
          <FileText className="h-5 w-5 text-primary shrink-0" />
          <span className="text-sm font-medium truncate flex-1">{file.name}</span>
          <Button
            variant="ghost"
            size="icon"
            className="h-7 w-7 shrink-0"
            onClick={onFileRemove}
          >
            <X className="h-4 w-4" />
          </Button>
        </div>
      ) : (
        <button
          onClick={() => inputRef.current?.click()}
          onDragOver={(e) => e.preventDefault()}
          onDrop={handleDrop}
          className="flex w-full flex-col items-center justify-center gap-2 rounded-md border-2 border-dashed border-muted-foreground/25 px-4 py-8 text-muted-foreground transition-colors hover:border-primary/40 hover:bg-primary/5 cursor-pointer"
        >
          <Upload className="h-6 w-6" />
          <span className="text-sm font-medium">Click or drag to upload</span>
          <span className="text-xs">.txt file</span>
        </button>
      )}
    </div>
  );
};

export default FileUploadCard;
