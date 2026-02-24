# The Vibe Coder’s Manifesto (Terminal Edition)

1. The Core Philosophy: AI is a Mirror
The Rule: AI doesn't make you better; it amplifies who you are.
Lazy Mirror: If you delegate without thinking, it produces "workslop"—fragile, unmaintainable code.
Sharp Mirror: If you are rigorous with TDD and architecture, AI scales that excellence across the entire codebase.

2. The Strategy: Compounding vs. Finish Line
The Finish Line Game: Coding to just "make it work." AI wins this, but the project dies of technical debt.
The Compounding Game: Every line must be a resource for the next feature. You aren't building "features"; you are building "futures"—the possibility of future change.

3. The Workflow: Harness Engineering
Shift your focus from reading code to reading the verification layer.
Specifications: Define exactly what should exist.Harnesses: Build the automated tests and linters that catch the agent when it fails.
AGENTS.md: Your repo's "System of Record." Use it to anchor AI to your specific domain knowledge, architecture, and coding conventions.
Ralph Wiggum Loop: Let the agent iterate locally (running tests/scripts) until it satisfies all agentic reviewers before you ever look at the PR.

4. The Skill Ladder: Steve Yegge’s 8 LevelsLevelModeBehavior
|-------+---------------------------------------|
| L1-L2 | JuniorAutocomplete and manual reviews.
| L3-L4 | TransitionChat-driven; starting to trust tests over diffs.
| L5-L6 | Heavy UserMultiple agents running in parallel; "Dracula Effect" hits.
| L7-L8 | OrchestratorYou build custom orchestrators to manage the swarm.
|-------+---------------------------------------|

5. The Mental Game: The Dracula Effect
Productivity Limit: AI can give you a 100x speed boost, but the "judgment load" is exhausting.
The Cap: 3 hours of intense vibe coding is the hard limit for a human brain per day. After that, judgment fails and errors spike.

6. The Trap: The -17% Knowledge GapAnthropic's 2026 study found AI users scored 17% lower on comprehension quizzes.
To Prevent This: Ask for concepts first.
Prompt Secret: Instead of giving commands, say: "You are an AI expert. Ask me questions one by one until you have enough context to make a recommendation".

7. The New Literacy: What to Read
Read the Harness: Are the tests strong enough to catch an AI hallucination? 
Read the Invariants: Focus on data shapes at the boundaries, not the implementation logic.
Read for Failure: Always ask: "What are the failure modes of this generated code?" 

“Reading syntax is for machines. Reading systems is for engineers.”