#!/bin/bash
# chemin du dossier courant et fichiet txt avec URL
PATH=$(dirname "$0")
PATH_LIST="${PATH}/urls.txt"

#verification que le fichier urls.txt est present
if [ ! -e "${PATH_LIST}" ] ; then
	echo "Le fichier 'urls.txt' est absent."
	exit 1
fi
#fonction pour retirer l'entete
header(){
	grep -v '^$\|^#' "$1" | sed 's/$/ /'| tr -d '\n'
}
# on retire l'entete
URL_LIST=$(header "${PATH_LIST}")
#recupere le contenu de chaque ligne dans un tableau
EASYLIST_URL=(${URL_LIST}) 
#boucle sur chaque ligne
for URL in "${EASYLIST_URL[@]}"
do
	echo "Telechargement de la EasyList."
	#telecharge le easylist.txt
	wget -q --no-check-certificate -p ${PATH} "${URL}"

done
echo "EasyList à jour"

