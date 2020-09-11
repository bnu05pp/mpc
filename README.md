This is the codes of the paper, Minimum Property-Cut RDF Graph Partitioning to Minimize Inter-Partition Joins in Distributed SPARQL Execution, submitted in SIGMOD 2021.

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
	
