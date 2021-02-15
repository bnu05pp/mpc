SELECT ?phenotype WHERE{
	?phenotype <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://bio2rdf.org/omim_vocabulary:Phenotype> .
	?phenotype <http://www.w3.org/2000/01/rdf-schema#label> ?label .
	?gene <http://bio2rdf.org/omim_vocabulary:phenotype> ?phenotype .
}
