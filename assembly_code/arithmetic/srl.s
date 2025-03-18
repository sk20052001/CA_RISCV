 # Test case 1: Random values and shift amounts within the range of 0 to 31
    addi x3, x0, 0xAA    # Load lower byte (0b10101010)
    slli x3, x3, 8       
    ori  x3, x3, 0xAA    # OR with next byte (0b10101010)
    slli x3, x3, 8
    ori  x3, x3, 0xAA    # OR with next byte (0b10101010)
    slli x3, x3, 8
    ori  x3, x3, 0xAA    # Complete 32-bit value: 0xAAAAAAAA
    addi x4, x0, 10      # Shift amount = 10
    srl  x5, x3, x4      # x5 = x3 >> 10 (logical shift)

    # Test case 2: Input is positive, shift amount is 1
    addi x6, x0, 16      # Load 16 (positive)
    addi x7, x0, 1       # Shift amount = 1
    srl  x8, x6, x7      # x8 = 16 >> 1 = 8

    # Test case 3: Some input value, shift amount = 31
    addi x9, x0, 1       # Load 1
    slli x9, x9, 31      # Shift left to form 0x80000000
    addi x10, x0, 31     # Shift amount = 31
    srl  x11, x9, x10    # x11 = x9 >> 31 (should be 1)

    # Test case 4: Input value is all ones, shift amount is zero
    addi x12, x0, -1     # Load all ones (0xFFFFFFFF)
    addi x13, x0, 0      # Shift amount = 0
    srl  x14, x12, x13   # x14 = x12 >> 0 (should remain 0xFFFFFFFF)

    # Test case 5: Input value is zero, shift amount is zero
    addi x15, x0, 0      # Load 0
    addi x16, x0, 0      # Shift amount = 0
    srl  x17, x15, x16   # x17 = 0 >> 0 (should remain 0)

    # Test case 6: Checking whether only lower 5 bits of rs2 are used
    addi x18, x0, 0xF0   # Load 0b11110000
    slli x18, x18, 8
    ori  x18, x18, 0xF0  # Extend to 0b1111000011110000
    slli x18, x18, 8
    ori  x18, x18, 0xF0  # Extend to 0b111100001111000011110000
    slli x18, x18, 8
    ori  x18, x18, 0xF0  # Complete 32-bit value: 0xF0F0F0F0
    addi x19, x0, 37     # Load 37 (0b100101)
    srl  x20, x18, x19   # x20 = x18 >> (37 % 32) = x18 >> 5
