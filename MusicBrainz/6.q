select ?x ?y where {
?artist <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://purl.org/ontology/mo/SoloMusicArtist> .
?artist <http://xmlns.com/foaf/0.1/name> "Hakim Ludin" .
?artist <http://purl.org/ontology/mo/member_of> ?x .
?x <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> ?y . }
