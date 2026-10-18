# Student C: Advanced Data Structures and Navigation (design notes)

Files: `HashTable`, `Tree` (BST), `Graph`, `CampusManager` (menus 7, 8, 9)
(plus small hooks in `ResourceManager`, `LinkedList` and `main.cpp`).
Location data file format (`data/locations.txt`): `A|B|metres` joins two locations; a line with only a name is a location with no walkways.

## How the pieces fit
- Student A's linked list is the **master store**: exactly one copy of every resource lives in it.
- The **hash table** and the **BST** are *indexes*: they store pointers to those same resources, not copies.
  So an update, an undo or a request approval is seen by every structure immediately.
- `ResourceManager` keeps the indexes in step: add, load and undo-of-remove insert into them; remove and undo-of-add
  take the resource out of them **before** the list node is deleted (otherwise they would hold a dangling pointer).
- `findResource()` (used by Student B's requests) now goes through the hash table: an O(1) average lookup.
- The **graph** holds campus locations. A resource's `Location` field matches a location name, which lets
  "Find the nearest available resource" combine the graph with the resource list.

## Why each structure (report material)
| Structure | Problem it solves | Why chosen | Alternatives considered |
|---|---|---|---|
| Hash table (separate chaining) | Find a resource from its ID fast | O(1) average; chains never fill up; deleting is easy | Linear probing (clustering, deletion needs tombstones), double hashing (more complex), searching the list O(n) |
| BST ordered by ID | Keep resources sorted by ID with fast search and in-order listing | O(log n) average, gives sorted output with an inorder walk | Sorted array (insert O(n)), AVL tree (balanced but more code) |
| Graph (adjacency list) | Model walkways between locations | Few walkways per building, so memory is O(V+E) and neighbours are quick to list | Adjacency matrix (V x V memory, mostly empty) |

Hash function: djb2 (`h = h * 33 + character`, then `h mod buckets`). It uses every character, so IDs that differ in one
digit (LAB101, LAB102) land in different buckets. The table starts with 53 buckets and doubles (re-hashing everything)
when entries exceed buckets, keeping the load factor at 1 or below.

## BFS vs DFS (the brief asks for this)
| | BFS | DFS |
|---|---|---|
| Uses | a queue | a stack (here the recursion) |
| Order | all 1-walkway neighbours first, then 2-walkway, ... | follows one path as deep as it can, then backs up |
| Good for | fewest walkways between two places, nearest-first | exploring everything, finding connected groups, detecting loops |
| Cost | O(V + E) | O(V + E) |

Example from the sample map starting at Library: BFS visits CS Building, Administration, Student Affairs and Lecture Hall Block
(all 1 walkway away) before Building 32 (2 away). DFS goes Library -> CS Building -> Building 32 -> Technical Annex -> ... deep first.
BFS finds the route with the fewest walkways, not the shortest distance. For distance in metres the project uses Dijkstra.

## Complexity of Student C's operations
| Operation | Structure / algorithm | Time | Case |
|---|---|---|---|
| Hash insert / find / remove | Hash table | O(1) | average (O(n) worst if every key collides) |
| Rehash | Hash table | O(n) | rare, amortised O(1) per insert |
| BST insert / search / remove | BST | O(log n) | average, balanced tree |
| BST insert / search / remove | BST | O(n) | worst case, IDs inserted in sorted order |
| Inorder / preorder / postorder / level order | BST | O(n) | all cases |
| BFS, DFS, reachability, components | Graph (adjacency list) | O(V + E) | all cases |
| Shortest path (Dijkstra, array scan) | Graph | O(V^2) | all cases |
| Add connection | Graph | O(degree) | walks the neighbour list to add at the end and check for a duplicate |

## Test results observed
- **Randomised check**: 40,000 random insert/remove operations on a hash table (started tiny so it re-hashed many times) and a
  BST, compared against a simple model: 0 errors. With about 2,000 entries: load factor 0.48, longest chain 4, BST height 23.
- **Graph known-answer test**: Dijkstra gave the expected distances on a hand-checked graph, including an unreachable location (-1);
  the location array grew correctly past its initial size (200 locations).
- **Menu runs** on the sample data: all four tree traversals, tree drawing, hash look-ups (found and not found), BFS, DFS,
  reachability (yes and no), shortest route (Library to Technical Annex: 260 m via CS Building and Building 32), nearest available
  resource, connected groups (2 groups: the main campus and the isolated Remote Research Station).
- **Index consistency**: after add, remove, update, undo and an approved request, the hash table and tree always agreed with the list
  (a removed resource disappears from both; an undo brings it back; an approved request shows Unavailable through the hash lookup).
- **Large data (5006 resources)**: hash lookup compared 1 ID (load factor 0.72, longest chain 2); binary search 13 comparisons;
  linear search 5005; **BST search 5002 comparisons**, because the generated IDs were inserted in sorted order and the tree became a
  stick of 5003 levels. This is the BST worst case, O(n). Good example for the best/average/worst-case discussion.
- **Invalid and boundary cases**: empty tree, empty hash table and empty graph give messages; unknown locations, duplicate connections,
  self-connections, bad distances, duplicate IDs, bad or missing `locations.txt` lines are all handled.
- Memory check (AddressSanitizer) reported no leaks or errors in any of these runs.

## Known limits (mention honestly in the report)
- The BST is not self-balancing; sorted input makes it a stick. An AVL or red-black tree would fix this (challenge extension).
- Dijkstra scans an array to find the closest location, so it is O(V^2). With a heap it would be O((V + E) log V); Student B's heap
  could be adapted.
- The hash value printed in the hash menu depends on the computer (32-bit vs 64-bit `unsigned long`), so bucket numbers can differ
  between machines. Lookups are still correct on every machine.
- Deletion from the tree happens through Resource Management (remove), not from the tree menu, so the two structures cannot disagree.
