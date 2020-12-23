This is the codes of the paper, Minimum Property-Cut RDF Graph Partitioning to Minimize Inter-Partition Joins in Distributed SPARQL Execution.

COMPILE
	
	g++ main.cpp -std=c++11 -o main

RUN
	
	./main RDFData Data_name tag part
	(
		tag:split the triple
			1 " "
			2 "\t"

		part:the number of partitons
	)
	Ex:	./main /data/RDFData/LUBM/LUBM10M.nt LUBM10M 1 4

	Results are in Data_nameInternalPoints.txt and Data_namecrossingEdges.txt(1,2,3)
	

The above version of mpc is to load the whole RDF dataset into memory. For large RDF datasets, you may not have enough memory. We also provide another version that uses less memory. It is slower but needs less memory.

COMPILE
	
	chmod 777 build.sh
	./build.sh

RUN
	
	./run.sh RDFData Data_name tag part
	(
		tag:split the triple
			1 " "
			2 "\t"

		part:the number of partitons
	)
	Ex:	./run.sh /data/RDFData/LUBM/LUBM10M.nt LUBM10M 1 4

	Results are in Data_nameInternalPoints.txt and Data_namecrossingEdges.txt(1,2,3)

