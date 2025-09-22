# Evaluation 02 – Practical Assignment (Moment 2)

**Course:** Data Structures and Algorithms  
**University:** EAFIT  

---

## Project Description
This project was developed as part of **Evaluation 02 – Practical Assignment (Moment 2)** for the course *Data Structures and Algorithms*.  

The goal is to implement and document solutions in **C++** focusing on **matrix operations and algorithmic efficiency**.  

The repository contains the source code and documentation for the following exercises:  

- **Point 1 – Matrix Multiplication Performance**  
  - Implementation of three algorithms for square matrix multiplication:  
    - **Base (i-j-k):** classical triple nested loops.  
    - **Optimized (i-k-j):** improved memory access order.  
    - **Blocked (tiling, BS=64):** divides matrices into blocks to take advantage of cache locality.  
  - Function to measure execution time using `<chrono>`.  
  - Demonstration with matrix sizes 256×256, 512×512, 768×768, 1024×1024, and 2048×2048.  
  - Comparison of runtimes between different implementations.  

- **Point 2 – Binary Search Tree (BST) Implementation**  
  - Implementation of a **Binary Search Tree (BST)** with integer keys.  
  - Features included:  
    - **Insertion:** new keys are inserted preserving BST ordering. Duplicate values are inserted to the right subtree.  
    - **Tree traversals:** Inorder, Preorder, and Postorder traversal functions that display the nodes in the console.  
    - **Height calculation:** function to compute the height of the tree (longest path from root to leaf).  
    - **Maximum degree:** function that returns the maximum degree (number of children) of any node in the tree.  
    - **Search with path tracking:** function that searches for a key and prints the path followed to reach it (whether found or not).  
  - Demonstration with test data:  
    - Inserted nodes: `{15, 10, 20, 8, 12, 17, 25, 23, 11, 28, 99, 12}`  
    - Traversals displayed in console.  
    - Height and maximum degree computed.  
    - Search tested with existing values (`12`, `99`) and a non-existing one (`30`).  
 

---

## Features
- Random generation of square matrices with values in range [0,1).  
- Three multiplication strategies: **Base**, **IKJ**, and **Blocked**.  
- Timing of each algorithm with high resolution clock.  
- Performance comparison across different matrix sizes.  
- Documentation of difficulties, approach, testing strategy, and results.  

---

## Language and Environment
- **Language:** C++  
- **IDE/Environment:** Visual Studio Code  
- **Compiler:** g++ (C++17 or later)  
- **Compilation (portable):**  
  ```bash
  g++ -O3 -std=c++17 main.cpp -o matmul
