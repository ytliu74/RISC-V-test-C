USER=yliu1047
MACHINE=acme01
FILE_PATH=/data/home/yliu1047/Intel/UnivDesignChallenge/verif/tb/heather_lake
FILE=SMEM_init.txt

CFILE = $(wildcard *.c)

compile:
	echo "Compiling $(CFILE)"
	docker run --rm -v $(shell pwd):/mount ytliu74/cva6:intel_compile /usr/bin/bash /mount/run.sh $(CFILE)

clean:
	rm -r jtag_cfg.txt SMEM_init.txt *.lst *.hex *.elf

clean_all: clean
	rm -r *.c

upload_SMEM:
	ssh $(USER)@$(MACHINE) "cp $(FILE_PATH)/$(FILE) $(FILE_PATH)/$(FILE).bak"
	scp $(FILE) $(USER)@$(MACHINE):$(FILE_PATH)/$(FILE)
	
upload_jtag:
	ssh $(USER)@$(MACHINE) "cp $(FILE_PATH)/jtag_cfg.txt $(FILE_PATH)/jtag_cfg.txt.bak"
	scp jtag_cfg.txt $(USER)@$(MACHINE):$(FILE_PATH)/jtag_cfg.txt