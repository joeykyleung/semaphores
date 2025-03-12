# Semaphore Implementation and Synchronization Examples

## Overview
This repository demonstrates practical implementations of semaphores and their applications in solving classical synchronization problems. It serves as both an educational resource and a demonstration of concurrent programming expertise.

## Key Concepts Covered

### 1. Semaphore Implementation
- Binary and counting semaphores
- Atomic operations
- Mutex vs Semaphore distinctions
- Thread-safe implementations

### 2. Classical Problems Solved
- Producer-Consumer Problem
- Dining Philosophers Problem
- Readers-Writers Problem
- Sleeping Barber Problem

## Technical Implementation Details

### Core Components
- **Atomic Operations**: Implementation of thread-safe operations using atomic primitives
- **Queue Management**: FIFO queue for blocked processes
- **Deadlock Prevention**: Implementation of various deadlock prevention strategies
- **Starvation Avoidance**: Fair scheduling mechanisms

### Technical Challenges & Solutions

#### 1. Race Condition Prevention
- **Challenge**: Ensuring atomic operations in critical sections
- **Solution**: Implemented mutex locks and atomic operations to maintain thread safety
```c
// Example of atomic implementation
atomic_int value;
atomic_flag lock = ATOMIC_FLAG_INIT;
```

#### 2. Deadlock Prevention
- **Challenge**: Avoiding circular wait conditions in multi-resource scenarios
- **Solution**: Implemented resource hierarchy and timeout mechanisms

#### 3. Performance Optimization
- **Challenge**: Minimizing context switches and overhead
- **Solution**: Used spin-locks for short critical sections and implemented adaptive waiting

## Usage Examples

### Basic Semaphore Usage
```c
// Creating a semaphore
sem_t semaphore;
sem_init(&semaphore, 0, 1);

// Wait operation
sem_wait(&semaphore);
// Critical section
sem_post(&semaphore);
```

### Producer-Consumer Implementation
```c
// Buffer operations protected by semaphores
sem_t empty, full, mutex;

void producer() {
    sem_wait(&empty);
    sem_wait(&mutex);
    // Add to buffer
    sem_post(&mutex);
    sem_post(&full);
}

void consumer() {
    sem_wait(&full);
    sem_wait(&mutex);
    // Remove from buffer
    sem_post(&mutex);
    sem_post(&empty);
}
```

## Project Structure
```
semaphores/
├── src/
│   ├── semaphore.c      # Core semaphore implementation
│   ├── producer_consumer.c
│   ├── dining_philosophers.c
│   └── readers_writers.c
├── include/
│   └── semaphore.h      # Header files
└── tests/               # Test cases
```

## Building and Running

### Prerequisites
- GCC compiler with C11 support
- POSIX threads library (pthread)
- CMake 3.x or higher

### Build Instructions
```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests
```bash
./run_tests
```

## Performance Considerations
- Minimal context switching overhead
- Efficient queue management for blocked processes
- Optimized for both short and long critical sections
- Memory-efficient implementation