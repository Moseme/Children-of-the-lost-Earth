# Team workflow

## Branches
- `main`: always compiles. Never push directly.
- `dev`: integration branch.
- Feature branches: `a/linkedlist`, `b/priority-queue`, `c/hashtable`, etc.

## Rules
1. Branch from `dev`, open a pull request into `dev`, and get one teammate to review it.
2. Run `make` before opening a PR. Code that does not compile is not merged.
3. Merge `dev` into `main` only at the end of each phase.
4. Do not edit another student's files without telling them. Shared files (`Resource.h`, `main.cpp`) need agreement from all three.
5. Commit messages: `[module] short description` e.g. `[hash] add chaining collision handling`.
6. Everyone must be able to explain all code (individual accountability).

## Ownership
| Files | Owner |
|---|---|
| Resource.*, LinkedList.*, Sorting.*, Search.*, FileIO.* | A |
| Request.*, Queue.*, Stack.*, PriorityQueue.* | B |
| Tree.*, HashTable.*, Graph.* | C |
| main.cpp, ResourceManager.* | shared |
