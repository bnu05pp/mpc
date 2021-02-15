PREFIX pharmkb:<http://bio2rdf.org/pharmgkb_vocabulary>
PREFIX irefindex:<http://bio2rdf.org/irefindex_vocabulary>
PREFIX pubmd:<http://bio2rdf.org/pubmed_vocabulary>
PREFIX pubmdrc:<http://bio2rdf.org/pubmed_resource>
PREFIX omim:<http://bio2rdf.org/omim_vocabulary>
PREFIX drug:<http://bio2rdf.org/drugbank>
PREFIX uniprot:<http://bio2rdf.org/uniprot>


SELECT ?pharmgkbid WHERE{
	?pharmgkbid <http://bio2rdf.org/pharmgkb_vocabulary:xref> <http://bio2rdf.org/drugbank:DB00126> .
	?pharmgkbid <http://bio2rdf.org/pharmgkb_vocabulary:xref> ?pccid .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:chemical> ?pccid .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:event> ?DDIevent .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:chemical> ?drug2 .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:p-value> ?pvalue .
}
