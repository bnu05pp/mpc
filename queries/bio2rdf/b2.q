SELECT ?articleToMesh WHERE{
	<http://bio2rdf.org/pubmed:126183> <http://bio2rdf.org/pubmed_vocabulary:mesh_heading> ?articleToMesh .
	?articleToMesh <http://bio2rdf.org/pubmed_vocabulary:mesh_descriptor_name> ?mesh .
}
