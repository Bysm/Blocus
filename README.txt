Projet de RSA : Proxy MyAdBlock

Auteurs : Jean-Etienne SCHOUVER & Marion TOUSSAINT

Contenu : 
	*Tous les fichiers sources
		- get-easy.sh
		- myadblock.sh
		- makefile (Linux)
		- proxy.c
		- getList.c
		- urls.txt
	*README.txt
	*Makefile
	*Rapport de projet

INSTALLATION et UTILISATION du Proxy:

### Dependances ###
	*bash
	*wget

==> Verifier la presence de tout les sources avant de compiler

### Compilation ###

	* execution du Makefile utiliser la commande : 
		$make

### Lancement ###

	* le lancement du proxy s'effectue avec la commande:
		$./myadblock.sh

### ERREUR POSSIBLE ###

	#la commande utilisé en exemple est '$chmod 777 $fichier' mais elle peut eter remplacer par '$chmod +x $fichier'

	* en cas de probleme de permission sur l'execution entrer les commande suivante:
		$chmod 777 get-easy.sh
		$chmod 777 myadblock.sh

	* si le probleme persiste il faut autoriser les fichier generere par le Makefile a etre executer:
		$chmod 777 proxyHTTP
		$chmod 777 getlist

Travail réalisé :
Tout d'abord, nous avons réalisé un serveur proxy qui laissait passer toutes les pages envoyées par le serveur web suite à la demande du client. Puis, nous avons ajouté un filtrage à ce proxy pour l'empêcher de transmettre au client les pages provenant de domaines hébergeurs de publicités. Pour cela, nous nous sommes aidés d'une liste de ces hébergeurs et nous parcourons cette liste pour déterminer si les requêtes peuvent être relayées au client ou non.

