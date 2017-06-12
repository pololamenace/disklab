#--------------------------------------------------------------------------------
# Student ID / Name
# replace the values for ID/NAME below with your student ID/name
#
ID   = "2016-00000"
NAME = "Invalid"
#--------------------------------------------------------------------------------

CXX_OPTS=-O0 -g

.PHONY: disklab

all: disklab

%.o: %.c
	$(CXX) $(CXX_OPTS) -Wall -c -o $@ $<

test: cache.o cache_driver.o
	$(CXX) $(CXX_OPTS) -Wall -o cache $^

disklab: hdd.o cache.o disk_driver.o
	$(CXX) $(CXX_OPTS) -Wall -o disklab $^

handin:
	@echo "----------------------------------------------------------------------------------------"
	@echo "Creating handin for $(ID) $(NAME) (if this is not you, edit the Makefile)..."
	@echo ""
	@mkdir -p $(ID)
	@touch $(ID)/STUDENT.INFO
	@echo "Created by: $(NAME) ($(ID))" > $(ID)/STUDENT.INFO
	@date >> $(ID)/STUDENT.INFO
	@cp *.h *.cpp $(ID)/
	@tar czf $(ID).tgz $(ID)/
	@echo "Done. To complete the handin, email the tarball $(ID).tgz to comparch@csap.snu.ac.kr."
	@echo "----------------------------------------------------------------------------------------"

clean:
	rm -rf *.o disklab cache $(ID)

