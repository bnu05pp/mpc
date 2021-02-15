SELECT ?pharmgkbid WHERE{
	?pharmgkbid <http://bio2rdf.org/pharmgkb_vocabulary:xref> <http://bio2rdf.org/drugbank:DB00126> .
	?pharmgkbid <http://bio2rdf.org/pharmgkb_vocabulary:xref> ?pccid .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:chemical> ?pccid .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:event> ?DDIevent .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:chemical> ?drug2 .
	?DDIassociation <http://bio2rdf.org/pharmgkb_vocabulary:p-value> ?pvalue .
}
