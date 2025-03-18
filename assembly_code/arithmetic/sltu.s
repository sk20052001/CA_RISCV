 # Test Case 1: One operand is zero
    li t0, 0      # t0 = 0
    li t1, 5      # t1 = 5
    sltu t2, t0, t1  # t2 = (t0 < t1) ? 1 : 0 (Expected: 1)
    sltu t3, t1, t0  # t3 = (t1 < t0) ? 1 : 0 (Expected: 0)

    # Test Case 2: Both operands are equal
    li t0, 10     # t0 = 10
    li t1, 10     # t1 = 10
    sltu t4, t0, t1  # t4 = (t0 < t1) ? 1 : 0 (Expected: 0)

    # Test Case 3: Both operands are negative (unsigned comparison)
    li t0, -5     # t0 = 0xFFFFFFFB (Unsigned large value)
    li t1, -2     # t1 = 0xFFFFFFFE (Unsigned large value)
    sltu t5, t0, t1  # t5 = (t0 < t1) ? 1 : 0 (Expected: 1)

    # Test Case 4: Both operands are zero
    li t0, 0      # t0 = 0
    li t1, 0      # t1 = 0
    sltu t6, t0, t1  # t6 = (t0 < t1) ? 1 : 0 (Expected: 0)
