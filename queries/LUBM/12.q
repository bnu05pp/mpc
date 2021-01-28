select ?x ?y where {
?x  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>        <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#FullProfessor> .
?y	<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Department> . 
?x	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#workFor>	?y . 
?y	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#subOrganizationOf>	<http://www.Department0.University0.edu> . 
}
