# Project 3
## File Structure
proj3_25sp
├── proj3/
├── include/
│   └── bankers_resource_manager.h
├── src/
│   └── bankers_resource_manager.cc
|
├── sync/
├── include/
│   └── thread_mutex.h
├── src/
│   └── thread_mutex.cc
| 
└── README.md

## Build Instructions
From the proj3 directory, run:
  make

To clean the build, run:
  make clean

## Execution Instructions
The executable is named "bankers-threads" and expects the following command-line arguments:
1. A random seed
2. A space-separated string for the available resources
3. One or more space-separated strings for each thread's maximum resource need

Example commands:
  ./bankers-threads 7 "5 5 5" "2 3 4" "1 5 5" "2 3 3" "5 5 1"
  ./bankers-threads 13 "7 5 9" "1 1 9" "7 1 1" "1 1 9" "7 5 9"

## Provided Files
- **include/bankers_resource_manager.h**  
  Declares the BankersResourceManager class, its public interface (constructor, AddMax, Request, Release), and any helper methods.

- **src/bankers_resource_manager.cc**  
  Implements the BankersResourceManager class, including the safety-check logic and resource allocation updates.
