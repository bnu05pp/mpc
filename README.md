#MPC

## Overview
Minimum Property-Cut (MPC) is a vertex-centric RDF graph partitioning approach, where the objective function is to minimize the cuts to the number of distinct crossing properties. This approach can be used to avoid inter-partition joins in a wider set of SPARQL workloads in the context of distributed SPARQL query evaluation.

## Install Steps

System Requirement: 64-bit linux server with GCC.
*We have tested on linux server with CentOS 7.4 x86_64 where the version of GCC is 5.4.0.*

You can compile mpc in one command. Just run
	
	g++ main.cpp -std=c++11 -o mpc

to compile the code and build executable "mpc".

## Usage
mpc is used to partition an RDF graph.

RUN
	
	./mpc rdf_triple_file_name output separator number_of_parts
	

Now, mpc only support RDF datasets in NT format. Each line is a triple of subject, property and object, where the tab "\t" or the space " " are used as the separator. If `separator` in the above command equals to "1", the separator is the space " "; if `separator` in the above command equals to "2", the separator is the tab "\t".

The partitioning results are written in the file named `#output#InternalPoints.txt`, where each line in the file consists of two parts: the first part is the URI of an resource and the second part is the identifier of the partition that the resource belongs to.

## Benchmark Queries

The benchmark queries used in our experimental evaluation exists in #queries# folder.

If you encounter any problems, please send emails to me (email address: hnu16pp@hnu.edu.cn).

## Acknowledgement

The code is also contributed by Cen Yan (email address: yan_cen@hnu.edu.cn) and Chengjun Liu (email address: lcj2021@hnu.edu.cn).
