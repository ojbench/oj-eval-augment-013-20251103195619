# Submission Summary for Problem 013

## Overview
Successfully implemented a Red-Black Tree based map data structure for STLite Map assignment.

## Results

### Problem 2671 (map)
- **Submission ID**: 706989
- **Status**: Accepted ✅
- **Score**: 100/100 (100%)
- **Time**: 13687 ms
- **Memory**: 212 MB

All test groups passed:
- one: 10/10
- one.memcheck: 10/10
- two: 10/10
- two.memcheck: 10/10
- three: 10/10
- three.memcheck: 10/10
- four: 10/10
- four.memcheck: 10/10
- five: 10/10
- five.memcheck: 10/10

### Problem 2672 (map extra)
- **Submission ID**: 706991
- **Status**: Accepted ✅
- **Score**: 100/100 (100%)
- **Time**: 37455 ms
- **Memory**: 372 MB

All test groups passed:
- Extra corner tests: 30/30
- Benchmark: Insert: 10/10
- Benchmark: Erase: 10/10
- Benchmark: Travel: 10/10
- Benchmark: Find: 10/10
- Benchmark: Min & Max: 10/10
- Benchmark: Operator []: 10/10
- Benchmark: Copy & Clear: 10/10

## Implementation Details

### Data Structure
- **Type**: Red-Black Tree (self-balancing binary search tree)
- **Features**:
  - Full iterator support (bidirectional)
  - Const iterator support
  - All required map operations (insert, erase, find, at, operator[], etc.)
  - Proper memory management (no memory leaks)
  - Exception handling for invalid operations

### Key Features
1. **Red-Black Tree Properties**:
   - Self-balancing for O(log n) operations
   - Maintains sorted order
   - Efficient insertion and deletion with rebalancing

2. **Iterator Implementation**:
   - Bidirectional traversal
   - Support for both iterator and const_iterator
   - Proper handling of end() iterator
   - Decrement from end() returns last element

3. **Memory Management**:
   - No memory leaks (verified with memcheck tests)
   - Proper cleanup in destructor
   - Deep copy in copy constructor and assignment operator

4. **Robustness**:
   - Handles types without default constructors
   - Proper exception throwing for invalid operations
   - Boundary condition handling

## Submission Attempts Used
- **Total Attempts**: 2 out of 3 allowed
- **Problem 2671**: 1 attempt (100% score)
- **Problem 2672**: 1 attempt (100% score)
- **Remaining Attempts**: 1

## Performance Notes
The implementation passes all benchmarks with acceptable performance:
- Find operations: Comparable to std::map
- Operator[] operations: Comparable to std::map
- Insert/Erase/Travel: Slightly slower than std::map but within acceptable range
- All operations complete within time limits

## Files Modified
1. `src/map.hpp` - Complete Red-Black Tree implementation
2. `submit_acmoj/acmoj_client.py` - Fixed missing submit_code method

## Git Commits
1. `b25b8a6` - Implement Red-Black Tree based map with all required functionality
2. `3a830f5` - Fix acmoj_client.py and achieve 100% score on both problems 2671 and 2672

## Conclusion
Successfully completed the assignment with perfect scores on both problems using only 2 out of 3 allowed submission attempts.
