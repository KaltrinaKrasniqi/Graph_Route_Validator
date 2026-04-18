import React from "react";
import BatchValidationView from "@/components/BatchValidationView";
import SingleValidationView from "@/components/SingleValidationView";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";

const Index: React.FC = () => {
  return (
    <div className="min-h-screen bg-background">
      <header className="border-b bg-card">
        <div className="mx-auto max-w-4xl px-4 py-8 sm:px-6">
          <h1 className="text-2xl font-bold tracking-tight sm:text-3xl">
            Graph Route Validator
          </h1>
          <p className="mt-1.5 max-w-2xl text-sm leading-relaxed text-muted-foreground">
            Validate problem instances and submissions, inspect diagnostics, and review score
            breakdowns.
          </p>
        </div>
      </header>

      <main className="mx-auto max-w-4xl space-y-6 px-4 py-8 sm:px-6">
        <Tabs defaultValue="single" className="space-y-6">
          <TabsList className="grid w-full grid-cols-2">
            <TabsTrigger value="single">Single Validation</TabsTrigger>
            <TabsTrigger value="batch">Batch Validation</TabsTrigger>
          </TabsList>

          <TabsContent value="single" className="mt-0">
            <SingleValidationView />
          </TabsContent>

          <TabsContent value="batch" className="mt-0">
            <BatchValidationView />
          </TabsContent>
        </Tabs>
      </main>
    </div>
  );
};

export default Index;
