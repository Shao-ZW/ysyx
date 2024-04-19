#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

// int sprintf(char *out, const char *fmt, ...) {
//   va_list args;
//   int fi, oi;
//   char c, *s;
//   char buf[12];

//   va_start(args, fmt);

//   for(fi = 0, oi = 0; (c = fmt[fi]) != '\0'; fi++) {
//     if(c != '%') {
//       out[oi++] = c;
//     }
//     else {
//       switch((c = fmt[++fi])) {
//         case 'd': {
//           int arg = va_arg(args, int);
//           int t = 0;

//           if (arg < 0) {
//             out[oi++] = '-';
//             arg = -arg;
//           }
//           while(arg) {
//             buf[t++] = arg % 10;
//             arg /= 10;
//           }
//           while(t--) {
//             out[oi++] = buf[t];
//           }
//           break;
//         }
//         case 's': {
//           if((s = va_arg(args, char*)) == 0)
//             s = "(null)";
//           while(*s) {
//             out[oi++] = *s;
//             s++;
//           }
//           break;
//         }
//         case '%':
//           out[oi++] = '%';
//           break;
//         default:
//           out[oi++] = '%';
//           out[oi++] = c;
//           break;
//       }
//     }
//   }

//   va_end(args);
//   out[oi] = '\0';
  
//   return oi;
// }
int sprintf(char *out, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int oi = 0;

    while (*fmt != '\0') {
        if (*fmt != '%') {
            out[oi++] = *fmt;
            fmt++;
            continue;
        }

        fmt++;

        switch (*fmt) {
            case 'd': {
                int arg = va_arg(args, int);
                int t = 0;
                char buf[20]; // 可能的整数最大位数
                if (arg < 0) {
                    out[oi++] = '-';
                    arg = -arg;
                }
                do {
                    buf[t++] = arg % 10 + '0';
                    arg /= 10;
                } while (arg != 0);
                while (t > 0) {
                    out[oi++] = buf[--t];
                }
                break;
            }
            case 's': {
                const char *s = va_arg(args, const char*);
                if (!s) {
                    s = "(null)";
                }
                while (*s != '\0') {
                    out[oi++] = *s++;
                }
                break;
            }
            case '%':
                out[oi++] = '%';
                break;
            default:
                out[oi++] = '%';
                out[oi++] = *fmt;
                break;
        }
        fmt++;
    }

    va_end(args);

    out[oi] = '\0'; // 添加字符串结束符

    return oi;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
