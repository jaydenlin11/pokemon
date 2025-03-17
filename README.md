# 🏆 PokePath Finder

A tool inspired by the University of Michigan's EECS 281 course, designed to explore graph algorithms through a Pokémon-themed adventure. Built to optimize travel routes for catching Pokémon across a map, this project tackles Minimum Spanning Trees (MST) and the Travelling Salesperson Problem (TSP).

📅 **Current Date:** March 17, 2025

## 🚀 Overview

PokePath Finder processes a graph of x-y coordinates (Pokémon locations) and applies rules to connect them. It features three modes:

- 🌳 **MST Mode**: Computes a Minimum Spanning Tree to connect all locations with minimal edge weight, respecting land, sea, and coastline constraints.
- ⚡ **FASTTSP Mode**: Approximates a TSP solution quickly using heuristics for a near-optimal route.
- 🎯 **OPTTSP Mode**: Finds the optimal TSP route using a branch-and-bound approach.

## 🎯 Goals

- 🔍 Master MST algorithms (Prim’s or Kruskal’s) and determine their efficiency.
- 🏃 Implement a fast, effective TSP heuristic and an optimal branch-and-bound solution.
- 🖥️ Use graph visualization for debugging and explore trade-offs in algorithm design.

## 🎮 Backstory

As a Pokémon Trainer starting with Pikachu ⚡, I set out from Pallet Town to catch every Pokémon. The map spans land, sea, and coastline, with trainers complicating travel. MST mode clears paths by minimizing trainer battles, while TSP modes optimize a full journey using flight. ✈️

## 🛠️ Usage

Run the program with:

```bash
./poke --mode {MST|FASTTSP|OPTTSP} < input.txt
```

- `-m, --mode`: Specify MST, FASTTSP, or OPTTSP (required).
- `-h, --help`: Display usage info and exit.
- Input via `cin` or file redirection (e.g., `< input.txt`).

### 📄 Input Format
A list of coordinates:

```
5
6 1
2 3
-5 -4
-1 6
0 -1
```

### 📤 Output Format

- **MST**: Total weight followed by edges (e.g., `19.02\\n0 1\\n2 4`).
- **FASTTSP/OPTTSP**: Tour length and node sequence (e.g., `31.64\\n0 4 2 3 1`).

## 🔬 Logic and Implementation

### 🌳 MST Mode

- **Algorithm**: Uses **Prim’s or Kruskal’s** depending on graph density.
  - **Sparse graphs**: Kruskal’s edge-sorting approach (`O(E log E)`) is preferred.
  - **Dense graphs**: Prim’s priority-queue method (`O(E log V)`) is better.
  - **Choice**: Kruskal’s due to Euclidean complete graphs, using **Union-Find** for cycle detection.
- **Area Constraints**: 
  - Land (QI, QII, QIV), Sea (QIII), and Coastline (origin, negative axes).
  - Direct connections only within areas or via coastline; land-sea without coastline is ❌.
- **Distance Calculation**: Uses **Euclidean distance**, computed on-the-fly to save memory.

### ⚡ FASTTSP Mode

- **Heuristic**: Implements **greedy nearest-neighbor**, starting at vertex 0 and picking the closest unvisited node.
- **2-opt Optimization**: Iteratively swaps edges to improve the tour (`O(n^2)`).
- **Distance**: Uses **Euclidean distances**, ignoring area constraints (assumes flight ✈️).

### 🎯 OPTTSP Mode

- **Algorithm**: Uses **branch-and-bound** with a **permutation generator (`genPerms`)**.
- **Bounding Function**: 
  - Uses MST of unvisited nodes + shortest connections.
  - Skips branches exceeding the best-known tour to reduce search space.
- **Optimization**: 
  - Efficient **O(n!)** permutation swaps.
  - Precomputed MST bounds from Part A for faster pruning.

## 🔥 Features

✅ **MST Mode**: Dynamic graph handling with **Union-Find**.  
✅ **FASTTSP Mode**: **Greedy heuristic** + **2-opt refinement**.  
✅ **OPTTSP Mode**: **Guaranteed optimal solution** via exhaustive search.  
✅ **Efficient Data Structures**: **Vectors, priority queues, Union-Find**.  

## 🏗️ Building

Requires **g++ 6.2.0** with `-O3` optimization. Use:

```bash
make -R -r
```

## 🧪 Testing

Includes test files (`test-n-MODE.txt`) to debug and validate solutions. Run with up to **10 coordinates per file**.

## ⚠️ Constraints

- ❌ No C++11 regex, threads, or smart pointers.
- 💾 **Memory-efficient** for large inputs (up to **10,000 nodes**).

## 🎨 Inspiration

Crafted from my **love of graph theory and Pokémon nostalgia** 🎮🐉. Enjoy catching 'em all! 🔥
