  # Test case 1: Subtracting 0 (edge case)
    addi x3, x0, 25      # Load 25 into x3
    addi x4, x0, 0       # Load 0 into x4
    sub  x5, x3, x4      # x5 = x3 - x4 (should be 25)

    # Test case 2: Basic subtraction between two registers
    addi x6, x0, 50      # Load 50 into x6
    addi x7, x0, 20      # Load 20 into x7
    sub  x8, x6, x7      # x8 = x6 - x7 (should be 30)

    # Test case 3: Negative and positive number combinations
    addi x9, x0, -30     # Load -30 into x9
    addi x10, x0, 15     # Load 15 into x10
    sub  x11, x9, x10    # x11 = x9 - x10 (should be -45)

    addi x12, x0, 40     # Load 40 into x12
    addi x13, x0, -20    # Load -20 into x13
    sub  x14, x12, x13   # x14 = x12 - x13 (should be 60)

    # Test case 4: Overflow (edge case)
    lui  x15, 0x80000    # Load upper 20 bits of -2147483648 (0x80000000)
    addi x15, x15, 0     # Ensure lower bits are 0
    addi x16, x0, 1      # Load 1 into x16
    sub  x17, x15, x16   # x17 = x15 - x16 (should overflow to 2147483647)

    # Test case 5: Underflow (edge case)
    lui  x18, 0x7FFF0    # Load upper 20 bits of 2147483647 (0x7FFFFFFF)
    addi x18, x18, 2047  # Add lower 12 bits (0x7FF = 2047)
    addi x19, x0, -1     # Load -1 into x19
    sub  x20, x18, x19   # x20 = x18 - x19 (should underflow to -2147483648)
