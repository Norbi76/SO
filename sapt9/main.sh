#!/bin/bash

corect_sentences=0

if [ -z $1 ] || [ `echo $1 | grep '^[a-zA-Z0-9]$' | wc -c` -eq 0 ]
    then echo "./main.sh <caracter alfa-numeric>"
    else 
        while read linie
        do 
            regex_rez1=`echo $linie | grep '^[A-Z][a-zA-Z0-9 ,]*[.!?]$' | wc -c` #verifica daca propozitia incepe cu litera mare, se termina cu . ? sau ! si conține doar litere mari, mici, cifre, spații si virgula
            regex_rez2=`echo $linie | grep ',\s*si' | wc -c` # vrefica daca apare , inainte de si
            regex_rez3=`echo $linie | grep $1 | wc -c` #verifica daca caracter dat ca si parametru apare in prop

            if [ $regex_rez1 -gt 0 ] && [ $regex_rez2 -eq 0 ] && [ $regex_rez3 -gt 0 ]
                then corect_sentences=`expr $corect_sentences + 1`
            fi
        done

        echo $corect_sentences
fi
