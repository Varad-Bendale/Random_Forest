## Random_Forest

Every `.fit()` call hides a hundred small decisions — which column to split on, where to draw the line, which tree gets the final vote. Random forests are one of the most-used algorithms in ML, yet one of the least understood at the code level.

Random_Forest prys that open, built from scratch in C++ — not just to work, but to run fast enough to actually mean something. Trees are grown in parallel, and the way the data gets sorted and stored was built to keep that process as lean as possible.

## Repository Structure

```
Random_Forest/
├── README.md                # Project documentation
├── rf.cpp                    # Entire code
├── rf                        # The executed binary file
├── issues / future scope     # Issues or the future scope of the project
└── docs/
     └── documentation.txt    # Pipeline / design notes
```

## Architecture Overview

```
+-------------------------------------------------------------------+
|                          PRE-PROCESSING                           |
|                                                                     |
|   +---------------+     +---------------+     +----------------+   |
|   |   Transpose   | --> |  Sort columns | --> |  Collect unique|   |
|   |    dataset    |     |               |     | target classes |   |
|   +---------------+     +---------------+     +----------------+   |
+------------------------------|--------------------------------------+
                                v
+-------------------------------------------------------------------+
|                  FOREST BUILDING  [parallel, OpenMP]               |
|                                                                     |
|        +----------------+       +----------------+                 |
|        | Bootstrap rows |       | Subsample cols |                 |
|        +--------+-------+       +--------+-------+                 |
|                 \_________________________/                        |
|                            |                                        |
|                            v                                        |
|                   +------------------+                              |
|                   |   build_tree()   |                              |
|                   +--------+---------+                              |
|                            |                                        |
|                 +----------+----------+                             |
|                 v                     v                             |
|          +--------------+     +-------------------+                 |
|          |  perfect()   | --> | gini_impurity_    |                 |
|          |  best split  |     |   subtree()        |                 |
|          +------+-------+     +-------------------+                 |
|                 |                                                    |
|                 v                                                    |
|          +----------------+                                         |
|          | new_dataset()  |                                         |
|          |  upper/lower   |                                         |
|          +-------+--------+                                         |
|                  |                                                   |
|            +-----+-----+                                            |
|            v           v                                            |
|         +-----+     +-----+                                         |
|         | rec |     | rec |                                         |
|         +--+--+     +--+--+                                         |
|            |           |                                            |
|            v           v                                            |
|         +--------------------+                                      |
|         |     leaf(node)     |                                      |
|         |   majority class   |                                      |
|         +--------------------+                                      |
+------------------------------|--------------------------------------+
                                v
+-------------------------------------------------------------------+
|                            PREDICTION                              |
|                                                                     |
|      +----------------+       +--------------------+               |
|      |   traverse()   |  -->  |   majority_vote()  |               |
|      | per tree, L/R  |       |   final prediction |               |
|      +----------------+       +--------------------+               |
+-------------------------------------------------------------------+
```

## Prerequisites

* `g++` (or `clang++`) — C++ compiler, with OpenMP support
* A Linux/macOS environment — file I/O assumes POSIX-style paths

## Steps to Access

```
g++ -fopenmp rf.cpp -o rf
./rf filename.csv
```

Currently only supports CSV data.
