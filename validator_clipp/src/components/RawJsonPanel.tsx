import React, { useState } from "react";
import { ChevronDown, ChevronRight, Copy, Check } from "lucide-react";
import { Button } from "@/components/ui/button";

interface RawJsonPanelProps {
  data: unknown;
}

const RawJsonPanel: React.FC<RawJsonPanelProps> = ({ data }) => {
  const [open, setOpen] = useState(false);
  const [copied, setCopied] = useState(false);
  const json = JSON.stringify(data, null, 2);

  const handleCopy = () => {
    navigator.clipboard.writeText(json);
    setCopied(true);
    setTimeout(() => setCopied(false), 2000);
  };

  return (
    <div className="rounded-lg border bg-card">
      <button
        onClick={() => setOpen(!open)}
        className="flex w-full items-center gap-2 px-6 py-4 text-left transition-colors hover:bg-muted/50"
      >
        {open ? <ChevronDown className="h-4 w-4 text-muted-foreground" /> : <ChevronRight className="h-4 w-4 text-muted-foreground" />}
        <span className="text-sm font-semibold uppercase tracking-wider text-muted-foreground">
          Raw Validation Response
        </span>
      </button>
      {open && (
        <div className="border-t px-6 py-4">
          <div className="flex justify-end mb-2">
            <Button variant="ghost" size="sm" onClick={handleCopy} className="h-7 gap-1.5 text-xs">
              {copied ? <Check className="h-3.5 w-3.5" /> : <Copy className="h-3.5 w-3.5" />}
              {copied ? "Copied" : "Copy"}
            </Button>
          </div>
          <pre className="overflow-x-auto rounded-md bg-muted/50 p-4 text-xs leading-relaxed font-mono">
            {json}
          </pre>
        </div>
      )}
    </div>
  );
};

export default RawJsonPanel;
