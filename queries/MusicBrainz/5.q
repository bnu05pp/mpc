select ?artist where {
?track <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> ?a .
?track <http://purl.org/dc/elements/1.1/title> "This Magic Moment" .
?artist <http://xmlns.com/foaf/0.1/made> ?track .
?artist <http://xmlns.com/foaf/0.1/name> "Lou Reed" . }
