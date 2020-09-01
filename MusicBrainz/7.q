select ?composition where {
?artist <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://purl.org/ontology/mo/SoloMusicArtist> .
?artist <http://xmlns.com/foaf/0.1/name> "Wolfgang Amadeus Mozart" .
?artist <http://xmlns.com/foaf/0.1/made> ?composition .
?artist <http://xmlns.com/foaf/0.1/based_near> ?area .
?area <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.w3.org/2003/01/geo/wgs84_pos#SpatialThing> .
?area <http://www.w3.org/2000/01/rdf-schema#label> ?label .
}
