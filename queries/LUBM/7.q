select ?x ?z where {
?x	<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#UndergraduateStudent> . 
?z	<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Course> . 
?x	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#takesCourse>	?z . 
<http://www.Department0.University0.edu/AssociateProfessor0>	 <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#teacherOf>	?z . 
}
