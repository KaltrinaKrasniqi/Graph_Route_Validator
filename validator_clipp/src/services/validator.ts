import { ValidationResponse } from "@/types/validation";

let wasmModulePromise: Promise<any> | null = null;

async function getWasmModule() {
  if (!wasmModulePromise) {
    const moduleUrl = `${window.location.origin}/wasm/validator.js`;

    wasmModulePromise = import(/* @vite-ignore */ moduleUrl).then(async (mod) => {
      const factory = mod.default;
      return await factory({
        locateFile: (path: string) => `/wasm/${path}`,
      });
    });
  }

  return wasmModulePromise;
}

export async function runValidator(
  instanceText: string,
  submissionText: string
): Promise<ValidationResponse> {
  const wasm = await getWasmModule();

  const resultJson = wasm.ccall(
    "validate",
    "string",
    ["string", "string"],
    [instanceText, submissionText]
  );

  return JSON.parse(resultJson) as ValidationResponse;
}