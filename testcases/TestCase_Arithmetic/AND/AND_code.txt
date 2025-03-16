    # Test case 1: OR - One register all zero and other is all 1s
    addi x3, x0, 0        # x3 = 0 (all bits 0)
    addi x4, x0, -1       # x4 = -1 (0xFFFFFFFF, all bits 1)
    or   x5, x3, x4       # x5 = x3 | x4 → Expect 0xFFFFFFFF (all bits 1)

    # Test case 2: OR - One register all 1s and other is all zero
    addi x6, x0, -1       # x6 = -1 (0xFFFFFFFF, all bits 1)
    addi x7, x0, 0        # x7 = 0 (all bits 0)
    or   x8, x6, x7       # x8 = x6 | x7 → Expect 0xFFFFFFFF (all bits 1)

    # Test case 3: AND - One register all zero and other is all 1s
    addi x9, x0, 0        # x9 = 0 (all bits 0)
    addi x10, x0, -1      # x10 = -1 (0xFFFFFFFF, all bits 1)
    and  x11, x9, x10     # x11 = x9 & x10 → Expect 0 (all bits 0)

    # Test case 4: AND - One register all 1s and other is all zero
    addi x12, x0, -1      # x12 = -1 (0xFFFFFFFF, all bits 1)
    addi x13, x0, 0       # x13 = 0 (all bits 0)
    and  x14, x12, x13    # x14 = x12 & x13 → Expect 0 (all bits 0)