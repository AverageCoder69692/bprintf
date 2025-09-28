# bprintf

**bprintf** is a freestanding, libc-free formatted print engine for bootloaders, kernels, and bare-metal environments. It writes directly to VGA memory and supports basic format specifiers without relying on any standard library.

Built entirely in C, `bprintf` is designed for low-level systems where `printf()` isn't available. It uses manual stack walking (`va_list`) and custom base conversion routines to render output directly to screen.

NOTE: "This is a custom bare-metal implementation of bprintf, not derived from any existing code"

---

## ✨ Features

- ✅ Direct VGA text output (`0xB8000`)
- ✅ Supports format specifiers:
  - `%c` → character
  - `%s` → string
  - `%d` → signed decimal
  - `%u` → unsigned decimal
  - `%x` → hexadecimal
  - `%b` → binary
  - `%%` → literal percent sign
- ✅ Graceful fallback for unknown specifiers (e.g., `%z` → prints `%z`)
- ✅ Manual variadic argument parsing (`va_list`)
- ✅ No dependencies, no libc, no OS

---

## 📦 Size

The compiled `bprintf` implementation typically occupies **~0.5 to 0.6 KB** of machine code when targeting a 32-bit freestanding x86 environment with no optimization. With `-Os` or `-O2`, it may shrink to **~0.4 KB**.

This makes it ideal for bootloaders, microkernels, and minimal OS projects where every byte counts.

---

## 🧠 Use Case

```c
bprintf("Hello %s, value = %d, hex = %x\n", "world", 42, 42);
