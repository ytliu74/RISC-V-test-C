#!/usr/bin/bash

# This is for compiling the source code for CVA6 core.

TARGET_FILE=$1

# Check if the file is provided
if [ -z "$TARGET_FILE" ]
then
  echo "Error: No file provided. Please provide the source file as an argument."
  exit 1
fi

# Remove the postfix of the file name
TARGET=${TARGET_FILE%.*}

echo "Starting the process with target file: $TARGET_FILE"

# Copy the file and ./lib
if cp /mount/"$TARGET_FILE"  /home/cva6/jtag_generating && cp -r /mount/lib/*.h /home/cva6/jtag_generating && cp -f /mount/compile.sh /home/cva6/jtag_generating 
then
  echo "File copied successfully."
else
  echo "Error in copying the file. Exiting..."
  exit 1
fi

cd /home/cva6/jtag_generating

export RISCV=/home/toolchain
export PATH=$PATH:$RISCV/bin

export target=$TARGET

# Run the compile.sh script
if bash compile.sh
then
  echo "Compilation successful."
else
  echo "Error in compilation. Exiting..."
  exit 1
fi

# Run the dump.sh script
if bash dump.sh
then
  echo "Dump successful."
else
  echo "Error in dump. Exiting..."
  exit 1
fi

cd ./out_files

# Run the python script
if python3 gen_jtag_smem.py "$TARGET"
then
  echo "Python script run successful."
else
  echo "Error in running Python script. Exiting..."
  exit 1
fi

# Copy the output files
if cp "$TARGET".elf /mount && cp "$TARGET"_data.hex /mount && cp "$TARGET"_text.hex /mount && cp "$TARGET"_assembly.lst /mount && cp SMEM_init.txt /mount && cp jtag_cfg.txt /mount
then
  echo "Files copied successfully. Process completed."
else
  echo "Error in copying the output files. Exiting..."
  exit 1
fi
