# Student A: Core Data Management (design notes)

Files: `Utils`, `LinkedList`, `Sorting`, `Search`, `ResourceManager` (+ `main.cpp` wiring).
Resource data file format: `id|name|type|location|capacity|available(1/0)`.

## How the pieces fit
- `Resource` (shared struct) is what every module stores.
- `LinkedList` is the master store of resources. `ResourceManager` owns it and provides menus 1, 2 and 3.
- `Sorting` and `Search` work on a dynamic array copy made by `LinkedList::toArray()` (caller frees with `delete[]`).
- `main.cpp` loads `data/resources.txt` at start and saves it on exit.

## Hooks for the rest of the team (marked TODO in the code)
- Student B: push the removed/old resource onto the undo stack in `removeResource()` and `updateResource()`.
- Student C: after load, fill the BST and hash table from `getAllAsArray()`. Insert/remove there too in `addResource()` / `removeResource()`.

## Why a linked list (report material)
Resources are added and removed often and the count is not known in advance. A list grows one node at a time and
insert/delete needs no shifting. Cost: no direct access by index, so searching is O(n).

## Complexity of Student A's operations
| Operation | Structure / algorithm | Time | Case |
|---|---|---|---|
| Insert at beginning | Linked list | O(1) | all |
| Insert at end | Linked list with tail pointer | O(1) | all |
| Insert at position p | Linked list | O(n) | worst (walk to p) |
| Remove by ID | Linked list | O(n) | worst; O(1) if first |
| Find by ID / name | Linear search | O(n) | worst; O(1) best |
| Find by ID | Binary search (sorted array) | O(log n) | worst/average |
| Insertion sort | Array | O(n^2) average/worst, O(n) best | best = already sorted |
| Merge sort | Array + temp array | O(n log n) all cases, O(n) extra space | all |

## Observed test results (3006 resources, from the Sort > Compare option)
- Sort by Capacity: insertion sort 2,213,301 comparisons vs merge sort 30,960.
- Sort by ID on generated data: insertion sort only 15,012 comparisons, because the generated IDs were already in
  order, which is insertion sort's best case O(n). Good example for the report on best vs average case.
- Search for the last of 3006 resources: linear 3005 comparisons vs binary 9.
  (Binary search needs sorted data: sorting the copy cost 21,071 comparisons, so it pays off only when you search many times.)

## Test cases covered
Normal: add (beginning/end/position), update, remove, display, search, sort, save/load.
Boundary: empty list (remove, search, sort), position 1 and size+1, removing first and last node.
Invalid: duplicate ID, unknown ID, letters or out-of-range numbers in menus, '|' or empty text in fields,
bad or duplicate lines in the data file (skipped with a message).
Large data: Resource Management > option 7 generates up to 5000 resources.
