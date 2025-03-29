bin=processbar
src1=Processbar.c
src2=Main.c

$(bin):$(src1) $(src2)
	@gcc -o $@ $^
	@echo "complie $(src1) $(src2) to $(bin)"
.PHONY:clean
clean:
	@rm -f $(bin)
	@echo "clean project..."

