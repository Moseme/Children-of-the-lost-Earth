# CS3400 Mini Project: Intelligent Campus Resource and Activity Management System

National University of Lesotho, CS3400 Data Structures and Algorithms.
Scenario (Group Type A, Campus Facilities): technical support facilities and lab allocation.
Language: C++ (plain C++98 style). All core data structures are written by the group; the only standard library
parts used are `std::string`, console input/output and file streams.

## Group members
| Student | Name | Student number | Role |
|---|---|---|---|
| A | _TODO_ | _TODO_ | Core data management: resources, linked list, searching, sorting, file I/O |
| B | _TODO_ | _TODO_ | Requests and activities: queue, stack (undo), priority queue (heap), request processing |
| C | _TODO_ | _TODO_ | Advanced structures: hash table, BST, campus graph (BFS, DFS, shortest path) |

## How to compile
You need a C++ compiler such as g++ (MinGW on Windows). From the project folder (the one containing `src` and `data`):

```
g++ -std=gnu++98 -Wall -Wextra -Isrc src/*.cpp -o campus_system.exe
```

(With `make` installed you can run `make` instead; on Linux or macOS the output is `campus_system`.)
`gnu++98` is used instead of `c++98` because older MinGW versions fail to compile their own headers in strict mode.

## How to run
Always start the program from the project folder so that it can find the `data` folder:

```
.\campus_system.exe        (Windows PowerShell)
./campus_system            (Linux / macOS)
```

The program loads `data/resources.txt`, `data/requests.txt` and `data/locations.txt` when it starts and saves them
again when you leave with option 0. If you stop the program some other way (for example Ctrl + C), nothing is saved.
To go back to the original sample data, restore the three files with `git restore data`.

## Major features (main menu)
| Option | Feature | Main data structure / algorithm |
|---|---|---|
| 1 | Resource management: add (beginning, end, position), remove, update, display, generate test data | Singly linked list (own implementation) |
| 2 | Search resources: linear and binary search by ID, linear search by name | Arrays, with comparison counts |
| 3 | Sort resources by ID, name, capacity or type, and compare the algorithms | Insertion sort, merge sort |
| 4 | Resource requests: submit, view, process in first-come-first-served order, release a resource, sort requests | Queue (FIFO), merge sort |
| 5 | Priority requests: Emergency, High, Normal, Low; most urgent processed first; show the heap array | Binary min-heap (priority queue) |
| 6 | Undo the last add, remove or update of a resource | Stack (LIFO) |
| 7 | Tree operations: inorder, preorder, postorder, level-order, drawing, search, statistics | Binary search tree by resource ID |
| 8 | Hash table lookup, statistics, and a comparison of four lookup methods | Hash table with separate chaining |
| 9 | Campus navigation: BFS, DFS, reachability, shortest route, nearest available resource, connected groups | Weighted graph (adjacency list), Dijkstra |
| 10 | System analysis: complexity table, measured experiments, live snapshot | Comparison counting and timing |

How the parts connect: the linked list is the master store of resources. The hash table and the BST point to the same
resources (one copy only), so updates are seen everywhere. Processing a request approves it when the resource exists and is
available (the resource then becomes Unavailable) and rejects it otherwise.

## Data files
- `data/resources.txt`: `id|name|type|location|capacity|available(1/0)`
- `data/requests.txt`: `id|user|resourceId|time|priority|status|kind` (kind: N = normal queue, P = priority heap)
- `data/locations.txt`: `A|B|metres` joins two locations; a line with only a name is a location with no walkways

Lines that are empty or start with `#` are ignored. Bad or duplicate lines are skipped with a message.

## Project layout
```
src/    source and header files
data/   sample data files
docs/   design notes for each student, and the technical report
tests/  test cases and results
```

## Group workflow
Each student worked on a branch (`a/core-data`, `b/requests`, `c/advanced-structures`) and merged by pull request.
See `CONTRIBUTING.md`.
