# List of hexadecimal data strings.
hex_data = [
    "0x0000016000001fd01100000000000000000000000000000",
    "0x00840180000004500000000000000000000000000000000",
    "0x00840060000000000038000000000010000020007000000",
    "0x00840060000000000006001000000000100000000100071",
    "0x08840180000000700000000000000000000000000000000",
    "0x08840060000000000074001002000000000000000000000",
    "0x088400e0000000200000000000000000000000000000000",
    "0x00840000000000000000000000000000000000000000000",
    "0x08840000000000000000000000000000000000000000000",
    "0x00000120000000001100000000000000000000000000000",
    "0x00000120000000000180000000000000000000000000000"
]

# Prepare a list to store the computed rows.
c_rows = []

# Process each hex string.
for hexstr in hex_data:
    # Remove "0x" prefix if present.
    if hexstr.startswith("0x") or hexstr.startswith("0X"):
        hexstr = hexstr[2:]
    
    # Convert the hex string to an integer.
    value = int(hexstr, 16)
    
    # Extract each 64-bit part.
    mask64 = (1 << 64) - 1
    # Part 0: least-significant 64 bits.
    part0 = value & mask64
    # Part 1: next 64 bits.
    part1 = (value >> 64) & mask64
    # Part 2: the remaining most-significant bits (padded to 64 bits).
    part2 = (value >> 128) & mask64
    
    # Format the row as a C initializer, using lowercase hex with 16 digits each.
    row = f"{{ 0x{part0:016x}, 0x{part1:016x}, 0x{part2:016x} }}"
    c_rows.append(row)

# Determine the number of lines.
num_lines = len(hex_data)

# Join all rows with commas and newlines.
c_rows_joined = ",\n    ".join(c_rows)

# Format the complete C code block.
c_code = f"uint64_t INPUT_DATA[{num_lines}][3] = {{\n    {c_rows_joined}\n}};"

# Print the C code.
print(c_code)
