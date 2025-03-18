    # Test case 1: XOR-ing with 1 (edge case)
    addi x3, x0, 0b1010  # Load binary 1010 (decimal 10) into x3
    addi x4, x0, 0b0001  # Load binary 0001 (decimal 1) into x4
    xor  x5, x3, x4      # x5 = 1010 XOR 0001 = 1011 (decimal 11)

    # Test case 2: XOR-ing with 0 (edge case)
    addi x6, x0, 0b1100  # Load binary 1100 (decimal 12) into x6
    addi x7, x0, 0b0000  # Load binary 0000 (decimal 0) into x7
    xor  x8, x6, x7      # x8 = 1100 XOR 0000 = 1100 (decimal 12, unchanged)

    # Test case 3: XOR correctly updates rd
    addi x9, x0, 0b1111  # Load binary 1111 (decimal 15) into x9
    addi x10, x0, 0b1010 # Load binary 1010 (decimal 10) into x10
    xor  x11, x9, x10    # x11 should now contain 0101 (decimal 5)

    # Test case 4: Correct bitwise XOR operation
    addi x12, x0, 0b0110 # Load binary 0110 (decimal 6) into x12
    addi x13, x0, 0b1011 # Load binary 1011 (decimal 11) into x13
    xor  x14, x12, x13   # x14 = 0110 XOR 1011 = 1101 (decimal 13)