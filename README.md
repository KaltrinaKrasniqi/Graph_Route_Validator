# Graph Route Validator

This repository contains the Graph Route Validator — a tool designed to validate routing solutions for a street-cleaning optimization problem based on a set of defined rules and constraints. 
The validator checks whether a submitted solution satisfies all structural, logical, and semantic constraints, and computes the final score for valid submissions.
The core validation logic is implemented in C++ and compiled to WebAssembly (WASM) so it can run directly in the browser.

## Hosted Version

You can access the live validator here:
[Graph Route Validator](https://graph-route-validator.netlify.app/)

The web interface allows users to upload instance and submission files and view validation results and score breakdowns.

## Features

- **Web Interface** – Upload instance and submission files and run validation directly in the browser.
- **Multi-stage Validation** – Solutions are validated through parsing, format validation, semantic validation, and scoring.
- **Detailed Diagnostics** – The validator reports errors and violations with descriptive messages.
- **Score Calculation** – Valid submissions receive a score based on cleaned street length and water waste.

## Repository Structure

```text
validator_clipp
│
├── validator_clipp
├── validator_core
│   ├── include
│   │   ├── engine
│   │   ├── models
│   │   ├── parsers
│   │   ├── scoring
│   │   └── validation
│   │
│   ├── src
│   │   ├── engine
│   │   ├── parsers
│   │   ├── scoring
│   │   └── validation
│   │
│   ├── wasm
│   └── tests
│
├── build_wasm.sh
├── main.cc
└── CMakeLists.txt
```

## Local Setup
### Prerequisites

1. **Node.js**
2. **CMake**
3. **C++17 compiler**
4. **Emscripten (for building the WASM module)**

Install Emscripten using the official guide:
https://emscripten.org/docs/getting_started/downloads.html

- **Activate the Emscripten environment**:
- **source ~/emsdk/emsdk_env.sh**
- **Clone the repository**:
- **git clone https://github.com/YOUR_USERNAME/validator_clipp.git**
- **cd validator_clipp**
- **Build the WebAssembly module**:
- **cd wasm**
- **./build.sh**
- **Run the development server**:
- **npm install**
- **npm run dev**

## Technologies Used

- **C++17**
- **CMake**
- **Emscripten**
- **WebAssembly**
- **TypeScript**
- **Netlify**
