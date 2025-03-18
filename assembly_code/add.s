 #---------------------------------------
# Helper: Load immediate macro (manually expanded)
# ---------------------------------------
# Load -5 into x3: (since -5 fits in 12-bit immediate)
    addi x3, x0, -5         # x3 = -5

# Load 10 into x4:
    addi x4, x0, 10         # x4 = 10

# Load 7 into x5:
    addi x5, x0, 7          # x5 = 7

# Load -7 into x6:
    addi x6, x0, -7         # x6 = -7

# Load 25 into x8:
    addi x8, x0, 25         # x8 = 25

# ---------------------------------------
# Test 1: Both operands are zero (x1=0, x2=0)
# Result -> x10
# ---------------------------------------
    addi x1, x0, 0          # x1 = 0
    addi x2, x0, 0          # x2 = 0
    add x10, x1, x2         # x10 = 0 + 0 = 0

# ---------------------------------------
# Test 2: Signed addition (-5 + 10)
# Result -> x11
# ---------------------------------------
    add x11, x3, x4         # x11 = -5 + 10 = 5

# ---------------------------------------
# Test 3: Adding a register with its negative (7 + -7)
# Result -> x12
# ---------------------------------------
    add x12, x5, x6         # x12 = 7 + (-7) = 0

# ---------------------------------------
# Test 4: Adding zero with a non-zero register (0 + 25)
# Result -> x13
# ---------------------------------------
    addi x7, x0, 0          # x7 = 0
    add x13, x7, x8         # x13 = 0 + 25 = 25

# ---------------------------------------
# End of tests: Results stored in x10-x13
# x10 = 0, x11 = 5, x12 = 0, x13 = 25
# ---------------------------------------
