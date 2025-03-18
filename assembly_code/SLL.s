# ---------------------------------------
# Load values using ADDI & LUI
# ---------------------------------------

# Random value: 0x12345678
lui x5, 0x12345           # x5 = 0x12345000
addi x5, x5, 0x678        # x5 = 0x12345678 (legal immediate: 0x678 ≤ 2047 decimal)

# Positive value: 0x15 (21 in decimal)
addi x6, x0, 21           

# Zero value
addi x7, x0, 0            

# Input with all ones: 0xFFFFFFFF (use LUI + ADDI with -1)
lui x8, 0xFFFFF           # x8 = 0xFFFFF000
addi x8, x8, -1           # x8 = 0xFFFFFFFF

# Random shift amount: 0x1F (31 decimal)
addi x9, x0, 31           

# ---------------------------------------
# Test 1: Random value (0x12345678) shifted by 4
# Expected: x10 = 0x23456780
# ---------------------------------------
addi x11, x0, 4          # Shift amount = 4
sll x10, x5, x11         

# ---------------------------------------
# Test 2: Positive value (21) shifted by 1
# Expected: x12 = 42
# ---------------------------------------
addi x11, x0, 1         
sll x12, x6, x11        

# ---------------------------------------
# Test 3: Zero value shifted by zero
# Expected: x13 = 0
# ---------------------------------------
addi x11, x0, 0         
sll x13, x7, x11        

# ---------------------------------------
# Test 4: Random value shifted by 31
# Expected: x14 = 0x80000000
# ---------------------------------------
sll x14, x5, x9         

# ---------------------------------------
# Test 5: All ones (0xFFFFFFFF) shifted by zero
# Expected: x15 = 0xFFFFFFFF
# ---------------------------------------
addi x11, x0, 0         
sll x15, x8, x11        

# ---------------------------------------
# Test 6: Check lower 5 bits extraction from rs2
# Shift amount in x17: 0xFF (binary: 11111111) → lower 5 bits = 31
# Expected: x16 = x6 << 31 (verify only lower 5 bits used)
# ---------------------------------------
addi x17, x0, -1        # x17 = 0xFFFFFFFF (rs2 with all ones)
sll x16, x6, x17        

# ---------------------------------------
# End: Results stored in x10 - x16
# ---------------------------------------
