# bprintf

**bprintf** is a freestanding, libc-free formatted print engine for bootloaders, kernels, and bare-metal environments. It writes directly to VGA memory (`0xB8000`) and supports essential format specifiers without relying on any standard library.

Built entirely in raw C, `bprintf` is designed for low-level systems where `printf()` isn't available. It uses manual stack walking (`va_list`) and custom base conversion routines to render output directly to screen.

NOTE: "This is a custom bare-metal implementation of bprintf, not derived from any existing codebase"

---

## ✨ Features

- ✅ Direct VGA text output (`0xB8000`)
- ✅ Format specifier support:
  - `%c` → character
  - `%s` → string
  - `%d` → signed decimal
  - `%u` → unsigned decimal
  - `%x` → hexadecimal (with optional `0x` prefix)
  - `%b` → binary (with optional `0b` prefix)
  - `%p` → pointer (hex with `0x` prefix)
  - `%%` → literal percent sign
- ✅ Graceful fallback for unknown specifiers (e.g., `%z` → prints `%z`)
- ✅ Manual variadic argument parsing via `va_list`
- ✅ No dependencies, no libc, no OS—fully freestanding
- ✅ Optional prefix control for hex and binary output

---

## 📦 Size

The compiled `bprintf` implementation currently occupies **~2.3 KB** of machine code when targeting a 32-bit freestanding x86 environment with `-O2` optimization.

This makes it lightweight enough for kernels and minimal OS projects, while still supporting multiple format specifiers and VGA output.

---


🛠️ Bare-Metal Output Preview
Here’s a real-time VGA output from bprintf running in a freestanding kernel environment:
<img width="1761" height="865" alt="Screenshot From 2025-10-02 23-06-00" src="https://github.com/user-attachments/assets/87f226ac-5e5c-4ac5-acad-2b0373ec74e2" />


---

## 🧠 Example Usage

```c
bprintf("Hello %s, value = %d, hex = %x, ptr = %p, bin = %b\n",
        "world", 42, 42, (void*)0xCAFEBABE, 42);


