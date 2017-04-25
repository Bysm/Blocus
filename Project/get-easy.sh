#!/bin/bash

WORKING_DIR=$(dirname "$0")
URL_LIST_FILE="${WORKING_DIR}/urls.txt"


if [ ! -e "${URL_LIST_FILE}" ] ; then
	echo "Le fichier 'urls.txt' est absent."
	exit 1
fi

remove_com(){
	grep -v '^$\|^#' "$1" | sed 's/$/ /'| tr -d '\n'
}

URL_LIST=$(remove_com "${URL_LIST_FILE}")

EASYLIST_URL_LIST=(${URL_LIST}) # URL list as array to loop

for URL in "${EASYLIST_URL_LIST[@]}"
do
	echo "Telechargement de la EasyList."
	wget -q --no-check-certificate -p ${WORKING_DIR} "${URL}"

done
echo "EasyList à jour"

