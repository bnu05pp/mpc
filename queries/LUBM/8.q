select ?x ?y ?z where {
?x	<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#UndergraduateStudent> . 
?y	<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department> . 
?x	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#memberOf>	?y . 
?y	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>	<http://www.University0.edu> . 
?y	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#emailAddress>	?z . 
}