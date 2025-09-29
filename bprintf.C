typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((unsigned short*)0xB8000)

static unsigned short* vga_buffer = VGA_MEMORY;
static unsigned int cursor_row = 0;
static unsigned int cursor_col = 0;

void putchar(char c) {
  if (c == '\n') {
    cursor_row++;
    cursor_col = 0;
  } else {
    unsigned int index = cursor_row * VGA_WIDTH + cursor_col;
    vga_buffer[index] = (15 << 8) | c;
    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
      cursor_col = 0;
      cursor_row++;
    }
  }
  if (cursor_row >= VGA_HEIGHT) {
    cursor_row = 0;
  }
}

void print_str(const char* str) {
  while (*str) putchar(*str++);
}

void print_hex(unsigned int val) {
  putchar('0');
  putchar('x');
  char hex[] = "0123456789ABCDEF";
  int hexbuf_size = sizeof(val) * 2;
  char buf[hexbuf_size];
  int i = 0;
  do {
    if (i >= hexbuf_size) break;
    buf[i++] = hex[val & 0xF];
    val >>= 4;
  } while (val);
  while (i--) putchar(buf[i]);
}

void print_bin(unsigned int val) {
  putchar('0');
  putchar('b');
  int bits = sizeof(val) * 8;
  char buf[bits];
  int i = 0;
  do {
    if (i >= bits) break;
    buf[i++] = '0' + (val & 1);
    val >>= 1;
  } while (val);
  while (i--) putchar(buf[i]);
}

void print_uint(unsigned int val) {
  int digits = 0;
  unsigned int temp = val;
  do {
    digits++;
    temp /= 10;
  } while (temp);

  char buf[digits];
  int i = 0;
  do {
    buf[i++] = '0' + (val % 10);
    val /= 10;
  } while (val);
  while (i--) putchar(buf[i]);
}

void print_int(int val) {
  char buf[12];
  int i = 0;
  if (val < 0) {
    putchar('-');
    val = -val;
  }
  do {
    buf[i++] = '0' + (val % 10);
    val /= 10;
  } while (val);
  while (i--) putchar(buf[i]);
}

void bprintf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  for (unsigned int i = 0; fmt[i]; i++) {
    if (fmt[i] == '%' && fmt[i + 1]) {
      i++;
      switch (fmt[i]) {
        case 'c': {
          char c = va_arg(args, int);
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
          print_hex(x);
          break;
        }
        case 'b': {
          unsigned int b = va_arg(args, unsigned int);
          print_bin(b);
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
