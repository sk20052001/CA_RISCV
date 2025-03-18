    # Test case 1: One register is 'b0 (all bits 0)
    addi x3, x0, 0       # Load 0 into x3
    addi x4, x0, 0b1010  # Load 0b1010 into x4
    or   x5, x3, x4      # x5 = x3 | x4 (should be 0b1010)

    # Test case 2: One register is 'b1 (all bits 1)
    addi x6, x0, -1      # Load -1 (0xFFFFFFFF, all bits 1)
    addi x7, x0, 0b0101  # Load 0b0101 into x7
    or   x8, x6, x7      # x8 = x6 | x7 (should be 0xFFFFFFFF, all bits remain 1)

    # Test case 3: One register is 'b1010 and other is 'b0100
    addi x9, x0, 0b1010  # Load 0b1010 into x9
    addi x10, x0, 0b0100 # Load 0b0100 into x10
    or   x11, x9, x10    # x11 = x9 | x10 (should be 0b1110)