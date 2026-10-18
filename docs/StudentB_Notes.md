# Student B: Request and Activity Management (design notes)

Files: `Request`, `Queue`, `Stack`, `PriorityQueue`, `RequestSort`, `RequestManager`
(plus small hooks in `ResourceManager`, `LinkedList` and `main.cpp`).
Request data file format: `id|user|resourceId|time|priority|status|kind` (kind: N = normal queue, P = priority heap).

## How the pieces fit
- **Normal requests** (menu 4) go into a linked-node FIFO `RequestQueue`.
- **Priority requests** (menu 5) go into a `PriorityQueue` built on a binary min-heap (dynamic array).
- **Processing** takes the next request from the right structure and decides: if the resource exists and is Available it is
  **Approved** and the resource becomes Unavailable (allocated); otherwise the request is **Rejected**. Finished requests go to a
  `processedLog` (another `RequestQueue`). "Release a resource" in menu 4 makes a resource Available again.
- **Undo** (menu 6) uses an `UndoStack` inside `ResourceManager`. Add, remove and update (in Resource Management) each push an action.
  Undo pops the top action and reverses it: an add is removed again, a removed resource goes back to its old position, an
  update gets its previous values back.
- Requests are saved to / loaded from `data/requests.txt` together with the resources file.

## Why each structure (report material)
| Structure | Problem it solves | Why chosen | Alternatives considered |
|---|---|---|---|
| Queue (linked nodes, front + rear pointers) | Serve normal requests fairly, oldest first | FIFO is exactly the rule in the brief; enqueue and dequeue are O(1) | Array queue (needs a size limit or shifting), always scanning for the oldest request O(n) |
| Stack (linked nodes) | Undo: the LAST action must be reversed FIRST | LIFO matches undo; push/pop/peek O(1) | A list searched for the newest item; a queue (wrong order) |
| Heap / priority queue | Always serve the MOST URGENT request next | insert and remove-highest are O(log n) | Sorted list (insert O(n)), unsorted list (remove-highest O(n)) |

## Heap vs priority queue (the brief asks for this)
A priority queue is the idea: "give me the most urgent item first". A heap is one efficient way to build it: a complete binary tree
stored in an array where every parent is more urgent than its children, so the most urgent item is always at index 0.
Parent of i is (i-1)/2, children are 2i+1 and 2i+2. Menu 5 option 5 prints the array so you can show this in the demo.

Ties: requests with the same priority are ordered by submission time, then by ID, so they are still served first come first served.
(IDs are zero-padded to 4 digits, so text order equals number order.)

## Complexity of Student B's operations
| Operation | Structure | Time | Case |
|---|---|---|---|
| Enqueue | Queue | O(1) | all |
| Dequeue / peek front | Queue | O(1) | all |
| Push / pop / peek | Stack | O(1) | all |
| Heap insert | Heap | O(log n) | worst; O(1) best (already in place) |
| Remove highest | Heap | O(log n) | worst and average |
| Peek highest | Heap | O(1) | all |
| Check an ID exists (duplicate check) | Queue / heap | O(n) | worst |
| Sort all requests | Merge sort | O(n log n) | all cases, O(n) extra space |

Heap array growth: when full the array doubles, which is O(n) for that one insert but O(1) amortised per insert.

## Test results observed
- 2000 random requests with many equal priorities and times: heap removal order matched a reference sort exactly (0 mismatches);
  queue order matched insertion order exactly (0 mismatches).
- Empty cases: dequeue, remove-highest, stack pop and undo on empty structures all give a clear message and do not crash.
- Invalid cases: unknown resource ID, empty or `|` user name, bad lines and duplicate IDs in `requests.txt` (skipped with a message).
- Integration: approving a request marks the resource Unavailable; a later request for it is Rejected; releasing it makes it
  Available again. Emergency request on an already-allocated lab is rejected.
- Undo: add, remove (restored at the same position) and update (old values back) all reverse correctly, in LIFO order.

## Known limits (mention honestly in the report)
- Undo covers only changes made through Resource Management (add / remove / update). It does not cover request processing,
  "Generate test data", or data loaded from files.
- Because approving a request changes a resource's availability outside the undo stack, undoing an older update can restore an
  older availability value.
- The BST and hash table (Student C) are not yet notified by undo or by add/remove: see the TODO comments in `ResourceManager.cpp`.
