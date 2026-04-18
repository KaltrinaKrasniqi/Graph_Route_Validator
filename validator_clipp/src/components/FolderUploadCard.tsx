import React, { useMemo, useRef } from "react";
import { FolderOpen, X } from "lucide-react";
import { Button } from "@/components/ui/button";

interface FolderUploadCardProps {
  label: string;
  description: string;
  files: File[];
  onFilesSelect: (files: File[]) => void;
  onFilesRemove: () => void;
}

type DirectoryInputProps = React.InputHTMLAttributes<HTMLInputElement> & {
  webkitdirectory?: string;
  directory?: string;
};

const FolderUploadCard: React.FC<FolderUploadCardProps> = ({
  label,
  description,
  files,
  onFilesSelect,
  onFilesRemove,
}) => {
  const inputRef = useRef<HTMLInputElement>(null);

  const folderName = useMemo(() => {
    const firstPath = files[0]?.webkitRelativePath;
    return firstPath ? firstPath.split("/")[0] : null;
  }, [files]);

  const handleInputChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const selectedFiles = event.target.files ? Array.from(event.target.files) : [];
    onFilesSelect(selectedFiles);
  };

  const handleRemove = () => {
    onFilesRemove();

    if (inputRef.current) {
      inputRef.current.value = "";
    }
  };

  const directoryProps: DirectoryInputProps = {
    webkitdirectory: "",
    directory: "",
  };

  return (
    <div className="rounded-lg border bg-card p-6 transition-shadow hover:shadow-md">
      <h3 className="mb-1 text-sm font-semibold uppercase tracking-wider text-muted-foreground">
        {label}
      </h3>
      <p className="mb-4 text-xs text-muted-foreground">{description}</p>

      <input
        ref={inputRef}
        type="file"
        multiple
        accept=".txt,text/plain"
        className="hidden"
        onChange={handleInputChange}
        {...directoryProps}
      />

      {files.length > 0 ? (
        <div className="space-y-3 rounded-md border border-primary/20 bg-primary/5 px-4 py-3">
          <div className="flex items-center gap-3">
            <FolderOpen className="h-5 w-5 shrink-0 text-primary" />
            <div className="min-w-0 flex-1">
              <p className="truncate text-sm font-medium">{folderName ?? "Selected folder"}</p>
              <p className="text-xs text-muted-foreground">
                {files.length} file{files.length === 1 ? "" : "s"} selected
              </p>
            </div>
            <Button
              variant="ghost"
              size="icon"
              className="h-7 w-7 shrink-0"
              onClick={handleRemove}
            >
              <X className="h-4 w-4" />
            </Button>
          </div>

          <div className="rounded-md border bg-background/70 px-3 py-2">
            <p className="text-[11px] font-medium uppercase tracking-wider text-muted-foreground">
              Sample files
            </p>
            <div className="mt-2 space-y-1 text-xs text-muted-foreground">
              {files.slice(0, 3).map((file) => (
                <p key={file.webkitRelativePath || file.name} className="truncate font-mono">
                  {file.webkitRelativePath || file.name}
                </p>
              ))}
              {files.length > 3 && <p>+ {files.length - 3} more</p>}
            </div>
          </div>
        </div>
      ) : (
        <button
          type="button"
          onClick={() => inputRef.current?.click()}
          className="flex w-full cursor-pointer flex-col items-center justify-center gap-2 rounded-md border-2 border-dashed border-muted-foreground/25 px-4 py-8 text-muted-foreground transition-colors hover:border-primary/40 hover:bg-primary/5"
        >
          <FolderOpen className="h-6 w-6" />
          <span className="text-sm font-medium">Choose folder</span>
          <span className="text-xs">Reads all files from the selected folder in the browser</span>
        </button>
      )}
    </div>
  );
};

export default FolderUploadCard;
