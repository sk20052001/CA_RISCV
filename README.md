simulation of RISC-V 32I based Instruction Set Architecture.

## Project File Structure

```
+---Assembly Code
|       +---arithmetic
|           |    arithmetic assembly codes
|       +---Branch
|           |    Branching assembly codes
|       Loads and Stores assembly codes
|       
+---Memory Image
|       +---arithmetic
|           |    arithmetic memory images
|       +---Branch
|           |    Branching memory images
|       +---Logical
|           |    Logical memory images
|       Loads and Stores memory images
defines.c
riscv32i.c
```
# Simulation and Debugging Commands
## To Create .exe file
```bash
"gcc riscv32i.c -o riscv32i"
```
## To execute the .exe file
```bash
"./riscv32i"
```
## To give custom input file, starting address and stack address
```bash
 "./ddr5_command 'input_file' 'starting address' 'stack address'"
```

This project was undertaken as part of the **ECE586 course** at **Portland State University** under the guidance of **Prof. Mark G. Faust**.
