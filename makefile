TARGET  = smoked 
CC      = gcc

INCLUDES =  include
ALLSRC =  $(wildcard sys/*.c utility/*.c handler/*.c misc/*.c) 
ALLOBJ =  $(patsubst %.c,%.o,$(ALLSRC))
ALLINC =  $(wildcard $(INCLUDES)/*.h)
LIBDLL =  $(INCLUDES)/hybServKit.a 

.PHONY:clean all

$(TARGET): $(ALLOBJ) $(LIBDLL) $(ALLINC)  
	$(CC) -o $@  $(ALLOBJ) $(LIBDLL) -I $(INCLUDES) -lpthread -lmysqlclient -lm -I/usr/include/mysql/ -L/usr/lib64/mysql
	@echo "...Finished!"
sys/%.o: sys/%.c $(ALLINC)
	$(CC) -c $< -o $@ -I $(INCLUDES) -g -Wall
utility/%.o: utility/%.c $(ALLINC)
	$(CC) -c $< -o $@ -I $(INCLUDES) -g -Wall
misc/%.o: misc/%.c $(ALLINC)
	$(CC) -c $< -o $@ -I $(INCLUDES) -g -Wall
handler/%.o: handler/%.c $(ALLINC)
	$(CC) -c $< -o $@ -I $(INCLUDES) -g -Wall
	
all:
	@$(MAKE) -f $(INCLUDES)/mklib.mk
	@$(MAKE)
clean: 
	rm -f sys/*.o utility/*.o handler/*.o misc/*.o include/*.o
	
#  $@--Ŀ���ļ���$^--���е������ļ���$<--��һ�������ļ���
