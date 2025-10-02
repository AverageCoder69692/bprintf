typedef unsigned char bool;
#define true 1
#define false 0

typedef unsigned int uintptr_t;

// VGA definitions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((unsigned short*)0xB8000)

static unsigned short* vga_buffer = VGA_MEMORY;
static unsigned int cursor_row = 0;
static unsigned int cursor_col = 0;

// ---- Basic putchar with scrolling ----
void putchar(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else {
        unsigned int index = cursor_row * VGA_WIDTH + cursor_col;
        vga_buffer[index] = (15 << 8) | c; // white on black
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    // Scroll if needed
    if (cursor_row >= VGA_HEIGHT) {
        for (int r = 1; r < VGA_HEIGHT; r++) {
            for (int c = 0; c < VGA_WIDTH; c++) {
                vga_buffer[(r-1)*VGA_WIDTH + c] = vga_buffer[r*VGA_WIDTH + c];
            }
        }
        // clear last row
        for (int c = 0; c < VGA_WIDTH; c++) {
            vga_buffer[(VGA_HEIGHT-1)*VGA_WIDTH + c] = (15 << 8) | ' ';
        }
        cursor_row = VGA_HEIGHT - 1;
    }
}

// ---- String printing ----
void print_str(const char* str) {
    while (*str) putchar(*str++);
}

// ---- Unsigned integer printing ----
void print_uint(unsigned int val) {
    char buf[10]; // max digits in 32-bit uint
    int i = 0;
    if (val == 0) {
        putchar('0');
        return;
    }
    while (val) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    while (i--) putchar(buf[i]);
}

// ---- Signed integer printing (handles INT_MIN) ----
void print_int(int val) {
    if (val < 0) {
        putchar('-');
        if (val == -2147483648) { // INT_MIN
            print_str("2147483648");
            return;
        }
        val = -val;
    }
    print_uint((unsigned int)val);
}

// ---- Hex printing ----
void print_hex(uintptr_t val, bool prefix) {
    if (prefix) {
        putchar('0');
        putchar('x');
    }
    if (val == 0) {
        putchar('0');
        return;
    }

    char hex[] = "0123456789ABCDEF";
    char buf[sizeof(val)*2];
    int i = 0;
    while (val) {
        buf[i++] = hex[val & 0xF];
        val >>= 4;
    }
    while (i--) putchar(buf[i]);
}

// ---- Binary printing ----
void print_bin(unsigned int val, bool prefix) {
    if (prefix) {
        putchar('0');
        putchar('b');
    }
    if (val == 0) {
        putchar('0');
        return;
    }

    char buf[32];
    int i = 0;
    while (val) {
        buf[i++] = '0' + (val & 1);
        val >>= 1;
    }
    while (i--) putchar(buf[i]);
}

// ---- Variadic macros for bare-metal ----
typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

// ---- Main printf function ----
void bprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (unsigned int i = 0; fmt[i]; i++) {
        if (fmt[i] == '%' && fmt[i+1]) {
            i++;
            switch (fmt[i]) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    putchar(c);
                    break;
                }
                case 's': {
                    const char* s = va_arg(args, const char*);
                    print_str(s);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int);
                    print_int(d);
                    break;
                }
                case 'u': {
                    unsigned int u = va_arg(args, unsigned int);
                    print_uint(u);
                    break;
                }
                case 'x': {
                    unsigned int x = va_arg(args, unsigned int);
                    print_hex(x, true);
                    break;
                }
                case 'b': {
                    unsigned int b = va_arg(args, unsigned int);
                    print_bin(b, true);
                    break;
                }
                case 'p': {
                    void* ptr = va_arg(args, void*);
                    print_hex((uintptr_t)ptr, true);
                    break;
                }
                case '%': {
                    putchar('%');
                    break;
                }
                default: {
                    putchar('%');
                    putchar(fmt[i]);
                    break;
                }
            }
        } else {
            putchar(fmt[i]);
        }
    }

    va_end(args);
}
