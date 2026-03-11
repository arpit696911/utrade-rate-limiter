# Concurrent Rate Limiter Service

This project implements a **thread-safe rate limiter** designed to control the number of requests a client can make within a given time window.  
Rate limiting is an important component of **API gateways, trading systems, and backend infrastructure**, where it helps prevent abuse, protects services from overload, and ensures fair usage among clients.

The implementation is written in **C++** and demonstrates concurrent request handling using multiple threads.

---

## Features

- **Fixed Window Rate Limiting**
  - Limits the number of requests allowed within a fixed time window.

- **Sliding Window Rate Limiting**
  - Provides smoother rate limiting compared to the fixed window approach.

- **Thread-safe Implementation**
  - Uses `std::mutex` to safely handle concurrent requests from multiple threads.

- **Multi-client Simulation**
  - Simulates multiple clients sending requests simultaneously.

- **Configurable Limits**
  - Rate limits can be configured using a configuration file.

- **Request Logging**
  - Each request logs the client ID, request number, and whether it was allowed or blocked.

- **Summary Statistics**
  - Displays the total number of requests processed along with allowed and rejected counts.

---

## Project Structure

utrade-rate-limiter
│
├── src
│ ├── main.cpp
│ ├── rate_limiter.cpp
│ └── rate_limiter.h
│
├── config.json
├── README.md
└── .gitignore

---

## How It Works

The rate limiter keeps track of requests made by each client.  
If a client exceeds the allowed number of requests within the configured time window, further requests are blocked until the window resets.

Multiple threads simulate clients sending requests concurrently, allowing us to test the thread-safety and performance of the limiter.

---

## Example Output

Below is a sample output from the rate limiter simulation.

<img src="https://github.com/user-attachments/assets/9aa5ae3f-a4fe-414f-b463-9f7008b8b370" width="700">

This indicates that once a client reaches the request limit, further requests are rejected until the window resets.

---

## Running the Project

### Compile

### Run

---

## Test Simulation

The program simulates:

- **5 clients**
- **20 requests per client**
- **100 total requests**

Requests are sent concurrently using multiple threads.

---

## Technologies Used

- **C++**
- `std::thread`
- `std::mutex`
- `std::unordered_map`
- Standard C++ concurrency utilities

---

## Possible Improvements

Some possible extensions to this project include:

- Adding an **HTTP interface** for real API usage
- Supporting **per-client custom limits**
- Implementing **Token Bucket rate limiting**
- Adding **real-time configuration updates**

---

## Summary

This project demonstrates a simple but practical implementation of a **concurrent rate limiter** that can be used as a building block for API gateways and backend infrastructure systems.


