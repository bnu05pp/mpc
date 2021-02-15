PREFIX pharmkb:<http://bio2rdf.org/pharmgkb_vocabulary>
PREFIX irefindex:<http://bio2rdf.org/irefindex_vocabulary>
PREFIX pubmd:<http://bio2rdf.org/pubmed_vocabulary>
PREFIX pubmdrc:<http://bio2rdf.org/pubmed_resource>
PREFIX omim:<http://bio2rdf.org/omim_vocabulary>
PREFIX drug:<http://bio2rdf.org/drugbank>
PREFIX uniprot:<http://bio2rdf.org/uniprot>

SELECT ?phenotype WHERE{
	?phenotype <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> omim:Phenotype .
	?phenotype <http://www.w3.org/2000/01/rdf-schema#label> ?label .
	?gene <http://bio2rdf.org/omim_vocabulary:phenotype> ?phenotype .
}
