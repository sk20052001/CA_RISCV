    # Test case 1: Basic shifting
    addi x3, x0, 64      # Load positive number 64
    addi x4, x0, 3       # Shift amount = 3
    sra  x5, x3, x4      # x5 = 64 >> 3 (Arithmetic shift)

    # Test case 2: MSB = 1 (negative number shift)
    addi x6, x0, -32     # Load -32 (MSB=1)
    addi x7, x0, 2       # Shift amount = 2
    sra  x8, x6, x7      # x8 = -32 >> 2 (Arithmetic shift, should keep sign extension)

    # Test case 3: rs1 = -64, rs2 = 3
    addi x9, x0, -64     # Load -64
    addi x10, x0, 3      # Shift amount = 3
    sra  x11, x9, x10    # x11 = -64 >> 3 (should be -8)

    # Test case 4: rs1 = +56, rs2 = 2
    addi x12, x0, 56     # Load 56
    addi x13, x0, 2      # Shift amount = 2
    sra  x14, x12, x13   # x14 = 56 >> 2 (should be 14)

    # Test case 5: rs1 = -48, rs2 = 5 (Only lower 5 bits of rs2 are used)
    addi x15, x0, -48    # Load -48
    addi x16, x0, 37     # Load 37 (0b100101) -> lower 5 bits = 5
    sra  x17, x15, x16   # x17 = -48 >> 5 (should be -2)

    # Test case 6: rs1 = +40, rs2 = 6 (Only lower 5 bits of rs2 are used)
    addi x18, x0, 40     # Load 40
    addi x19, x0, 38     # Load 38 (0b100110) -> lower 5 bits = 6
    sra  x20, x18, x19   # x20 = 40 >> 6 (should be 0)

    # Test case 7: rs1 = +32, rs2 = 1 (Only lower 5 bits of rs2 are used)
    addi x21, x0, 32     # Load 32
    addi x22, x0, 33     # Load 33 (0b100001) -> lower 5 bits = 1
    sra  x23, x21, x22   # x23 = 32 >> 1 (should be 16)
