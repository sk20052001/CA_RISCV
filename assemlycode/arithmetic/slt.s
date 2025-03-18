    # Test case 1: rs1 = +45, rs2 = -10, sets rd = 0
    addi x3, x0, 45     # Load immediate +45 into x3 (rs1)
    addi x4, x0, -10    # Load immediate -10 into x4 (rs2)
    slt  x5, x3, x4     # slt sets x5 (rd) to 0 (x3 is not less than x4)

    # Test case 2: rs1 = -50, rs2 = -20, sets rd = 1
    addi x6, x0, -50    # Load immediate -50 into x6 (rs1)
    addi x7, x0, -20    # Load immediate -20 into x7 (rs2)
    slt  x8, x6, x7     # slt sets x8 (rd) to 1 (x6 is less than x7)

    # Test case 3: rs1 = -60, rs2 = +25, sets rd = 1
    addi x9, x0, -60    # Load immediate -60 into x9 (rs1)
    addi x10, x0, 25    # Load immediate +25 into x10 (rs2)
    slt  x11, x9, x10   # slt sets x11 (rd) to 1 (x9 is less than x10)

    # Test case 4: rs1 = +30, rs2 = +15, sets rd = 0
    addi x12, x0, 30    # Load immediate +30 into x12 (rs1)
    addi x13, x0, 15    # Load immediate +15 into x13 (rs2)
    slt  x14, x12, x13  # slt sets x14 (rd) to 0 (x12 is not less than x13)
    
    # Test case 5: rs1 = -5, rs2 = 0, sets rd = 1
    addi x15, x0, -5    # Load immediate -5 into x15 (rs1)
    addi x16, x0, 0     # Load immediate 0 into x16 (rs2)
    slt  x17, x15, x16  # slt sets x17 (rd) to 1 (x15 is less than x16)

    # Test case 6: rs1 = 0, rs2 = 0, sets rd = 0
    addi x18, x0, 0     # Load immediate 0 into x18 (rs1)
    addi x19, x0, 0     # Load immediate 0 into x19 (rs2)
    slt  x20, x18, x19  # slt sets x20 (rd) to 0 (x18 is not less than x19)
