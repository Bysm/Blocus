Projet de RSA : Proxy MyAdBlock

Auteurs : Jean-Etienne SCHOUVER & Marion TOUSSAINT

Contenu : 
Tous les fichiers sources
ce README
Makefile
Rapport de projet

Fonctionnement : make proxyHTTP 

Travail réalisé :
Tout d'abord, nous avons réalisé un serveur proxy qui laissait passer toutes les pages envoyées par le serveur web suite à la demande du client. Puis, nous avons ajouté un filtrage à ce proxy pour l'empêcher de transmettre au client les pages provenant de domaines hébergeurs de publicités. Pour cela, nous nous sommes aidés d'une liste de ces hébergeurs et nous parcourons cette liste pour déterminer si les requêtes peuvent être relayées au client ou non.

