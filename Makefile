
TARGET = server
 
all:
	gcc PrintStat.c -o $(TARGET)
 
run:
	./$(TARGET)
 
clean:
	rm -f $(TARGET)
