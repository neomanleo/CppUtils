#define ALIGN_MOD8(num)                                                        \
  if ((capacity % 8) != 0) {                                                   \
    capacity += (8 - (capacity % 8));                                          \
  }
