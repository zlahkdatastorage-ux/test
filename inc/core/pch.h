#pragma once

// ── Platform / compiler compat ──────────────────────────────
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <conio.h>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <tuple>
#include <utility>
#include <memory>

// ════════════════════════════════════════════════════════════════
// Container headers (needed by various modules)
// ════════════════════════════════════════════════════════════════
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <deque>
#include <bitset>
#include <array>

// ════════════════════════════════════════════════════════════════
// Numeric / exception / C-string headers
// ════════════════════════════════════════════════════════════════
#include <limits>
#include <stdexcept>
#include <cstring>
#include <cctype>
#include <ctime>
#include <cwchar>
#include <new>

// ════════════════════════════════════════════════════════════════
// DirectX headers (for renderer)
// ════════════════════════════════════════════════════════════════
#include <d3d11.h>
#include <d3dcompiler.h>

// ════════════════════════════════════════════════════════════════
// Synchronization headers
// ════════════════════════════════════════════════════════════════
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>

// ════════════════════════════════════════════════════════════════
// I/O headers
// ════════════════════════════════════════════════════════════════
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <algorithm>
#include <thread>
#include <chrono>

// ── Basic type aliases (available everywhere) ───────────────
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
